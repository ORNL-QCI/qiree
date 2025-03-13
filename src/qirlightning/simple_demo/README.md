# Simple Demo for Catalyst/Lightning runtime

This is a super simple demo for driving Lightning devices. The example here uses `lightning.kokkos`, but can easily be updated to target other devices, e.g. lightning.gpu (if an Nvidia GPU is present). 

Some Catalyst include files are copied here for convenience - they are in `./snapshot_catalyst_runtime/include`. These are required for the QuantumDevice interface. For the qiree source, these files are fetched automatically during CMake, and these are not used.

## Installing a lightning simulator

When installing [Pennylane-Lightning](https://github.com/PennyLaneAI/pennylane-lightning) from pip or source, you will have the shared objects for each of the simulator installed. These are named `liblightning_kokkos_catalyst.so`/`liblightning_GPU_catalyst.so` etc. 

To get started, run `pip install pennylane` or `pip install pennylane-lightning` - this will install the `lightning.qubit` (CPU) simulator, and other simulators can be installed by running `pip install pennylane-lightning-kokkos / pennylane-lightning-gpu`.

Example:
```
$ pip install pennylane-lightning-kokkos

$ pip show pennylane-lightning-kokkos
Name: PennyLane_Lightning_Kokkos
Version: 0.40.0
Summary: PennyLane-Lightning plugin
Home-page: https://github.com/PennyLaneAI/pennylane-lightning
Author: 
Author-email: 
License: Apache License 2.0
Location: <site packages path>
Requires: pennylane, pennylane-lightning

$ ls <site packages path>/pennylane_lightning
... liblightning_kokkos_catalyst.so ...
```

You can swap `pennylane-lightning-kokkos` for `pennylane-lightning-gpu` for lightning.gpu and `pennylane-lightning` for lightning.gpu simulators.

## Compilation

To compile:

```
$ clang++ --std=c++20 test_rt_device.cpp -I./snapshot_catalyst_runtime/include -o test_rt_device.out
```

## Running the example

To run:

```
$ ./test_rt_device.out 
Kokkos::OpenMP::initialize WARNING: OMP_PROC_BIND environment variable not set
  In general, for best performance with OpenMP 4.0 or better set OMP_PROC_BIND=spread and OMP_PLACES=threads
  For best performance with OpenMP 3.1 set OMP_PROC_BIND=true
  For unit testing set OMP_PROC_BIND=false

Num Qubits = 3
State = 
*** State-Vector of Size 8 ***
[(0.707107,0), (0,0), (0,0), (0,0), (0.707107,0), (0,0), (0,0), (0,0)]
Measure on wire 0 = 0
```

## Running on other devices

To run on other devices, e.g. lightning.gpu, you need to change:
- Install pennylane-lightning-gpu: `pip install pennylane-lightning-gpu`
In the c++ file:
- replace `RTDLIB` from `kokkos` to `gpu`
- replace `RTDDEVICE` from `Kokkos` to `GPU`
- Include `cuquantum` libraries when running (which was installed as a dependency), i.e. `LD_LIBRARY_PATH=$(python -c "import site; print( f'{site.getsitepackages()[0]}/cuquantum')")/lib:$LD_LIBRARY_PATH ./test_rt_device.out`
