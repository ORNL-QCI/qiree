; ModuleID = 'bit\_flip\_error\_correction'
source\_filename = "bit\_flip\_error\_correction"

%Qubit  = type opaque
%Result = type opaque

define void @main() #0 {
entry:
; Encode data qubit q0 into q1 and q2
call void @\_\_quantum\_\_qis\_\_cnot\_\_body(%Qubit\* null,                      %Qubit\* inttoptr (i64 1 to %Qubit\*))
call void @\_\_quantum\_\_qis\_\_cnot\_\_body(%Qubit\* null,                      %Qubit\* inttoptr (i64 2 to %Qubit\*))

; === Inject deterministic bit-flip error on data qubit q0 ===
call void @\_\_quantum\_\_qis\_\_x\_\_body(%Qubit\* null)

; Syndrome 1: parity(q0,q1) → s0,r0
call void @\_\_quantum\_\_qis\_\_cx\_\_body(%Qubit\* null,                       %Qubit\* inttoptr (i64 3 to %Qubit\*))
call void @\_\_quantum\_\_qis\_\_cx\_\_body(%Qubit\* inttoptr (i64 1 to %Qubit\*), %Qubit\* inttoptr (i64 3 to %Qubit\*))
call void @\_\_quantum\_\_qis\_\_mz\_\_body(%Qubit\* inttoptr (i64 3 to %Qubit\*), %Result\* null)
call void @\_\_quantum\_\_rt\_\_array\_record\_output(i64 3, i8\* null)
call void @\_\_quantum\_\_rt\_\_result\_record\_output(%Result\* null,            i8\* null)
%0 = call i1 @\_\_quantum\_\_qis\_\_read\_result\_\_body(%Result\* null)
br i1 %0, label %then, label %else

then:                                             ; preds = %entry
; If r0=1 → flip q0
call void @\_\_quantum\_\_qis\_\_x\_\_body(%Qubit\* null)
br label %continue

else:                                             ; preds = %entry
br label %continue

continue:                                         ; preds = %else, %then
; Syndrome 2: parity(q1,q2) → s1,r1
call void @\_\_quantum\_\_qis\_\_cx\_\_body(%Qubit\* inttoptr (i64 1 to %Qubit\*), %Qubit\* inttoptr (i64 4 to %Qubit\*))
call void @\_\_quantum\_\_qis\_\_cx\_\_body(%Qubit\* inttoptr (i64 2 to %Qubit\*), %Qubit\* inttoptr (i64 4 to %Qubit\*))
call void @\_\_quantum\_\_qis\_\_mz\_\_body(%Qubit\* inttoptr (i64 4 to %Qubit\*), %Result\* inttoptr (i64 1 to %Result\*))
call void @\_\_quantum\_\_rt\_\_result\_record\_output(%Result\* inttoptr (i64 1 to %Result\*), i8\* null)
%1 = call i1 @\_\_quantum\_\_qis\_\_read\_result\_\_body(%Result\* inttoptr (i64 1 to %Result\*))
br i1 %1, label %then1, label %else2

then1:                                            ; preds = %continue
; If r1=1 → flip q2
call void @\_\_quantum\_\_qis\_\_x\_\_body(%Qubit\* inttoptr (i64 2 to %Qubit\*))
br label %continue3

else2:                                            ; preds = %continue
br label %continue3

continue3:                                        ; preds = %else2, %then1
; Final data qubit measurement → r2
call void @\_\_quantum\_\_qis\_\_mz\_\_body(%Qubit\* null,                       %Result\* inttoptr (i64 2 to %Result\*))
call void @\_\_quantum\_\_rt\_\_result\_record\_output(%Result\* inttoptr (i64 2 to %Result\*), i8\* null)
ret void
}

; === Intrinsic declarations ===
declare void @\_\_quantum\_\_qis\_\_cnot\_\_body(%Qubit\*, %Qubit\*)
declare void @\_\_quantum\_\_qis\_\_cx\_\_body(%Qubit\*, %Qubit\*)
declare void @\_\_quantum\_\_qis\_\_mz\_\_body(%Qubit\*, %Result\* writeonly) #1
declare i1   @\_\_quantum\_\_qis\_\_read\_result\_\_body(%Result\*)
declare void @\_\_quantum\_\_qis\_\_x\_\_body(%Qubit\*)
declare void @\_\_quantum\_\_qis\_\_reset\_\_body(%Qubit\*)

declare void @\_\_quantum\_\_rt\_\_array\_record\_output(i64, i8\*)
declare void @\_\_quantum\_\_rt\_\_result\_record\_output(%Result\*, i8\*)

; === Attributes & Flags ===
attributes #0 = {
"entry\_point"
"output\_labeling\_schema"
"qir\_profiles"="custom"
"required\_num\_qubits"="5"
"required\_num\_results"="3"
}
attributes #1 = { "irreversible" }

!llvm.module.flags = !{!0, !1, !2, !3}

!0 = !{i32 1, !"qir\_major\_version",          i32 1}
!1 = !{i32 7, !"qir\_minor\_version",          i32 0}
!2 = !{i32 1, !"dynamic\_qubit\_management",   i1 false}
!3 = !{i32 1, !"dynamic\_result\_management", i1 false}
