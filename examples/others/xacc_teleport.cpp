
#include "xacc.hpp"

int main(int argc, char **argv) {
    xacc::Initialize(argc, argv);
    xacc::set_verbose(true);
    // Get reference to the Accelerator
    auto accelerator =  xacc::getAccelerator("aer");

    // Allocate some qubits
    auto buffer = xacc::qalloc(3);
    // Create a classical buffer to store measurement results (for conditional)
    buffer->setName("q");
    xacc::storeBuffer(buffer);
    auto xasmCompiler = xacc::getCompiler("xasm");
    auto ir = xasmCompiler->compile(R"(__qpu__ void teleport(qbit q) {
        // State preparation (Bob)
        Rx(q[0], 3.141592653589793);
        // Bell channel setup
        H(q[1]);
        CX(q[1], q[2]);
        // Alice Bell measurement
        CX(q[0], q[1]);
        H(q[0]);
        Measure(q[0]);
        Measure(q[1]);
        // Correction
        if (q[0])
        {
            Z(q[2]);
        }
        if (q[1])
        {
            X(q[2]);
        }
        // Measure teleported qubit
        Measure(q[2]);
    })", accelerator);
    
    // Printing out the quantum circuit's instructions
    std::cout << "Quantum Circuit Instructions:\n";
    std::cout << ir->getComposites()[0]->toString() << std::endl;
    
    accelerator->updateConfiguration({std::make_pair("shots", 1024)});
    
    accelerator->execute(buffer, ir->getComposites()[0]);
    
    int outputQubit2 = buffer->getMarginalCounts({2},xacc::AcceleratorBuffer::BitOrder::LSB)[std::string("1")];
    std::cout<<"measuring the third qubit at state 1  "<< outputQubit2 <<std::endl;
    
    int outputQubit3 = buffer->getMarginalCounts({2},xacc::AcceleratorBuffer::BitOrder::LSB)[std::string("0")];
    std::cout<<"measuring the third qubit at state 0  "<< outputQubit3 <<std::endl;
    
    buffer->print();

    xacc::Finalize();

    return 0;
}
