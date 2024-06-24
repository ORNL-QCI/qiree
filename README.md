# qir-xir
Private repo for adapting QIR for use in XACC.

<strong>${\color{lightgreen}Abstract}$</strong>

The Quantum Intermediate Representation (QIR) provides an abstraction of programs that include a mixture of conventional and quantum instructions. This project develops a tool that translates a QIR program into a hardware-specific implementation using the XACC programming framework. This tool provides a unique capability to translating abstract QIR programs into executable programs on specific hardware. By mapping specific LLVM function calls to their corresponding C++ implementations, the code bridges the LLVM Execution Engine's classical execution environment with XACC's quantum computing capabilities. The integration allows for the smooth transition and combination of classical and quantum instructions, providing a unified platform for hybrid quantum-classical computing. The results from the quantum computations are captured in a globally accessible buffer, facilitating their use in classical post-processing or control logic.

<strong>${\color{lightgreen}Testing}$ ${\color{lightgreen} Instructions}$</strong>
<ol>
  <li> Install LLVM-16 and Clang-16 and XACC. Check your version and make sure they are the correct versions. Do it! </li>
  <li> Clone this repo. Enter the repo. </li>
  <li> <code>mkdir build; cd build</code></li>
  <li> <code>cmake -DTARGET_SOURCE=../src/QuantumExecutionEngine.cpp .. </code></li>
  <li> <code>make</code> </li>
  <li> <code>./qir_execute [sim choice] ../examples/[name of *.ll file] </code> where <code>[sim choice]</code> can be one of <code>aer, qpp, qsim</code> </li> 
  <li> Check out those results! </li>
</ol>

<strong>${\color{lightgreen}What}$ ${\color{lightgreen}is}$ ${\color{lightgreen}Here}$ </strong>

The <code>src/QuantumExecutionEngine.cpp</code> integrates the XACC quantum computing framework with LLVM's Execution Engine to provide a seamless environment for executing both classical and quantum code. The Execution Engine runs the quantum code by calling mapped C++ functions, which in turn use XACC's quantum accelerators to execute the quantum circuit. These functions serve as the bridge between the LLVM Execution Engine and XACC's quantum execution environment. 

<strong>${\color{lightgreen}Data}$ ${\color{lightgreen}Transfer}$ ${\color{lightgreen}from}$ ${\color{lightgreen}the}$ ${\color{lightgreen}Execution}$ ${\color{lightgreen}Engine}$ </strong>

The XACC execution results are stored in a buffer, accessible through a Singleton pattern to ensure consistency and global access. C++ functions like `quantum__qis__read_result__body` and `quantum__rt__result_record_output` fetch data from this buffer. These functions are then dynamically linked to their LLVM counterparts (specified in the QIR) using LLVM's Execution Engine. This way, the quantum execution results can be used by subsequent classical computations in the LLVM code.

<strong>${\color{lightgreen}Handling}$ ${\color{lightgreen}Mixture}$ ${\color{lightgreen}of}$ ${\color{lightgreen}Quantum}$ ${\color{lightgreen}and}$ ${\color{lightgreen}Classical}$ ${\color{lightgreen}Instructions}$ </strong>

The flexible architecture enables the combination of quantum and classical instructions. Quantum instructions are executed on the quantum accelerator via XACC, and the results are stored back into a common buffer. Classical operations can then read this buffer for further computation or conditional logic. Classical operations in the LLVM code can operate on the results of quantum computations through the mapped C++ functions. This makes it possible to design algorithms that interplay between classical and quantum computations, using the buffer as the common data link.

<strong>${\color{lightgreen}Dependencies}$ </strong>
<ol>
  <li> XACC: The eXtreme-scale Accelerator programming framework is used for quantum computation. You can install it <a href="https://xacc.readthedocs.io/en/latest/index.html">here</a></li>
  <li> LLVM 16: The LLVM compiler infrastructure, specifically version 16, is required for the LLVM Execution Engine and for parsing LLVM IR files. You can find the installation guide <a href="https://llvm.org/docs/GettingStarted.html#install">here</a>. </li>
</ol>

<strong>${\color{lightgreen}In}$ ${\color{lightgreen} Progress}$</strong>
<ol>
  <li> Link LLVM via <code>CMakeLists.txt</code> </li>
  <li>Quantum Execution Engine (parses and executes quantum instructions only) </li>
  <ol>
    <li> Declare accelerator.
      <ol>
        <li> <code>auto accelerator = xacc::getAccelerator(accelerator_name);</code>
      </ol>
    </li>
    <li> Parse <code>*.ll</code> for quantum instructions. </li>
      <ol>
        <li> Main block parsing. Example: <code>bell.ll</code> </li>
        <li> Persist through entire level one scope. Examples: <code>if_else.ll, loop1.ll </code> </li>
        <li> Mutiple calls to an outside function. Example: <code>multiple1.ll</code> </li>
        <li> Implementations of functions for basic arithmetic with some user input. See <code>input.ll</code></li>
        <li> ${\color{red}TODO}$ Handling MIXED instruction with user input (what if user input is quantum input?). </li>
      </ol>
    <li> Construct quantum ciruit or kernel in XACCIR. </li>
     <ol>
     <li> Persist through entire level one scope.</li>
      <li> Extend to level two scope. This might include calls to external functions and user input.</li>
     </ol>
    <li> Use pointers to save readout. </li>
    <li> Execute quantum circuit on accelerator by checking global flag from readout. </li>
    <ol>
      <li> <code>EE->addGlobalMapping(quantumFunc, (void*)&executeQuantumWithXACC)</code> </li>
    </ol>
  </ol>
  <li>JIT Compilation and Execution </li>
  <ol>
    <li> LLVM EE created for JIT compilation. </li>
    <li> Quantum functions within LLVM module are mapped (or bound) to the QEE function. </li>
    <li> Main function of LLVM module is executed using LLVM EE. </li>
    <li> Quantum readout from hardware stored in buffer. </li>
  </ol>
  <li>Cleanup ouptut for user. Clear cache (if needed). </li>
</ol>

<strong>${\color{red}TODO}$ ${\color{lightgreen}For}$ ${\color{lightgreen}Public}$ ${\color{lightgreen}Release}$</strong>
<ol>
  <li> Complete C++ implementations for quantum operations. </li>
  <li> Handle parametrized quantum circuits and operations. </li>
  <li> Enhanced output record keeping. </li>
  <li> Examples and documentation. </li>
  <li> Improve error handling. </li>
  <li> Validate the functionality and robustness of the integration. </li>
  <li> <strike>Handling of multiple functions, multiple blocks, multiple entry points</strike> </li>
  <li> <strike>Handling of user created functions.</strike></li>
  <li> <strike>Handling of basic arithmetic with user input.</strike></li>
  <li> Fix 'target' qubit output of <code>if_else.ll</code>. It might be related to the number of shots. </li>
  <li> <strike>Check difference between 'requiredQubits' and 'num_required_qubits' in attributes.</strike></li>
  <li> <strike>If only classical code is present in '*.ll' file, insert "num_required_qubits"="0" inside list 'attributes #0'.</strike></li>
  <li> LLVM version handling!
  <li> Check parallel implementations from Microsoft, Xanadu, Intel, Munich Team. </li>
</ol>

<strong>${\color{lightgreen}Funding}$ </strong>

This work was performed at Oak Ridge National Laboratory, operated by UT-Battelle, LLC under contract DE-AC05-00OR22725 for the US Department of Energy (DOE). Support for the work came from the DOE Advanced Scientific Computing Research (ASCR) Accelerated Research in Quantum Computing (ARQC) Program under field work proposal ERKJ332.
