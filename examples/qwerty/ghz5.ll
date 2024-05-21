; ModuleID = 'qwerty'
source_filename = "qwerty"

@tag_ret = internal constant [4 x i8] c"ret\00"
@tag_discarded = internal constant [10 x i8] c"discarded\00"

define i64 @kernel() #0 {
  call void @__quantum__rt__initialize(ptr null)
  call void @__quantum__qis__h__body(ptr null)
  call void @__quantum__qis__cx__body(ptr null, ptr inttoptr (i64 1 to ptr))
  call void @__quantum__qis__cx__body(ptr null, ptr inttoptr (i64 2 to ptr))
  call void @__quantum__qis__cx__body(ptr null, ptr inttoptr (i64 3 to ptr))
  call void @__quantum__qis__cx__body(ptr null, ptr inttoptr (i64 4 to ptr))
  call void @__quantum__qis__mz__body(ptr null, ptr null)
  call void @__quantum__qis__mz__body(ptr inttoptr (i64 1 to ptr), ptr inttoptr (i64 1 to ptr))
  call void @__quantum__qis__mz__body(ptr inttoptr (i64 2 to ptr), ptr inttoptr (i64 2 to ptr))
  call void @__quantum__qis__mz__body(ptr inttoptr (i64 3 to ptr), ptr inttoptr (i64 3 to ptr))
  call void @__quantum__qis__mz__body(ptr inttoptr (i64 4 to ptr), ptr inttoptr (i64 4 to ptr))
  call void @__quantum__rt__tuple_record_output(i64 5, ptr @tag_ret)
  call void @__quantum__rt__result_record_output(ptr null, ptr null)
  call void @__quantum__rt__result_record_output(ptr inttoptr (i64 1 to ptr), ptr null)
  call void @__quantum__rt__result_record_output(ptr inttoptr (i64 2 to ptr), ptr null)
  call void @__quantum__rt__result_record_output(ptr inttoptr (i64 3 to ptr), ptr null)
  call void @__quantum__rt__result_record_output(ptr inttoptr (i64 4 to ptr), ptr null)
  call void @__quantum__rt__tuple_record_output(i64 0, ptr @tag_discarded)
  ret i64 0
}

declare void @__quantum__qis__h__body(ptr %0)

declare void @__quantum__qis__cx__body(ptr %0, ptr %1)

declare void @__quantum__rt__initialize(ptr %0)

declare void @__quantum__qis__mz__body(ptr %0, ptr %1)

declare void @__quantum__rt__tuple_record_output(i64 %0, ptr %1)

declare void @__quantum__rt__result_record_output(ptr %0, ptr %1)

attributes #0 = { "entry_point" "output_labeling_schema"="qwerty_v1" "qir_profiles"="base_profile" "required_num_qubits"="5" "required_num_results"="5" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 1, !"dynamic_qubit_management", i1 false}
!2 = !{i32 1, !"dynamic_result_management", i1 false}
!3 = !{i32 1, !"qir_major_version", i32 1}
!4 = !{i32 1, !"qir_minor_version", i32 0}
