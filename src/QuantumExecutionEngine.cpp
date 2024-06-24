#include "GlobalMappingAssociator.hpp"
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Value.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Attributes.h>
#include <llvm/IR/Metadata.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/IRBuilder.h>
#include <xacc/xacc.hpp>
#include <iostream>
#include <fstream>
#include <set>
#include <functional>
#include "ExecutionBackendHandle.cpp"

using namespace llvm;
// Here we open a file in write mode 
std::ofstream out("output.txt");
// Hash function for pair of objects, used in GlobalMappingAssociator class. When we have the qubit and the classical register, we need to hash them to create a key for the map
int extractLabelIndex(int8_t label) {
    return label ? reinterpret_cast<int8_t>(label) : 0;
}

// Here, we store the output of the quantum program to the file output.txt
void quantum__rt__result_record_output(Result* result, int8_t label) {
    //auto& buffer = Singleton::Instance().getBuffer();
    int index = extractResultIndex(result);
    int labelIndex = extractLabelIndex(label);
    int outputQubit0 = SingletonHandle::Instance().getOutputQubitIndex(index, "0");
    int outputQubit1 = SingletonHandle::Instance().getOutputQubitIndex(index, "1");
    // store the outputQubit0 and outputQubit1 to the file output.txt
    // Quantinum is creating a convention to store this output (weird but it is what it is)
    out << "qubit" << ' ' << static_cast<int>(index)<< ' ' << "Experiment" << ' ' << labelIndex << std::endl;
    out << "0" << ' ' << outputQubit0 << std::endl;
    out << "1" << ' ' << outputQubit1 << std::endl;
}

// The quantum array record function will execute the quantum program, it should be called at the end of the quantum program
void quantum__rt__array_record_output(int64_t elements, int8_t label) {
    // Here we write the metadata to the file in an array way
    int labelIndex = extractLabelIndex(label);
    out << "Experimental Output" << std::endl;
    out << "number of ouputs"<< ' ' << elements << ' ' << "Experiment" << ' ' << labelIndex << std::endl;
    SingletonHandle::Instance().execute();
}

bool quantum__qis__read_result__body(Result* result) {
    // find what is the qubit index associated to result index
    int qubitIndex = extractResultIndex(result);    
    SingletonHandle::Instance().setIfStmtExecuted(true);
    SingletonHandle::Instance().addIfStmt(qubitIndex);
    return true;
}

void IfStmt_call(int x){
    SingletonHandle::Instance().setIfStmtExecuted(false);
    // Dan: I commented this out because it doesn't seem
    // we use it for anything
    //auto ifStmt = SingletonHandle::Instance().getIfStmt();
}

// Extract the number of required qubits from LLVM module's attributes
int extractNumQubits(const llvm::Module& Mod) {
    llvm::Function *F = Mod.getFunction("main");
    if (!F) return -1;  // Return -1 if main function is not found

    // Loop through function attributes to find "num_required_qubits" or "requiredQubits"
    for (const auto &AttrSet : F->getAttributes()) {
        for (const auto &Attr : AttrSet) {
            auto attrStr = Attr.getAsString();
            size_t foundPos = attrStr.find("num_required_qubits");
            if (foundPos == std::string::npos) {
                foundPos = attrStr.find("requiredQubits");
            }

            if (foundPos != std::string::npos) {
                std::string value = Attr.getValueAsString().str();
                if (!value.empty() && std::all_of(value.begin(), value.end(), ::isdigit)) {
                    return std::stoi(value);
                }
            }
        }
    }
    return 0;  // Return 0, indicating classical code
}

