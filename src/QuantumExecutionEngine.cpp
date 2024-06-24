#include "GlobalMappingAssociator.hpp"
#include "Singleton.hpp"
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

using namespace llvm;
// Here we open a file in write mode 
std::ofstream out("output.txt");

int extractLableIndex(int8_t label) {
    return label ? reinterpret_cast<int8_t>(label) : 0;
}

void quantum__rt__result_record_output(Result* result, int8_t label) {
    auto& buffer = Singleton::Instance().getBuffer();
    int index = extractResultIndex(result);
    int labelIndex = extractLableIndex(label);
    int outputQubit0 = buffer->getMarginalCounts({static_cast<int>(index)},xacc::AcceleratorBuffer::BitOrder::LSB)[std::string("0")];
    int outputQubit1 = buffer->getMarginalCounts({static_cast<int>(index)},xacc::AcceleratorBuffer::BitOrder::LSB)[std::string("1")];
    // store the outputQubit0 and outputQubit1 to the file output.txt
    out << "qubit" << ' ' << static_cast<int>(index)<< ' ' << "Experiment" << ' ' << labelIndex << std::endl;
    out << "0" << ' ' << outputQubit0 << std::endl;
    out << "1" << ' ' << outputQubit1 << std::endl;
}

void quantum__rt__array_record_output(int64_t elements, int8_t label) {
    // Here we write the metadata to the file in an array way
    int labelIndex = extractLableIndex(label);
    out << "Experimental Output" << std::endl;
    out << "number of ouputs"<< ' ' << elements << ' ' << "Experiment" << ' ' << labelIndex << std::endl;
    auto& buffer = Singleton::Instance().getBuffer();
    auto& ansatz = Singleton::Instance().getAnsatz();
    auto& accelerator = Singleton::Instance().getAccelerator();
    accelerator->updateConfiguration({std::make_pair("shots", 1024)});
    accelerator->execute(buffer, ansatz);
    buffer->print();
}

// Read result and print it
bool quantum__qis__read_result__body(Result* result) {
    auto& buffer = Singleton::Instance().getBuffer();
    auto& accelerator = Singleton::Instance().getAccelerator();
    auto& ansatz = Singleton::Instance().getAnsatz();
    auto& ifStmt = Singleton::Instance().getIfStmt();
    auto& provider = Singleton::Instance().getProvider();
    // find what is the qubit index associated to result index 
    int qubitIndex = Singleton::Instance().getQubitIndex(extractResultIndex(result));
    int index = extractResultIndex(result);
    Singleton::Instance().setIfStmtExecuted(true);
    auto ifStmt_aux = xacc::ir::asComposite(provider->createInstruction("ifstmt", {static_cast<unsigned long>(qubitIndex)}, {"q"}));
    ansatz->addInstruction(ifStmt_aux);
    ifStmt = ifStmt_aux;
    return true;
}

void IfStmt_call(int x){
    Singleton::Instance().setIfStmtExecuted(false);
    auto ifStmt = Singleton::Instance().getIfStmt();
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
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <accelerator_name> <path_to_llvm_file.ll>" << std::endl;
        return 1;
    }
    std::string accelerator_name = argv[1];
    std::string llvm_file_path = argv[2];

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
    xacc::Initialize();
    Singleton& instance = Singleton::Instance();
    instance.getAccelerator() = xacc::getAccelerator(accelerator_name);
    instance.getProvider() = xacc::getIRProvider("quantum");
    instance.getBuffer() = xacc::qalloc(numQubits);
    instance.getAnsatz() = instance.getProvider()->createComposite("quantum_circuit");
    instance.getIfStmt() = instance.getProvider()->createComposite("ifstmt");
    Singleton::Instance().setIfStmtExecuted(false);
    auto& buffer = Singleton::Instance().getBuffer();
    //buffer->setName("q");
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
    //print the llvm module
    //std::cout<<"printing the llvm module"<<std::endl;
    //Mod->print(llvm::outs(), nullptr);

    
    // Execute main function
    uint64_t funcAddr = EE->getFunctionAddress("main");
    typedef void (*FuncType)();
    FuncType funcPtr = (FuncType)funcAddr;
    funcPtr();

    xacc::Finalize();
    return 0;
}
