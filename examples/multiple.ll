; ModuleID = 'multiple-functions'
source_filename = "multiple-functions"

%Qubit = type opaque
%Result = type opaque
%String = type opaque

@0 = internal constant [3 x i8] c"()\00"

declare %Qubit* @__quantum__rt__qubit_allocate()

define internal void @main() #0 {
entry:
  call void @SimpleLoop__MiniCircuit__body(%Qubit* null, %Qubit* nonnull inttoptr (i64 1 to %Qubit*))
  call void @SimpleLoop__MiniCircuit__body(%Qubit* null, %Qubit* nonnull inttoptr (i64 1 to %Qubit*))
  call void @__quantum__qis__mz__body(%Qubit* null, %Result* null)
  call void @__quantum__rt__array_record_output(i64 1, i8* null)
  call void @__quantum__rt__result_record_output(%Result* null, i8* null)
  ret void
}

define internal void @SimpleLoop__MiniCircuit__body(%Qubit* %q1, %Qubit* %q2) {
entry:
  call void @Intrinsic__Function__H__body(%Qubit* %q1)
  call void @Intrinsic__Function__H__body(%Qubit* %q2)
  call void @Intrinsic__Function__CNOT__body(%Qubit* %q1, %Qubit* %q2)
  ret void
}

declare void @__quantum__rt__qubit_release(%Qubit*)

define internal void @Intrinsic__Function__H__body(%Qubit* %qubit) {
entry:
  call void @__quantum__qis__h__body(%Qubit* %qubit)
  ret void
}

define internal void @Intrinsic__Function__CNOT__body(%Qubit* %control, %Qubit* %target) {
entry:
  call void @__quantum__qis__cnot__body(%Qubit* %control, %Qubit* %target)
  ret void
}

declare void @__quantum__qis__cnot__body(%Qubit*, %Qubit*)

declare void @__quantum__qis__h__body(%Qubit*)
declare %String* @__quantum__rt__string_create(i8*)
declare void @__quantum__rt__message(%String*)
declare void @__quantum__rt__string_update_reference_count(%String*, i32)


declare void @__quantum__qis__mz__body(%Qubit*, %Result* writeonly) #1
declare void @__quantum__rt__array_record_output(i64, i8*)
declare void @__quantum__rt__result_record_output(%Result*, i8*)

attributes #0 = { "entry_point" "num_required_qubits"="2" "num_required_results"="2"}

