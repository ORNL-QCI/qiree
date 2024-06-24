#include "ExecutionBackendHandle.hpp"
#include "ExecutionBackendFactory.hpp"

SingletonHandle::SingletonHandle(ExecutionBackend &plugin)
    : backendPlugin(plugin) {}

SingletonHandle &SingletonHandle::Instance() {
  static SingletonHandle instance(
      ExecutionBackendFactory::createExecutionBackend());
  return instance;
}

void SingletonHandle::initialize(const std::string accelerator_name,
                                 const int numQubits, const int shots) {
  backendPlugin.initialize(accelerator_name, numQubits, shots);
}

void SingletonHandle::finalize() { backendPlugin.finalize(); }

void SingletonHandle::addInstructionToAnsatz(
    const std::string &gate, const std::vector<unsigned long> qubitIndices) {
  backendPlugin.addInstructionToAnsatz(gate, qubitIndices);
}

void SingletonHandle::addParametrizedInstructionToAnsatz(
    const std::string &gate, std::vector<unsigned long> qubitIndices,
    double params) {
  backendPlugin.addParametrizedInstructionToAnsatz(gate, qubitIndices, params);
}

int SingletonHandle::getOutputQubitIndex(int qubitIndex, std::string bit) {
  return backendPlugin.getOutputQubitIndex(qubitIndex, bit);
}

void SingletonHandle::addMeasure(const int qubit, const int result) {
  backendPlugin.addMeasure(qubit, result);
}

void SingletonHandle::execute() { backendPlugin.execute(); }

void SingletonHandle::addIfStmt(int qubitIndex) {
  backendPlugin.addIfStmt(qubitIndex);
}