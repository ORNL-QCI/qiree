#include "ExecutionBackendHandle.hpp"
#include "ExecutionBackendFactory.hpp"

SingletonHandle::SingletonHandle(ExecutionBackend &plugin)
    : _backendPlugin(plugin) {}

SingletonHandle &SingletonHandle::Instance() {
  static SingletonHandle instance(
      ExecutionBackendFactory::createExecutionBackend());
  return instance;
}

void SingletonHandle::initialize(const std::string accelerator_name,
                                 const int numQubits, const int shots) {
  _backendPlugin.initialize(accelerator_name, numQubits, shots);
}

void SingletonHandle::finalize() { _backendPlugin.finalize(); }

void SingletonHandle::addInstruction(
    const std::string &gate, const std::vector<unsigned long> qubitIndices) {
  _backendPlugin.addInstruction(gate, qubitIndices);
}

void SingletonHandle::addInstruction(
    const std::string &gate,
    const std::vector<unsigned long> qubitIndices,
    const double params) {
  _backendPlugin.addInstruction(gate, qubitIndices, params);
}

int SingletonHandle::getQubitOutput(const int qubitIndex, const std::string bit) {
  return _backendPlugin.getQubitOutput(qubitIndex, bit);
}

void SingletonHandle::addMeasure(const int qubit, const int result) {
  _backendPlugin.addMeasure(qubit, result);
}

void SingletonHandle::execute() { _backendPlugin.execute(); }

void SingletonHandle::addIfStmt(const int qubitIndex) {
  _backendPlugin.addIfStmt(qubitIndex);
}