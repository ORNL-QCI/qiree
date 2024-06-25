#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/IR/IRBuilder.h>
#include <fstream>
#include "GlobalMappingAssociator.hpp"

// Here we open a file in write mode 
std::ofstream out("output.txt");
using namespace llvm;

namespace qiree {

// Hash function for pair of objects, used in GlobalMappingAssociator class. When we have the qubit and the classical register, we need to hash them to create a key for the map
int extractLabelIndex(int8_t label) {
    return label ? reinterpret_cast<int8_t>(label) : 0;
}

// Here, we store the output of the quantum program to the file output.txt
void quantum__rt__result_record_output(Result* result, int8_t label) {
    //auto& buffer = Singleton::Instance().getBuffer();
    int index = extractResultIndex(result);
    int labelIndex = extractLabelIndex(label);
    int outputQubit0 = SingletonHandle::Instance().getQubitOutput(index, "0");
    int outputQubit1 = SingletonHandle::Instance().getQubitOutput(index, "1");
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



llvm::LLVMContext& getContext()
{
    static llvm::LLVMContext context;
    return context;
}

class QIREE {

private:
  std::unique_ptr<llvm::Module> _module;
  llvm::ExecutionEngine* _ee;
  int _numQubits;

  std::unordered_map<std::string, std::function<void(llvm::Function*)>> getFunctionActions() {
    return {
        {"__quantum__rt__array_record_output", [&](llvm::Function* Func){ _ee->addGlobalMapping(Func, (void *)&quantum__rt__array_record_output); }},
        {"__quantum__qis__read_result__body", [&](llvm::Function* Func){ _ee->addGlobalMapping(Func, (void *)&quantum__qis__read_result__body); }},
        {"__quantum__rt__result_record_output", [&](llvm::Function* Func){ _ee->addGlobalMapping(Func, (void *)&quantum__rt__result_record_output); }}
    };
  }

  int extractNumQubits() {
    llvm::Function *F = _module->getFunction("main");
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

public:
  QIREE() = default;
  QIREE(const int numQubits) : _numQubits(numQubits) {}

  void initializeLLVM() {
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
  }

  void createModule(const std::string& llvm_file_path) {
    llvm::SMDiagnostic Err;
    _module = parseIRFile(llvm_file_path, Err, getContext());
  }

  void createExecutionEngine() {
    // the only way this pointer survives is by doing this
    auto* ModPtr = _module.get();
    _ee = EngineBuilder(std::unique_ptr<llvm::Module>(ModPtr)).create();
  }

  void checkIfStmt() {

    // Process LLVMIR for conditional quantum instructions handling
    auto* ModPtr = _module.get();
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
    
  }

  void bindFunctions(GlobalMappingAssociator mapper) {

    auto* ModPtr = _module.get();
    auto funcActions = getFunctionActions();
    for (auto& Func : *ModPtr) {
      std::string funcName = Func.getName().str();
      if (funcName=="IfStmt"){
          _ee->addGlobalMapping(&Func, (void *)&IfStmt_call);
      }

      if (Func.getName().startswith("__quantum__")) {
        if (_numQubits == 0) {
            std::cerr << "Quantum operations cannot operate without qubits. Please use the flag --n-qubits after the file path to indicate number of qubits." << std::endl;
            return;
        }

        std::string funcName = Func.getName().str();
        auto it = funcActions.find(funcName);
        if (it != funcActions.end()) {
            // Call the corresponding function if found in the map
            it->second(&Func);
        } else {
            int argSize = Func.arg_size();
            void* mappedFunc = mapper.getMapping(funcName, argSize);
            _ee->addGlobalMapping(&Func, mappedFunc);
        }
      }
    }
  }

  void execute() {

    if (_numQubits != extractNumQubits()) {
      std::cerr << "Number of qubits in the .ll file don't match the provided number of qubits." << std::endl;
      return;
    }

    uint64_t funcAddr = _ee->getFunctionAddress("main");
    typedef void (*FuncType)();
    FuncType funcPtr = (FuncType)funcAddr;
    funcPtr();
  }

};

}
