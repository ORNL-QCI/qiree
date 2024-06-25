// Wrote this example that use staq plugin

#include "xacc.hpp"

int main(int argc, char** argv)
{
    xacc::Initialize(argc, argv);
    auto compiler = xacc::getCompiler("staq");

    auto q = xacc::qalloc(5);
    q->setName("q");
    xacc::storeBuffer(q);
    auto IR = compiler->compile(R"(__qpu__ void f(qreg q) {
                                    OPENQASM 2.0;
                                    include "qelib1.inc";
                                    creg c[16];
                                    cx q[2],q[1];
                                    cx q[1],q[2];
                                    cx q[3],q[2];
                                    cx q[2],q[3];
                                    cx q[4],q[3];
                                    cx q[3],q[4];
                                    h q[0];
                                    t q[4];
                                    t q[3];
                                    t q[0];
                                    cx q[3],q[4];
                                    cx q[0],q[3];
                                    cx q[4],q[0];
                                    tdg q[3];
                                    })");

    auto ansatz = xacc::getCompiled("f");
    std::cout << ansatz->toString();

    std::cout << "code running successfully";

    return 0;
}
