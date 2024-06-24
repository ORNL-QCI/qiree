#include "xacc.hpp"

int main(int argc, char **argv) {
    xacc::Initialize(argc, argv);
    xacc::set_verbose(true);

    auto accelerator = xacc::getAccelerator("aer");
    auto accelerator2 = xacc::getAccelerator("aer");
    auto provider = xacc::getIRProvider("quantum");

    auto buffer = xacc::qalloc(3);
    buffer->setName("q");

    auto teleport = provider->createComposite("teleport");
    
    // State preparation (Bob)
    auto rx0 = provider->createInstruction("Rx", {0}, {3.141592653589793});
    teleport->addInstruction(rx0);

    // Bell channel setup
    auto h1 = provider->createInstruction("H", {1});
    teleport->addInstruction(h1);
    
    auto cx1_2 = provider->createInstruction("CX", {1, 2});
    teleport->addInstruction(cx1_2);
    
    // Alice Bell measurement of the first qubit
    auto cx0_1 = provider->createInstruction("CX", {0, 1});
    teleport->addInstruction(cx0_1);
    
    auto h0 = provider->createInstruction("H", {0});
    teleport->addInstruction(h0);
    
    auto measure0 = provider->createInstruction("Measure", {0}, {0});
    auto measure1 = provider->createInstruction("Measure", {1}, {1});

    teleport->addInstruction(measure0);
    teleport->addInstruction(measure1);

    auto z2 = provider->createInstruction("Z", {2});
    auto ifstmt1 = xacc::ir::asComposite(provider->createInstruction("ifstmt", {0}, {"q"}));
    teleport->addInstruction(ifstmt1);
    // print the boolean value of the measurement
    ifstmt1->addInstruction(z2);

    auto x2 = provider->createInstruction("X", {2});
    auto ifstmt2 = xacc::ir::asComposite(provider->createInstruction("ifstmt", {1}, {"q"}));
    teleport->addInstruction(ifstmt2);
    ifstmt2->addInstruction(x2);
    
    auto measure2 = provider->createInstruction("Measure", {2}, {2});
    teleport->addInstruction(measure2);
    
    accelerator->updateConfiguration({std::make_pair("shots", 1024)});
    accelerator->execute(buffer, teleport);
    
    // print the quantum circuit to be executed
    std::cout<<"printing the quantum circuit to be executed"<<std::endl;
    std::cout<<teleport->toString()<<std::endl;

    xacc::Finalize();

    return 0;
}