; ModuleID = 'kitaev_phase_estimation'
source_filename = "kitaev_phase_estimation"

%Qubit  = type opaque
%Result = type opaque

define void @main() #0 {
entry:
  ; Prepare system qubit q1 in |1> state
  call void @__quantum__qis__x__body(%Qubit* inttoptr (i64 1 to %Qubit*))

  ; === Round 1: estimate most significant bit (bit 1) ===
  ; 1. Prepare ancilla q0 in |+>
  call void @__quantum__qis__h__body(%Qubit* inttoptr (i64 0 to %Qubit*))
  ; 2. Controlled-Z for phase kickback
  call void @__quantum__qis__cz__body(%Qubit* inttoptr (i64 0 to %Qubit*), %Qubit* inttoptr (i64 1 to %Qubit*))
  ; 3. Rotate ancilla into Z-basis for measurement
  call void @__quantum__qis__h__body(%Qubit* inttoptr (i64 0 to %Qubit*))
  ; 4. Measure ancilla → r0
  call void @__quantum__qis__mz__body(%Qubit* inttoptr (i64 0 to %Qubit*), %Result* inttoptr (i64 0 to %Result*))
  ; 5. Record both bits (2 results total)
  call void @__quantum__rt__array_record_output(i64 2, i8* null)
  ; 6. Record this bit
  call void @__quantum__rt__result_record_output(%Result* inttoptr (i64 0 to %Result*), i8* null)
  ; 7. Read measurement and apply feed‑forward Z if needed
  %r0 = call i1 @__quantum__qis__read_result__body(%Result* inttoptr (i64 0 to %Result*))
  br i1 %r0, label %feed1, label %cont1

feed1: ; If bit 1 was 1, apply Z
  call void @__quantum__qis__z__body(%Qubit* inttoptr (i64 1 to %Qubit*))
  br label %cont1

cont1:
  ; 8. Reset ancilla for next round
  call void @__quantum__qis__reset__body(%Qubit* inttoptr (i64 0 to %Qubit*))

  ; === Round 2: estimate next bit (bit 0) ===
  ; 1. Prepare ancilla |+>
  call void @__quantum__qis__h__body(%Qubit* inttoptr (i64 0 to %Qubit*))
  ; 2. Apply U² = cz²
  call void @__quantum__qis__cz__body(%Qubit* inttoptr (i64 0 to %Qubit*), %Qubit* inttoptr (i64 1 to %Qubit*))
  call void @__quantum__qis__cz__body(%Qubit* inttoptr (i64 0 to %Qubit*), %Qubit* inttoptr (i64 1 to %Qubit*))
  ; 3. Rotate back into Z-basis
  call void @__quantum__qis__h__body(%Qubit* inttoptr (i64 0 to %Qubit*))
  ; 4. Measure ancilla → r1
  call void @__quantum__qis__mz__body(%Qubit* inttoptr (i64 0 to %Qubit*), %Result* inttoptr (i64 1 to %Result*))
  ; 5. Record second bit
  call void @__quantum__rt__result_record_output(%Result* inttoptr (i64 1 to %Result*), i8* null)
  br label %end

end:
  ret void
}

; === Intrinsics ===
declare void @__quantum__qis__x__body(%Qubit*)
declare void @__quantum__qis__h__body(%Qubit*)
declare void @__quantum__qis__cz__body(%Qubit*, %Qubit*)
declare void @__quantum__qis__mz__body(%Qubit*, %Result* writeonly) #1
declare i1   @__quantum__qis__read_result__body(%Result*)
declare void @__quantum__qis__z__body(%Qubit*)
declare void @__quantum__qis__reset__body(%Qubit*)
declare void @__quantum__rt__array_record_output(i64, i8*)
declare void @__quantum__rt__result_record_output(%Result*, i8*)

; === Metadata ===
attributes #0 = {
  "entry_point"
  "output_labeling_schema"
  "qir_profiles"="custom"
  "required_num_qubits"="2"
  "required_num_results"="2"
}
attributes #1 = { "irreversible" }

!llvm.module.flags = !{!0, !1, !2, !3}
!0 = !{i32 1, !"qir_major_version",        i32 1}
!1 = !{i32 7, !"qir_minor_version",        i32 0}
!2 = !{i32 1, !"dynamic_qubit_management", i1 false}
!3 = !{i32 1, !"dynamic_result_management", i1 false}
