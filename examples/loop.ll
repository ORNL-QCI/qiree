; ModuleID = 'branching-loop'
source_filename = "branching-loop"

%Qubit = type opaque
%Result = type opaque

define void @main() #0 {
entry:
  br label %branch

branch:                                      ; preds = %then, %entry
  %0 = phi i64 [ 1, %entry ], [ %2, %then ]
  %1 = icmp slt i64 %0, 6
  br i1 %1, label %then, label %else

then:                                        ; preds = %branch
  call void @__quantum__qis__h__body(%Qubit* null)
  %2 = add i64 %0, 1
  br label %branch

else:                      ; preds = %branch
  call void @__quantum__qis__mz__body(%Qubit* null, %Result* null)
  call void @__quantum__rt__array_record_output(i64 1, i8* null)
  call void @__quantum__rt__result_record_output(%Result* null, i8* null)
  ret void
}

declare void @__quantum__qis__h__body(%Qubit*)
declare void @__quantum__qis__mz__body(%Qubit*, %Result* writeonly) #1
declare void @__quantum__rt__array_record_output(i64, i8*)
declare void @__quantum__rt__result_record_output(%Result*, i8*)

attributes #0 = { "EntryPoint" "num_required_qubits"="1" "num_required_results"="1"}
