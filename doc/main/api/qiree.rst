.. Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
.. See the doc/COPYRIGHT file for details.
.. SPDX-License-Identifier: CC-BY-4.0

.. _api_qiree:

Core package
============

The ``qiree`` directory contains functionality shared by QIR-EE and ORANGE
primarily pertaining to GPU abstractions.

Configuration
-------------

The ``qiree_config.h`` configure file contains all-caps definitions of the
CMake configuration options as 0/1 defines so they can be used with ``if
constexpr`` and other C++ expressions. The version is defined in
``qiree_version.h``.

.. doxygenfile:: qiree_version.h

Fundamentals
------------

.. doxygenfile:: qiree/Macros.hh

QIR-EE assertions are only enabled when the ``CELERITAS_DEBUG``
configuration option is set. The macros ``CELER_EXPECT``, ``CELER_ASSERT``, and
``CELER_ENSURE`` correspond to "precondition contract", "internal assertion",
and "postcondition contract".

.. doxygenfile:: qiree/Assert.hh

.. doxygenfile:: qiree/Types.hh

QIR Interfaces
--------------

.. doxygenclass:: qiree::QuantumInterface

.. doxygenclass:: qiree::RuntimeInterface

.. doxygenclass:: qiree::MathInterface


Execution
---------

.. doxygenclass:: qiree::Module

.. doxygenclass:: qiree::Executor

