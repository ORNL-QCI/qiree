# from pyqir import BasicQisBuilder, Builder, FunctionType, SimpleModule, PointerType, IntType, Constant, const
from pyqir import *
import pyqir.rt as rt
from math import pi

# Create the module with four qubits and three results.
module = SimpleModule("module", num_qubits=4, num_results=3)
qis = BasicQisBuilder(module.builder)

# Add instructions to the module to create a module pair and measure both qubits.
q0 = module.qubits[0]
q1 = module.qubits[1]
q2 = module.qubits[2]
q3 = module.qubits[3]

r0 = module.results[0]
r1 = module.results[1]
r2 = module.results[2]

qis.h(q0)
qis.h(q1)
qis.h(q2)
qis.x(q3)

qis.rz(pi/8, q0)
qis.rz(pi/8, q3)
qis.cx(q0, q3)
qis.rz(-pi/8, q3)
qis.cx(q0, q3)

qis.rz(pi/8, q1)
qis.rz(pi/8, q3)
qis.cx(q1, q3)
qis.rz(-pi/8, q3)
qis.cx(q1, q3)

qis.rz(pi/8, q1)
qis.rz(pi/8, q3)
qis.cx(q1, q3)
qis.rz(-pi/8, q3)
qis.cx(q1, q3)

qis.rz(pi/8, q2)
qis.rz(pi/8, q3)
qis.cx(q2, q3)
qis.rz(-pi/8, q3)
qis.cx(q2, q3)

qis.rz(pi/8, q2)
qis.rz(pi/8, q3)
qis.cx(q2, q3)
qis.rz(-pi/8, q3)
qis.cx(q2, q3)

qis.rz(pi/8, q2)
qis.rz(pi/8, q3)
qis.cx(q2, q3)
qis.rz(-pi/8, q3)
qis.cx(q2, q3)

qis.rz(pi/8, q2)
qis.rz(pi/8, q3)
qis.cx(q2, q3)
qis.rz(-pi/8, q3)
qis.cx(q2, q3)

qis.cx(q0, q2)
qis.cx(q2, q0)
qis.cx(q0, q2)

qis.h(q0)

qis.rz(-pi/4, q0)
qis.rz(-pi/4, q1)
qis.cx(q0, q1)
qis.rz(pi/4, q1)
qis.cx(q0, q1)

qis.h(q1)

qis.rz(-pi/8, q0)
qis.rz(-pi/8, q2)
qis.cx(q0, q2)
qis.rz(pi/8, q2)
qis.cx(q0, q2)

qis.rz(-pi/4, q1)
qis.rz(-pi/4, q2)
qis.cx(q1, q2)
qis.rz(pi/4, q2)
qis.cx(q1, q2)

qis.h(q2)

qis.mz(q0, r0)
qis.mz(q1, r1)
qis.mz(q2, r2)

i8p = PointerType(IntType(module.context, 8))
rt.array_record_output(
    module.builder, const(IntType(module.context, 64), 3), Constant.null(i8p)
)

rt.result_record_output(module.builder, r0, Constant.null(i8p))
rt.result_record_output(module.builder, r1, Constant.null(i8p))
rt.result_record_output(module.builder, r2, Constant.null(i8p))


if __name__ == "__main__":
    print(module.ir())
