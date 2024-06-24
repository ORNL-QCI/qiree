# from pyqir import BasicQisBuilder, Builder, FunctionType, SimpleModule, PointerType, IntType, Constant, const
from pyqir import *
import pyqir.rt as rt
from math import pi

module = SimpleModule("module", num_qubits=4, num_results=3)
qis = BasicQisBuilder(module.builder)

# Fix num_qubits and num_results here:
nq=4
nr=3

# Apply h-gates to counting qubits:

for i in range(nq-1):
    qis.h(module.qubits[i])

# Prepare our eigenstate |psi>:

qis.x(module.qubits[nq-1])

# Do the CU operations:

repetitions = 1
for counting_qubit in range(nq-1):
    for i in range(repetitions):
        # Each round is P(Pi/4)
        qis.rz(pi/8, module.qubits[counting_qubit])
        qis.rz(pi/8, module.qubits[nq-1])
        qis.cx(module.qubits[counting_qubit], module.qubits[nq-1])
        qis.rz(-pi/8, module.qubits[nq-1])
        qis.cx(module.qubits[counting_qubit], module.qubits[nq-1])
    repetitions *= 2

# Set up qft_dagger:

def qft_dagger(n):
    # Don't forget the Swaps!
    for qubit in range(n//2):
        qis.cx(module.qubits[qubit], module.qubits[n-qubit])
        qis.cx(module.qubits[n-qubit], module.qubits[qubit])
        qis.cx(module.qubits[qubit], module.qubits[n-qubit])
    # Apply CU operations:
    for j in range(n):
        qis.h(module.qubits[j])
        for m in range(j+1):
            # Each round is P(-pi/float(2**(j-m+1)))
            # qc.cp(-pi/(2*float(2**(j-m))), m, j)
            theta = -pi/(2*float(2**(j-m+1)))
            qis.rz(theta, module.qubits[m])
            qis.rz(theta, module.qubits[j+1])
            qis.cx(module.qubits[m], module.qubits[j+1])
            qis.rz(-theta, module.qubits[j+1])
            qis.cx(module.qubits[m], module.qubits[j+1])
    qis.h(module.qubits[n])

# Call qft_dagger:

qft_dagger(nr-1)

# Measure:

for i in range(nr):
    qis.mz(module.qubits[i], module.results[i])

# Print results:

i8p = PointerType(IntType(module.context, 8))
rt.array_record_output(module.builder, const(IntType(module.context, 64), nq-1), Constant.null(i8p))

for i in range(nr):
    rt.result_record_output(module.builder, module.results[i], Constant.null(i8p))

if __name__ == "__main__":
    print(module.ir())

