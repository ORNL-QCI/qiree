#include <xacc/xacc.hpp>
#include <iostream>
#include <fstream>
#include <set>
#include <functional>
#include "ExecutionBackendHandle.cpp"
#include "QIREE.hpp"

using namespace llvm;

int main(int argc, char** argv) {

    int shots = 1024, numQubits; // default value
    std::string accelerator_name = "aer"; // default value
    std::string llvm_file_path; // the first argument is always the llvm file path

    std::vector<std::string> arguments(argv + 1, argv + argc);
    for (int i = 0; i < arguments.size(); i++) {
      if (arguments[i] == "--shots") {
        shots = std::stoi(arguments[i + 1]);
      }
      if (arguments[i] == "--n-qubits") {
        numQubits = std::stoi(arguments[i + 1]);
      }
      if (arguments[i] == "--accelerator") {
        accelerator_name = arguments[i + 1];
      }
      if (arguments[i] == "--llvm-file-path") {
        llvm_file_path = arguments[i + 1];
      }
    }

    if (llvm_file_path.empty()) {
      std::cerr << "Please provide the path to the .ll file using the flag --llvm-file-path." << std::endl;
      return 1;
    }

    if (shots <= 0){
        std::cerr << "The number of shots is not valid. Please provide a valid number of shots using the flag --shots." << std::endl;
        return 1;
    }

    std::set<std::string> supportedAccelerators = {"aer", "qpp", "qsim", "honeywell:H1-1E", "honeywell:H1-1SC", "honeywell:H1-1", "honeywell:H2-1", "ionq"};
    if (supportedAccelerators.find(accelerator_name) == supportedAccelerators.end()){
            std::cerr << "The accelerator name is not supported. Please provide a valid accelerator name using the flag --accelerator." << std::endl;
            // write what are the supported accelerators (may depend on which plugin is being used)
            std::cerr << "The supported accelerators are: " << std::endl;
            for (auto it = supportedAccelerators.begin(); it != supportedAccelerators.end(); ++it){
                std::cerr << *it << std::endl;
            }
            return 1;
        }

    SingletonHandle& singletonInstance = SingletonHandle::Instance();
    SingletonHandle::Instance().setIfStmtExecuted(false);
    SingletonHandle::Instance().initialize(accelerator_name, numQubits, shots);

    qiree::QIREE q(numQubits);
    q.initializeLLVM();
    q.createModule(llvm_file_path);
    q.createExecutionEngine();
    q.checkIfStmt();

    GlobalMappingAssociator globalMapping;
    q.bindFunctions(globalMapping);
    q.execute();

    SingletonHandle::Instance().finalize();

    return 0;
}
