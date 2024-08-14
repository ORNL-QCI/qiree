; ModuleID = 'LLVMDialectModule'
source_filename = "LLVMDialectModule"

@tag_ret = internal constant [4 x i8] c"ret\00"
@tag_discarded = internal constant [10 x i8] c"discarded\00"

define i64 @kernel_0() #0 {
  call void @__quantum__rt__initialize(ptr null)
  call void @__quantum__qis__h__body(ptr null)
  call void @__quantum__qis__cx__body(ptr null, ptr inttoptr (i64 1 to ptr))
  call void @__quantum__qis__mz__body(ptr null, ptr null)
  call void @__quantum__qis__mz__body(ptr inttoptr (i64 1 to ptr), ptr inttoptr (i64 1 to ptr))
  call void @__quantum__rt__tuple_record_output(i64 2, ptr @tag_ret)
  call void @__quantum__rt__result_record_output(ptr null, ptr null)
  call void @__quantum__rt__result_record_output(ptr inttoptr (i64 1 to ptr), ptr null)
  call void @__quantum__rt__tuple_record_output(i64 0, ptr @tag_discarded)
  ret i64 0
}

declare void @__quantum__qis__h__body(ptr)

declare void @__quantum__qis__cx__body(ptr, ptr)

declare void @__quantum__rt__initialize(ptr)

declare void @__quantum__qis__mz__body(ptr, ptr)

declare void @__quantum__rt__tuple_record_output(i64, ptr)

declare void @__quantum__rt__result_record_output(ptr, ptr)

attributes #0 = { "entry_point" "output_labeling_schema"="qwerty_v1" "qir_profiles"="base_profile" "required_num_qubits"="2" "required_num_results"="2" }

!llvm.module.flags = !{!0}

!0 = !{i32 2, !"Debug Info Version", i32 3}
