#include <dlfcn.h>

#include "QuantumDevice.hpp"

// Runtime libraries (kokkos/GPU/qubit etc.)
// Update these paths to point to the correct library
#define RTDLIB                                                         \
    "<UPDATE: site packages path>/" \
    "pennylane_lightning/liblightning_kokkos_catalyst.so";
#define RTDDEVICE "LightningKokkosSimulator";

extern "C" Catalyst::Runtime::QuantumDevice*
GenericDeviceFactory(char const* kwargs);

using namespace Catalyst::Runtime;

int main()
{
    try
    {
        // Load lightning simulation library
        std::string rtd_lib = RTDLIB;
        std::string rtd_device = RTDDEVICE;
        std::string kwargs = {};
        auto rtld_flags = RTLD_LAZY | RTLD_NODELETE;
        auto rtd_dylib_handler = dlopen(rtd_lib.c_str(), rtld_flags);

        if (!rtd_dylib_handler)
        {
            throw std::runtime_error("Failed to load library: " + rtd_lib);
        }

        // Find device factory
        std::string factory_name = rtd_device + "Factory";
        void* f_ptr = dlsym(rtd_dylib_handler, factory_name.c_str());

        if (!f_ptr)
        {
            dlclose(rtd_dylib_handler);
            throw std::runtime_error("Failed to find factory function: "
                                     + factory_name);
        }
        std::string rtd_kwargs = {};
        auto rtd_qdevice = std::unique_ptr<QuantumDevice>(
            reinterpret_cast<decltype(GenericDeviceFactory)*>(f_ptr)(
                rtd_kwargs.c_str()));

        // Allocate Qubits
        rtd_qdevice->AllocateQubits(3);

        // Get Num Qubits
        std::cout << "Num Qubits = " << rtd_qdevice->GetNumQubits()
                  << std::endl;

        // Apply Gate
        rtd_qdevice->NamedOperation("Hadamard", {}, {0});

        // Print State
        std::cout << "State = " << std::endl;
        rtd_qdevice->PrintState();

        // Measure
        QubitIdType wire{0};
        Result result = rtd_qdevice->Measure(wire, std::nullopt);
        std::cout << "Measure on wire 0 = " << *result << std::endl;
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
