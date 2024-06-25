.. Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
.. See the doc/COPYRIGHT file for details.
.. SPDX-License-Identifier: CC-BY-4.0

.. highlight:: cmake

.. _infrastructure:

************
Installation
************

QIR-EE is designed to be easy to install for a multitude of use cases.

.. _dependencies:

Dependencies
============

QIR-EE is built using modern CMake_. It has multiple dependencies to operate
as a full-featured code, but each dependency can be individually disabled as
needed.

.. _CMake: https://cmake.org

The code requires external dependencies to build with full functionality.
However, any combination of these requirements can be omitted to enable
limited development on experimental HPC systems or personal machines with
fewer available components.

.. tabularcolumns:: lll

.. csv-table::
   :header: Component, Category, Description
   :widths: 10, 10, 20

   XACC_, Runtime, "Multi-platform quantum software backend"
   Breathe_, Docs, "Generating code documentation inside user docs"
   Doxygen_, Docs, "Code documentation"
   Sphinx_, Docs, "User documentation"
   sphinxbib_, Docs, "Reference generation for user documentation"
   clang-format_, Development, "Code formatting enforcement"
   CMake_, Development, "Build system"
   Git_, Development, "Repository management"
   GoogleTest_, Development, "Test harness"

.. _CMake: https://cmake.org
.. _XACC: h
.. _Doxygen: https://www.doxygen.nl
.. _Git: https://git-scm.com
.. _GoogleTest: https://github.com/google/googletest
.. _Sphinx: https://www.sphinx-doc.org/
.. _breathe: https://github.com/michaeljones/breathe#readme
.. _clang-format: https://clang.llvm.org/docs/ClangFormat.html
.. _sphinxbib: https://pypi.org/project/sphinxcontrib-bibtex/


Ideally you will build QIR-EE with all dependencies to gain the full
functionality of the code, but there are circumstances in which you may not
have (or want) all the dependencies or features available.

Toolchain installation
======================

TODO: spack instructions once XACC plays nice with externals

Building QIR-EE
==================

Once the QIR-EE Spack environment has been installed, set your shell's environment
variables (``PATH``, ``CMAKE_PREFIX_PATH``, ...) by activating it.

To clone the latest development version of QIR-EE:

.. code-block:: console

   $ git clone https://github.com/qiree-project/qiree.git

or download it from the GitHub-generated `zip file`_.

, you can configure, build, and test using the provided helper script:

.. code-block:: console

   $ cd qiree
   $ spack env activate qiree
   $ ./scripts/build.sh base

or manually with:

.. code-block:: console

   $ cd qiree
   $ spack env activate qiree
   $ mkdir build && cd build
   $ cmake ..
   $ make && ctest

.. _zip file: https://github.com/qiree-project/qiree/archive/refs/heads/develop.zip

CMake Presets
=============

To manage multiple builds with different
configure options (debug or release, VecGeom or ORANGE), you can use the
CMake presets provided by QIR-EE via the ``CMakePresets.json`` file for CMake
3.21 and higher:

.. code-block:: console

   $ cmake --preset=default

The three main options are "minimal", "default", and "full", which all set
different expectations for available dependencies.

.. note::

   If your CMake version is too old, you may get an unhelpful message:

   .. code-block:: console

      CMake Error: Could not read presets from qiree: Unrecognized "version"
      field

   which is just a poor way of saying the version in the ``CMakePresets.json``
   file is newer than that version knows how to handle.

If you want to add your own set of custom options and flags, create a
``CMakeUserPresets.json`` file or, if you wish to contribute on a regular
basis, create a preset at :file:`scripts/cmake-presets/{HOSTNAME}.json` and
call ``scripts/build.sh {preset}`` to create the symlink, configure the preset,
build, and test. See :file:`scripts/README.md` in the code repository for more
details.
