#!/usr/bin/env python3
# Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
"""\
Generate file stubs for QIR-EE.
"""

from datetime import datetime
import os
import os.path
import re
import subprocess
import stat
import sys

###############################################################################

CXX_TOP = '''\
//{modeline:-^75s}//
// Copyright {year} UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \\file {dirname}{basename}
//---------------------------------------------------------------------------//
'''

HEADER_FILE = '''\
#pragma once

{namespace_begin}
//---------------------------------------------------------------------------//
/*!
 * Brief class description.
 *
 * Optional detailed class description, and possibly example usage:
 * \\code
    {name} ...;
   \\endcode
 */
class {name}
{{
  public:
    //!@{{
    //! \\name Type aliases
    <++>
    //!@}}

  public:
    // Construct with defaults
    inline {name}();
}};

//---------------------------------------------------------------------------//
// INLINE DEFINITIONS
//---------------------------------------------------------------------------//
/*!
 * Construct with defaults.
 */
{name}::{name}()
{{
}}

//---------------------------------------------------------------------------//
{namespace_end}
'''

CODE_FILE = '''\
#include "{name}.{hext}"

{namespace_begin}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
{namespace_end}
'''

TEST_HARNESS_FILE = '''\
#include "qiree/{dirname}{name}.{hext}"

#include "qiree_test.hh"
// #include "{name}.test.hh"

{namespace_begin}
//---------------------------------------------------------------------------//

class {name}Test : public ::qiree::test::Test
{{
  protected:
    void SetUp() override {{}}
}};

TEST_F({name}Test, all)
{{
    // PRINT_EXPECTED(result.foo);
    // EXPECT_VEC_SOFT_EQ(expected_foo, result.foo);
}}

//---------------------------------------------------------------------------//
{namespace_end}
'''

SWIG_FILE = '''\
%{{
#include "{name}.{hext}"
%}}

%include "{name}.{hext}"
'''


CMAKE_TOP = '''\
#{modeline:-^77s}#
# Copyright {year} UT-Battelle, LLC, and otrher QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
'''

CMAKELISTS_FILE = '''\
#-----------------------------------------------------------------------------#


#-----------------------------------------------------------------------------#
'''


CMAKE_FILE = '''\
#[=======================================================================[.rst:

{name}
-------------------

Description of overall module contents goes here.

.. command:: my_command_name

  Pass the given compiler-dependent warning flags to a library target::

    my_command_name(<target>
                    <INTERFACE|PUBLIC|PRIVATE>
                    LANGUAGE <lang> [<lang>...]
                    [CACHE_VARIABLE <name>])

  ``target``
    Name of the library target.

  ``scope``
    One of ``INTERFACE``, ``PUBLIC``, or ``PRIVATE``. ...

#]=======================================================================]

function(my_command_name)
endfunction()

#-----------------------------------------------------------------------------#
'''

PYTHON_TOP = '''\
#!/usr/bin/env python3
# Copyright {year} UT-Battelle, LLC, and otrher QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
'''

PYTHON_FILE = '''\
"""
"""

'''

SHELL_TOP = '''\
#!/bin/sh -ex
# Copyright {year} UT-Battelle, LLC, and otrher QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
'''

SHELL_FILE = '''\

'''

RST_TOP = '''\
.. Copyright {year} UT-Battelle, LLC, and otrher QIR-EE developers.
.. See the doc/COPYRIGHT file for details.
.. SPDX-License-Identifier: CC-BY-4.0
'''

RST_FILE = '''
.. _{name}:

****************
{name}
****************

Text with a link to `Sphinx primer`_ and `RST`_ docs.

.. _Sphinx primer : https://www.sphinx-doc.org/en/master/usage/restructuredtext/basics.html
.. _RST : https://docutils.sourceforge.io/docs/user/rst/quickref.html

Subsection
==========

Another paragraph.

.. note:: Don't start a subsection immediately after a section: make sure
   there's something to say at the start of each one.

Subsubsection
-------------

These are useful for heavily nested documentation such as API descriptions. ::

    // This code block will be highlighted in the default language, which for
    // qiree is C++.
    int i = 0;
'''

