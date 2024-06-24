# User Documentation: Quantum Execution Engine (QEE)

## Table of Contents

1. [Introduction](#introduction)
2. [Getting Started](#getting-started)
3. [Executing Quantum Circuits](#executing-quantum-circuits)
4. [Understanding the Results](#understanding-the-results)
5. [Advanced Features](#advanced-features)
6. [Troubleshooting](#troubleshooting)
7. [FAQs](#faqs)
8. [Getting Help](#getting-help)

## Introduction

Welcome to the Quantum Execution Engine (QEE), a state-of-the-art tool designed to streamline the process of running quantum circuits and algorithms. Whether you're a researcher, student, or enthusiast, the QEE is designed to make your journey into quantum computing as seamless as possible.

## Getting Started
hmmmm, where to find the info about xacc... 
### System Requirements

- A compatible quantum computing simulator or real quantum hardware access, aer, qpp, qsim....etc, the api to get access to IBM, Quantinumm, etc, how to configure that api-key in xacc 
- An LLVM file with the defined quantum circuit, how to get that, for instance using pyqir

### Installation

Provide clear, step-by-step instructions on how to install the Quantum Execution Engine, including where to download it and any additional configuration steps.
1. create a build folder and go to that folder
2. in that folder execute $cmake .. 
3. $make 
4. this will create a execution file called qir_execute

## Executing Quantum Circuits

### Basic Execution

To execute a quantum circuit, you will need to use the following command-line syntax:

```
./qir_execute <llvm_file_path> <shots> <acceleratorName>
```

- `<llvm_file_path>`: The path to your LLVM file that specifies the quantum circuit (required).
- `<shots>`: The number of times to execute the circuit. Defaults to 1024 if not specified (optional).
- `<acceleratorName>`: The name of the quantum accelerator (hardware or simulator) you wish to use. Defaults to "aer" if not specified (optional).

### Example:

```
./qir_execute /path/to/your/file.ll 2048 qsim
```

This command will execute the quantum circuit described in `file.ll` 2048 times using the "qsim" accelerator.

## Understanding the Results

After execution, the QEE will provide a summary of the quantum circuit's results. This may include the xacc buffer output.

## Advanced Features

### Adding Custom Operations

Advanced users can extend the engine's capabilities by introducing new operations or functionalities. This is done through the `addMapping` function available in the `GlobalMappingAssociator` class. However, this requires a deep understanding of the QEE's internal workings and quantum operations.

## Troubleshooting

This section includes guidance on resolving common issues users might encounter when using the QEE, such as installation problems, execution errors, and unexpected results.

## FAQs

A list of Frequently Asked Questions to assist users in quickly resolving common queries and issues without having to deep-dive into the more technical sections of the documentation.

## Getting Help

Information on how users can get further help, including contact information, and links to additional resources.
