#include "../src/GlobalMappingAssociator.hpp"
#include <xacc.hpp>
#include <iostream>

int main(int argc, char **argv) {
    xacc::Initialize(argc, argv);
    xacc::set_verbose(true);
    QuantumIRParser parser;
    const std::string llvmFilePath = "pyqir_teleport.ll";

    auto extractedInstructions = parser.extractInstructions(llvmFilePath);
    
    // Print out the extracted instructions for validation
    std::cout << "Extracted Instructions:" << std::endl;
    for (const auto& inst : extractedInstructions) {
        std::cout << "Instruction: " << inst.name << std::endl;
        std::cout << "Qubits: ";
        std::vector<std::size_t> qubitsForInstruction(inst.qubits.begin(), inst.qubits.end());
        for (const auto& q : qubitsForInstruction) {
            std::cout << q << " ";
        }
        std::cout << std::endl;
        std::cout << "Classical Registers: ";
        for (const auto& p : inst.readouts) {
            std::cout << p << " ";
        }
        std::cout << std::endl << std::endl;
        std::cout << "Parameters: ";
        for (const auto& p : inst.parameters) {
            std::cout << p << " ";
        }
        std::cout << std::endl << std::endl;
    }

    // Create XACC instructions based on extracted instructions
    std::vector<std::string> instNames;
    for (const auto& inst : extractedInstructions) {
        instNames.push_back(inst.name);
    }
    auto xaccInstructions = parser.createXaccInstructions(extractedInstructions);
    // Print out the created XACC instructions
    std::cout << "XACC Instructions:" << std::endl;
    for (const auto& inst : xaccInstructions) {
        std::cout << inst->toString() << std::endl;
    }

    return 0;
}