YEAR = datetime.today().year

TEMPLATES = {
    'hh': HEADER_FILE,
    'cc': CODE_FILE,
    'test.cc': TEST_HARNESS_FILE,
    'i': SWIG_FILE,
    'cmake': CMAKE_FILE,
    'CMakeLists.txt': CMAKELISTS_FILE,
    'py': PYTHON_FILE,
    'sh': SHELL_FILE,
    'rst': RST_FILE,
}

LANG = {
    'h': "C",
    'hh': "C++",
    'cc': "C++",
    'cmake': "CMake",
    'i': "SWIG",
    'CMakeLists.txt': "CMake",
    'py': "Python",
    'sh': "Shell",
    'omn': "Omnibus",
    'rst': "RST",
}

TOPS = {
    'C': CXX_TOP,
    'C++': CXX_TOP,
    'SWIG': CXX_TOP,
    'CMake': CMAKE_TOP,
    'Python': PYTHON_TOP,
    'Shell': SHELL_TOP,
    'RST': RST_TOP,
}

HEXT = {
    'C': "h",
    'C++': "hh",
    'SWIG': "hh",
}


def generate(repodir, filename, namespace):
    if os.path.exists(filename):
        print("Skipping existing file " + filename)
        return

    dirname = os.path.relpath(filename, start=repodir)
    all_dirs = dirname.split(os.sep)[:-1]

    if namespace is None:
        namespace = 'qiree'
        if all_dirs[0] == 'app':
            namespace += '::app'
        elif all_dirs[0] == 'test':
            namespace += '::test'
        if all_dirs[-1] == 'detail':
            namespace += '::detail'

    # Construct directory name with src/app/test dropped
    dirname = os.sep.join(all_dirs[1:])
    if dirname:
        dirname += os.sep

    basename = os.path.basename(filename)
    (name, _, longext) = basename.partition('.')

    lang = None
    template = None
    ext = longext.split('.')[-1]
    for check_lang in [basename, longext, ext]:
        if not lang:
            lang = LANG.get(check_lang, None)
        if not template:
            template = TEMPLATES.get(check_lang, None)
    if not lang:
        print(f"No known language for '.{ext}' files")
    if not template:
        print(f"No configured template for '.{ext}' files")
    if not lang or not template:
        sys.exit(1)

    top = TOPS[lang]
    nsbeg = []
    nsend = []
    for subns in namespace.split('::'):
        nsbeg.append(f'namespace {subns}\n{{')
        nsend.append(f'}}  // namespace {subns}')

    capabbr = re.sub(r'[^A-Z]+', '', name)
    variables = {
        'longext': longext,
        'ext': ext,
        'hext': "hh",
        'modeline': "-*-{}-*-".format(lang),
        'name': name,
        'namespace': namespace,
        'namespace_begin': "\n".join(nsbeg),
        'namespace_end': "\n".join(reversed(nsend)),
        'basename': basename,
        'dirname': dirname,
        'capabbr': capabbr,
        'lowabbr': capabbr.lower(),
        'year': YEAR,
        'qiree_ns': "",
    }
    with open(filename, 'w') as f:
        f.write((top + template).format(**variables))
        if top.startswith('#!'):
            # Set executable bits
            mode = os.fstat(f.fileno()).st_mode
            mode |= 0o111
            os.fchmod(f.fileno(), stat.S_IMODE(mode))
    return filename


def main():
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        'filename', nargs='+',
        help='file names to generate')
    parser.add_argument(
        '--repodir',
        help='root source directory for file naming')
    parser.add_argument('-o', '--open',
        action='store_true',
        help='call "open" on the created files')
    parser.add_argument(
        '--namespace', '-n',
        default=None,
        help='C++ namespace to generate')
    args = parser.parse_args()
    repodir = args.repodir or (
        subprocess.check_output(['git', 'rev-parse', '--show-toplevel'])
        .decode().strip()
    )
    generated = []
    for fn in args.filename:
        fn = generate(repodir, fn, args.namespace)
        if fn:
            generated.append(fn)

    if args.open and generated:
        subprocess.call(["open"] + generated)


if __name__ == '__main__':
    main()
