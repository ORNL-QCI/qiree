.. Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
.. See the doc/COPYRIGHT file for details.
.. SPDX-License-Identifier: CC-BY-4.0

.. highlight:: none

.. _usage:

***************
Using QIR-EE
***************

QIR-EE includes a core set of libraries for internal/external use, applications, and front ends.


Interface Application (qir-xacc)
================================

The ``qir-xacc`` application processes an LLVM QIR file and dispatches to XACC.

Usage::

   ./../build/bin/qir-xacc [OPTIONS] input

   Positionals:
     input TEXT REQUIRED              QIR input file

   Options:
     -h,--help                        Print this help message and exit
     -i,--input TEXT REQUIRED         QIR input file
     -a,--accelerator TEXT REQUIRED   Accelerator name
     -s,--shots INT [1024]            Number of shots


Syntax for Execution
====================

Export Path to Executable::

    export PATH=${YOUR-QIREE-INSTALL-DIR}/bin:$PATH

This will allow you to access installed QIR-EE executables from anywhere in command line.

Basic Usage with ``qir-xacc`` Executable::

    qir-xacc llvm-file-path --flag-name flag-value

With XACC we have tested: ``aer``, ``qpp``, ``qsim``, ``quantinuum:H1-1SC``, ``quantinuum:H1-1E``, ``quantinuum:H1-1``, ``ionq``, ``ionq:sim.harmony``, ``ionq:sim.aria-1``, ``ionq:qpu.harmony``.

* The a minimal XACC build only includes ``aer`` and ``qpp`` for local simulators.

* Credentials are required for access to ``ionq`` and ``quantinuum`` and can be stored in ``.ionq_config`` and ``.quantinuum_config`` files in the home directory, respectively.


Example
=======

.. code-block:: console

    qir-xacc $HOME/qiree/examples/bell.ll --accelerator qpp

This command will execute the quantum Bell circuit described in bell.ll the (default)
1024 times using the ``qpp`` accelerator.

The resulting output looks like:

.. code-block:: console

    {
        "AcceleratorBuffer": {
            "name": "qreg_0x5653927715a0",
            "size": 2,
            "Information": {},
            "Measurements": {
                "00": 526,
                "11": 498
            }
        }
    }
    array <null> length 2
    qubit 0 experiment <null>: {0: 526, 1: 498}
    qubit 1 experiment <null>: {0: 526, 1: 498}


Software library
================

TODO
