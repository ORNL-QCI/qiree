#ifndef EXECUTION_BACKEND_HPP
#define EXECUTION_BACKEND_HPP

#include <iostream>
#include <unordered_map>
#include <vector>

/**
 * @brief Interface for methods carried out by backends.
 */
class ExecutionBackend {

protected:
  /**
   * @brief True if IfStmt instruction has been executed.
   */
  bool _ifStmtExecuted = false;

  /**
   * @brief Map of qubits to cbits.
   */
  std::unordered_map<int, int> _qubitResultMap;

public:
  /**
   * @brief Initializes backend framework.
   *
   * @param acceleratorName Name of the chosen accelerator backend.
   * @param numQubits Number of qubits.
   * @param shots Number of shots.
   */
  virtual void initialize(const std::string acceleratorName,
                          const int numQubits, const int shots) = 0;

  /**
   * @brief Finalizes backend framework.
   */
  virtual void finalize() = 0;

  /**
   * @brief Adds instruction to the circuit.
   *
   * @param gate Name of gate.
   * @param qubitIndex Index of qubit(s) to which gate applies.
   */
  virtual void addInstruction(const std::string &gate,
                              const std::vector<unsigned long> qubitIndex) = 0;

  /**
   * @brief Adds parameterized instruction to the circuit.
   *
   * @param gate Name of gate.
   * @param qubitIndex Index of qubit(s) to which gate applies.
   * @param param Gate parameter.
   */
  virtual void addInstruction(const std::string &gate,
                              const std::vector<unsigned long> qubitIndex,
                              const double param) = 0;

  /**
   * @brief Adds a measurement instruction to the circuit.
   *
   * @param qubitIndex Qubit index.
   * @param cbitIndex Classical bit index.
   */
  virtual void addMeasure(const int qubitIndex, const int cbitIndex) = 0;

  /**
   * @brief Gets marginal counts for a given qubit.
   *
   * @param qubitIndex Qubit index.
   * @param bitValue Value of the bit. Either 0 or 1.
   *
   * @return Number of times bitValue was measured for qubit in qubitIndex.
   */
  virtual int getQubitOutput(const int qubitIndex,
                             const std::string bitValue) = 0;

  /**
   * @brief Execute backend.
   */
  virtual void execute() = 0;

  /**
   * @brief Applies ifStmt to qubit.
   *
   * @param qubitIndex Qubit index.
   */
  virtual void addIfStmt(const int qubitIndex) = 0;

  /**
   * @brief Sets whether ifStmt instruction has been executed.
   *
   * @param value True if ifStmt has been executed.
   */
  void setIfStmtExecuted(const bool value) { _ifStmtExecuted = value; }

  /**
   * @brief Gets whether ifStmt instruction has been executed.
   *
   * @return True if ifStmt has been executed.
   */
  bool getIfStmtExecuted() const { return _ifStmtExecuted; }

  /**
   * @brief Maps qubit indices to cbit indices.
   *
   * @param qubitIndex Qubit index.
   * @param resultIndex Classical bit index.
   */
  void addQubitResultMapping(const int qubitIndex, const int resultIndex) {
    _qubitResultMap.insert(std::make_pair(qubitIndex, resultIndex));
  }

  /**
   * @brief Gets qubit index that maps to a given cbit.
   *
   * @param resultIndex Classical bit index.
   *
   * @return Index of qubit that maps to resultIndex.
   */
  int getQubitIndex(const int resultIndex) const {
    return _qubitResultMap.find(resultIndex)->second;
  }

  /**
   * @brief Gets cbit index that qubit maps to.
   *
   * @param qubitIndex Qubit index.
   *
   * @return Index of cbit that qubitIndex maps to.
   */
  int getResultIndex(const int qubitIndex) const {
    return _qubitResultMap.find(qubitIndex)->first;
  }

  virtual ~ExecutionBackend() = default;
};

#endif