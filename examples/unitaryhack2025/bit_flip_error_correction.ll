; ModuleID = 'bit_flip_error_correction'
source_filename = "bit_flip_error_correction"

%Qubit  = type opaque
%Result = type opaque

define void @main() #0 {
entry:
  ; Encode data qubit q0 into q1 and q2
  call void @__quantum__qis__cnot__body(%Qubit* null,                      %Qubit* inttoptr (i64 1 to %Qubit*))
  call void @__quantum__qis__cnot__body(%Qubit* null,                      %Qubit* inttoptr (i64 2 to %Qubit*))

  ; Syndrome 1: parity(q0,q1) → s0,r0
  call void @__quantum__qis__cx__body(%Qubit* null,                       %Qubit* inttoptr (i64 3 to %Qubit*))
  call void @__quantum__qis__cx__body(%Qubit* inttoptr (i64 1 to %Qubit*), %Qubit* inttoptr (i64 3 to %Qubit*))
  call void @__quantum__qis__mz__body(%Qubit* inttoptr (i64 3 to %Qubit*), %Result* null)
  call void @__quantum__rt__array_record_output(i64 [num_results], i8* null)
  call void @__quantum__rt__result_record_output(%Result* null,            i8* null)
  %0 = call i1 @__quantum__qis__read_result__body(%Result* null)
  br i1 %0, label %then, label %else

then:                                             ; preds = %entry
  ; If r0=1 → flip q0
  call void @__quantum__qis__x__body(%Qubit* null)
  br label %continue

else:                                             ; preds = %entry
  br label %continue

continue:                                         ; preds = %else, %then
  ; Syndrome 2: parity(q1,q2) → s1,r1
  call void @__quantum__qis__cx__body(%Qubit* inttoptr (i64 1 to %Qubit*), %Qubit* inttoptr (i64 4 to %Qubit*))
  call void @__quantum__qis__cx__body(%Qubit* inttoptr (i64 2 to %Qubit*), %Qubit* inttoptr (i64 4 to %Qubit*))
  call void @__quantum__qis__mz__body(%Qubit* inttoptr (i64 4 to %Qubit*), %Result* inttoptr (i64 1 to %Result*))
  call void @__quantum__rt__result_record_output(%Result* inttoptr (i64 1 to %Result*), i8* null)
  %1 = call i1 @__quantum__qis__read_result__body(%Result* inttoptr (i64 1 to %Result*))
  br i1 %1, label %then1, label %else2

then1:                                            ; preds = %continue
  ; If r1=1 → flip q2
  call void @__quantum__qis__x__body(%Qubit* inttoptr (i64 2 to %Qubit*))
  br label %continue3

else2:                                            ; preds = %continue
  br label %continue3

continue3:                                        ; preds = %else2, %then1
  ; Final data qubit measurement → r2
  call void @__quantum__qis__mz__body(%Qubit* null,                       %Result* inttoptr (i64 2 to %Result*))
  call void @__quantum__rt__result_record_output(%Result* inttoptr (i64 2 to %Result*), i8* null)
  ret void
}

; === Intrinsic declarations ===
declare void @__quantum__qis__cnot__body(%Qubit*, %Qubit*)
declare void @__quantum__qis__cx__body(%Qubit*, %Qubit*)
declare void @__quantum__qis__mz__body(%Qubit*, %Result* writeonly) #1
declare i1   @__quantum__qis__read_result__body(%Result*)
declare void @__quantum__qis__x__body(%Qubit*)
declare void @__quantum__rt__result_record_output(%Result*, i8*)

; === Attributes & Flags ===
attributes #0 = {
  "entry_point"
  "output_labeling_schema"
  "qir_profiles"="custom"
  "required_num_qubits"="5"
  "required_num_results"="3"
}
attributes #1 = { "irreversible" }

!llvm.module.flags = !{!0, !1, !2, !3}

!0 = !{i32 1, !"qir_major_version",          i32 1}
!1 = !{i32 7, !"qir_minor_version",          i32 0}
!2 = !{i32 1, !"dynamic_qubit_management",   i1 false}
!3 = !{i32 1, !"dynamic_result_management", i1 false}

declare void @__quantum__rt__array_record_output(i64, i8*)