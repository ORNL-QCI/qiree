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

   Usage: ./../build/bin/qir-xacc [OPTIONS] input

   Positionals:
     input TEXT REQUIRED              QIR input file

   Options:
     -h,--help                        Print this help message and exit
     -i,--input TEXT REQUIRED         QIR input file
     -a,--accelerator TEXT REQUIRED   Accelerator name
     -s,--shots INT [1024]            Number of shots
