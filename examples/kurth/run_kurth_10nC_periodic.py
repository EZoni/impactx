#!/usr/bin/env python3
#
# Copyright 2022-2023 ImpactX contributors
# Authors: Ryan Sandberg, Axel Huebl, Chad Mitchell
# License: BSD-3-Clause-LBNL
#
# -*- coding: utf-8 -*-

from impactx import ImpactX, distribution, elements

sim = ImpactX()

# set numerical parameters and IO control
sim.n_cell = [48, 48, 40]  # use [72, 72, 72] for increased precision
sim.particle_shape = 2  # B-spline order
sim.space_charge = True
# sim.diagnostics = False  # benchmarking
sim.slice_step_diagnostics = True

# domain decomposition & space charge mesh
sim.init_grids()

# load a 2 GeV proton beam with an initial
# unnormalized rms emittance of 1 um in each
# coordinate plane
kin_energy_MeV = 2.0e3  # reference energy
bunch_charge_C = 1.0e-8  # used with space charge
npart = 10000  # number of macro particles; use 1e5 for increased precision

#   reference particle
ref = sim.particle_container().ref_particle()
ref.set_charge_qe(1.0).set_mass_MeV(938.27208816).set_kin_energy_MeV(kin_energy_MeV)

#   particle bunch
distr = distribution.Kurth6D(
    lambdaX=1.46e-3,
    lambdaY=1.46e-3,
    lambdaT=4.9197638312420749e-4,
    lambdaPx=6.84931506849e-4,
    lambdaPy=6.84931506849e-4,
    lambdaPt=2.0326178944803812e-3,
)
sim.add_particles(bunch_charge_C, distr, npart)

# add beam diagnostics
monitor = elements.BeamMonitor("monitor", backend="h5")

# design the accelerator lattice
nslice = 20  # use 30 for increased precision
constf1 = elements.ConstF(name="constf1", ds=2.0, kx=0.7, ky=0.7, kt=0.7, nslice=nslice)
drift1 = elements.Drift(name="drift1", ds=1.0, nslice=nslice)
sim.lattice.extend([monitor, drift1, constf1, drift1, monitor])

# run simulation
sim.track_particles()

# clean shutdown
sim.finalize()
