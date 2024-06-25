#include <iostream>
#include <xacc.hpp>

#include "../src/GlobalMappingAssociator.hpp"

int main(int argc, char** argv)
{
    xacc::Initialize(argc, argv);
    xacc::set_verbose(true);
    QuantumIRParser parser;
    std::string const llvmFilePath = "pyqir_teleport.ll";

    auto extractedInstructions = parser.extractInstructions(llvmFilePath);

    // Print out the extracted instructions for validation
    std::cout << "Extracted Instructions:" << std::endl;
    for (auto const& inst : extractedInstructions)
    {
        std::cout << "Instruction: " << inst.name << std::endl;
        std::cout << "Qubits: ";
        std::vector<std::size_t> qubitsForInstruction(inst.qubits.begin(),
                                                      inst.qubits.end());
        for (auto const& q : qubitsForInstruction)
        {
            std::cout << q << " ";
        }
        std::cout << std::endl;
        std::cout << "Classical Registers: ";
        for (auto const& p : inst.readouts)
        {
            std::cout << p << " ";
        }
        std::cout << std::endl << std::endl;
        std::cout << "Parameters: ";
        for (auto const& p : inst.parameters)
        {
            std::cout << p << " ";
        }
        std::cout << std::endl << std::endl;
    }

    // Create XACC instructions based on extracted instructions
    std::vector<std::string> instNames;
    for (auto const& inst : extractedInstructions)
    {
        instNames.push_back(inst.name);
    }
    auto xaccInstructions
        = parser.createXaccInstructions(extractedInstructions);
    // Print out the created XACC instructions
    std::cout << "XACC Instructions:" << std::endl;
    for (auto const& inst : xaccInstructions)
    {
        std::cout << inst->toString() << std::endl;
    }

    return 0;
}
