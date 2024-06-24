# from pyqir import BasicQisBuilder, Builder, FunctionType, SimpleModule, PointerType, IntType, Constant, const
from pyqir import *
import pyqir.rt as rt
from math import pi

# Create the module with six qubits and five results.
module = SimpleModule("module", num_qubits=6, num_results=5)
qis = BasicQisBuilder(module.builder)

# Add instructions to the module to create a module pair and measure both qubits.
q0 = module.qubits[0]
q1 = module.qubits[1]
q2 = module.qubits[2]
q3 = module.qubits[3]
q4 = module.qubits[4]
q5 = module.qubits[5]

r0 = module.results[0]
r1 = module.results[1]
r2 = module.results[2]
r3 = module.results[3]
r4 = module.results[4]

qis.x(q5)

qis.h(q0)
qis.h(q1)
qis.h(q2)
qis.h(q3)
qis.h(q4)
qis.h(q5)

qis.cx(q1, q5)
qis.cx(q3, q5)
qis.cx(q4, q5)

qis.h(q0)
qis.h(q1)
qis.h(q2)
qis.h(q3)
qis.h(q4)
qis.h(q5)

qis.mz(q0, r0)
qis.mz(q1, r1)
qis.mz(q2, r2)
qis.mz(q3, r3)
qis.mz(q4, r4)

i8p = PointerType(IntType(module.context, 8))
rt.array_record_output(
    module.builder, const(IntType(module.context, 64), 5), Constant.null(i8p)
)

rt.result_record_output(module.builder, r0, Constant.null(i8p))
rt.result_record_output(module.builder, r1, Constant.null(i8p))
rt.result_record_output(module.builder, r2, Constant.null(i8p))
rt.result_record_output(module.builder, r3, Constant.null(i8p))
rt.result_record_output(module.builder, r4, Constant.null(i8p))

if __name__ == "__main__":
    print(module.ir())
