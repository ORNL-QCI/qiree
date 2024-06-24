#include "XACCExecutionBackend.hpp"

XACCExecutionBackend &XACCExecutionBackend::Instance() {
  static XACCExecutionBackend instance;
  return instance;
}

void XACCExecutionBackend::initialize(const std::string accelerator_name,
                                      const int numQubits, const int shots) {

  xacc::Initialize();
  XACCExecutionBackend &instance = XACCExecutionBackend::Instance();
  accelerator = xacc::getAccelerator(accelerator_name);
  provider = xacc::getIRProvider("quantum");
  buffer = xacc::qalloc(numQubits);
  ansatz = provider->createComposite("quantum_circuit");
  ifStmt = provider->createComposite("ifstmt");
  accelerator->updateConfiguration({std::make_pair("shots", shots)});
}

void XACCExecutionBackend::finalize() { xacc::Finalize(); }

void XACCExecutionBackend::addInstructionToAnsatz(
    const std::string &gate, std::vector<unsigned long> qubitIndices) {

  if (ifStmtExecuted) {
    ifStmt->addInstruction(provider->createInstruction(gate, qubitIndices));
  } else {
    ansatz->addInstruction(provider->createInstruction(gate, qubitIndices));
  }
}

void XACCExecutionBackend::addParametrizedInstructionToAnsatz(
    const std::string &gate, std::vector<unsigned long> qubitIndices,
    double params) {
  ansatz->addInstruction(
      provider->createInstruction(gate, qubitIndices, {params}));
}

int XACCExecutionBackend::getOutputQubitIndex(int qubitIndex, std::string bit) {
  return buffer->getMarginalCounts({qubitIndex},
                                   xacc::AcceleratorBuffer::BitOrder::LSB)[bit];
}

void XACCExecutionBackend::addMeasure(const int qubit, const int result) {
  addQubitResultMapping(qubit, result);
  auto measure = provider->createInstruction(
      "Measure", {static_cast<unsigned long>(qubit)}, {result});
  ansatz->addInstruction(measure);
}

void XACCExecutionBackend::execute() {
  accelerator->execute(buffer, ansatz);
  buffer->print();
}

void XACCExecutionBackend::addIfStmt(int qubitIndex) {
  auto ifStmt_aux = xacc::ir::asComposite(provider->createInstruction(
      "ifstmt", {static_cast<unsigned long>(qubitIndex)}, {"q"}));
  ansatz->addInstruction(ifStmt_aux);
  ifStmt = ifStmt_aux;
}