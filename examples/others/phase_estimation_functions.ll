; ModuleID = 'phase-estimation-with-functions'
source_filename = "phase-estimation-with-functions"

%Qubit = type opaque
%Result = type opaque

define internal void @CU__Function__body(%Qubit* %q0, %Qubit* %q1, %Qubit* %q2, %Qubit* %q3) {
entry:
  call void @__quantum__qis__h__body(%Qubit* %q0)
  call void @__quantum__qis__h__body(%Qubit* %q1)
  call void @__quantum__qis__h__body(%Qubit* %q2)
  call void @__quantum__qis__x__body(%Qubit* %q3)
  call void @__quantum__qis__rz__body(double 0x3FD921FB54442D18, %Qubit* %q0)
  call void @__quantum__qis__rz__body(double 0x3FD921FB54442D18, %Qubit* %q3)
  call void @__quantum__qis__cnot__body(%Qubit* %q0, %Qubit* %q3)
  call void @__quantum__qis__rz__body(double 0xBFD921FB54442D18, %Qubit* %q3)
  call void @__quantum__qis__cnot__body(%Qubit* %q0, %Qubit* %q3)
  call void @__quantum__qis__rz__body(double 0x3FD921FB54442D18, %Qubit* %q0)
  call void @__quantum__qis__rz__body(double 0x3FD921FB54442D18, %Qubit* %q3)
  call void @__quantum__qis__cnot__body(%Qubit* %q1, %Qubit* %q3)
  call void @__quantum__qis__rz__body(double 0xBFD921FB54442D18, %Qubit* %q3)
  call void @__quantum__qis__cnot__body(%Qubit* %q1, %Qubit* %q3)
  call void @__quantum__qis__rz__body(double 0x3FD921FB54442D18, %Qubit* %q1)
  call void @__quantum__qis__rz__body(double 0x3FD921FB54442D18, %Qubit* %q3)
  call void @__quantum__qis__cnot__body(%Qubit* %q1, %Qubit* %q3)
  call void @__quantum__qis__rz__body(double 0xBFD921FB54442D18, %Qubit* %q3)
  call void @__quantum__qis__cnot__body(%Qubit* %q1, %Qubit* %q3)
  call void @__quantum__qis__rz__body(double 0x3FD921FB54442D18, %Qubit* %q2)
  call void @__quantum__qis__rz__body(double 0x3FD921FB54442D18, %Qubit* %q3)
  call void @__quantum__qis__cnot__body(%Qubit* %q2, %Qubit* %q3)
  call void @__quantum__qis__rz__body(double 0xBFD921FB54442D18, %Qubit* %q3)
  call void @__quantum__qis__cnot__body(%Qubit* %q2, %Qubit* %q3)
  call void @__quantum__qis__rz__body(double 0x3FD921FB54442D18, %Qubit* %q2)
  call void @__quantum__qis__rz__body(double 0x3FD921FB54442D18, %Qubit* %q3)
  call void @__quantum__qis__cnot__body(%Qubit* %q2, %Qubit* %q3)
  call void @__quantum__qis__rz__body(double 0xBFD921FB54442D18, %Qubit* %q3)
  call void @__quantum__qis__cnot__body(%Qubit* %q2, %Qubit* %q3)
  call void @__quantum__qis__rz__body(double 0x3FD921FB54442D18, %Qubit* %q2)
  call void @__quantum__qis__rz__body(double 0x3FD921FB54442D18, %Qubit* %q3)
  call void @__quantum__qis__cnot__body(%Qubit* %q2, %Qubit* %q3)
  call void @__quantum__qis__rz__body(double 0xBFD921FB54442D18, %Qubit* %q3)
  call void @__quantum__qis__cnot__body(%Qubit* %q2, %Qubit* %q3)
  call void @__quantum__qis__rz__body(double 0x3FD921FB54442D18, %Qubit* %q2)
  call void @__quantum__qis__rz__body(double 0x3FD921FB54442D18, %Qubit* %q3)
  call void @__quantum__qis__cnot__body(%Qubit* %q2, %Qubit* %q3)
  call void @__quantum__qis__rz__body(double 0xBFD921FB54442D18, %Qubit* %q3)
  call void @__quantum__qis__cnot__body(%Qubit* %q2, %Qubit* %q3)
  ret void
}

