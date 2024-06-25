.. Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
.. See the doc/COPYRIGHT file for details.
.. SPDX-License-Identifier: CC-BY-4.0

.. _development:

*****************
Development Guide
*****************

The agility, extensibility, and performance of QIR-EE depend strongly on
software infrastructure and best practices. This appendix describes how to
modify and extend the codebase.

.. include:: ../../CONTRIBUTING.rst


.. _code_guidelines:

Code development guidelines
===========================

Every new piece of code is a commitment for you and other developers to
maintain it in the future (or delete it if obsolete). There are numerous
considerations to making code easier to update or understand, including testing
and documentation.


Document implicitly and explicitly
----------------------------------

Code should be self-documenting as far as possible (see details below for
naming conventions). This means that variable names, function names, and
function arguments should be as "obvious" as possible. Take particular care
with constants that appear in physics implementations. They should
be multiplied by units in the native QIR-EE unit system if applicable, or
defined as ``Quantity`` instances. The numerical value of the constant must
also be documented with a paper citation or other comment.


Test thoroughly
---------------

Functions should use programmatic assertions whenever assumptions are made:

- Use the ``QIREE_EXPECT(x)`` assertion macro to test preconditions about
  incoming data or initial internal states.
- Use ``QIREE_ASSERT(x)`` to express an assumption internal to a function (e.g.,
  "this index is not out of range of the array").
- Use ``QIREE_ENSURE(x)`` to mark expectations about data being returned from a
  function and side effects resulting from the function.

These assertions are enabled only when the ``CELERITAS_DEBUG`` CMake option is
set.

Additionally, user-provided data and potentially volatile runtime conditions
(such as the presence of an environment variable) should be checked with
the always-on assertion ``QIREE_VALIDATE(x, << "streamable message")`` macro. See
:ref:`api_qiree` for more details about these macros.

Each class must be thoroughly tested with an independent unit test in the
`test` directory.  For complete coverage, each function of the class must have
at least as many tests as the number of possible code flow paths (cyclomatic
complexity).

Implementation detail classes (in the ``qiree::detail`` namespace, in
``detail/`` subdirectories) are exempt from the testing requirement, but
testing the detail classes is a good way to simplify edge case testing compared
to testing the higher-level code.


Maximize encapsulation
----------------------

Encapsulation is about making a piece of code into a black box. The fewer lines
connecting these black boxes, the more maintainable the code. Black boxes can
often be improved internally by making tiny black boxes inside the larger black
box.

Motivation:

- Developers don't have to understand implementation details when looking at a
  class interface.
- Compilers can optimize better when dealing with more localized components.
- Good encapsulation allows components to be interchanged easily because they
  have well-defined interfaces.
- Pausing to think about how to minimize input and output from an algorithm can
  improve it *and* make it easier to write.

Applications:

- Refactor large functions (> 50-ish statements?) into small functors that take
  "invariant" values (the larger context) for constructors and use
  ``operator()`` to transform some input into the desired output
- Use only ``const`` data when sharing. Non-const shared data is almost like
  using global variables.
- Use ``OpaqueId`` instead of integers and magic sentinel values for
  integer identifiers that aren't supposed to be arithmetical.

Examples:

- Random number sampling: write a unit sphere sampling functor instead of
  replicating a polar-to-Cartesian transform in a thousand places.
- Volume IDs: Opaque IDs add type safety so that you can't accidentally convert
  a volume identifier into a double or switch a volume and material ID. It also
  makes code more readable of course.

Encapsulation is also useful for code reuse. Always avoid copy-pasting code, as
it means potentially duplicating bugs, duplicating the amount of work needed
when refactoring, and missing optimizations.


Minimize compile time
---------------------

Code performance is important but so is developer time. When possible,
minimize the amount of code touched by NVCC. (NVCC's error output is also
rudimentary compared to modern clang/GCC, so that's another reason to prefer
them compiling your code.)


Prefer single-state classes
---------------------------

As much as possible, make classes "complete" and valid after calling the
constructor. Try to avoid "finalize" functions that have to be called in a
specific order to put the class in a workable state. If a finalize function
*is* used, implement assertions to detect and warn the developer if the
required order is not respected.

When a class has a single function (especially if you name that function
``operator()``), its usage is obvious. The reader also doesn't have to know
whether a class uses ``doIt`` or ``do_it`` or ``build``.

When you have a class that needs a lot of data to start in a valid state, use a
``struct`` of intuitive objects to pass the data to the class's constructor.
The constructor can do any necessary validation on the input data.


Learn from the pros
-------------------

Other entities devoted to sustainable programming have their own guidelines.
The `ISO C++ guidelines`_ are very long but offer a number of insightful
suggestions about C++ programming. The `Google style guide`_ is a little more
targeted toward legacy code and large production environments, but it still
offers good suggestions. For software engineering best practices, the
book `Software Engineering at Google`_ is an excellent reference.

.. _ISO C++ guidelines: http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
.. _Google style guide: https://google.github.io/styleguide/cppguide.html
.. _Software Engineering at Google: https://abseil.io/resources/swe-book

.. _style_guidelines:

Style guidelines
================

