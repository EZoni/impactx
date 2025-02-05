.. _examples-rotation:

Drift using a Pole-Face Rotation
================================

A drift that takes place in a rotated frame, using initial and final
applications of a pole rotation (``prot``).

We use a 2 GeV electron beam.

The second moments of x, y, and t should be nearly unchanged.

In this test, the initial and final values of :math:`\sigma_x`, :math:`\sigma_y`, :math:`\sigma_t`, :math:`\epsilon_x`, :math:`\epsilon_y`, and :math:`\epsilon_t` must agree with nominal values.


Run
---

This example can be run **either** as:

* **Python** script: ``python3 run_rotation.py`` or
* ImpactX **executable** using an input file: ``impactx input_rotation.in``

For `MPI-parallel <https://www.mpi-forum.org>`__ runs, prefix these lines with ``mpiexec -n 4 ...`` or ``srun -n 4 ...``, depending on the system.

.. tab-set::

   .. tab-item:: Python: Script

       .. literalinclude:: run_rotation.py
          :language: python3
          :caption: You can copy this file from ``examples/rotation/run_rotation.py``.

   .. tab-item:: Executable: Input File

       .. literalinclude:: input_rotation.in
          :language: ini
          :caption: You can copy this file from ``examples/rotation/input_rotation.in``.


Analyze
-------

We run the following script to analyze correctness:

.. dropdown:: Script ``analysis_rotation.py``

   .. literalinclude:: analysis_rotation.py
      :language: python3
      :caption: You can copy this file from ``examples/rotation/analysis_rotation.py``.


.. _examples-rotation-xy:

Simple rotation in the x-y plane
=================================

A beam is rotated counter-clockwise by 90 degrees about the direction of motion.

We use a 2 GeV electron beam.

The second beam moments associated with the phase planes (x,px) and (y,py) are unequal, and these are exchanged under a 90 degree rotation.

In this test, the initial and final values of :math:`\sigma_x`, :math:`\sigma_y`, :math:`\sigma_t`, :math:`\epsilon_x`, :math:`\epsilon_y`, and :math:`\epsilon_t` must agree with nominal values.


Run
---

This example can be run **either** as:

* **Python** script: ``python3 run_rotation_xy.py`` or
* ImpactX **executable** using an input file: ``impactx input_rotation_xy.in``

For `MPI-parallel <https://www.mpi-forum.org>`__ runs, prefix these lines with ``mpiexec -n 4 ...`` or ``srun -n 4 ...``, depending on the system.

.. tab-set::

   .. tab-item:: Python: Script

       .. literalinclude:: run_rotation_xy.py
          :language: python3
          :caption: You can copy this file from ``examples/rotation/run_rotation_xy.py``.

   .. tab-item:: Executable: Input File

       .. literalinclude:: input_rotation_xy.in
          :language: ini
          :caption: You can copy this file from ``examples/rotation/input_rotation_xy.in``.


Analyze
-------

We run the following script to analyze correctness:

.. dropdown:: Script ``analysis_rotation_xy.py``

   .. literalinclude:: analysis_rotation_xy.py
      :language: python3
      :caption: You can copy this file from ``examples/rotation/analysis_rotation_xy.py``.
