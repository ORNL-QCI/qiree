#include "GlobalMappingAssociator.hpp"
#include "Singleton.hpp"
#include <iostream>
#include <string>

// Hash function for pair of objects
template <class T1, class T2>
std::size_t _qis_pair_hash::operator () (const std::pair<T1,T2>& p) const {
    auto h1 = std::hash<T1>{}(p.first);
    auto h2 = std::hash<T2>{}(p.second); 
    return h1 ^ h2;
}

// Extract qubit index from Qubit pointer
int extractQubitIndex(Qubit* qubit) {
    return qubit ? reinterpret_cast<intptr_t>(qubit) : 0;
}

// Extract result index from Result pointer
int extractResultIndex(Result* result) {
    return result ? reinterpret_cast<intptr_t>(result) : 0;
}

// Add a quantum gate instruction to the ansatz
void addInstructionToAnsatz(const std::string& gate, std::vector<unsigned long> qubitIndices) {
    auto& ansatz = Singleton::Instance().getAnsatz();
    auto& provider = Singleton::Instance().getProvider();
    auto& ifStmt = Singleton::Instance().getIfStmt();
    bool ifStmtExecuted = Singleton::Instance().getIfStmtExecuted();
    if(ifStmtExecuted){
        ifStmt->addInstruction(provider->createInstruction(gate, qubitIndices));
    }
    else {
        ansatz->addInstruction(provider->createInstruction(gate, qubitIndices));
    }
}

// Definitions for different quantum instructions
void quantum__qis__h__body(Qubit* qubit){
    addInstructionToAnsatz("H", {static_cast<unsigned long>(extractQubitIndex(qubit))});
}

void quantum__qis__cnot__body(Qubit* qubit1, Qubit* qubit2){
    addInstructionToAnsatz("CNOT", {static_cast<unsigned long>(extractQubitIndex(qubit1)), static_cast<unsigned long>(extractQubitIndex(qubit2))});
}

void quantum__qis__x__body(Qubit* qubit){
    addInstructionToAnsatz("X", {static_cast<unsigned long>(extractQubitIndex(qubit))});
}

void quantum__qis__y__body(Qubit* qubit){
    addInstructionToAnsatz("Y", {static_cast<unsigned long>(extractQubitIndex(qubit))});
}

void quantum__qis__z__body(Qubit* qubit){
    addInstructionToAnsatz("Z", {static_cast<unsigned long>(extractQubitIndex(qubit))});
}

void quantum__qis__t__body(Qubit* qubit){
    addInstructionToAnsatz("T", {static_cast<unsigned long>(extractQubitIndex(qubit))});
}

void quantum__qis__reset__body(Qubit* qubit){
    //addInstructionToAnsatz("Reset", {static_cast<unsigned long>(extractQubitIndex(qubit))});
}

void quantum__qis__mz__body(Qubit* qubit, Qubit* result){
    auto& ansatz = Singleton::Instance().getAnsatz();
    auto& provider = Singleton::Instance().getProvider();
    Singleton::Instance().addQubitResultMapping(extractQubitIndex(qubit), extractQubitIndex(result));
    auto measure = provider->createInstruction("Measure", {static_cast<unsigned long>(extractQubitIndex(qubit))}, {extractQubitIndex(result)});
    ansatz->addInstruction(measure);
}

// local rotations

void quantum__qis__rx__body(double angle, Qubit* qubit){
    addInstructionToAnsatz("Rx", {static_cast<unsigned long>(extractQubitIndex(qubit))});
}

void quantum__qis__ry__body(double angle, Qubit* qubit){
    addInstructionToAnsatz("Ry", {static_cast<unsigned long>(extractQubitIndex(qubit))});
}

void quantum__qis__rz__body(double angle, Qubit* qubit){
    addInstructionToAnsatz("Rz", {static_cast<unsigned long>(extractQubitIndex(qubit))});
}



// Class constructor that initializes the mapping of quantum operations to functions
GlobalMappingAssociator::GlobalMappingAssociator() {
    addMapping("__quantum__qis__h__body", 1, &quantum__qis__h__body);
    addMapping("__quantum__qis__cnot__body", 2, (void(*)(Qubit*))&quantum__qis__cnot__body);
    addMapping("__quantum__qis__x__body", 1, &quantum__qis__x__body);
    addMapping("__quantum__qis__y__body", 1, &quantum__qis__y__body);
    addMapping("__quantum__qis__z__body", 1, &quantum__qis__z__body);
    addMapping("__quantum__qis__t__body", 1, &quantum__qis__t__body);
    addMapping("__quantum__qis__mz__body", 2, (void(*)(Qubit*))&quantum__qis__mz__body);
    addMapping("__quantum__qis__reset__body", 1, &quantum__qis__reset__body);
    addMapping("__quantum__qis__rx__body", 2, (void(*)(Qubit*))&quantum__qis__rx__body);
    addMapping("__quantum__qis__ry__body", 2, (void(*)(Qubit*))&quantum__qis__ry__body);
    addMapping("__quantum__qis__rz__body", 2, (void(*)(Qubit*))&quantum__qis__rz__body);
    // ...
}

// Adds a new mapping

void GlobalMappingAssociator::addMapping(const std::string& name, std::int16_t argCount, void(*func)(double)) {
    quantum_rotation_map[{name, argCount}] = func;
}

void GlobalMappingAssociator::addMapping(const std::string& name, std::int16_t argCount, void(*func)(Qubit*)) {
    quantum_operation_map[{name, argCount}] = func;
}


void* GlobalMappingAssociator::getMapping(const std::string& name, std::int16_t argCount) const {
    auto it = quantum_operation_map.find({name, argCount});
    return (it != quantum_operation_map.end()) ? (void*)it->second : nullptr;

    auto it_rot = quantum_rotation_map.find({name, argCount});
    return (it_rot != quantum_rotation_map.end()) ? (void*)it_rot->second: nullptr;
}