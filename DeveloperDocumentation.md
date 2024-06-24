# Developer Documentation: Quantum Execution Engine (QEE)

## Table of Contents

1. [Introduction](#introduction)
2. [System Architecture](#system-architecture)
3. [Core Components](#core-components)
   - [QuantumExecutionEngine](#quantumexecutionengine)
   - [GlobalMappingAssociator](#globalmappingassociator)
   - [Singleton Utility](#singleton-utility)
4. [Extending the Engine](#extending-the-engine)
5. [Code Examples](#code-examples)
6. [Testing](#testing)
7. [Troubleshooting](#troubleshooting)
8. [Future Development](#future-development)
9. [Contributing](#contributing)

## Introduction

The Quantum Execution Engine (QEE) is an advanced computational tool designed to bridge the gap between classical and quantum computing. By leveraging the Quantum Intermediate Representation (QIR), it facilitates the execution of quantum programs across various platforms and hardware, handling an extensive set of quantum instructions and managing quantum results. This documentation provides an in-depth look at the QEE's internal workings, designed to assist developers in understanding, utilizing, and extending the engine.

## System Architecture

The QEE is built with modularity and extensibility in mind, consisting of several key components that handle different aspects of quantum computation:

- **QuantumExecutionEngine**: The main driver that interfaces with quantum hardware or simulators, managing the execution lifecycle of quantum circuits.
- **GlobalMappingAssociator**: Responsible for maintaining global mappings critical for QIR's translation and execution.
- **Singleton Utility**: A template class ensuring components like the GlobalMappingAssociator maintain a single instance throughout the application's runtime.

## Core Components

### QuantumExecutionEngine

Located in `QuantumExecutionEngine.cpp`, this is the central component responsible for executing quantum instructions and retrieving results. It takes an LLVM file path as input, containing the quantum circuit details, and supports additional parameters like the number of shots and the accelerator name.

#### Key Methods:

- `execute()`: Initiates the execution of the quantum circuit.
- `getResults()`: Retrieves the results after the circuit execution is complete.

### GlobalMappingAssociator

Defined in `GlobalMappingAssociator.cpp` and `.hpp`, this component uses the singleton design pattern to manage global mappings between quantum operations, results, and their respective global identifiers.

#### Key Methods:

- `getInstance()`: Retrieves the singleton instance of the GlobalMappingAssociator.
- `addMapping(std::string, void*)`: Registers a new global mapping into the system.

### Singleton Utility

The `Singleton.hpp` file defines a utility template class for creating singleton instances, used by components like GlobalMappingAssociator.

## Extending the Engine

Developers can introduce new functionalities by utilizing the `addMapping` method in the `GlobalMappingAssociator`, allowing the integration of custom quantum operations or results.

## Code Examples

```cpp
// Example of adding a new quantum operation
GlobalMappingAssociator& gma = GlobalMappingAssociator::getInstance();
gma.addMapping("customOperation", reinterpret_cast<void*>(/*Function or data pointer*/));
```

## Testing

We should write comprehensive unit tests for new code, and update existing tests to reflect any changes. 

## Troubleshooting

This section should provide common issues and solutions or debugging methods to resolve potential problems developers might encounter.

## Future Development

Discuss potential areas for expansion or improvement within the QEE, such as support for more quantum instructions, improved error handling, or enhanced performance optimization.

## Contributing

Guidelines on how developers can contribute to the project, including coding standards, pull request process, and ethical code of conduct.
