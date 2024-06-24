from pyqir import BasicQisBuilder, SimpleModule, PointerType, IntType, Constant, const
import pyqir.rt as rt
from math import pi

# Create the module with four qubits and four results.
module = SimpleModule("module", num_qubits=4, num_results=4)
qis = BasicQisBuilder(module.builder)

# Add instructions to the module to create a module pair and measure both qubits.
q0 = module.qubits[0]
q1 = module.qubits[1]
q2 = module.qubits[2]
q3 = module.qubits[3]

r0 = module.results[0]
r1 = module.results[1]
r2 = module.results[2]
r3 = module.results[3]



qis.h(q0)
qis.cx(q0, q1)
qis.cz(q0, q2)
qis.rx(pi/6, q1)
qis.ry(1.047, q2)
qis.rz(1.571, q3)
qis.s(q0)
qis.s_adj(q1)
qis.t(q2)
qis.t_adj(q3)
qis.x(q1)
qis.y(q2)
qis.z(q3)

qis.mz(q0, r0)
qis.mz(q1, r1)
qis.mz(q2, r2)
qis.mz(q3, r3)



i8p = PointerType(IntType(module.context, 8))
rt.array_record_output(
    module.builder, const(IntType(module.context, 64), 4), Constant.null(i8p)
)

rt.result_record_output(module.builder, r0, Constant.null(i8p))
rt.result_record_output(module.builder, r1, Constant.null(i8p))
rt.result_record_output(module.builder, r2, Constant.null(i8p))
rt.result_record_output(module.builder, r3, Constant.null(i8p))


if __name__ == "__main__":
    print(module.ir())