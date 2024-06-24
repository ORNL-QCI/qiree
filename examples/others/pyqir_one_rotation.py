from pyqir import BasicQisBuilder, SimpleModule, PointerType, IntType, Constant, const
import pyqir.rt as rt
from math import pi

module = SimpleModule("module", num_qubits=1, num_results=1)
qis = BasicQisBuilder(module.builder)

q0 = module.qubits[0]
r0 = module.results[0]

qis.h(q0)
qis.rx(pi/6, q0)
qis.mz(q0, r0)


i8p = PointerType(IntType(module.context, 8))

rt.array_record_output(
    module.builder, const(IntType(module.context, 64), 1), Constant.null(i8p)
)

rt.result_record_output(module.builder, r0, Constant.null(i8p))

if __name__ == "__main__":
    print(module.ir())