Having a consistent code style makes it more readable and maintainable. (For
example, you don't have to guess whether a symbol is a function or class.)

As a historical note, the style conventions in QIR-EE should be identical those
in `Celeritas`_, which are derived from
the `Draco project style`_ of which `Tom Evans`_ was primary author and which
became the style standard for the GPU-enabled Monte Carlo code `Shift`_.

.. _Celeritas: https://github.com/celeritas-project/celeritas
.. _Draco project style: https://github.com/lanl/Draco/wiki/Style-Guide
.. _Tom Evans: https://github.com/tmdelellis
.. _Shift: https://doi.org/10.1016/j.anucene.2019.01.012


.. _formatting:

Formatting
----------

Formatting is determined by the clang-format file inside the top-level
directory. One key restriction is the 80-column limit, which enables multiple
code windows to be open side-by-side. Generally, statements longer than 80
columns should be broken into sub-expressions for improved readability anyway
-- the ``auto`` keyword can help a lot with this. The post-commit formatting
hook in :file:`scripts/dev` (execute
:file:`scripts/dev/install-commit-hooks.sh` to set up this script) can take
care of clang formatting automatically. The clang-format script will also
enforce the use of "`East const`_", where the ``const`` keyword is always to
the right of the type that it modifies.

Certain decorations (separators, Doxygen comment structure,
etc.) are standard throughout the code. Use the :file:`qiree-gen.py` script
(in the :file:`scripts/dev` directory) to generate skeletons for new files, and
use existing source code as a guide to how to structure the decorations.
Doxygen comments should be provided next to the *definition* of functions (both
member and free) and classes.

.. _East const: https://hackingcpp.com/cpp/design/east_vs_west_const.html

Symbol names
------------

Functions should be verbs; classes should be names. As in standard Python
(PEP-8-compliant) code, classes should use ``CapWordsStyle`` and variables use
``snake_case_style``. A symbol should have a trailing underscore *always and
only* if it is private member data: neither public member data nor private
member functions should have them.

Functors (classes whose instances act like a function) should be an *agent
noun*: the noun form of an action verb. Instances of a functor should be a
verb. For example::

   ModelEvaluator evaluate_something(parameters...);
   auto result = evaluate_something(arguments...);

There are many opportunities to use ``OpaqueId`` in GPU code to indicate
indexing into particular vectors. To maintain consistency, we let an
index into a vector of ``Foo`` objects have a corresponding ``OpaqueId``
type::

    using FooId = OpaqueId<Foo>;

and ideally be defined either immediately after ``Foo`` or in a
:file:`Types.hh` file.  Some ``OpaqueId`` types may have only a "symbolic"
corresponding type, in which case a tag struct can be be defined inline, using
an underscore suffix as a convention indicating the type does not correspond to
an actual class::

   using BarId = OpaqueId<struct Bar_>;

.. note:: Public functions in user-facing Geant4 classes (those in ``accel``)
   should try to conform to Geant4-style naming conventions, especially because
   many will derive from Geant4 class interfaces.


File names
----------

We choose the convention of ``.cc`` for C++ translation units and
corresponding ``.hh`` files for C++ headers.

Some files have special extensions:

- ``.t.hh`` is for non-inlined ``template`` implementations that can be
  included and instantiated elsewhere. However, if the function
  declaration in the ``.hh`` file is declared ``inline``, the definition
  should be provided there as well.
- ``.test.cc`` are unit test executables corresponding to the main ``.cc``
  file. These should only be in the main ``/test`` directory.


Variable names
--------------

Generally speaking, variables should have short lifetimes and should be
self-documenting. Avoid shorthand and "transliterated" mathematical
expressions: prefer ``constants::na_avogadro`` to ``N_A`` (or express the
constant functionally with ``atoms_per_mole``) and use ``atomic_number``
instead of ``Z``. Physical constants should try to have the symbol concatenated
to the context or meaning (e.g. `c_light` or `h_planck`).

Use scoped enumerations (``enum class``) where possible (named like classes) so
their values can safely be named like member variables (lowercase with
underscores). Prefer enumerations to boolean values in function interfaces
(since ``do_something(true)`` requires looking up the function interface
definition to understand).


Function arguments and return values
------------------------------------

- Always pass value types for arguments when the data is small (``sizeof(arg)
  <= sizeof(void*)``). Using values instead of pointers/references allows the
  compiler to optimize better. If the argument is nontrivial but you need to
  make a local copy anyway, it's OK to make the function argument a value (and
  use ``std::move`` internally as needed, but this is a more complicated
  topic).
- In general, avoid ``const`` values (e.g. ``const int``), because the decision
  to modify a local variable or not is an implementation detail of the
  function, not part of its interface.
- Use const *references* for types that are nontrivial and that you only need
  to access or pass to other const-reference functions.
- Prefer return values or structs rather than mutable function arguments. This
  makes it clear that there are no preconditions on the input value's state.
- In QIR-EE we use the google style of passing mutable pointers instead of
  mutable references, so that it's more obvious to the calling code that a
  value is going to be modified. Add ``QIREE_EXPECT(input);`` to make it clear
  that the pointer needs to be valid, and add any other preconditions.
- Host-only (e.g., runtime setup) code should almost never return raw pointers;
  use shared pointers instead to make the ownership semantics clear. When
  interfacing with older libraries such as Geant4, try to use ``unique_ptr``
  and its ``release``/``get`` semantics to indicate the transfer of pointer
  ownership.
- Since we don't yet support C++17's ``string_view`` it's OK to use ``const
  char*`` to indicate a read-only string.

Odds and ends
-------------

Although ``struct`` and ``class`` are interchangeable for class definitions
(modifying only the default visibility as public or private), use ``struct``
for data-oriented classes that don't declare constructors and have only
public data; and use ``class`` for classes designed to encapsulate
functionality and/or data.

With template parameters, ``typename T`` and ``class T`` are also
interchangeable, but use ``template <class T>`` to be consistent internally and
with the standard library. (It's also possible to have ``template <typename``
where ``typename`` *doesn't* mean a class: namely,
``template <typename U::value_type Value>``.)

Use ``this->`` when calling member functions inside a class to convey that the
``this`` pointer is implicitly being passed to the function and to make it
easier to differentiate from a free function in the current scope.

