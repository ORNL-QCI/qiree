#include "GlobalMappingAssociator.hpp"
#include "ExecutionBackendHandle.hpp"
#include <iostream>
#include <string>

double PI = 3.14159265358979323846;
// Hash function for pair of objects
template <class T1, class T2>
std::size_t _qis_pair_hash::operator()(const std::pair<T1, T2> &p) const {
  auto h1 = std::hash<T1>{}(p.first);
  auto h2 = std::hash<T2>{}(p.second);
  return h1 ^ h2;
}

// Extract qubit index from Qubit pointer
int extractQubitIndex(Qubit *qubit) {
  return qubit ? reinterpret_cast<intptr_t>(qubit) : 0;
}

// Extract result index from Result pointer
int extractResultIndex(Result *result) {
  return result ? reinterpret_cast<intptr_t>(result) : 0;
}

// Definitions for different quantum instructions

// local operations
void quantum__qis__h__body(Qubit *qubit) {
  SingletonHandle::Instance().addInstructionToAnsatz(
      "H", {static_cast<unsigned long>(extractQubitIndex(qubit))});
}

void quantum__qis__x__body(Qubit *qubit) {
  SingletonHandle::Instance().addInstructionToAnsatz(
      "X", {static_cast<unsigned long>(extractQubitIndex(qubit))});
}

void quantum__qis__y__body(Qubit *qubit) {
  SingletonHandle::Instance().addInstructionToAnsatz(
      "Y", {static_cast<unsigned long>(extractQubitIndex(qubit))});
}

void quantum__qis__z__body(Qubit *qubit) {
  SingletonHandle::Instance().addInstructionToAnsatz(
      "Z", {static_cast<unsigned long>(extractQubitIndex(qubit))});
}

void quantum__qis__t__body(Qubit *qubit) {
  // this is a pi/4 rotation around Z
  SingletonHandle::Instance().addParametrizedInstructionToAnsatz(
      "Rz", {static_cast<unsigned long>(extractQubitIndex(qubit))}, PI / 4);
}

void quantum__qis__t__adj(Qubit *qubit) {
  // this is a -pi/4 rotation around Z
  SingletonHandle::Instance().addParametrizedInstructionToAnsatz(
      "Rz", {static_cast<unsigned long>(extractQubitIndex(qubit))}, -PI / 4);
}

void quantum__qis__s__body(Qubit *qubit) {
  // this is a pi/2 rotation around Z
  SingletonHandle::Instance().addParametrizedInstructionToAnsatz(
      "Rz", {static_cast<unsigned long>(extractQubitIndex(qubit))}, PI / 2);
}

void quantum__qis__s__adj(Qubit *qubit) {
  // this is a -pi/2 rotation around Z
  SingletonHandle::Instance().addParametrizedInstructionToAnsatz(
      "Rz", {static_cast<unsigned long>(extractQubitIndex(qubit))}, -PI / 2);
}

void quantum__qis__reset__body(Qubit *qubit) {
  SingletonHandle::Instance().addInstructionToAnsatz(
      "Reset", {static_cast<unsigned long>(extractQubitIndex(qubit))});
}

void quantum__qis__mz__body(Qubit *qubit, Qubit *result) {
  auto qubitIndex = extractQubitIndex(qubit);
  auto resultIndex = extractQubitIndex(result);
  SingletonHandle::Instance().addMeasure(qubitIndex, resultIndex);
}

// local rotations

void quantum__qis__rx__body(double angle, Qubit *qubit) {
  SingletonHandle::Instance().addParametrizedInstructionToAnsatz(
      "Rx", {static_cast<unsigned long>(extractQubitIndex(qubit))}, angle);
}

void quantum__qis__ry__body(double angle, Qubit *qubit) {
  SingletonHandle::Instance().addParametrizedInstructionToAnsatz(
      "Ry", {static_cast<unsigned long>(extractQubitIndex(qubit))}, angle);
}

void quantum__qis__rz__body(double angle, Qubit *qubit) {
  SingletonHandle::Instance().addParametrizedInstructionToAnsatz(
      "Rz", {static_cast<unsigned long>(extractQubitIndex(qubit))}, angle);
}

void quantum__qis__rzz__body(double angle, Qubit *qubit1, Qubit *qubit2) {
  SingletonHandle::Instance().addParametrizedInstructionToAnsatz(
      "RZZ",
      {static_cast<unsigned long>(extractQubitIndex(qubit1)),
       static_cast<unsigned long>(extractQubitIndex(qubit2))},
      angle);
}

// non local operations

void quantum__qis__cnot__body(Qubit *qubit1, Qubit *qubit2) {
  SingletonHandle::Instance().addInstructionToAnsatz(
      "CNOT", {static_cast<unsigned long>(extractQubitIndex(qubit1)),
               static_cast<unsigned long>(extractQubitIndex(qubit2))});
}

