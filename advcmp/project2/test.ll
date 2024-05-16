; ModuleID = './project2/test.c'
source_filename = "./project2/test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [10 x i8] c"%d %d %d\0A\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %z = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  store i32 1, ptr %x, align 4
  %0 = load i32, ptr %x, align 4
  %add = add nsw i32 %0, 2
  store i32 %add, ptr %y, align 4
  %1 = load i32, ptr %y, align 4
  %add1 = add nsw i32 %1, 2
  store i32 %add1, ptr %z, align 4
  %2 = load i32, ptr %y, align 4
  %cmp = icmp slt i32 %2, 2
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 1, ptr %z, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %3 = load i32, ptr %x, align 4
  %4 = load i32, ptr %y, align 4
  %5 = load i32, ptr %z, align 4
  %call = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %3, i32 noundef %4, i32 noundef %5)
  ret i32 0
}

declare i32 @printf(ptr noundef, ...) #1

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 17.0.6 (++20231209124227+6009708b4367-1~exp1~20231209124336.77)"}
