#----------------------------------*-CMake-*----------------------------------#
# Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
# Derived from QIR-EE (v0.5.0-dev-21-gddb329e28)
#-----------------------------------------------------------------------------#

#-----------------------------------------------------------------------------#
# DEVELOPER MANUAL (Doxygen)
#-----------------------------------------------------------------------------#

# Use full version string instead of short version
set(DOXYGEN_PROJECT_NUMBER "${QIREE_VERSION_STRING}")

# File setup
set(DOXYGEN_EXCLUDE_PATTERNS
  "*/generated/*"
)
if(NOT QIREE_DEBUG)
  list(APPEND DOXYGEN_EXCLUDE_PATTERNS "*/detail/*")
endif()

set(DOXYGEN_FILE_PATTERNS
  "*.h"
  "*.hh"
  "*.cc"
  "*.cu"
  "*.md"
)
set(DOXYGEN_EXTENSION_MAPPING "cu=C++")
set(_DOXYGEN_EXTRA_SOURCE
  "${PROJECT_SOURCE_DIR}/README.md"
  "${PROJECT_BINARY_DIR}/include/qiree_config.h"
  "${PROJECT_BINARY_DIR}/include/qiree_version.h"
)
if((QIREE_DEBUG AND QIREE_BUILD_TESTS)
  OR QIREE_DOXYGEN_BUILD_TESTS)
  # Add documentation for QIR-EE test harnesses when debugging or for CI
  list(APPEND _DOXYGEN_TEST_SOURCE
    "${PROJECT_SOURCE_DIR}/test"
  )
  list(APPEND DOXYGEN_EXCLUDE_PATTERNS
    "*.test.cc" "*TestBase.cc"
  )
else()
  set(_DOXYGEN_TEST_SOURCE)
endif()

set(_doxy_version_hex 1) # TODO only if needed

# Documentation usage
set(DOXYGEN_FULL_PATH_NAMES NO)
set(DOXYGEN_MACRO_EXPANSION YES)
set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
set(DOXYGEN_QT_AUTOBRIEF YES)
set(DOXYGEN_DISTRIBUTE_GROUP_DOC YES)
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/README.md")
set(DOXYGEN_FORMULA_MACROFILE "${CMAKE_CURRENT_SOURCE_DIR}/_static/macros.tex")
set(DOXYGEN_EXTRA_PACKAGES "amsmath")
set(DOXYGEN_PREDEFINED "__DOXYGEN__=${_doxy_version_hex}")
set(DOXYGEN_USE_MATHJAX YES)

# Verbosity/warning levels
qiree_set_default(DOXYGEN_QUIET YES)
qiree_set_default(DOXYGEN_WARN_IF_UNDOCUMENTED NO)

# Output options
# set(DOXYGEN_PROJECT_LOGO
#   "${CMAKE_CURRENT_SOURCE_DIR}/_static/qiree-square.svg")
set(DOXYGEN_HTML_EXTRA_STYLESHEET
  "${CMAKE_CURRENT_SOURCE_DIR}/_static/doxygen-custom.css")
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_HTML_OUTPUT "doxygen-html")
set(DOXYGEN_CREATE_SUBDIRS YES)
set(DOXYGEN_REFERENCES_LINK_SOURCE YES)

qiree_set_default(DOXYGEN_VERBATIM_HEADERS NO)
qiree_set_default(DOXYGEN_MAX_DOT_GRAPH_DEPTH 2)

doxygen_add_docs(doxygen
  "${PROJECT_SOURCE_DIR}/src"
  ${_DOXYGEN_EXTRA_SOURCE}
  ${_DOXYGEN_TEST_SOURCE}
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
  COMMENT "Generating Doxygen to doc/doxygen-html/index.html"
)

#-----------------------------------------------------------------------------#
# USER MANUAL (Sphinx/Breathe)
#-----------------------------------------------------------------------------#

if(QIREE_PYTHONPATH_RERUN)
  message(SEND_ERROR "The last command must be manually rerun because "
    "Sphinx was not found.")
  unset(QIREE_PYTHONPATH_RERUN CACHE)
endif()

if(NOT Sphinx_FOUND)
  function(qiree_build_sphinx type output)
    add_custom_command(
      OUTPUT "${output}"
      VERBATIM COMMAND
        "${CMAKE_COMMAND}"
        -UQIREE_PYTHONPATH
        -UQIREE_CHECK_PYTHON_MODULE_sphinx
        -DQIREE_PYTHONPATH_RERUN:BOOL=ON
        "${CMAKE_BINARY_DIR}"
      COMMENT "Rerunning CMake because Sphinx was not found"
      ${ARGN}
    )
  endfunction()
else()
  function(qiree_build_sphinx type output)
    add_custom_command(
      OUTPUT "${output}"
      VERBATIM COMMAND
        "${CMAKE_COMMAND}" -E env
          "PYTHONPATH=${QIREE_PYTHONPATH}"
          "CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}"
        "$<TARGET_FILE:Python::Interpreter>" -m sphinx.cmd.build -q
          -d "${CMAKE_CURRENT_BINARY_DIR}/doctrees"
          -b ${type}
          "${CMAKE_CURRENT_SOURCE_DIR}"
          "${CMAKE_CURRENT_BINARY_DIR}/${type}"
      COMMENT "Building ${type} documentation with Sphinx"
      DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/conf.py"
      WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
      ${ARGN}
    )
  endfunction()
endif()

