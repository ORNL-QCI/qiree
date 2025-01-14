# Simple Demo for Catalyst/Lightning runtime

This is a super simple demo for using Catalyst runtime to drive Lightning devices. The example here uses `lightning.kokkos`, but can easily be updated to target other devices, e.g. lightning.gpu (if an Nvidia GPU is present). 

The new files required are in `../catalyst_runtime`, which contains a subset of files from the [Catalyst Runtime](https://github.com/PennyLaneAI/catalyst/tree/main/runtime).

## Installing a lightning simulator

When installing [Pennylane-Lightning](https://github.com/PennyLaneAI/pennylane-lightning) from pip or source, you will have the shared objects for each of the simulator installed. These are named `liblightning_kokkos_catalyst.so`/`liblightning_GPU_catalyst.so` etc. 

Running `pip install pennylane` or `pip install pennylane-lightning` will install the `lightning.qubit` (CPU) simulator, and other simulators can be installed by running `pip install pennylane-lightning-kokkos / pennylane-lightning-gpu`.

Example:
```
$ pip install pennylane-lightning-kokkos

$ pip show pennylane-lightning-kokkos
Name: PennyLane_Lightning_Kokkos
Version: 0.39.0
Summary: PennyLane-Lightning plugin
Home-page: https://github.com/PennyLaneAI/pennylane-lightning
Author: 
Author-email: 
License: Apache License 2.0
Location: /home/joseph/work/qiree/venv-qiree/lib/python3.10/site-packages
Requires: pennylane, pennylane-lightning

$ ls /home/joseph/work/qiree/venv-qiree/lib/python3.10/site-packages/pennylane_lightning
... liblightning_kokkos_catalyst.so ...
```

You can swap `pennylane-lightning-kokkos` for `pennylane-lightning-gpu` for lightning.gpu and `pennylane-lightning` for lightning.gpu simulators.

## Compilation

To compile:

```
$ clang++ --std=c++20 test_rt_device.cpp -I/home/joseph/work/qiree/catalyst/runtime/include -I/home/joseph/work/qiree/catalyst/runtime/lib/capi -I/home/joseph/work/qiree/catalyst/runtime/lib/backend/common -o test_rt_device.out
```

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

To run on other devices, e.g. lightning.gpu, you need to change:
- `pip install custatevec-cu12 pennylane-lightning-gpu` (custatevec is a dependency)
- replace `RTDLIB` and `RTDNAME` from `kokkos` to `GPU`
- include `cuquantum` libraries when running, e.g. `LD_LIBRARY_PATH=/home/joseph/work/qiree/venv-qiree/lib/python3.10/site-packages/cuquantum/lib/:$LD_LIBRARY_PATH ./test_rt_device.out`