define internal void @QFT__DAGGER__Function__body(%Qubit* %q0, %Qubit* %q1, %Qubit* %q2) {
entry:
  call void @__quantum__qis__cnot__body(%Qubit* %q0, %Qubit* %q2)
  call void @__quantum__qis__cnot__body(%Qubit* %q2, %Qubit* %q0)
  call void @__quantum__qis__cnot__body(%Qubit* %q0, %Qubit* %q2)
  call void @__quantum__qis__h__body(%Qubit* %q0)
  call void @__quantum__qis__rz__body(double 0xBFE921FB54442D18, %Qubit* %q0)
  call void @__quantum__qis__rz__body(double 0xBFE921FB54442D18, %Qubit* %q1)
  call void @__quantum__qis__cnot__body(%Qubit* %q0, %Qubit* %q1)
  call void @__quantum__qis__rz__body(double 0x3FE921FB54442D18, %Qubit* %q1)
  call void @__quantum__qis__cnot__body(%Qubit* %q0, %Qubit* %q1)
  call void @__quantum__qis__h__body(%Qubit* %q1)
  call void @__quantum__qis__rz__body(double 0xBFD921FB54442D18, %Qubit* %q0)
  call void @__quantum__qis__rz__body(double 0xBFD921FB54442D18, %Qubit* %q2)
  call void @__quantum__qis__cnot__body(%Qubit* %q0, %Qubit* %q2)
  call void @__quantum__qis__rz__body(double 0x3FD921FB54442D18, %Qubit* %q2)
  call void @__quantum__qis__cnot__body(%Qubit* %q0, %Qubit* %q2)
  call void @__quantum__qis__rz__body(double 0xBFE921FB54442D18, %Qubit* %q1)
  call void @__quantum__qis__rz__body(double 0xBFE921FB54442D18, %Qubit* %q2)
  call void @__quantum__qis__cnot__body(%Qubit* %q1, %Qubit* %q2)
  call void @__quantum__qis__rz__body(double 0x3FE921FB54442D18, %Qubit* %q2)
  call void @__quantum__qis__cnot__body(%Qubit* %q1, %Qubit* %q2)
  call void @__quantum__qis__h__body(%Qubit* %q2)
  ret void
}

define void @main() #0 {
entry:
  call void @CU__Function__body(%Qubit* null, %Qubit* inttoptr (i64 1 to %Qubit*), %Qubit* inttoptr (i64 2 to %Qubit*), %Qubit* inttoptr (i64 3 to %Qubit*))
  call void @QFT__DAGGER__Function__body(%Qubit* null, %Qubit* inttoptr (i64 1 to %Qubit*), %Qubit* inttoptr (i64 2 to %Qubit*))
  call void @__quantum__qis__mz__body(%Qubit* null, %Result* null)
  call void @__quantum__qis__mz__body(%Qubit* inttoptr (i64 1 to %Qubit*), %Result* inttoptr (i64 1 to %Result*))
  call void @__quantum__qis__mz__body(%Qubit* inttoptr (i64 2 to %Qubit*), %Result* inttoptr (i64 2 to %Result*))
  call void @__quantum__rt__array_record_output(i64 3, i8* null)
  call void @__quantum__rt__result_record_output(%Result* null, i8* null)
  call void @__quantum__rt__result_record_output(%Result* inttoptr (i64 1 to %Result*), i8* null)
  call void @__quantum__rt__result_record_output(%Result* inttoptr (i64 2 to %Result*), i8* null)
  ret void
}

declare void @__quantum__qis__h__body(%Qubit*)

declare void @__quantum__qis__x__body(%Qubit*)

declare void @__quantum__qis__rz__body(double, %Qubit*)

declare void @__quantum__qis__cnot__body(%Qubit*, %Qubit*)

declare void @__quantum__qis__mz__body(%Qubit*, %Result* writeonly) #1

declare void @__quantum__rt__array_record_output(i64, i8*)

declare void @__quantum__rt__result_record_output(%Result*, i8*)

attributes #0 = { "entry_point" "num_required_qubits"="4" "num_required_results"="3" "output_labeling_schema" "qir_profiles"="custom" "InteropFriendly"}

!llvm.module.flags = !{!0, !1, !2, !3}

!0 = !{i32 1, !"qir_major_version", i32 1}
!1 = !{i32 7, !"qir_minor_version", i32 0}
!2 = !{i32 1, !"dynamic_qubit_management", i1 false}
!3 = !{i32 1, !"dynamic_result_management", i1 false}
