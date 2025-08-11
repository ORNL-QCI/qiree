# from pyqir import BasicQisBuilder, Builder, FunctionType, SimpleModule, PointerType, IntType, Constant, const
from pyqir import *
import pyqir.rt as rt

module = SimpleModule("ghz", num_qubits=5, num_results=5)
qis = BasicQisBuilder(module.builder)

qis.h(module.qubits[0])

for i in range(4):
    qis.cx(module.qubits[i], module.qubits[i+1])

for i in range(5):
    qis.mz(module.qubits[i], module.results[i])

i8p = PointerType(IntType(module.context, 8))
rt.array_record_output(
    module.builder, const(IntType(module.context, 64), 5), Constant.null(i8p)
)

for i in range(5):
    rt.result_record_output(module.builder, module.results[i], Constant.null(i8p))

if __name__ == "__main__":
    print(module.ir())
