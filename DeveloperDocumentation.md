# Developer Documentation for QIR-EE

## Table of Contents

1. [Introduction](#introduction)
2. [System Architecture](#system-architecture)
3. [Core Components](#core-components)
   - [QIREE Class](#1qireeclass)
   - [QuantumExecutionEngine](#2quantumexecutionengine)
   - [GlobalMappingAssociator](#3globalmappingassociator)
   - [ExecutionBackendHandle](#4executionbackendhandle)
5. [Extending the Engine and Establishing an Instance](#extending-the-engine-and-establishing-an-instance)
6. [Code Examples](#code-examples)
7. [Testing](#testing)
8. [Troubleshooting](#troubleshooting)
9. [Future Development](#future-development)

## Introduction

The QIR Execution Engine (QIR-EE) is an advanced computational tool designed to bridge the gap between classical and quantum computing. By leveraging the Quantum Intermediate Representation (QIR), it facilitates the execution of quantum programs across various platforms and hardware, handling an extensive set of quantum instructions and managing quantum results. Furthermore, its modular design enables customized hardware accelerators to plug-in and create a new instance of our tool. This documentation provides an in-depth look at QIR-EE's internal workings, designed to assist developers in understanding, utilizing, and extending the engine.

## System Architecture

Built with modularity and extensibility in mind, this tool consists of several key components that handle different aspects of quantum computation:

- **QIREE-Class**: A class that activates the LLVM context and contain the main methods for the engine.
- **QuantumExecutionEngine**: The main driver that interfaces with quantum hardware or simulators, managing the execution lifecycle of quantum circuits.
- **GlobalMappingAssociator**: Responsible for maintaining global mappings critical for QIR's translation and execution.
- **ExecutionBackendHandle**: A template class ensuring components like the `GlobalMappingAssociator` to maintain a single instance throughout the application's runtime.

## Core Components

### 1&nbsp;&#8209;&nbsp;QIREE&nbsp;Class

Located in `QIREE.hpp`, this class puts into effect the LLVM execution engine and consists of the main components responsible for initializing the environment, parsing and binding quantum instruction set functions, and executing the QIR program. 

### 2&nbsp;&#8209;&nbsp;QuantumExecutionEngine

Located in `QuantumExecutionEngine.cpp`, this engine manages all user input, invokes an instance of the execution backend, and activates the LLVM context via the QIREE class for the purpose of execution. It takes an LLVM file path as input, which contains the deatils of the quantum circuit, and supports additional parameters like the number of shots and the accelerator name.

#### Key Methods:

- `execute()`: Initiates the execution of the quantum circuit.
- `getResults()`: Retrieves the results after the circuit execution is complete.

### 3&nbsp;&#8209;&nbsp;GlobalMappingAssociator

Defined in `GlobalMappingAssociator.cpp` and `.hpp`, this component uses the singleton design pattern to manage global mappings between quantum operations, results, and their respective global identifiers.

#### Key Methods:

- `addMapping(std::string, void*)`: Registers a new global mapping into the system.

### 4&nbsp;&#8209;&nbsp;ExecutionBackendHandle

The `ExecutionBackendHandle.hpp` file defines a utility template class for creating singleton instances, used by components like `GlobalMappingAssociator`.

## Extending the Engine and Establishing an Instance

Developers can introduce new functionalities by utilizing the `addMapping` method in `GlobalMappingAssociator`, allowing the integration of custom quantum operations or results.

Furthermore, developers of hardware accelerators may enable execution of QIR by adding their files to the `src` folder to create their own instance of QIR-EE. More specifically, **ExecutionBackendFactory** provides a template for new implementations to establish a QIR-EE instance with a user-defined accelerator (we provide QIR-EE with XACC as an example). New implementations need only to include the header with the implementation and adjust the definition of `createExecutionBackend()` in `ExecutionBackendFactory.cpp`.

## Code Examples

```cpp
// Example of adding a new quantum operation
GlobalMappingAssociator& gma = GlobalMappingAssociator::getInstance();
gma.addMapping("customOperation", reinterpret_cast<void*>(/*Function or data pointer*/));
```

## Testing

Under Developement (unit tests)

## Troubleshooting

Under Development

## Future Development

We welcome hardware accelerator developers to utilize our tool to work with QIR programs and enable execution of QIR. We hope to offer support for more quantum instructions, improved error handling, and enhanced performance optimization.
