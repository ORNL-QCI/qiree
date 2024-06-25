# Quantum Intermediate Representation Execution Engine (QIR-EE)

## Table of Contents

1. [Introduction](#introduction)
2. [Getting Started](#getting-started)
3. [Executing Quantum Circuits](#executing-quantum-circuits)
4. [Understanding the Results](#understanding-the-results)
5. [Adding Custom Operations](#adding-custom-operations)
6. [FAQs and Troubleshooting](#faqs-and-troubleshooting)
7. [Getting Help](#getting-help)

## Introduction

Welcome to the Quantum Intermediate Representation Execution Engine (QIR-EE), a state-of-the-art tool designed to streamline the process of running quantum circuits and algorithms. Whether you're a researcher, student, or enthusiast, QIR-EE (pronounced 'cure-ee') is designed to make your journey into quantum computing as seamless as possible. This implementation is associated to the paper [A Cross-Platform Execution Engine for the Quantum Intermediate Representation](https://doi.org/10.48550/arXiv.2404.14299) and is maintained by the QIR-EE Developers.

At version 0.1, this work represents a proof-of-concept for the feasibility of a modular workflow at the lower end of the quantum software stack. We welcome feedback and ideas for collaborations.

## Getting Started

There are two dependencies for QIR-EE to work properly. Please make sure to download and install the most current versions of:
1. [LLVM](https://releases.llvm.org/) (minimum version 16.0.0)
2. [XACC](https://xacc.readthedocs.io/en/latest/install.html) or an equivalent runtime system/hardware accelerator

### System Requirements

- Access to a compatible quantum computing simulator or real quantum hardware.
- Configuration files set up in your home folder that contain credential to access vendor backends. (Under construction: Formats for .ionq_config and .honeywell_config.)
- A QIR file with the quantum program that you want to run (see examples folder).

### Installation

QIR-EE Setup in Command Line

1. Clone this repo. Enter the repo.
2. `mkdir build; cd build`
3. `cmake ..`
4. `make`

The resulting executable is `./[YOUR-DIR]/qir-xir/build/QuantumExecutionEngine`.

## Executing Quantum Circuits
(via QIR-EE and XACC)

1. Check that your cmake prefixes for XACC are correct.
2. Typing `echo $CMAKE_PREFIX_PATH` should give you the path to your XACC installation.
3. If empty, then add it: `export CMAKE_PREFIX_PATH=$HOME/.xacc` or an equivalent path to your XACC installation.
4. Check your `$PYTHONPATH` for pointing to your XACC installation.
5. If empty, then add it: `export PYTHONPATH=$PYTHONPATH:$HOME/.xacc` or an equivalent path to your XACC installation.

### Syntax for executing QIR via XACC:

```
./QuantumExecutionEngine --flag-name flag-value
```

1. `./QuantumExecutionEngine` may be replaced with an equivalent path to the executable.
2. `--llvm-file-path` is used to indicate path of the LLVM (`*.ll`) file that specifices the quantum program (required).
3. `--n-qubits` is used to indicate the number of qubits needed to construct the circuit (required and must come after the file path).
4. `--shots` is the number of times you would like to execute the circuit (optional, with default at 1024). 
5. `--accelerator` is the name of the quantum accelerator (hardware or simulator) that you wish to use (optional, with default `aer`).

   With XACC we have tested: `aer`, `qpp`, `qsim`, `honeywell:H1-1SC`, `honeywell:H1-1E`, `honeywell:H1-1`, `ionq`.

### Example:

```
./QuantumExecutionEngine --llvm-file-path $HOME/qir-xir/examples/bell.ll --n-qubits 2 --shots 2048 --accelerator qsim
```

This command will execute the quantum Bell circuit described in `bell.ll` 2048 times using the "qsim" accelerator.

## Understanding the Results

After execution, you will receive a summary of the quantum circuit's results. This may include the xacc buffer output. Example outputs from experiments can be found here: [qiree-data](https://github.com/wongey/qiree-data).

## Adding Custom Operations

Advanced users can extend the engine's capabilities by introducing new operations or functionalities. This is done through the `addMapping` function available in the `GlobalMappingAssociator` class. However, this requires a deep understanding of the QEE's internal workings and quantum operations.

## FAQs and Troubleshooting

Under construction.

## Getting Help
