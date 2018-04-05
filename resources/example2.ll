; ModuleID = 'resources/example2.c'
source_filename = "resources/example2.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %z = alloca i32, align 4
  %N = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  store i32 0, i32* %z, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %0 = load i32, i32* %i, align 4
  %1 = load i32, i32* %N, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %2 = load i32, i32* %x, align 4
  %3 = load i32, i32* %y, align 4
  %mul = mul nsw i32 2, %3
  %mul1 = mul nsw i32 %mul, 3
  %4 = load i32, i32* %z, align 4
  %mul2 = mul nsw i32 %mul1, %4
  %add = add nsw i32 %2, %mul2
  %rem = srem i32 %add, 3
  %sub = sub nsw i32 0, %rem
  store i32 %sub, i32* %x, align 4
  %5 = load i32, i32* %y, align 4
  %mul3 = mul nsw i32 3, %5
  %6 = load i32, i32* %y, align 4
  %mul4 = mul nsw i32 2, %6
  %add5 = add nsw i32 %mul3, %mul4
  %7 = load i32, i32* %z, align 4
  %add6 = add nsw i32 %add5, %7
  %rem7 = srem i32 %add6, 11
  store i32 %rem7, i32* %y, align 4
  %8 = load i32, i32* %z, align 4
  %inc = add nsw i32 %8, 1
  store i32 %inc, i32* %z, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %9 = load i32, i32* %retval, align 4
  ret i32 %9
}

attributes #0 = { noinline nounwind optnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 7.0.0 (trunk 325693) (llvm/trunk 325690)"}