int main(int argc, char** argv) {
    int shots = 1024; // default value
    std::string accelerator_name = "aer"; // default value
    std::string llvm_file_path = argv[1]; // the first argument is always the llvm file path

    if (argc > 2){
        shots = std::atoi(argv[2]); // convert the first argument to an integer for the number of shots
        // validate the input for the number of shots. If it is zero, std::atoi could not convert it to an integer
        if (shots == 0){
            std::cerr << "The number of shots is not valid. Please provide a valid number of shots." << std::endl;
            return 1;
        }
    }
    if (argc > 3){
        accelerator_name = argv[3]; // get the accelerator name
        // validate the input for the accelerator name. If it is empty, the user did not provide an accelerator name
        if (accelerator_name.empty()){
            std::cerr << "The accelerator name is not valid. Please provide a valid accelerator name." << std::endl;
            return 1;
        }
        // check if the accelerator name is supported by XACC, i.e. if the name belongs to the list {"qpp", "aer", "qsim"}
        std::set<std::string> supportedAccelerators = {"qpp", "aer", "qsim", "honeywell:H1-1E", "honeywell:H1-1SC", "ionq"};
        if (supportedAccelerators.find(accelerator_name) == supportedAccelerators.end()){
            std::cerr << "The accelerator name is not supported. Please provide a valid accelerator name." << std::endl;
            // write what are the supported accelerators
            std::cerr << "The supported accelerators are: " << std::endl;
            for (auto it = supportedAccelerators.begin(); it != supportedAccelerators.end(); ++it){
                std::cerr << *it << std::endl;
            } 
            return 1;
        }
    }

    // Initialize LLVM
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();

    // Load LLVM IR file
    SMDiagnostic Err;
    LLVMContext context;
    // Parse the LLVM file into a module and check if there is an error
    std::unique_ptr<Module> Mod = parseIRFile(llvm_file_path, Err, context);
    if (!Mod) {
        Err.print("Error reading LLVM file:", errs());
        return 1;
    }
    // Extract the number of qubits from the LLVM module's attributes.
    int numQubits = extractNumQubits(*Mod);
    // Check if we are dealing with classical code by verifying if the number of qubits is zero.
    if (numQubits == 0) {
        // Print a message to inform the user that either the number of qubits is not set or it is set to zero.
        std::cerr << "Unable to find the number of qubits in attributes, or the number of qubits is explicitly set to zero, indicating classical code." << std::endl;
        }

    // Initialize xacc and Singleton
    SingletonHandle& singletonInstance = SingletonHandle::Instance();
    SingletonHandle::Instance().setIfStmtExecuted(false);
    SingletonHandle::Instance().initialize(accelerator_name, numQubits, shots);
    

    // Create Execution Engine
    Module *ModPtr = Mod.get();
    std::string errStr;
    ExecutionEngine *EE = EngineBuilder(std::unique_ptr<Module>(ModPtr)) 
                                        .create();
    if (!EE) {
        std::cerr << "Failed to create execution engine." << std::endl;
        return 1;
    }
    // Define a map for function names and their corresponding actions
    GlobalMappingAssociator globalMapping;
    std::unordered_map<std::string, std::function<void(llvm::Function*)>> funcActions = {
        {"__quantum__rt__array_record_output", [&](llvm::Function* Func){ EE->addGlobalMapping(Func, (void *)&quantum__rt__array_record_output); }},
        {"__quantum__qis__read_result__body", [&](llvm::Function* Func){ EE->addGlobalMapping(Func, (void *)&quantum__qis__read_result__body); }},
        {"__quantum__rt__result_record_output", [&](llvm::Function* Func){ EE->addGlobalMapping(Func, (void *)&quantum__rt__result_record_output); }}
    };

    
    for (auto& Func : *ModPtr) {
        for (auto& basicBlock : Func) {
            for (auto& inst : basicBlock) {
                if (auto* brInst = llvm::dyn_cast<llvm::BranchInst>(&inst)) {
                    if (brInst->isConditional()) {
                        llvm::BasicBlock* thenBlock = brInst->getSuccessor(0);
                        llvm::Instruction* terminator = thenBlock->getTerminator();
                        //
                        llvm::LLVMContext &context = thenBlock->getContext(); // Get the context of the function
                        llvm::IRBuilder<> builder(terminator); // Create an IRBuilder
                        // the argument is a integer
                        llvm::Type *intType = llvm::Type::getInt32Ty(context); // Get the integer type
                        // check if "IfStmt" function is already defined
                        llvm::Function *ifStmtFunc = ModPtr->getFunction("IfStmt");
                        if (!ifStmtFunc){
                            llvm::Type *voidType = llvm::Type::getVoidTy(context); // Get the void 
                            llvm::FunctionType *ifStmtType = llvm::FunctionType::get(voidType, intType, false); // Create the function type
                            // create the function with integer argument
                            ifStmtFunc = llvm::Function::Create(ifStmtType, llvm::Function::ExternalLinkage, "IfStmt", ModPtr); // Create the function
                        }
                        // create a dummy value for resultArg that will be passed to the IfStmt function
                        llvm::Value *resultArg = llvm::ConstantInt::get(intType, 1);
                        llvm::Value *args[] = {resultArg};
                        llvm::CallInst *callIfStmt = builder.CreateCall(ifStmtFunc, args);
                    }
                }
            }
        }
    }

    for (auto& Func : *ModPtr) {
        std::string funcName = Func.getName().str();
        if (funcName=="IfStmt"){
            EE->addGlobalMapping(&Func, (void *)&IfStmt_call);
        }

        if (Func.getName().startswith("__quantum__")) {
            if (numQubits == 0) {
                std::cerr << "Quantum operations cannot operate without qubits" << std::endl;
                return 1;
            }

            std::string funcName = Func.getName().str();
            auto it = funcActions.find(funcName);
            if (it != funcActions.end()) {
                // Call the corresponding function if found in the map
                it->second(&Func);
            } else {
                int argSize = Func.arg_size();
                void* mappedFunc = globalMapping.getMapping(funcName, argSize);
                EE->addGlobalMapping(&Func, mappedFunc);
            }
        }
    }

    // Execute main function
    uint64_t funcAddr = EE->getFunctionAddress("main");
    typedef void (*FuncType)();
    FuncType funcPtr = (FuncType)funcAddr;
    funcPtr();

    xacc::Finalize();
    return 0;
}
