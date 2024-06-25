.. Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
.. See the doc/COPYRIGHT file for details.
.. SPDX-License-Identifier: CC-BY-4.0

.. highlight:: none

.. _usage:

***************
Using QIR-EE
***************

QIR-EE includes a core set of libraries for internal and external use, as
well as applications and front ends.

Software library
================

TODO

.. _celer-sim:

XACC interface (qir-xacc)
=========================

The ``qir-xacc`` application processes an LLVM QIR file and dispatches to XACC.

Usage::

   usage: qir-xacc {input}.ll accelerator num_shots
          qir-xacc [--help|-h]
          qir-xacc --version


- :file:`{input}.ll` is the path to the LLVM IR file.
