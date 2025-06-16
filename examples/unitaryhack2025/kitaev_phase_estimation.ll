; ModuleID = 'kitaev_phase_estimation'
source_filename = "kitaev_phase_estimation"

%Qubit  = type opaque
%Result = type opaque

define void @main() #0 {
entry:
  ; Prepare system qubit q1 in |1> state
  call void @__quantum__qis__x__body(%Qubit* inttoptr (i64 1 to %Qubit*))

  ; Hadamard on ancilla q0
  call void @__quantum__qis__h__body(%Qubit* null)

  ; Controlled-Z (phase kickback) between q0 and q1
  call void @__quantum__qis__cz__body(%Qubit* null, %Qubit* inttoptr (i64 1 to %Qubit*))

  ; Measure ancilla into result r0 and record
  call void @__quantum__qis__mz__body(%Qubit* null, %Result* null)
  call void @__quantum__rt__array_record_output(i64 [num_results], i8* null)
  call void @__quantum__rt__result_record_output(%Result* null, i8* null)
  %0 = call i1 @__quantum__qis__read_result__body(%Result* null)
  br i1 %0, label %then, label %else

then:                                             ; preds = %entry
  ; Feed-forward Z correction on system qubit if phase bit = 1
  call void @__quantum__qis__z__body(%Qubit* inttoptr (i64 1 to %Qubit*))
  br label %continue

else:                                             ; preds = %entry
  br label %continue

continue:                                         ; preds = %else, %then
  ; Final system measurement into r1 and record
  call void @__quantum__qis__mz__body(%Qubit* inttoptr (i64 1 to %Qubit*), %Result* inttoptr (i64 1 to %Result*))
  call void @__quantum__rt__result_record_output(%Result* inttoptr (i64 1 to %Result*), i8* null)
  ret void
}

; === Intrinsic declarations ===

declare void @__quantum__qis__x__body(%Qubit*)
declare void @__quantum__qis__h__body(%Qubit*)
declare void @__quantum__qis__cz__body(%Qubit*, %Qubit*)
declare void @__quantum__qis__mz__body(%Qubit*, %Result* writeonly) #1
declare i1   @__quantum__qis__read_result__body(%Result*)
declare void @__quantum__qis__z__body(%Qubit*)

; Runtime recording intrinsic
declare void @__quantum__rt__result_record_output(%Result*, i8*)

; === Attributes & Flags ===

attributes #0 = {
  "entry_point"
  "output_labeling_schema"
  "qir_profiles"="custom"
  "required_num_qubits"="2"
  "required_num_results"="2"
}
attributes #1 = { "irreversible" }

!llvm.module.flags = !{!0, !1, !2, !3}

!0 = !{i32 1, !"qir_major_version", i32 1}
!1 = !{i32 7, !"qir_minor_version", i32 0}
!2 = !{i32 1, !"dynamic_qubit_management", i1 false}
!3 = !{i32 1, !"dynamic_result_management", i1 false}

declare void @__quantum__rt__array_record_output(i64, i8*)