; ModuleID = 'simple_print_arithmetic.c'
source_filename = "simple_print_arithmetic.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [42 x i8] c"Multiplication result of %d and %d is %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [43 x i8] c"The program take two simple integer input\0A\00", align 1
@.str.2 = private unnamed_addr constant [22 x i8] c"Enter first integer: \00", align 1
@.str.3 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.4 = private unnamed_addr constant [23 x i8] c"Enter second integer: \00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @get_output(i32 noundef %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, ptr %3, align 4
  store i32 %1, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = load i32, ptr %4, align 4
  %8 = mul nsw i32 %6, %7
  store i32 %8, ptr %5, align 4
  %9 = load i32, ptr %3, align 4
  %10 = load i32, ptr %4, align 4
  %11 = load i32, ptr %5, align 4
  %12 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %9, i32 noundef %10, i32 noundef %11)
  ret void
}

declare i32 @printf(ptr noundef, ...) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.1)
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.2)
  %6 = call i32 (ptr, ...) @__isoc99_scanf(ptr noundef @.str.3, ptr noundef %2)
  %7 = call i32 (ptr, ...) @printf(ptr noundef @.str.4)
  %8 = call i32 (ptr, ...) @__isoc99_scanf(ptr noundef @.str.3, ptr noundef %3)
  %9 = load i32, ptr %2, align 4
  %10 = load i32, ptr %3, align 4
  call void @get_output(i32 noundef %9, i32 noundef %10)
  ret i32 0
}

declare i32 @__isoc99_scanf(ptr noundef, ...) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 18.0.0 (++20230924052956+15f29c482b74-1~exp1~20230924173114.772)"}
