# QIR-EE with Lightning simulator backend

## Installing a lightning simulator

More information on installing Pennylane Lightning simulators can be found in [lightning repository](https://github.com/PennyLaneAI/pennylane-lightning).

### Quick start
The easiest way to get started is install a Lightning simulator from PyPI via pip:

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
Location: /home/joseph/work/qiree/venv-qiree/lib/python3.10/site-packages
Requires: pennylane, pennylane-lightning
```
Running `pip install pennylane` or `pip install pennylane-lightning` will install the `lightning.qubit` (CPU) simulator, and other simulators can be installed by running `pip install pennylane-lightning-kokkos / pennylane-lightning-gpu`.

When installing [Pennylane-Lightning](https://github.com/PennyLaneAI/pennylane-lightning) from pip or source, you will have the shared objects for each of the simulator installed. These are named `liblightning_qubit_catalyst.so`/`liblightning_kokkos_catalyst.so`/`liblightning_GPU_catalyst.so` respectively.

Example:
```
$ ls /home/joseph/work/qiree/venv-qiree/lib/python3.10/site-packages/pennylane_lightning
... liblightning_kokkos_catalyst.so ...
```

You can swap `pennylane-lightning-kokkos` for `pennylane-lightning-gpu` for lightning.gpu and `pennylane-lightning` for lightning.gpu simulators.

### Compiling Lightning from Source

The [lightning repository page](https://github.com/PennyLaneAI/pennylane-lightning) contains information on how to install Lightning simulators from source. This will necessary for e.g. Kokkos with HIP backend.

## Compile QIR-EE with Lightning backend

- Set `QIREE_USE_LIGHTNING` to `ON` in `qiree/CMakeLists.txt`
- Specify the simulator path and name in `qiree/src/qirlightning/CMakeLists`, set:
    - `RTDLIB_PATH` to the path of the simulator `.so`
    - `RTDDEVICE_NAME` to `LightningSimulator`/`LightningKokkosSimulator`/`LightningGPUSimulator`
These could also be set in cmake using the variables `-D...`

Note: when running on GPU, include `cuquantum` libraries in the library path (which will be installed as a dependency from Python), i.e. `LD_LIBRARY_PATH=$(python -c "import site; print( f'{site.getsitepackages()[0]}/cuquantum')")/lib:$LD_LIBRARY_PATH ./test_rt_device.out`

To compile:

```
cd qiree/
mkdir build; cd build
cmake ..
make

```

## Running the example

To run:

```
$ ./bin/qir-lightning ../examples/bell.ll -s 100
{"00":43,"11":57}
```

