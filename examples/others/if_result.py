# Copyright (c) Microsoft Corporation.
# Licensed under the MIT License.

from pyqir import BasicQisBuilder, SimpleModule, PointerType, IntType, Constant, const
import pyqir.rt as rt

mod = SimpleModule("if_result", num_qubits=2, num_results=2)
qis = BasicQisBuilder(mod.builder)

# Manually reset a qubit by measuring it and applying the X gate if the result
# is one.
qis.h(mod.qubits[0])
qis.mz(mod.qubits[0], mod.results[0])
qis.if_result(mod.results[0], lambda: qis.x(mod.qubits[0]))

# Branches can be nested, for example, to execute an instruction only if both
# results are one.
for i in range(2):
    qis.h(mod.qubits[i])
    qis.mz(mod.qubits[i], mod.results[i])

def x_both() -> None:
    qis.x(mod.qubits[0])
    qis.x(mod.qubits[1])

qis.if_result(mod.results[0], lambda: qis.if_result(mod.results[1], x_both))

# You can also add instructions that will execeute only when the result is zero.
qis.if_result(mod.results[0], zero=lambda: qis.x(mod.qubits[0]))

# In general, you can provide both the one and zero branches at the same time.
qis.if_result(
    mod.results[0],
    one=lambda: qis.z(mod.qubits[0]),
    zero=lambda: qis.y(mod.qubits[0]),
)

qis.mz(mod.qubits[0], mod.results[0])
qis.mz(mod.qubits[1], mod.results[1])

i8p = PointerType(IntType(mod.context, 8))
rt.array_record_output(
    mod.builder, const(IntType(mod.context, 64), 2), Constant.null(i8p)
)

rt.result_record_output(mod.builder, mod.results[0], Constant.null(i8p))
rt.result_record_output(mod.builder, mod.results[1], Constant.null(i8p))

if __name__ == "__main__":
    print(mod.ir())