void quantum__qis__swap__body(Qubit *qubit1, Qubit *qubit2) {
  // compile swap operation into cnots
  SingletonHandle::Instance().addInstructionToAnsatz(
      "CNOT", {static_cast<unsigned long>(extractQubitIndex(qubit1)),
               static_cast<unsigned long>(extractQubitIndex(qubit2))});
  SingletonHandle::Instance().addInstructionToAnsatz(
      "CNOT", {static_cast<unsigned long>(extractQubitIndex(qubit2)),
               static_cast<unsigned long>(extractQubitIndex(qubit1))});
  SingletonHandle::Instance().addInstructionToAnsatz(
      "CNOT", {static_cast<unsigned long>(extractQubitIndex(qubit1)),
               static_cast<unsigned long>(extractQubitIndex(qubit2))});
}

void quantum__qis__ccnot__body(Qubit *qubit1, Qubit *qubit2, Qubit *qubit3) {
  SingletonHandle::Instance().addInstructionToAnsatz(
      "CCNOT", {static_cast<unsigned long>(extractQubitIndex(qubit1)),
                static_cast<unsigned long>(extractQubitIndex(qubit2)),
                static_cast<unsigned long>(extractQubitIndex(qubit3))});
}

void quantum__qis__cz__body(Qubit *qubit1, Qubit *qubit2) {
  SingletonHandle::Instance().addInstructionToAnsatz(
      "CZ", {static_cast<unsigned long>(extractQubitIndex(qubit1)),
             static_cast<unsigned long>(extractQubitIndex(qubit2))});
}

void quantum__qis__cy__body(Qubit *qubit1, Qubit *qubit2) {
  SingletonHandle::Instance().addInstructionToAnsatz(
      "CY", {static_cast<unsigned long>(extractQubitIndex(qubit1)),
             static_cast<unsigned long>(extractQubitIndex(qubit2))});
}

void quantum__qis__cx__body(Qubit *qubit1, Qubit *qubit2) {
  SingletonHandle::Instance().addInstructionToAnsatz(
      "CX", {static_cast<unsigned long>(extractQubitIndex(qubit1)),
             static_cast<unsigned long>(extractQubitIndex(qubit2))});
}
// Class constructor that initializes the mapping of quantum operations to functions
GlobalMappingAssociator::GlobalMappingAssociator() {
    addMapping("__quantum__qis__h__body", 1, &quantum__qis__h__body);
    addMapping("__quantum__qis__x__body", 1, &quantum__qis__x__body);
    addMapping("__quantum__qis__y__body", 1, &quantum__qis__y__body);
    addMapping("__quantum__qis__z__body", 1, &quantum__qis__z__body);
    addMapping("__quantum__qis__t__body", 1, &quantum__qis__t__body);
    addMapping("__quantum__qis__t__adj", 1, &quantum__qis__t__adj);
    addMapping("__quantum__qis__s__body", 1, &quantum__qis__s__body);
    addMapping("__quantum__qis__s__adj", 1, &quantum__qis__s__adj);
    addMapping("__quantum__qis__mz__body", 2, (void(*)(Qubit*))&quantum__qis__mz__body);
    addMapping("__quantum__qis__reset__body", 1, &quantum__qis__reset__body);
    addMapping("__quantum__qis__rx__body", 2, (void(*)(Qubit*))&quantum__qis__rx__body);
    addMapping("__quantum__qis__ry__body", 2, (void(*)(Qubit*))&quantum__qis__ry__body);
    addMapping("__quantum__qis__rz__body", 2, (void(*)(Qubit*))&quantum__qis__rz__body);
    addMapping("__quantum__qis__cnot__body", 2, (void(*)(Qubit*))&quantum__qis__cnot__body);
    addMapping("__quantum__qis__swap__body", 2, (void(*)(Qubit*))&quantum__qis__swap__body);
    addMapping("__quantum__qis__ccnot__body", 3, (void(*)(Qubit*))&quantum__qis__ccnot__body);
    addMapping("__quantum__qis__cz__body", 2, (void(*)(Qubit*))&quantum__qis__cz__body);
    addMapping("__quantum__qis__cy__body", 2, (void(*)(Qubit*))&quantum__qis__cy__body);
    addMapping("__quantum__qis__cx__body", 2, (void(*)(Qubit*))&quantum__qis__cx__body);
    addMapping("__quantum__qis__rzz__body", 3, (void(*)(Qubit*))&quantum__qis__rzz__body);
    // ...
}

// Adds a new mapping

void GlobalMappingAssociator::addMapping(const std::string &name,
                                         std::int16_t argCount,
                                         void (*func)(double)) {
  quantum_rotation_map[{name, argCount}] = func;
}

void GlobalMappingAssociator::addMapping(const std::string &name,
                                         std::int16_t argCount,
                                         void (*func)(Qubit *)) {
  quantum_operation_map[{name, argCount}] = func;
}

void *GlobalMappingAssociator::getMapping(const std::string &name,
                                          std::int16_t argCount) const {
  auto it = quantum_operation_map.find({name, argCount});
  return (it != quantum_operation_map.end()) ? (void *)it->second : nullptr;

  auto it_rot = quantum_rotation_map.find({name, argCount});
  return (it_rot != quantum_rotation_map.end()) ? (void *)it_rot->second
                                                : nullptr;
}