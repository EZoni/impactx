/* Copyright 2022-2023 The Regents of the University of California, through Lawrence
 *           Berkeley National Laboratory (subject to receipt of any required
 *           approvals from the U.S. Dept. of Energy). All rights reserved.
 *
 * This file is part of ImpactX.
 *
 * Authors: Chad Mitchell, Axel Huebl
 * License: BSD-3-Clause-LBNL
 */
#ifndef IMPACTX_APERTURE_H
#define IMPACTX_APERTURE_H


#include "particles/ImpactXParticleContainer.H"
#include "mixin/alignment.H"
#include "mixin/beamoptic.H"
#include "mixin/thin.H"
#include "mixin/named.H"
#include "mixin/nofinalize.H"

#include <AMReX_Extension.H>
#include <AMReX_REAL.H>

#include <cmath>

namespace impactx
{
    struct Aperture
    : public elements::Named,
      public elements::BeamOptic<Aperture>,
      public elements::Thin,
      public elements::Alignment,
      public elements::NoFinalize
    {
        static constexpr auto type = "Aperture";
        using PType = ImpactXParticleContainer::ParticleType;

        // TODO: make AMREX_ENUM and simplify @see shape_name implementation with it
        enum Shape
        {
            rectangular,
            elliptical
        };

        static std::string
        shape_name (Shape const & shape);

        /** A thin collimator element that applies a transverse aperture boundary.
         *  Particles outside the boundary are considered lost.
         *
         * @param shape aperture shape
         * @param xmax maximum value of horizontal coordinate (m)
         * @param ymax maximum value of vertical coordinate (m)
         * @param repeat_x horizontal period for repeated masking, optional (m)
         * @param repeat_y vertical period for repeated masking, optional (m)
         * @param dx horizontal translation error in m
         * @param dy vertical translation error in m
         * @param rotation_degree rotation error in the transverse plane [degrees]
         * @param name a user defined and not necessarily unique name of the element
         */
        Aperture (
            amrex::ParticleReal xmax,
            amrex::ParticleReal ymax,
            amrex::ParticleReal repeat_x,
            amrex::ParticleReal repeat_y,
            Shape shape,
            amrex::ParticleReal dx = 0,
            amrex::ParticleReal dy = 0,
            amrex::ParticleReal rotation_degree = 0,
            std::optional<std::string> name = std::nullopt
        )
        : Named(name),
          Alignment(dx, dy, rotation_degree),
          m_shape(shape), m_xmax(xmax), m_ymax(ymax), m_repeat_x(repeat_x), m_repeat_y(repeat_y)
        {
        }

        /** Push all particles */
        using BeamOptic::operator();

        /** This is an aperture functor, so that a variable of this type can be used like an
         *  aperture function.
         *
         * @param x particle position in x
         * @param y particle position in y
         * @param t particle position in t (unused)
         * @param px particle momentum in x
         * @param py particle momentum in y
         * @param pt particle momentum in t (unused)
         * @param idcpu particle global index
         * @param refpart reference particle (unused)
         */
        AMREX_GPU_HOST_DEVICE AMREX_FORCE_INLINE
        void operator() (
            amrex::ParticleReal & AMREX_RESTRICT x,
            amrex::ParticleReal & AMREX_RESTRICT y,
            [[maybe_unused]] amrex::ParticleReal & AMREX_RESTRICT t,
            amrex::ParticleReal & AMREX_RESTRICT px,
            amrex::ParticleReal & AMREX_RESTRICT py,
            [[maybe_unused]] amrex::ParticleReal & AMREX_RESTRICT pt,
            uint64_t & AMREX_RESTRICT idcpu,
            [[maybe_unused]] RefPart const & refpart
        ) const
        {
            using namespace amrex::literals; // for _rt and _prt

            // shift due to alignment errors of the element
            shift_in(x, y, px, py);

            // define intermediate variables
            amrex::ParticleReal u;
            amrex::ParticleReal v;
            amrex::ParticleReal dx = m_xmax;
            amrex::ParticleReal dy = m_ymax;

            // if the aperture is periodic, shift x,y coordinates to the fundamental domain
            u = (m_repeat_x==0.0) ? x : (std::fmod(std::abs(x)+dx,m_repeat_x)-dx);
            v = (m_repeat_y==0.0) ? y : (std::fmod(std::abs(y)+dy,m_repeat_y)-dy);

            // scale horizontal and vertical coordinates
            u = u / m_xmax;
            v = v / m_ymax;

            // compare against the aperture boundary
            switch (m_shape)
            {
                case Shape::rectangular :  // default
                  if (std::pow(u,2)>1 || std::pow(v,2) > 1_prt) {
                      amrex::ParticleIDWrapper{idcpu}.make_invalid();
                  }
                  break;

               case Shape::elliptical :
                  if (std::pow(u,2) + std::pow(v,2) > 1_prt) {
                      amrex::ParticleIDWrapper{idcpu}.make_invalid();
                  }
                  break;
            }

            // undo shift due to alignment errors of the element
            shift_out(x, y, px, py);
        }

        /** This pushes the reference particle. */
        using Thin::operator();

        Shape m_shape; //! aperture type (rectangular, elliptical)
        amrex::ParticleReal m_xmax; //! maximum horizontal coordinate
        amrex::ParticleReal m_ymax; //! maximum vertical coordinate
        amrex::ParticleReal m_repeat_x; //! horizontal period for repeated masking
        amrex::ParticleReal m_repeat_y; //! vertical period for repeated masking

    };

} // namespace impactx

#endif // IMPACTX_APERTURE_H
