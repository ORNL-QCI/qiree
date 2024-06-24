# qir-xir
Private repo for adapting QIR for use in XACC.

<strong>${\color{lightgreen}Abstract}$</strong>

The Quantum Intermediate Representation (QIR) provides an abstraction of programs that include a mixture of conventional and quantum instructions. This project develops a tool that translates a QIR program into a hardware-specific implementation using the XACC programming framework. This tool provides a unique capability to translating abstract QIR programs into executable programs on specific hardware. By mapping specific LLVM function calls to their corresponding C++ implementations, the code bridges the LLVM Execution Engine's classical execution environment with XACC's quantum computing capabilities. The integration allows for the smooth transition and combination of classical and quantum instructions, providing a unified platform for hybrid quantum-classical computing. The results from the quantum computations are captured in a globally accessible buffer, facilitating their use in classical post-processing or control logic.

<strong>${\color{lightgreen}Testing}$ ${\color{lightgreen} Instructions}$</strong>
<ol>
  <li> Install <a href="https://llvm.org/docs/GettingStarted.html#install">LLVM </a> and  <a href="https://xacc.readthedocs.io/en/latest/index.html">XACC</a>. </li>
  <li> Check that your <code>cmake</code> prefixes for XACC are correct.
    <ol>
      <li> Typing <code>echo $CMAKE_PREFIX_PATH</code> should give you the path to your xacc installation.</li>
      <li> If empty, then add it: <code>export CMAKE_PREFIX_PATH=[path to your xacc install]</code></li>
      <li> Example: <code>[path to your xacc install]</code> might be like <code>$HOME/.xacc</code></li>
    </ol>
  </li>
  <li> Clone this repo. Enter the repo. </li>
  <li> <code>mkdir build; cd build</code></li>
  <li> <code>cmake .. </code></li>
  <li> <code>make</code> </li>
  <li> <code>./QuantumExecutionEngine [sim choice] ../examples/[name of *.ll file] </code> where <code>[sim choice]</code> can be one of <code>aer, qpp, qsim</code> </li> 
  <li> Current results come from a default of <code>1024</code> shots. </li>
</ol>

<strong>${\color{lightgreen}What}$ ${\color{lightgreen}is}$ ${\color{lightgreen}Here}$ </strong>

The <code>src/QuantumExecutionEngine.cpp</code> integrates the XACC quantum computing framework with LLVM's Execution Engine to provide a seamless environment for executing both classical and quantum code. The Execution Engine runs the quantum code by calling mapped C++ functions, which in turn use XACC's quantum accelerators to execute the quantum circuit. These functions serve as the bridge between the LLVM Execution Engine and XACC's quantum execution environment. 

<strong>${\color{lightgreen}Data}$ ${\color{lightgreen}Transfer}$ ${\color{lightgreen}from}$ ${\color{lightgreen}the}$ ${\color{lightgreen}Execution}$ ${\color{lightgreen}Engine}$ </strong>

The XACC execution results are stored in a buffer, accessible through a Singleton pattern to ensure consistency and global access. C++ functions like `quantum__qis__read_result__body` and `quantum__rt__result_record_output` fetch data from this buffer. These functions are then dynamically linked to their LLVM counterparts (specified in the QIR) using LLVM's Execution Engine. This way, the quantum execution results can be used by subsequent classical computations in the LLVM code.

<strong>${\color{lightgreen}Handling}$ ${\color{lightgreen}Mixture}$ ${\color{lightgreen}of}$ ${\color{lightgreen}Quantum}$ ${\color{lightgreen}and}$ ${\color{lightgreen}Classical}$ ${\color{lightgreen}Instructions}$ </strong>

The flexible architecture enables the combination of quantum and classical instructions. Quantum instructions are executed on the quantum accelerator via XACC, and the results are stored back into a common buffer. Classical operations can then read this buffer for further computation or conditional logic. Classical operations in the LLVM code can operate on the results of quantum computations through the mapped C++ functions. This makes it possible to design algorithms that interplay between classical and quantum computations, using the buffer as the common data link.

<strong>${\color{lightgreen}Dependencies}$ </strong>
<ol>
  <li> <a href="https://xacc.readthedocs.io/en/latest/index.html">XACC</a>: The eXtreme-scale Accelerator programming framework is used for quantum computation. </li>
  <li> <a href="https://llvm.org/docs/GettingStarted.html#install">LLVM</a>: The LLVM compiler infrastructure is required for the LLVM Execution Engine and for parsing LLVM IR files. </li>
</ol>

<strong>${\color{lightgreen}Current}$ ${\color{lightgreen} Structure}$</strong>
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
        <li> Persist through entire level one scope. Examples: <code>teleport.ll, loop.ll </code> </li>
        <li> Mutiple calls to an outside function. Example: <code>multiple.ll</code> </li>
        <li> Parameterized functions. Example: <code>rotation.ll</code> </li>
        <li> Implementations of functions for basic arithmetic with some user input. Example: <code>input.ll</code></li>
        <li> Users can add instructions using the <code>addMapping</code> function. </li>
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

