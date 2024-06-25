#ifndef GLOBAL_MAPPING_ASSOCIATOR_HPP
#define GLOBAL_MAPPING_ASSOCIATOR_HPP

#include <unordered_map>
#include <string>
#include <functional>
#include "ExecutionBackend.hpp"

/**
 * @brief Represents a Qubit.
 */
struct Qubit {
  int index;
};

/**
 * @brief Represents a Result (cbit).
 */
struct Result {
  int index;
};

/**
 * @brief Extracts the qubit index from a Qubit pointer.
 *
 * @param qubit Qubit pointer.
 * @return Extracted qubit index.
 */
int extractQubitIndex(Qubit *qubit);

/**
 * @brief Extracts the cbit index from a Result pointer.
 *
 * @param result Result pointer.
 * @return Extracted cbit index.
 */
int extractResultIndex(Result *result);

/**
 * @brief Hash function for pairs of objects.
 */
struct _qis_pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &p) const;
};

/**
 * @brief Class handling mapping of LLVM functions to backend native
 * instructions.
 */
class GlobalMappingAssociator {
public:
  GlobalMappingAssociator();

  /**
   * @brief Maps LLVM function to backend instruction.
   *
   * @param name Name of the LLVM function.
   * @param argCount Number of arguments.
   * @param func Function pointer to backend instruction.
   */
  void addMapping(const std::string &name, std::int16_t argCount,
                  void (*func)(Qubit *));

  /**
   * @brief Maps LLVM function to backend instruction.
   *
   * @param name Name of the LLVM function.
   * @param argCount Number of arguments.
   * @param func Function pointer to backend instruction.
   */
  void addMapping(const std::string &name, std::int16_t argCount,
                  void (*func)(double));

  /**
   * @brief Retrieves the function pointer associated with a mapping.
   *
   * @param name Name of the LLVM function.
   * @param argCount Number of arguments.
   * @return Function pointer for backend instruction. nullptr instruction is
   * not found.
   */
  void *getMapping(const std::string &name, std::int16_t argCount) const;

private:
  /**
   * @brief Mapping for quantum operations with Qubit arguments.
   */
  std::unordered_map<std::pair<std::string, std::int16_t>, void (*)(Qubit *),
                     _qis_pair_hash>
      quantum_operation_map;

  /**
   * @brief Mapping for quantum rotations with double arguments.
   */
  std::unordered_map<std::pair<std::string, std::int16_t>, void (*)(double),
                     _qis_pair_hash>
      quantum_rotation_map;
};

#endif
