#ifndef EXECUTION_BACKEND_HPP
#define EXECUTION_BACKEND_HPP

#include <iostream>
#include <unordered_map>
#include <vector>

class ExecutionBackend {

protected:
  bool ifStmtExecuted = false;
  std::unordered_map<int, int> qubitResultMap;

public:
  virtual void initialize(const std::string, const int, const int) = 0;
  virtual void finalize() = 0;
  virtual void addInstructionToAnsatz(const std::string &,
                                      const std::vector<unsigned long>) = 0;
  // Dan: I wonder if we can't just overload addInstruction
  virtual void addParametrizedInstructionToAnsatz(const std::string &,
                                                  std::vector<unsigned long>,
                                                  double) = 0;
  virtual void addMeasure(const int, const int) = 0;
  virtual int getOutputQubitIndex(int, std::string) = 0;
  virtual void execute() = 0;
  virtual void addIfStmt(int) = 0;

  void setIfStmtExecuted(bool value) { ifStmtExecuted = value; }

  bool getIfStmtExecuted() { return ifStmtExecuted; }

  // create a function that will keep track of qubit indices and their
  // corresponding result indices this function will be called by the
  // quantum__qis__measure__body function
  void addQubitResultMapping(int qubitIndex, int resultIndex) {
    qubitResultMap.insert(std::make_pair(qubitIndex, resultIndex));
  }

  // create a function that will return the qubit index for a given result index
  int getQubitIndex(int resultIndex) {
    return qubitResultMap.find(resultIndex)->second;
  }
  // create a function that will return the result index for a given qubit index
  int getResultIndex(int qubitIndex) {
    return qubitResultMap.find(qubitIndex)->first;
  }
  virtual ~ExecutionBackend() = default;
};

#endif