- [x] Handling of multiple functions, multiple blocks, multiple entry points.
- [x] Handling of user created functions (related to above).
- [x] Handling of basic arithmetic with user input.
- [x] Check difference between <code>requiredQubits</code> and <code>num_required_qubits</code> in attributes.
- [x] If only classical code is present in <code>*.ll</code> file, insert <code>"num_required_qubits"="0"</code> inside list <code>attributes #0</code>.
- [x] Fix 'target' qubit output of <code>if_else.ll</code>.
- [x] Handling MIXED instruction with user input (what if user input is quantum input?).
- [x] Handle parameterized quantum circuits and operations.
- [x] LLVM version handling!
- [ ] 01. Add a variable for 'number of shots' input (but keep default at 1024). </li>
- [ ] 02. Complete C++ implementations for quantum operations (ongoing - see full list below).
- [ ] 03. Two more examples to illustrate functionality (multiple gates and phase estimation algorithm).
- [ ] 04. Improve error handling: We should try to catch <a href="https://github.com/qir-alliance/qir-spec/blob/main/specification/v0.1/4_Quantum_Runtime.md"><code>quantum_rt_fail</code></a> messages to know when XACC fails.
- [ ] 05. Enhanced output record keeping: what other diagnostics can we expose from the XACC buffer? Currently we are doing bare minimum: shots. Can we do more?
- [ ] 06. Inline code documentation (ongoing).
- [ ] 07. Validation: Construct unit tests where we already know what the output should be.
- [ ] 08. Integration 1: Make into a cohesive whole with XACC. Fork XACC from  <a href="https://github.com/eclipse/xacc">Eclipse</a> --> add program to the directory tree (where exactly?) -> apply unit tests --> make a pull request (which will pick up all commits pushed to the main branch of the repo.
- [ ] 09. Readthedocs: What is our tool good for and how can users use it? This will depend on how we do the integration step.
- [ ] 10. Robustness: How does our code with different hardware backends? Goal: IBM and Quantinuum.
- [ ] 11. Check/understand parallel implementations from Microsoft, Xanadu, Intel, Nvidia, Munich Team.
- [ ] 12. Write the paper for the above implementation (ongoing).
- [ ] [FUTURE] Integration 2: Working beyond just XACC (possibly with different hardware backends).
- [ ] [FUTURE] Next level examples: variational algorithm, circuit optimizations.
      
<strong>${\color{red}TODO}$ ${\color{lightgreen}Checklist}$ ${\color{lightgreen}For}$ ${\color{lightgreen}Implementations}$</strong>
- [x] <code>void @__quantum__qis__reset__body(%Qubit*)</code>
- [x] <code>void @__quantum__qis__cnot__body(%Qubit*, %Qubit*)</code>
- [x] <code>void @__quantum__qis__h__body(%Qubit*)</code>
- [x] <code>void @__quantum__qis__x__body(%Qubit*)</code>
- [x] <code>void @__quantum__qis__y__body(%Qubit*)</code>
- [x] <code>void @__quantum__qis__z__body(%Qubit*)</code>
- [x] <code>void @__quantum__qis__t__body(%Qubit*)</code>
- [x] <code>void @__quantum__qis__mz__body(%Qubit*, %Result*)</code>
- [x] <code>void @__quantum__qis__rx__body(double, %Qubit*)</code>
- [x] <code>void @__quantum__qis__ry__body(double, %Qubit*)</code>
- [x] <code>void @__quantum__qis__rz__body(double, %Qubit*)</code>
- [ ] <code>void @__quantum__qis__swap__body(%Qubit*, %Qubit*)</code>
- [ ] <code>void @__quantum__qis__h__ctl(%Array*, %Qubit*)</code>
- [ ] <code>void @__quantum__qis__x__ctl(%Array*, %Qubit*)</code>
- [ ] <code>void @__quantum__qis__y__ctl(%Array*, %Qubit*)</code>
- [ ] <code>void @__quantum__qis__z__ctl(%Array*, %Qubit*)</code>
- [ ] <code>void @__quantum__qis__t__ctl(%Array*, %Qubit*)</code>
- [ ] <code>void @__quantum__qis__t__adj(%Qubit*)</code>
- [ ] <code>void @__quantum__qis__t__ctladj(%Array*, %Qubit*)</code>
- [ ] <code>void @__quantum__qis__rx__ctl(%Array*, %Tuple*)</code>
- [ ] <code>void @__quantum__qis__rxx__body(double, %Qubit*, %Qubit*)</code>
- [ ] <code>void @__quantum__qis__ry__ctl(%Array*, %Tuple*)</code>
- [ ] <code>void @__quantum__qis__ryy__body(double, %Qubit*, %Qubit*)</code>
- [ ] <code>void @__quantum__qis__rz__ctl(%Array*, %Tuple*)</code>
- [ ] <code>void @__quantum__qis__rzz__body(double, %Qubit*, %Qubit*)</code>
- [ ] <code>void @__quantum__qis__ccx__body(%Qubit*, %Qubit*)</code>
- [ ] <code>void @__quantum__qis__cx__body(%Qubit*, %Qubit*)</code>
- [ ] <code>void @__quantum__qis__cy__body(%Qubit*, %Qubit*)</code>
- [ ] <code>void @__quantum__qis__cz__body(%Qubit*, %Qubit*)</code>
- [ ] <code>void @__quantum__qis__r__adj(i2, double, %Qubit*)</code>
- [ ] <code>void @__quantum__qis__r__body(i2, double, %Qubit*)</code>
- [ ] <code>void @__quantum__qis__r__ctl(%Array*, %Tuple*)</code>
- [ ] <code>void @__quantum__qis__r__ctladj(%Array*, %Tuple*)</code>
- [ ] <code>void @__quantum__qis__s__adj(%Qubit*)</code>
- [ ] <code>void @__quantum__qis__s__body(%Qubit*)</code>
- [ ] <code>void @__quantum__qis__s__ctl(%Array*, %Qubit*)</code>
- [ ] <code>void @__quantum__qis__s__ctladj(%Array*, %Qubit*)</code>


<strong>${\color{red}Other}$ ${\color{lightgreen}Measure,}$ ${\color{lightgreen}Results,}$ ${\color{lightgreen}Arrays,}$ ${\color{lightgreen}Strings,}$ ${\color{lightgreen}Tuples}$</strong>
- [ ] <code>bool @__quantum__qis__read_result__body(%Result*)</code>
- [ ] <code>bool @__quantum__rt__result_equal(%Result*, %Result*)</code>
- [ ] <code>%Result* @__quantum__rt__result_get_one()</code>
- [ ] <code>%Result* @__quantum__rt__result_get_zero()</code>
- [ ] <code>void @__quantum__rt__result_record_output(%Result*, i8*)</code>
- [ ] <code>%String* @__quantum__rt__result_to_string(%Result*)</code>
- [ ] <code>void @__quantum__rt__result_update_reference_count(%Result*, i32)</code>
- [ ] <code>%Result* @__quantum__qis__m__body(%Qubit*)</code>
- [ ] <code>%Result* @__quantum__qis__measure__body(%Array*, %Array*)</code>
- [ ] <code>%Result* @__quantum__qis__mresetz__body(%Qubit*)</code>
- [ ] <code>%Array* @__quantum__rt__array_concatenate(%Array*, %Array*)</code>
- [ ] <code>%Array* @__quantum__rt__array_copy(%Array*, bool)</code>
- [ ] <code>%Array* @__quantum__rt__array_create_1d(i32, i64)</code>
- [ ] <code>i8* @__quantum__rt__array_get_element_ptr_1d(%Array*, i64)</code>
- [ ] <code>i64 @__quantum__rt__array_get_size_1d(%Array*)</code>
- [ ] <code>%Array* @__quantum__rt__array_slice_1d(%Array*, %Range, i1)</code>
- [ ] <code>void @__quantum__rt__array_record_output(i64, i8*)</code>
- [ ] <code>void @__quantum__rt__array_update_alias_count(%Array*, i32)</code>
- [ ] <code>void @__quantum__rt__array_update_reference_count(%Array*, i32)</code>
- [ ] <code>%String* @__quantum__rt__string_concatenate(%String*, %String*)</code>
- [ ] <code>%String* @__quantum__rt__string_create(i8*)</code>
- [ ] <code>bool @__quantum__rt__string_equal(%String*, %String*)</code>
- [ ] <code>i8* @__quantum__rt__string_get_data(%String*)</code>
- [ ] <code>i32 @__quantum__rt__string_get_length(%String*)</code>
- [ ] <code>void @__quantum__rt__string_update_reference_count(%String*, i32)</code>
- [ ] <code>%Tuple* @__quantum__rt__tuple_copy(%Tuple*, i1)</code>
- [ ] <code>%Tuple* @__quantum__rt__tuple_create(i64)</code>
- [ ] <code>void @__quantum__rt__tuple_record_output(i64, i8*)</code>
- [ ] <code>void @__quantum__rt__tuple_update_alias_count(%Tuple*, i32)</code>
- [ ] <code>void @__quantum__rt__tuple_update_reference_count(%Tuple*, i32)</code>

<strong>${\color{lightgreen}Funding}$ </strong>

This work was performed at Oak Ridge National Laboratory, operated by UT-Battelle, LLC under contract DE-AC05-00OR22725 for the US Department of Energy (DOE). Support for the work came from the DOE Advanced Scientific Computing Research (ASCR) Accelerated Research in Quantum Computing (ARQC) Program under field work proposal ERKJ332.