set(_ORNLTM_BASE "${CMAKE_CURRENT_BINARY_DIR}/latex/ornltm")
function(qiree_build_latex input output)
  set(_texenv
    "LATEXOPTS="
    "TEXINPUTS=${_ORNLTM_BASE}/ornltm:"
  )
  if(NOT LATEXMK_EXECUTABLE)
    set(LATEXMK_EXECUTABLE "latexmk")
  endif()
  add_custom_command(OUTPUT "${output}"
    VERBATIM COMMAND
      "${CMAKE_COMMAND}" -E env ${_texenv}
        "${LATEXMK_EXECUTABLE}" -pdf "${input}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/latex"
    ${ARGN}
  )
endfunction()

#-----------------------------------------------------------------------------#

# Define a configure file with version and configuration info
set(QIREE_USE_BREATHE ${QIREE_USE_Breathe})
set(QIREE_USE_FURO ${QIREE_USE_Furo})
set(QIREE_USE_SPHINXBIBTEX ${QIREE_USE_SphinxBibtex})
if(QIR-EE_VERSION STREQUAL QIR-EE_VERSION_STRING)
  set(QIR-EE_SHORT_VERSION "${QIR-EE_VERSION}")
elseif("${QIR-EE_VERSION_STRING}" MATCHES
    "^(.*-rc[.-][0-9]+)(\\.[0-9]+\\+[0-9a-f]+)$")
  # Use release candidate, stripping off stuff since the tag
  set(QIR-EE_SHORT_VERSION "${CMAKE_MATCH_1}")
else()
  # Development version
  set(_patch "${PROJECT_VERSION_PATCH}")
  if(NOT QIR-EE_VERSION_STRING MATCHES "-dev")
    # Before the next release
    math(EXPR _patch "${PROJECT_VERSION_PATCH} + 1")
  endif()
  set(QIR-EE_SHORT_VERSION
    "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${_patch}-dev"
  )
endif()
configure_file("config.json.in" "config.json" @ONLY)

set(DOXYGEN_GENERATE_HTML NO)
set(DOXYGEN_GENERATE_XML YES)
set(DOXYGEN_XML_PROGRAMLISTING NO)

# Annotations break breathe parsing
foreach(_var
  CELER_FUNCTION CELER_FORCEINLINE CELER_FORCEINLINE_FUNCTION CELER_CONSTEXPR_FUNCTION
  [[maybe_unused]] CFIF_)
  list(APPEND DOXYGEN_PREDEFINED "${_var}=")
endforeach()
# Only process top levels
file(GLOB _DOXYGEN_SOURCE
  "${PROJECT_SOURCE_DIR}/src/*/*.hh"
  "${PROJECT_SOURCE_DIR}/src/*/*.cc"
)
doxygen_add_docs(doxygen_xml
  ${_DOXYGEN_SOURCE}
  ${_DOXYGEN_EXTRA_SOURCE}
  USE_STAMP_FILE
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
  COMMENT "Generating Doxygen XML"
)

#-----------------------------------------------------------------------------#
# HTML

set(_doc_html "${CMAKE_CURRENT_BINARY_DIR}/html/index.html")
qiree_build_sphinx(html "${_doc_html}")
add_custom_target(doc DEPENDS "${_doc_html}")
if(QIREE_USE_BREATHE)
  add_dependencies(doc doxygen_xml)
endif()

install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html/"
  DESTINATION "${CMAKE_INSTALL_DOCDIR}"
  COMPONENT documentation
)

#-----------------------------------------------------------------------------#
# PDF

set(_doc_tex "${CMAKE_CURRENT_BINARY_DIR}/latex/QIREE.tex")
qiree_build_sphinx(latex "${_doc_tex}")

if(Sphinx_FOUND)
  # Download ornltm class
  ExternalProject_Add(ornltm
    GIT_REPOSITORY https://code.ornl.gov/sethrj/ornltm.git
    GIT_TAG c9d2ddcce2bfc8d7d51db8f4a131d71883f46528
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    SOURCE_DIR "${_ORNLTM_BASE}"
  )

  # Generate header
  set(_header_pdf "${CMAKE_CURRENT_BINARY_DIR}/latex/ornltm-header-qiree.pdf")
  qiree_build_latex(
    "${CMAKE_CURRENT_SOURCE_DIR}/_static/ornltm-header-qiree.tex"
    "${_header_pdf}"
    COMMENT "Building tech memo header"
    DEPENDS ornltm
  )

  # Generate header
  set(_footer_pdf "${CMAKE_CURRENT_BINARY_DIR}/latex/ornltm-footer.pdf")
  qiree_build_latex(
    "${CMAKE_CURRENT_SOURCE_DIR}/_static/ornltm-footer.tex"
    "${_footer_pdf}"
    COMMENT "Building tech memo footer"
    DEPENDS ornltm
  )

  # Generate PDF
  set(_doc_pdf "${CMAKE_CURRENT_BINARY_DIR}/latex/QIR-EE.pdf")
  qiree_build_latex(
    QIR-EE
    "${_doc_pdf}"
    COMMENT "Building PDF documentation from LaTeX"
    DEPENDS "${_doc_tex}" "${_header_pdf}" "${_footer_pdf}" ornltm
  )
else()
  # No sphinx available: don't confuse people by downloading PDF classes
  set(_doc_pdf "${_doc_tex}")
endif()

add_custom_target(doc_pdf DEPENDS "${_doc_pdf}")
if(QIREE_USE_BREATHE)
  add_dependencies(doc_pdf doxygen_xml)
endif()

#-----------------------------------------------------------------------------#
