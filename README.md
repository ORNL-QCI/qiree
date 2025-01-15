# QIR-EE

The QIR Execution Engine library provides interfaces for easily processing
[Quantum Intermediate
Representation](https://github.com/qir-alliance/qir-spec/) code using the LLVM
execution engine.

## Introduction

Welcome to the Quantum Intermediate Representation Execution Engine (QIR-EE), a
state-of-the-art tool designed to streamline the process of running quantum
circuits and algorithms. Whether you're a researcher, student, or enthusiast,
QIR-EE (pronounced 'cure-ee') is designed to make your journey into quantum
computing as seamless as possible. This implementation is associated to the
paper [A Cross-Platform Execution Engine for the Quantum Intermediate
Representation](https://doi.org/10.48550/arXiv.2404.14299) and is maintained by
the QIR-EE Developers.

At version 0.1, this work represents a proof-of-concept for the feasibility of
a modular workflow at the lower end of the quantum software stack. We welcome
feedback and ideas for collaborations.

## Documentation

Most of the QIR-EE documentation is readable through the codebase through a
combination of [static RST documentation](doc/index.rst) and Doxygen-markup
comments in the source code itself. The full [QIR-EE user
documentation][user-docs] (including selected code documentation incorporated
by Breathe) and the [QIR-EE code documentation][dev-docs] are (TODO: will
be) mirrored on
our GitHub pages site. You can generate these yourself (if the necessary
prerequisites are installed) by
setting the `QIREE_BUILD_DOCS=ON` configuration option and running `make
doc` (user) or `make doxygen` (developer).

[user-docs]: https://ornl-qci.github.io/qir-ee/user/index.html
[dev-docs]: https://ornl-qci.github.io/qir-ee/dev/index.html

## Getting Started

There are two dependencies for QIR-EE to work properly. Please make sure to
download and install the most current versions of:
1. [LLVM](https://releases.llvm.org/) (we have tested versions 14 through 18)
2. [XACC](https://github.com/ORNL-QCI/xacc) (repo that is actively 
   maintained -- not the eclipse one; currently required for execution in 
   this version of qir-ee; we recommend setting option `-DQIREE_MINIMAL_BUILD=ON`
   during cmake for a faster build)

### System Requirements

- Access to a compatible quantum computing simulator or real quantum hardware.
- Configuration files set up in your home folder that contain credential to
  access vendor backends. (Under construction: Formats for .ionq_config and
  .honeywell_config.)
- A QIR file with the quantum program that you want to run (see examples folder).

### Installation

QIR-EE Setup in Command Line

1. Clone this repo. Enter the repo.
2. `mkdir build; cd build`
3. `cmake ..`
4. `make`

The resulting path to executable files can be exported 

`export PATH=${YOUR-QIREE-INSTALL-DIR}/bin:$PATH`

which would allow you to access the QIR-EE from anywhere in command line.

## Executing Quantum Circuits
(via QIR-EE and XACC)

1. Check that your cmake prefixes for XACC are correct.
2. Typing `echo $CMAKE_PREFIX_PATH` should give you the path to your XACC
   installation.
3. If empty, then add it: `export CMAKE_PREFIX_PATH=$HOME/.xacc` or an
   equivalent path to your XACC installation.
4. Check your `$PYTHONPATH` for pointing to your XACC installation.
5. If empty, then add it: `export PYTHONPATH=$PYTHONPATH:$HOME/.xacc` or an
   equivalent path to your XACC installation.

### Syntax for executing QIR via XACC:

For basic usage:

```
qir-xacc llvm-file-path --flag-name flag-value
```
1. `qir-xacc` may be replaced with an equivalent executable.
2. `llvm-file-path` is used to indicate path of the LLVM (`*.ll`) file that
   specifies the quantum program (required).
3. `-a` or `--accelerator` is the name of the quantum accelerator (hardware or
   simulator) that you wish to use (required).
4. `-s` or `--shots` is the number of shots (optional with default at 1024).

   With XACC we have tested: `aer`, `qpp`, `qsim`, `quantinuum:H1-1SC`, `quantinuum:H1-1E`, 
   `quantinuum:H1-1`, `ionq`, `ionq:sim.harmony`, `ionq:sim.aria-1`, `ionq:qpu.harmony`.

   Note that the minimal build only includes `aer` and `qpp` for local simulators.

Please refer to the documentation here (add link) for all flag options.

### Example:

```
qir-xacc $HOME/qiree/examples/bell.ll --accelerator qpp
```

This command will execute the quantum Bell circuit described in `bell.ll` the
(default) 1024 times using the "qpp" accelerator.

## Understanding the Results

After execution, you will receive a summary of the quantum circuit's results.
This may include the xacc buffer output. Example outputs from experiments can
be found here: [qiree-data](https://github.com/wongey/qiree-data).

## Adding Custom Operations

Advanced users can extend the engine's capabilities by introducing new
operations or functionalities. Please refer to the developer documetation (link
to be added) for further information.

## Directory structure

| **Directory** | **Description**                                       |
| ------------- | ---------------                                       |
| **app**       | Source code for installed executable applications     |
| **cmake**     | Implementation code for CMake build configuration     |
| **doc**       | Code documentation and manual                         |
| **scripts**   | Development and continuous integration helper scripts |
| **src**       | Library source code                                   |
| **test**      | Unit tests                                            |

## Citation

This code was was made public on github on June 24, 2024 and relased to [DOE CODE](https://www.osti.gov/doecode/biblio/149694) on January 15, 2025.
```
@misc{doecode_149694,
title = {QIR-EE},
author = {Wong, Elaine and Leyton Ortega, Vicente and Johnson, Seth R. and Claudino, Daniel and Adams, Austin and Afrose, Sharmin and Gowrishankar, Meenambika and Humble, Travis S.},
abstractNote = {The QIR Execution Engine library provides interfaces for easily processing Quantum Intermediate Representation code using the LLVM execution engine.},
doi = {10.11578/qiree/dc.20250114.1},
url = {https://doi.org/10.11578/qiree/dc.20250114.1},
howpublished = {[Computer Software] \url{https://doi.org/10.11578/qiree/dc.20250114.1}},
year = {2025},
month = {jan}
}
```
