; ModuleID = 'Minimal'
source_filename = "Minimal"

define void @main() #0 {
entry:
  ret void
}

attributes #0 = { "entry_point" "required_num_qubits"="0" "required_num_results"="0" "output_labeling_schema" "qir_profiles"="custom" }

!llvm.module.flags = !{!0, !1, !2, !3}

!0 = !{i32 1, !"qir_major_version", i32 1}
!1 = !{i32 7, !"qir_minor_version", i32 0}
!2 = !{i32 1, !"dynamic_qubit_management", i1 false}
!3 = !{i32 1, !"dynamic_result_management", i1 false}

