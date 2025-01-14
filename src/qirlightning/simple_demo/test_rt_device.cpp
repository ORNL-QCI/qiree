#include "ExecutionContext.hpp"

// Runtime libraries (kokkos/GPU/qubit etc.)
#define RTDLIB "/home/joseph/work/qiree/venv-qiree/lib/python3.10/site-packages/pennylane_lightning/liblightning_kokkos_catalyst.so" // change to liblightning_gpu_catalyst.so
#define RTDNAME "LightningKokkosSimulator" // change to LightningGPUSimulator

using namespace Catalyst::Runtime;

static inline std::shared_ptr<RTDevice> loadRTDevice(const std::string &rtd_lib,
                                                   const std::string &rtd_name = {},
                                                   const std::string &rtd_kwargs = {})
{
    ExecutionContext context;
    return context.getOrCreateDevice(rtd_lib, rtd_name, rtd_kwargs);
}

int main() {
    auto RTDevice = loadRTDevice(RTDLIB, RTDNAME, "");

    // Allocate Qubits
    RTDevice->getQuantumDevicePtr()->AllocateQubits(3);

    // Get Num Qubits
    std::cout << "Num Qubits = " << RTDevice->getQuantumDevicePtr()->GetNumQubits() << std::endl;

    // Apply Gate
    RTDevice->getQuantumDevicePtr()->NamedOperation("Hadamard", {}, {0});

    // Print State
    std::cout << "State = " << std::endl;
    RTDevice->getQuantumDevicePtr()->PrintState();

    // Measure
    QubitIdType wire{0};
    Result result = RTDevice->getQuantumDevicePtr()->Measure(wire, std::nullopt);
    std::cout << "Measure on wire 0 = " << *result << std::endl;


    return 0;
}
