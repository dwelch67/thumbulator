; ModuleID = 'io.clang.bc'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32"
target triple = "i386-unknown-linux-gnu"

%struct.text = type { i8*, i32 }
%struct.travlist = type { %struct.travlist*, i32, i32, i32 }

@getin.wd1buf = internal global [20 x i8] zeroinitializer, align 1
@getin.wd2buf = internal global [20 x i8] zeroinitializer, align 1
@.str = private unnamed_addr constant [19 x i8] c"Give me a break!!\0A\00"
@.str1 = private unnamed_addr constant [29 x i8] c"Please answer the question.\0A\00"
@inptr = common global i8* null, align 4
@data_file = external global [0 x i8]
@clsses = external global i32
@.str2 = private unnamed_addr constant [30 x i8] c"Invalid data section number: \00"
@.str3 = private unnamed_addr constant [2 x i8] c"\0A\00"
@breakch = common global i8 0, align 1
@nbf = common global [12 x i8] zeroinitializer, align 1
@seekhere = common global i8* null, align 4
@oldloc = external global i32
@ltext = external global [141 x %struct.text]
@stext = external global [141 x %struct.text]
@ptext = external global [101 x %struct.text]
@.str4 = private unnamed_addr constant [21 x i8] c"Too many random msgs\00"
@rtext = external global [205 x %struct.text]
@ctext = external global [12 x %struct.text]
@cval = external global [12 x i32]
@.str5 = private unnamed_addr constant [20 x i8] c"Too many magic msgs\00"
@mtext = external global [35 x %struct.text]
@.str6 = private unnamed_addr constant [30 x i8] c"rdesc called with bad section\00"
@travel = external global [141 x %struct.travlist*]
@obj = external global i32
@plac = external global [101 x i32]
@fixd = external global [101 x i32]
@verb = external global i32
@actspk = external global [35 x i32]
@setbit = external constant [16 x i32]
@cond = external global [141 x i32]
@hntmax = external global i32
@hints = external global [20 x [5 x i32]]
@blklin = external global i32

define void @getin(i8** %wrd1, i8** %wrd2) nounwind {
  %1 = alloca i8**, align 4
  %2 = alloca i8**, align 4
  %s = alloca i8*, align 4
  %first = alloca i32, align 4
  %numch = alloca i32, align 4
  %flushline_ch = alloca i32, align 4
  %flushline_ch1 = alloca i32, align 4
  store i8** %wrd1, i8*** %1, align 4
  store i8** %wrd2, i8*** %2, align 4
  %3 = load i8*** %1, align 4
  store i8* getelementptr inbounds ([20 x i8]* @getin.wd1buf, i32 0, i32 0), i8** %3
  %4 = load i8*** %2, align 4
  store i8* getelementptr inbounds ([20 x i8]* @getin.wd2buf, i32 0, i32 0), i8** %4
  store i8 0, i8* getelementptr inbounds ([20 x i8]* @getin.wd2buf, i32 0, i32 0)
  store i8* getelementptr inbounds ([20 x i8]* @getin.wd1buf, i32 0, i32 0), i8** %s, align 4
  store i32 1, i32* %first, align 4
  store i32 0, i32* %numch, align 4
  br label %5

; <label>:5                                       ; preds = %66, %34, %0
  %6 = call signext i8 @get_onechar()
  %7 = load i8** %s, align 4
  store i8 %6, i8* %7
  %8 = sext i8 %6 to i32
  %9 = icmp sge i32 %8, 65
  br i1 %9, label %10, label %22

; <label>:10                                      ; preds = %5
  %11 = load i8** %s, align 4
  %12 = load i8* %11
  %13 = sext i8 %12 to i32
  %14 = icmp sle i32 %13, 90
  br i1 %14, label %15, label %22

; <label>:15                                      ; preds = %10
  %16 = load i8** %s, align 4
  %17 = load i8* %16
  %18 = sext i8 %17 to i32
  %19 = sub nsw i32 %18, -32
  %20 = trunc i32 %19 to i8
  %21 = load i8** %s, align 4
  store i8 %20, i8* %21
  br label %22

; <label>:22                                      ; preds = %15, %10, %5
  %23 = load i8** %s, align 4
  %24 = load i8* %23
  %25 = sext i8 %24 to i32
  switch i32 %25, label %50 [
    i32 10, label %26
    i32 32, label %28
  ]

; <label>:26                                      ; preds = %22
  %27 = load i8** %s, align 4
  store i8 0, i8* %27
  br label %67

; <label>:28                                      ; preds = %22
  %29 = load i8** %s, align 4
  %30 = icmp eq i8* %29, getelementptr inbounds ([20 x i8]* @getin.wd1buf, i32 0, i32 0)
  br i1 %30, label %34, label %31

; <label>:31                                      ; preds = %28
  %32 = load i8** %s, align 4
  %33 = icmp eq i8* %32, getelementptr inbounds ([20 x i8]* @getin.wd2buf, i32 0, i32 0)
  br i1 %33, label %34, label %35

; <label>:34                                      ; preds = %31, %28
  br label %5

; <label>:35                                      ; preds = %31
  %36 = load i8** %s, align 4
  store i8 0, i8* %36
  %37 = load i32* %first, align 4
  %38 = icmp ne i32 %37, 0
  br i1 %38, label %39, label %40

; <label>:39                                      ; preds = %35
  store i32 0, i32* %numch, align 4
  store i32 0, i32* %first, align 4
  store i8* getelementptr inbounds ([20 x i8]* @getin.wd2buf, i32 0, i32 0), i8** %s, align 4
  br label %66

; <label>:40                                      ; preds = %35
  br label %41

; <label>:41                                      ; preds = %40
  br label %42

; <label>:42                                      ; preds = %46, %41
  %43 = call signext i8 @get_onechar()
  %44 = sext i8 %43 to i32
  store i32 %44, i32* %flushline_ch, align 4
  %45 = icmp ne i32 %44, 10
  br i1 %45, label %46, label %47

; <label>:46                                      ; preds = %42
  br label %42

; <label>:47                                      ; preds = %42
  br label %48

; <label>:48                                      ; preds = %47
  %49 = load i8** %s, align 4
  store i8 0, i8* %49
  br label %67

; <label>:50                                      ; preds = %22
  %51 = load i32* %numch, align 4
  %52 = add nsw i32 %51, 1
  store i32 %52, i32* %numch, align 4
  %53 = icmp sge i32 %52, 20
  br i1 %53, label %54, label %63

; <label>:54                                      ; preds = %50
  call void @show_string(i8* getelementptr inbounds ([19 x i8]* @.str, i32 0, i32 0))
  store i8 0, i8* getelementptr inbounds ([20 x i8]* @getin.wd2buf, i32 0, i32 0)
  store i8 0, i8* getelementptr inbounds ([20 x i8]* @getin.wd1buf, i32 0, i32 0)
  br label %55

; <label>:55                                      ; preds = %54
  br label %56

; <label>:56                                      ; preds = %60, %55
  %57 = call signext i8 @get_onechar()
  %58 = sext i8 %57 to i32
  store i32 %58, i32* %flushline_ch1, align 4
  %59 = icmp ne i32 %58, 10
  br i1 %59, label %60, label %61

; <label>:60                                      ; preds = %56
  br label %56

; <label>:61                                      ; preds = %56
  br label %62

; <label>:62                                      ; preds = %61
  br label %67

; <label>:63                                      ; preds = %50
  %64 = load i8** %s, align 4
  %65 = getelementptr inbounds i8* %64, i32 1
  store i8* %65, i8** %s, align 4
  br label %66

; <label>:66                                      ; preds = %63, %39
  br label %5

; <label>:67                                      ; preds = %62, %48, %26
  ret void
}

declare signext i8 @get_onechar()

declare void @show_string(i8*)

define i32 @yes(i32 %x, i32 %y, i32 %z) nounwind {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %result = alloca i32, align 4
  %ch = alloca i32, align 4
  %flushline_ch = alloca i32, align 4
  store i32 %x, i32* %1, align 4
  store i32 %y, i32* %2, align 4
  store i32 %z, i32* %3, align 4
  store i32 1, i32* %result, align 4
  br label %4

; <label>:4                                       ; preds = %30, %0
  %5 = load i32* %1, align 4
  call void @rspeak(i32 %5)
  %6 = call signext i8 @get_onechar()
  %7 = sext i8 %6 to i32
  store i32 %7, i32* %ch, align 4
  %8 = icmp eq i32 %7, 121
  br i1 %8, label %9, label %10

; <label>:9                                       ; preds = %4
  store i32 1, i32* %result, align 4
  br label %15

; <label>:10                                      ; preds = %4
  %11 = load i32* %ch, align 4
  %12 = icmp eq i32 %11, 110
  br i1 %12, label %13, label %14

; <label>:13                                      ; preds = %10
  store i32 0, i32* %result, align 4
  br label %14

; <label>:14                                      ; preds = %13, %10
  br label %15

; <label>:15                                      ; preds = %14, %9
  br label %16

; <label>:16                                      ; preds = %15
  br label %17

; <label>:17                                      ; preds = %21, %16
  %18 = call signext i8 @get_onechar()
  %19 = sext i8 %18 to i32
  store i32 %19, i32* %flushline_ch, align 4
  %20 = icmp ne i32 %19, 10
  br i1 %20, label %21, label %22

; <label>:21                                      ; preds = %17
  br label %17

; <label>:22                                      ; preds = %17
  br label %23

; <label>:23                                      ; preds = %22
  %24 = load i32* %ch, align 4
  %25 = icmp eq i32 %24, 121
  br i1 %25, label %29, label %26

; <label>:26                                      ; preds = %23
  %27 = load i32* %ch, align 4
  %28 = icmp eq i32 %27, 110
  br i1 %28, label %29, label %30

; <label>:29                                      ; preds = %26, %23
  br label %31

; <label>:30                                      ; preds = %26
  call void @show_string(i8* getelementptr inbounds ([29 x i8]* @.str1, i32 0, i32 0))
  br label %4

; <label>:31                                      ; preds = %29
  %32 = load i32* %result, align 4
  %33 = icmp eq i32 %32, 1
  br i1 %33, label %34, label %36

; <label>:34                                      ; preds = %31
  %35 = load i32* %2, align 4
  call void @rspeak(i32 %35)
  br label %36

; <label>:36                                      ; preds = %34, %31
  %37 = load i32* %result, align 4
  %38 = icmp eq i32 %37, 0
  br i1 %38, label %39, label %41

; <label>:39                                      ; preds = %36
  %40 = load i32* %3, align 4
  call void @rspeak(i32 %40)
  br label %41

; <label>:41                                      ; preds = %39, %36
  %42 = load i32* %result, align 4
  ret i32 %42
}

define void @rspeak(i32 %msg) nounwind {
  %1 = alloca i32, align 4
  store i32 %msg, i32* %1, align 4
  %2 = load i32* %1, align 4
  %3 = icmp ne i32 %2, 0
  br i1 %3, label %4, label %7

; <label>:4                                       ; preds = %0
  %5 = load i32* %1, align 4
  %6 = getelementptr inbounds [205 x %struct.text]* @rtext, i32 0, i32 %5
  call void @speak(%struct.text* %6)
  br label %7

; <label>:7                                       ; preds = %4, %0
  ret void
}

define i32 @yesm(i32 %x, i32 %y, i32 %z) nounwind {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %result = alloca i32, align 4
  %ch = alloca i32, align 4
  %flushline_ch = alloca i32, align 4
  store i32 %x, i32* %1, align 4
  store i32 %y, i32* %2, align 4
  store i32 %z, i32* %3, align 4
  store i32 1, i32* %result, align 4
  br label %4

; <label>:4                                       ; preds = %30, %0
  %5 = load i32* %1, align 4
  call void @mspeak(i32 %5)
  %6 = call signext i8 @get_onechar()
  %7 = sext i8 %6 to i32
  store i32 %7, i32* %ch, align 4
  %8 = icmp eq i32 %7, 121
  br i1 %8, label %9, label %10

; <label>:9                                       ; preds = %4
  store i32 1, i32* %result, align 4
  br label %15

; <label>:10                                      ; preds = %4
  %11 = load i32* %ch, align 4
  %12 = icmp eq i32 %11, 110
  br i1 %12, label %13, label %14

; <label>:13                                      ; preds = %10
  store i32 0, i32* %result, align 4
  br label %14

; <label>:14                                      ; preds = %13, %10
  br label %15

; <label>:15                                      ; preds = %14, %9
  br label %16

; <label>:16                                      ; preds = %15
  br label %17

; <label>:17                                      ; preds = %21, %16
  %18 = call signext i8 @get_onechar()
  %19 = sext i8 %18 to i32
  store i32 %19, i32* %flushline_ch, align 4
  %20 = icmp ne i32 %19, 10
  br i1 %20, label %21, label %22

; <label>:21                                      ; preds = %17
  br label %17

; <label>:22                                      ; preds = %17
  br label %23

; <label>:23                                      ; preds = %22
  %24 = load i32* %ch, align 4
  %25 = icmp eq i32 %24, 121
  br i1 %25, label %29, label %26

; <label>:26                                      ; preds = %23
  %27 = load i32* %ch, align 4
  %28 = icmp eq i32 %27, 110
  br i1 %28, label %29, label %30

; <label>:29                                      ; preds = %26, %23
  br label %31

; <label>:30                                      ; preds = %26
  call void @show_string(i8* getelementptr inbounds ([29 x i8]* @.str1, i32 0, i32 0))
  br label %4

; <label>:31                                      ; preds = %29
  %32 = load i32* %result, align 4
  %33 = icmp eq i32 %32, 1
  br i1 %33, label %34, label %36

; <label>:34                                      ; preds = %31
  %35 = load i32* %2, align 4
  call void @mspeak(i32 %35)
  br label %36

; <label>:36                                      ; preds = %34, %31
  %37 = load i32* %result, align 4
  %38 = icmp eq i32 %37, 0
  br i1 %38, label %39, label %41

; <label>:39                                      ; preds = %36
  %40 = load i32* %3, align 4
  call void @mspeak(i32 %40)
  br label %41

; <label>:41                                      ; preds = %39, %36
  %42 = load i32* %result, align 4
  ret i32 %42
}

define void @mspeak(i32 %msg) nounwind {
  %1 = alloca i32, align 4
  store i32 %msg, i32* %1, align 4
  %2 = load i32* %1, align 4
  %3 = icmp ne i32 %2, 0
  br i1 %3, label %4, label %7

; <label>:4                                       ; preds = %0
  %5 = load i32* %1, align 4
  %6 = getelementptr inbounds [35 x %struct.text]* @mtext, i32 0, i32 %5
  call void @speak(%struct.text* %6)
  br label %7

; <label>:7                                       ; preds = %4, %0
  ret void
}

define i32 @next() nounwind {
  %ch = alloca i32, align 4
  %1 = load i8** @inptr, align 4
  %2 = load i8* %1
  %3 = sext i8 %2 to i32
  %4 = and i32 %3, 255
  store i32 %4, i32* %ch, align 4
  %5 = load i8** @inptr, align 4
  %6 = getelementptr inbounds i8* %5, i32 1
  store i8* %6, i8** @inptr, align 4
  %7 = load i32* %ch, align 4
  ret i32 %7
}

define void @rdata() nounwind {
  %sect = alloca i32, align 4
  %ch = alloca i8, align 1
  store i8* getelementptr inbounds ([0 x i8]* @data_file, i32 0, i32 0), i8** @inptr, align 4
  store i32 1, i32* @clsses, align 4
  br label %1

; <label>:1                                       ; preds = %98, %0
  %2 = call i32 @next()
  %3 = sub nsw i32 %2, 48
  store i32 %3, i32* %sect, align 4
  %4 = call i32 @next()
  %5 = trunc i32 %4 to i8
  store i8 %5, i8* %ch, align 1
  %6 = sext i8 %5 to i32
  %7 = icmp ne i32 %6, 10
  br i1 %7, label %8, label %20

; <label>:8                                       ; preds = %1
  br label %9

; <label>:9                                       ; preds = %12, %8
  %10 = call i32 @next()
  %11 = icmp ne i32 %10, 10
  br i1 %11, label %12, label %13

; <label>:12                                      ; preds = %9
  br label %9

; <label>:13                                      ; preds = %9
  %14 = load i32* %sect, align 4
  %15 = mul nsw i32 10, %14
  %16 = load i8* %ch, align 1
  %17 = sext i8 %16 to i32
  %18 = add nsw i32 %15, %17
  %19 = sub nsw i32 %18, 48
  store i32 %19, i32* %sect, align 4
  br label %20

; <label>:20                                      ; preds = %13, %1
  %21 = load i32* %sect, align 4
  %22 = icmp eq i32 %21, 0
  br i1 %22, label %23, label %24

; <label>:23                                      ; preds = %20
  ret void

; <label>:24                                      ; preds = %20
  %25 = load i32* %sect, align 4
  %26 = icmp eq i32 %25, 1
  br i1 %26, label %27, label %28

; <label>:27                                      ; preds = %24
  call void @rdesc(i32 1)
  br label %88

; <label>:28                                      ; preds = %24
  %29 = load i32* %sect, align 4
  %30 = icmp eq i32 %29, 2
  br i1 %30, label %31, label %32

; <label>:31                                      ; preds = %28
  call void @rdesc(i32 2)
  br label %87

; <label>:32                                      ; preds = %28
  %33 = load i32* %sect, align 4
  %34 = icmp eq i32 %33, 3
  br i1 %34, label %35, label %36

; <label>:35                                      ; preds = %32
  call void @rtrav()
  br label %86

; <label>:36                                      ; preds = %32
  %37 = load i32* %sect, align 4
  %38 = icmp eq i32 %37, 4
  br i1 %38, label %39, label %40

; <label>:39                                      ; preds = %36
  call void @rvoc()
  br label %85

; <label>:40                                      ; preds = %36
  %41 = load i32* %sect, align 4
  %42 = icmp eq i32 %41, 5
  br i1 %42, label %43, label %44

; <label>:43                                      ; preds = %40
  call void @rdesc(i32 5)
  br label %84

; <label>:44                                      ; preds = %40
  %45 = load i32* %sect, align 4
  %46 = icmp eq i32 %45, 6
  br i1 %46, label %47, label %48

; <label>:47                                      ; preds = %44
  call void @rdesc(i32 6)
  br label %83

; <label>:48                                      ; preds = %44
  %49 = load i32* %sect, align 4
  %50 = icmp eq i32 %49, 7
  br i1 %50, label %51, label %52

; <label>:51                                      ; preds = %48
  call void @rlocs()
  br label %82

; <label>:52                                      ; preds = %48
  %53 = load i32* %sect, align 4
  %54 = icmp eq i32 %53, 8
  br i1 %54, label %55, label %56

; <label>:55                                      ; preds = %52
  call void @rdflt()
  br label %81

; <label>:56                                      ; preds = %52
  %57 = load i32* %sect, align 4
  %58 = icmp eq i32 %57, 9
  br i1 %58, label %59, label %60

; <label>:59                                      ; preds = %56
  call void @rliq()
  br label %80

; <label>:60                                      ; preds = %56
  %61 = load i32* %sect, align 4
  %62 = icmp eq i32 %61, 10
  br i1 %62, label %63, label %64

; <label>:63                                      ; preds = %60
  call void @rdesc(i32 10)
  br label %79

; <label>:64                                      ; preds = %60
  %65 = load i32* %sect, align 4
  %66 = icmp eq i32 %65, 11
  br i1 %66, label %67, label %68

; <label>:67                                      ; preds = %64
  call void @rhints()
  br label %78

; <label>:68                                      ; preds = %64
  %69 = load i32* %sect, align 4
  %70 = icmp eq i32 %69, 12
  br i1 %70, label %71, label %72

; <label>:71                                      ; preds = %68
  call void @rdesc(i32 12)
  br label %77

; <label>:72                                      ; preds = %68
  call void @show_string(i8* getelementptr inbounds ([30 x i8]* @.str2, i32 0, i32 0))
  %73 = load i32* %sect, align 4
  call void @show_int(i32 %73)
  call void @show_string(i8* getelementptr inbounds ([2 x i8]* @.str3, i32 0, i32 0))
  br label %74

; <label>:74                                      ; preds = %74, %72
  %75 = call i32 @next()
  %76 = trunc i32 %75 to i8
  call void @show_onechar(i8 signext %76)
  br label %74

; <label>:77                                      ; preds = %71
  br label %78

; <label>:78                                      ; preds = %77, %67
  br label %79

; <label>:79                                      ; preds = %78, %63
  br label %80

; <label>:80                                      ; preds = %79, %59
  br label %81

; <label>:81                                      ; preds = %80, %55
  br label %82

; <label>:82                                      ; preds = %81, %51
  br label %83

; <label>:83                                      ; preds = %82, %47
  br label %84

; <label>:84                                      ; preds = %83, %43
  br label %85

; <label>:85                                      ; preds = %84, %39
  br label %86

; <label>:86                                      ; preds = %85, %35
  br label %87

; <label>:87                                      ; preds = %86, %31
  br label %88

; <label>:88                                      ; preds = %87, %27
  %89 = load i8* @breakch, align 1
  %90 = sext i8 %89 to i32
  %91 = icmp ne i32 %90, 10
  br i1 %91, label %92, label %98

; <label>:92                                      ; preds = %88
  br label %93

; <label>:93                                      ; preds = %96, %92
  %94 = call i32 @next()
  %95 = icmp ne i32 %94, 10
  br i1 %95, label %96, label %97

; <label>:96                                      ; preds = %93
  br label %93

; <label>:97                                      ; preds = %93
  br label %98

; <label>:98                                      ; preds = %97, %88
  br label %1
}

define void @rdesc(i32 %sect) nounwind {
  %1 = alloca i32, align 4
  %locc = alloca i32, align 4
  %seekstart = alloca i8*, align 4
  %maystart = alloca i8*, align 4
  store i32 %sect, i32* %1, align 4
  %2 = load i8** @inptr, align 4
  store i8* %2, i8** @seekhere, align 4
  store i32 -1, i32* @oldloc, align 4
  %3 = load i8** @seekhere, align 4
  store i8* %3, i8** %seekstart, align 4
  br label %4

; <label>:4                                       ; preds = %150, %0
  %5 = load i8** @inptr, align 4
  store i8* %5, i8** %maystart, align 4
  %6 = call i32 @rnum()
  store i32 %6, i32* %locc, align 4
  %7 = load i32* @oldloc, align 4
  %8 = icmp ne i32 %6, %7
  br i1 %8, label %9, label %122

; <label>:9                                       ; preds = %4
  %10 = load i32* @oldloc, align 4
  %11 = icmp sge i32 %10, 0
  br i1 %11, label %12, label %122

; <label>:12                                      ; preds = %9
  %13 = load i32* %1, align 4
  %14 = icmp eq i32 %13, 5
  br i1 %14, label %15, label %21

; <label>:15                                      ; preds = %12
  %16 = load i32* %locc, align 4
  %17 = icmp eq i32 %16, 0
  br i1 %17, label %122, label %18

; <label>:18                                      ; preds = %15
  %19 = load i32* %locc, align 4
  %20 = icmp sge i32 %19, 100
  br i1 %20, label %122, label %21

; <label>:21                                      ; preds = %18, %12
  %22 = load i32* %1, align 4
  switch i32 %22, label %113 [
    i32 1, label %23
    i32 2, label %36
    i32 5, label %49
    i32 6, label %62
    i32 10, label %79
    i32 12, label %96
  ]

; <label>:23                                      ; preds = %21
  %24 = load i8** @seekhere, align 4
  %25 = load i32* @oldloc, align 4
  %26 = getelementptr inbounds [141 x %struct.text]* @ltext, i32 0, i32 %25
  %27 = getelementptr inbounds %struct.text* %26, i32 0, i32 0
  store i8* %24, i8** %27, align 4
  %28 = load i8** %maystart, align 4
  %29 = load i8** %seekstart, align 4
  %30 = ptrtoint i8* %28 to i32
  %31 = ptrtoint i8* %29 to i32
  %32 = sub i32 %30, %31
  %33 = load i32* @oldloc, align 4
  %34 = getelementptr inbounds [141 x %struct.text]* @ltext, i32 0, i32 %33
  %35 = getelementptr inbounds %struct.text* %34, i32 0, i32 1
  store i32 %32, i32* %35, align 4
  br label %114

; <label>:36                                      ; preds = %21
  %37 = load i8** @seekhere, align 4
  %38 = load i32* @oldloc, align 4
  %39 = getelementptr inbounds [141 x %struct.text]* @stext, i32 0, i32 %38
  %40 = getelementptr inbounds %struct.text* %39, i32 0, i32 0
  store i8* %37, i8** %40, align 4
  %41 = load i8** %maystart, align 4
  %42 = load i8** %seekstart, align 4
  %43 = ptrtoint i8* %41 to i32
  %44 = ptrtoint i8* %42 to i32
  %45 = sub i32 %43, %44
  %46 = load i32* @oldloc, align 4
  %47 = getelementptr inbounds [141 x %struct.text]* @stext, i32 0, i32 %46
  %48 = getelementptr inbounds %struct.text* %47, i32 0, i32 1
  store i32 %45, i32* %48, align 4
  br label %114

; <label>:49                                      ; preds = %21
  %50 = load i8** @seekhere, align 4
  %51 = load i32* @oldloc, align 4
  %52 = getelementptr inbounds [101 x %struct.text]* @ptext, i32 0, i32 %51
  %53 = getelementptr inbounds %struct.text* %52, i32 0, i32 0
  store i8* %50, i8** %53, align 4
  %54 = load i8** %maystart, align 4
  %55 = load i8** %seekstart, align 4
  %56 = ptrtoint i8* %54 to i32
  %57 = ptrtoint i8* %55 to i32
  %58 = sub i32 %56, %57
  %59 = load i32* @oldloc, align 4
  %60 = getelementptr inbounds [101 x %struct.text]* @ptext, i32 0, i32 %59
  %61 = getelementptr inbounds %struct.text* %60, i32 0, i32 1
  store i32 %58, i32* %61, align 4
  br label %114

; <label>:62                                      ; preds = %21
  %63 = load i32* @oldloc, align 4
  %64 = icmp sge i32 %63, 205
  br i1 %64, label %65, label %66

; <label>:65                                      ; preds = %62
  call void @errx(i32 1, i8* getelementptr inbounds ([21 x i8]* @.str4, i32 0, i32 0))
  br label %66

; <label>:66                                      ; preds = %65, %62
  %67 = load i8** @seekhere, align 4
  %68 = load i32* @oldloc, align 4
  %69 = getelementptr inbounds [205 x %struct.text]* @rtext, i32 0, i32 %68
  %70 = getelementptr inbounds %struct.text* %69, i32 0, i32 0
  store i8* %67, i8** %70, align 4
  %71 = load i8** %maystart, align 4
  %72 = load i8** %seekstart, align 4
  %73 = ptrtoint i8* %71 to i32
  %74 = ptrtoint i8* %72 to i32
  %75 = sub i32 %73, %74
  %76 = load i32* @oldloc, align 4
  %77 = getelementptr inbounds [205 x %struct.text]* @rtext, i32 0, i32 %76
  %78 = getelementptr inbounds %struct.text* %77, i32 0, i32 1
  store i32 %75, i32* %78, align 4
  br label %114

; <label>:79                                      ; preds = %21
  %80 = load i8** @seekhere, align 4
  %81 = load i32* @clsses, align 4
  %82 = getelementptr inbounds [12 x %struct.text]* @ctext, i32 0, i32 %81
  %83 = getelementptr inbounds %struct.text* %82, i32 0, i32 0
  store i8* %80, i8** %83, align 4
  %84 = load i8** %maystart, align 4
  %85 = load i8** %seekstart, align 4
  %86 = ptrtoint i8* %84 to i32
  %87 = ptrtoint i8* %85 to i32
  %88 = sub i32 %86, %87
  %89 = load i32* @clsses, align 4
  %90 = getelementptr inbounds [12 x %struct.text]* @ctext, i32 0, i32 %89
  %91 = getelementptr inbounds %struct.text* %90, i32 0, i32 1
  store i32 %88, i32* %91, align 4
  %92 = load i32* @oldloc, align 4
  %93 = load i32* @clsses, align 4
  %94 = add nsw i32 %93, 1
  store i32 %94, i32* @clsses, align 4
  %95 = getelementptr inbounds [12 x i32]* @cval, i32 0, i32 %93
  store i32 %92, i32* %95
  br label %114

; <label>:96                                      ; preds = %21
  %97 = load i32* @oldloc, align 4
  %98 = icmp sge i32 %97, 35
  br i1 %98, label %99, label %100

; <label>:99                                      ; preds = %96
  call void @errx(i32 1, i8* getelementptr inbounds ([20 x i8]* @.str5, i32 0, i32 0))
  br label %100

; <label>:100                                     ; preds = %99, %96
  %101 = load i8** @seekhere, align 4
  %102 = load i32* @oldloc, align 4
  %103 = getelementptr inbounds [35 x %struct.text]* @mtext, i32 0, i32 %102
  %104 = getelementptr inbounds %struct.text* %103, i32 0, i32 0
  store i8* %101, i8** %104, align 4
  %105 = load i8** %maystart, align 4
  %106 = load i8** %seekstart, align 4
  %107 = ptrtoint i8* %105 to i32
  %108 = ptrtoint i8* %106 to i32
  %109 = sub i32 %107, %108
  %110 = load i32* @oldloc, align 4
  %111 = getelementptr inbounds [35 x %struct.text]* @mtext, i32 0, i32 %110
  %112 = getelementptr inbounds %struct.text* %111, i32 0, i32 1
  store i32 %109, i32* %112, align 4
  br label %114

; <label>:113                                     ; preds = %21
  call void @errx(i32 1, i8* getelementptr inbounds ([30 x i8]* @.str6, i32 0, i32 0))
  br label %114

; <label>:114                                     ; preds = %113, %100, %79, %66, %49, %36, %23
  %115 = load i8** %maystart, align 4
  %116 = load i8** %seekstart, align 4
  %117 = ptrtoint i8* %115 to i32
  %118 = ptrtoint i8* %116 to i32
  %119 = sub i32 %117, %118
  %120 = load i8** @seekhere, align 4
  %121 = getelementptr inbounds i8* %120, i32 %119
  store i8* %121, i8** @seekhere, align 4
  br label %122

; <label>:122                                     ; preds = %114, %18, %15, %9, %4
  %123 = load i32* %locc, align 4
  %124 = icmp slt i32 %123, 0
  br i1 %124, label %125, label %128

; <label>:125                                     ; preds = %122
  %126 = load i8** @seekhere, align 4
  %127 = getelementptr inbounds i8* %126, i32 3
  store i8* %127, i8** @seekhere, align 4
  ret void

; <label>:128                                     ; preds = %122
  %129 = load i32* %1, align 4
  %130 = icmp ne i32 %129, 5
  br i1 %130, label %137, label %131

; <label>:131                                     ; preds = %128
  %132 = load i32* %locc, align 4
  %133 = icmp sgt i32 %132, 0
  br i1 %133, label %134, label %145

; <label>:134                                     ; preds = %131
  %135 = load i32* %locc, align 4
  %136 = icmp slt i32 %135, 100
  br i1 %136, label %137, label %145

; <label>:137                                     ; preds = %134, %128
  %138 = load i32* @oldloc, align 4
  %139 = load i32* %locc, align 4
  %140 = icmp ne i32 %138, %139
  br i1 %140, label %141, label %143

; <label>:141                                     ; preds = %137
  %142 = load i8** %maystart, align 4
  store i8* %142, i8** %seekstart, align 4
  br label %143

; <label>:143                                     ; preds = %141, %137
  %144 = load i32* %locc, align 4
  store i32 %144, i32* @oldloc, align 4
  br label %145

; <label>:145                                     ; preds = %143, %134, %131
  br label %146

; <label>:146                                     ; preds = %149, %145
  %147 = call i32 @next()
  %148 = icmp ne i32 %147, 10
  br i1 %148, label %149, label %150

; <label>:149                                     ; preds = %146
  br label %146

; <label>:150                                     ; preds = %146
  br label %4
}

define void @rtrav() nounwind {
  %locc = alloca i32, align 4
  %t = alloca %struct.travlist*, align 4
  %s = alloca i8*, align 4
  %buf = alloca [12 x i8], align 1
  %len = alloca i32, align 4
  %m = alloca i32, align 4
  %n = alloca i32, align 4
  %entries = alloca i32, align 4
  store %struct.travlist* null, %struct.travlist** %t, align 4
  store i32 0, i32* %entries, align 4
  store i32 -1, i32* @oldloc, align 4
  br label %1

; <label>:1                                       ; preds = %96, %0
  %2 = call i32 @rnum()
  store i32 %2, i32* %locc, align 4
  %3 = load i32* @oldloc, align 4
  %4 = icmp ne i32 %2, %3
  br i1 %4, label %5, label %11

; <label>:5                                       ; preds = %1
  %6 = load i32* @oldloc, align 4
  %7 = icmp sge i32 %6, 0
  br i1 %7, label %8, label %11

; <label>:8                                       ; preds = %5
  %9 = load %struct.travlist** %t, align 4
  %10 = getelementptr inbounds %struct.travlist* %9, i32 0, i32 0
  store %struct.travlist* null, %struct.travlist** %10, align 4
  br label %11

; <label>:11                                      ; preds = %8, %5, %1
  %12 = load i32* %locc, align 4
  %13 = icmp eq i32 %12, -1
  br i1 %13, label %14, label %15

; <label>:14                                      ; preds = %11
  ret void

; <label>:15                                      ; preds = %11
  %16 = load i32* %locc, align 4
  %17 = load i32* @oldloc, align 4
  %18 = icmp ne i32 %16, %17
  br i1 %18, label %19, label %29

; <label>:19                                      ; preds = %15
  %20 = call i8* @not_malloc(i32 16)
  %21 = bitcast i8* %20 to %struct.travlist*
  %22 = load i32* %locc, align 4
  %23 = getelementptr inbounds [141 x %struct.travlist*]* @travel, i32 0, i32 %22
  store %struct.travlist* %21, %struct.travlist** %23
  store %struct.travlist* %21, %struct.travlist** %t, align 4
  %24 = load %struct.travlist** %t, align 4
  %25 = icmp eq %struct.travlist* %24, null
  br i1 %25, label %26, label %27

; <label>:26                                      ; preds = %19
  call void @err(i32 1, i8* null)
  br label %27

; <label>:27                                      ; preds = %26, %19
  store i32 0, i32* %entries, align 4
  %28 = load i32* %locc, align 4
  store i32 %28, i32* @oldloc, align 4
  br label %29

; <label>:29                                      ; preds = %27, %15
  %30 = getelementptr inbounds [12 x i8]* %buf, i32 0, i32 0
  store i8* %30, i8** %s, align 4
  br label %31

; <label>:31                                      ; preds = %44, %29
  %32 = call i32 @next()
  %33 = trunc i32 %32 to i8
  %34 = load i8** %s, align 4
  store i8 %33, i8* %34
  %35 = sext i8 %33 to i32
  %36 = icmp eq i32 %35, 9
  br i1 %36, label %42, label %37

; <label>:37                                      ; preds = %31
  %38 = load i8** %s, align 4
  %39 = load i8* %38
  %40 = sext i8 %39 to i32
  %41 = icmp eq i32 %40, 10
  br i1 %41, label %42, label %43

; <label>:42                                      ; preds = %37, %31
  br label %47

; <label>:43                                      ; preds = %37
  br label %44

; <label>:44                                      ; preds = %43
  %45 = load i8** %s, align 4
  %46 = getelementptr inbounds i8* %45, i32 1
  store i8* %46, i8** %s, align 4
  br label %31

; <label>:47                                      ; preds = %42
  %48 = load i8** %s, align 4
  store i8 0, i8* %48
  %49 = getelementptr inbounds [12 x i8]* %buf, i32 0, i32 0
  %50 = call i32 @length(i8* %49)
  %51 = sub nsw i32 %50, 1
  store i32 %51, i32* %len, align 4
  %52 = load i32* %len, align 4
  %53 = icmp slt i32 %52, 4
  br i1 %53, label %54, label %57

; <label>:54                                      ; preds = %47
  store i32 0, i32* %m, align 4
  %55 = getelementptr inbounds [12 x i8]* %buf, i32 0, i32 0
  %56 = call i32 @not_atoi(i8* %55)
  store i32 %56, i32* %n, align 4
  br label %68

; <label>:57                                      ; preds = %47
  %58 = getelementptr inbounds [12 x i8]* %buf, i32 0, i32 0
  %59 = load i32* %len, align 4
  %60 = getelementptr inbounds i8* %58, i32 %59
  %61 = getelementptr inbounds i8* %60, i32 -3
  %62 = call i32 @not_atoi(i8* %61)
  store i32 %62, i32* %n, align 4
  %63 = load i32* %len, align 4
  %64 = sub nsw i32 %63, 3
  %65 = getelementptr inbounds [12 x i8]* %buf, i32 0, i32 %64
  store i8 0, i8* %65
  %66 = getelementptr inbounds [12 x i8]* %buf, i32 0, i32 0
  %67 = call i32 @not_atoi(i8* %66)
  store i32 %67, i32* %m, align 4
  br label %68

; <label>:68                                      ; preds = %57, %54
  br label %69

; <label>:69                                      ; preds = %86, %68
  %70 = load i8* @breakch, align 1
  %71 = sext i8 %70 to i32
  %72 = icmp ne i32 %71, 10
  br i1 %72, label %73, label %96

; <label>:73                                      ; preds = %69
  %74 = load i32* %entries, align 4
  %75 = add nsw i32 %74, 1
  store i32 %75, i32* %entries, align 4
  %76 = icmp ne i32 %74, 0
  br i1 %76, label %77, label %86

; <label>:77                                      ; preds = %73
  %78 = call i8* @not_malloc(i32 16)
  %79 = bitcast i8* %78 to %struct.travlist*
  %80 = load %struct.travlist** %t, align 4
  %81 = getelementptr inbounds %struct.travlist* %80, i32 0, i32 0
  store %struct.travlist* %79, %struct.travlist** %81, align 4
  store %struct.travlist* %79, %struct.travlist** %t, align 4
  %82 = load %struct.travlist** %t, align 4
  %83 = icmp eq %struct.travlist* %82, null
  br i1 %83, label %84, label %85

; <label>:84                                      ; preds = %77
  call void @err(i32 1, i8* null)
  br label %85

; <label>:85                                      ; preds = %84, %77
  br label %86

; <label>:86                                      ; preds = %85, %73
  %87 = call i32 @rnum()
  %88 = load %struct.travlist** %t, align 4
  %89 = getelementptr inbounds %struct.travlist* %88, i32 0, i32 3
  store i32 %87, i32* %89, align 4
  %90 = load i32* %n, align 4
  %91 = load %struct.travlist** %t, align 4
  %92 = getelementptr inbounds %struct.travlist* %91, i32 0, i32 2
  store i32 %90, i32* %92, align 4
  %93 = load i32* %m, align 4
  %94 = load %struct.travlist** %t, align 4
  %95 = getelementptr inbounds %struct.travlist* %94, i32 0, i32 1
  store i32 %93, i32* %95, align 4
  br label %69

; <label>:96                                      ; preds = %69
  br label %1
}

define void @rvoc() nounwind {
  %s = alloca i8*, align 4
  %index = alloca i32, align 4
  %buf = alloca [6 x i8], align 1
  br label %1

; <label>:1                                       ; preds = %51, %0
  %2 = call i32 @rnum()
  store i32 %2, i32* %index, align 4
  %3 = load i32* %index, align 4
  %4 = icmp slt i32 %3, 0
  br i1 %4, label %5, label %6

; <label>:5                                       ; preds = %1
  br label %56

; <label>:6                                       ; preds = %1
  %7 = getelementptr inbounds [6 x i8]* %buf, i32 0, i32 0
  store i8* %7, i8** %s, align 4
  %8 = load i8** %s, align 4
  store i8 0, i8* %8
  br label %9

; <label>:9                                       ; preds = %32, %6
  %10 = call i32 @next()
  %11 = trunc i32 %10 to i8
  %12 = load i8** %s, align 4
  store i8 %11, i8* %12
  %13 = sext i8 %11 to i32
  %14 = icmp eq i32 %13, 9
  br i1 %14, label %30, label %15

; <label>:15                                      ; preds = %9
  %16 = load i8** %s, align 4
  %17 = load i8* %16
  %18 = sext i8 %17 to i32
  %19 = icmp eq i32 %18, 10
  br i1 %19, label %30, label %20

; <label>:20                                      ; preds = %15
  %21 = load i8** %s, align 4
  %22 = load i8* %21
  %23 = sext i8 %22 to i32
  %24 = icmp eq i32 %23, 10
  br i1 %24, label %30, label %25

; <label>:25                                      ; preds = %20
  %26 = load i8** %s, align 4
  %27 = load i8* %26
  %28 = sext i8 %27 to i32
  %29 = icmp eq i32 %28, 32
  br i1 %29, label %30, label %31

; <label>:30                                      ; preds = %25, %20, %15, %9
  br label %35

; <label>:31                                      ; preds = %25
  br label %32

; <label>:32                                      ; preds = %31
  %33 = load i8** %s, align 4
  %34 = getelementptr inbounds i8* %33, i32 1
  store i8* %34, i8** %s, align 4
  br label %9

; <label>:35                                      ; preds = %30
  %36 = load i8** %s, align 4
  %37 = load i8* %36
  %38 = sext i8 %37 to i32
  %39 = icmp ne i32 %38, 10
  br i1 %39, label %40, label %51

; <label>:40                                      ; preds = %35
  %41 = load i8** %s, align 4
  %42 = load i8* %41
  %43 = sext i8 %42 to i32
  %44 = icmp ne i32 %43, 10
  br i1 %44, label %45, label %51

; <label>:45                                      ; preds = %40
  br label %46

; <label>:46                                      ; preds = %49, %45
  %47 = call i32 @next()
  %48 = icmp ne i32 %47, 10
  br i1 %48, label %49, label %50

; <label>:49                                      ; preds = %46
  br label %46

; <label>:50                                      ; preds = %46
  br label %51

; <label>:51                                      ; preds = %50, %40, %35
  %52 = load i8** %s, align 4
  store i8 0, i8* %52
  %53 = getelementptr inbounds [6 x i8]* %buf, i32 0, i32 0
  %54 = load i32* %index, align 4
  %55 = call i32 @vocab(i8* %53, i32 -2, i32 %54)
  br label %1

; <label>:56                                      ; preds = %5
  ret void
}

define void @rlocs() nounwind {
  br label %1

; <label>:1                                       ; preds = %19, %0
  %2 = call i32 @rnum()
  store i32 %2, i32* @obj, align 4
  %3 = icmp slt i32 %2, 0
  br i1 %3, label %4, label %5

; <label>:4                                       ; preds = %1
  br label %20

; <label>:5                                       ; preds = %1
  %6 = call i32 @rnum()
  %7 = load i32* @obj, align 4
  %8 = getelementptr inbounds [101 x i32]* @plac, i32 0, i32 %7
  store i32 %6, i32* %8
  %9 = load i8* @breakch, align 1
  %10 = sext i8 %9 to i32
  %11 = icmp eq i32 %10, 9
  br i1 %11, label %12, label %16

; <label>:12                                      ; preds = %5
  %13 = call i32 @rnum()
  %14 = load i32* @obj, align 4
  %15 = getelementptr inbounds [101 x i32]* @fixd, i32 0, i32 %14
  store i32 %13, i32* %15
  br label %19

; <label>:16                                      ; preds = %5
  %17 = load i32* @obj, align 4
  %18 = getelementptr inbounds [101 x i32]* @fixd, i32 0, i32 %17
  store i32 0, i32* %18
  br label %19

; <label>:19                                      ; preds = %16, %12
  br label %1

; <label>:20                                      ; preds = %4
  ret void
}

define void @rdflt() nounwind {
  br label %1

; <label>:1                                       ; preds = %5, %0
  %2 = call i32 @rnum()
  store i32 %2, i32* @verb, align 4
  %3 = icmp slt i32 %2, 0
  br i1 %3, label %4, label %5

; <label>:4                                       ; preds = %1
  br label %9

; <label>:5                                       ; preds = %1
  %6 = call i32 @rnum()
  %7 = load i32* @verb, align 4
  %8 = getelementptr inbounds [35 x i32]* @actspk, i32 0, i32 %7
  store i32 %6, i32* %8
  br label %1

; <label>:9                                       ; preds = %4
  ret void
}

define void @rliq() nounwind {
  %bitnum = alloca i32, align 4
  br label %1

; <label>:1                                       ; preds = %19, %0
  %2 = call i32 @rnum()
  store i32 %2, i32* %bitnum, align 4
  %3 = icmp slt i32 %2, 0
  br i1 %3, label %4, label %5

; <label>:4                                       ; preds = %1
  br label %20

; <label>:5                                       ; preds = %1
  br label %6

; <label>:6                                       ; preds = %18, %5
  %7 = load i32* %bitnum, align 4
  %8 = getelementptr inbounds [16 x i32]* @setbit, i32 0, i32 %7
  %9 = load i32* %8
  %10 = call i32 @rnum()
  %11 = getelementptr inbounds [141 x i32]* @cond, i32 0, i32 %10
  %12 = load i32* %11
  %13 = or i32 %12, %9
  store i32 %13, i32* %11
  %14 = load i8* @breakch, align 1
  %15 = sext i8 %14 to i32
  %16 = icmp eq i32 %15, 10
  br i1 %16, label %17, label %18

; <label>:17                                      ; preds = %6
  br label %19

; <label>:18                                      ; preds = %6
  br label %6

; <label>:19                                      ; preds = %17
  br label %1

; <label>:20                                      ; preds = %4
  ret void
}

define void @rhints() nounwind {
  %hintnum = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, i32* @hntmax, align 4
  br label %1

; <label>:1                                       ; preds = %24, %0
  %2 = call i32 @rnum()
  store i32 %2, i32* %hintnum, align 4
  %3 = icmp slt i32 %2, 0
  br i1 %3, label %4, label %5

; <label>:4                                       ; preds = %1
  br label %25

; <label>:5                                       ; preds = %1
  store i32 1, i32* %i, align 4
  br label %6

; <label>:6                                       ; preds = %15, %5
  %7 = load i32* %i, align 4
  %8 = icmp slt i32 %7, 5
  br i1 %8, label %9, label %18

; <label>:9                                       ; preds = %6
  %10 = call i32 @rnum()
  %11 = load i32* %i, align 4
  %12 = load i32* %hintnum, align 4
  %13 = getelementptr inbounds [20 x [5 x i32]]* @hints, i32 0, i32 %12
  %14 = getelementptr inbounds [5 x i32]* %13, i32 0, i32 %11
  store i32 %10, i32* %14
  br label %15

; <label>:15                                      ; preds = %9
  %16 = load i32* %i, align 4
  %17 = add nsw i32 %16, 1
  store i32 %17, i32* %i, align 4
  br label %6

; <label>:18                                      ; preds = %6
  %19 = load i32* %hintnum, align 4
  %20 = load i32* @hntmax, align 4
  %21 = icmp sgt i32 %19, %20
  br i1 %21, label %22, label %24

; <label>:22                                      ; preds = %18
  %23 = load i32* %hintnum, align 4
  store i32 %23, i32* @hntmax, align 4
  br label %24

; <label>:24                                      ; preds = %22, %18
  br label %1

; <label>:25                                      ; preds = %4
  ret void
}

declare void @show_int(i32)

declare void @show_onechar(i8 signext)

define i32 @rnum() nounwind {
  %1 = alloca i32, align 4
  %s = alloca i8*, align 4
  store i8* getelementptr inbounds ([12 x i8]* @nbf, i32 0, i32 0), i8** %s, align 4
  %2 = load i8** %s, align 4
  store i8 0, i8* %2
  br label %3

; <label>:3                                       ; preds = %21, %0
  %4 = call i32 @next()
  %5 = trunc i32 %4 to i8
  %6 = load i8** %s, align 4
  store i8 %5, i8* %6
  %7 = sext i8 %5 to i32
  %8 = icmp eq i32 %7, 9
  br i1 %8, label %19, label %9

; <label>:9                                       ; preds = %3
  %10 = load i8** %s, align 4
  %11 = load i8* %10
  %12 = sext i8 %11 to i32
  %13 = icmp eq i32 %12, 10
  br i1 %13, label %19, label %14

; <label>:14                                      ; preds = %9
  %15 = load i8** %s, align 4
  %16 = load i8* %15
  %17 = sext i8 %16 to i32
  %18 = icmp eq i32 %17, 10
  br i1 %18, label %19, label %20

; <label>:19                                      ; preds = %14, %9, %3
  br label %24

; <label>:20                                      ; preds = %14
  br label %21

; <label>:21                                      ; preds = %20
  %22 = load i8** %s, align 4
  %23 = getelementptr inbounds i8* %22, i32 1
  store i8* %23, i8** %s, align 4
  br label %3

; <label>:24                                      ; preds = %19
  %25 = load i8** %s, align 4
  %26 = load i8* %25
  store i8 %26, i8* @breakch, align 1
  %27 = load i8** %s, align 4
  store i8 0, i8* %27
  %28 = load i8* getelementptr inbounds ([12 x i8]* @nbf, i32 0, i32 0)
  %29 = sext i8 %28 to i32
  %30 = icmp eq i32 %29, 45
  br i1 %30, label %31, label %32

; <label>:31                                      ; preds = %24
  store i32 -1, i32* %1
  br label %34

; <label>:32                                      ; preds = %24
  %33 = call i32 @not_atoi(i8* getelementptr inbounds ([12 x i8]* @nbf, i32 0, i32 0))
  store i32 %33, i32* %1
  br label %34

; <label>:34                                      ; preds = %32, %31
  %35 = load i32* %1
  ret i32 %35
}

declare i32 @not_atoi(i8*)

declare void @errx(i32, i8*)

declare i8* @not_malloc(i32)

declare void @err(i32, i8*)

declare i32 @length(i8*)

declare i32 @vocab(i8*, i32, i32)

define void @speak(%struct.text* %msg) nounwind {
  %1 = alloca %struct.text*, align 4
  %s = alloca i8*, align 4
  %nonfirst = alloca i8, align 1
  store %struct.text* %msg, %struct.text** %1, align 4
  %2 = load %struct.text** %1, align 4
  %3 = getelementptr inbounds %struct.text* %2, i32 0, i32 0
  %4 = load i8** %3, align 4
  store i8* %4, i8** %s, align 4
  store i8 0, i8* %nonfirst, align 1
  br label %5

; <label>:5                                       ; preds = %61, %0
  %6 = load i8** %s, align 4
  %7 = load %struct.text** %1, align 4
  %8 = getelementptr inbounds %struct.text* %7, i32 0, i32 0
  %9 = load i8** %8, align 4
  %10 = ptrtoint i8* %6 to i32
  %11 = ptrtoint i8* %9 to i32
  %12 = sub i32 %10, %11
  %13 = load %struct.text** %1, align 4
  %14 = getelementptr inbounds %struct.text* %13, i32 0, i32 1
  %15 = load i32* %14, align 4
  %16 = icmp slt i32 %12, %15
  br i1 %16, label %17, label %62

; <label>:17                                      ; preds = %5
  br label %18

; <label>:18                                      ; preds = %24, %17
  %19 = load i8** %s, align 4
  %20 = getelementptr inbounds i8* %19, i32 1
  store i8* %20, i8** %s, align 4
  %21 = load i8* %19
  %22 = sext i8 %21 to i32
  %23 = icmp ne i32 %22, 9
  br i1 %23, label %24, label %25

; <label>:24                                      ; preds = %18
  br label %18

; <label>:25                                      ; preds = %18
  %26 = load i8** %s, align 4
  %27 = load i8* %26
  %28 = sext i8 %27 to i32
  %29 = icmp eq i32 %28, 62
  br i1 %29, label %30, label %43

; <label>:30                                      ; preds = %25
  %31 = load i8** %s, align 4
  %32 = getelementptr inbounds i8* %31, i32 1
  %33 = load i8* %32
  %34 = sext i8 %33 to i32
  %35 = icmp eq i32 %34, 36
  br i1 %35, label %36, label %43

; <label>:36                                      ; preds = %30
  %37 = load i8** %s, align 4
  %38 = getelementptr inbounds i8* %37, i32 2
  %39 = load i8* %38
  %40 = sext i8 %39 to i32
  %41 = icmp eq i32 %40, 60
  br i1 %41, label %42, label %43

; <label>:42                                      ; preds = %36
  br label %62

; <label>:43                                      ; preds = %36, %30, %25
  %44 = load i32* @blklin, align 4
  %45 = icmp ne i32 %44, 0
  br i1 %45, label %46, label %51

; <label>:46                                      ; preds = %43
  %47 = load i8* %nonfirst, align 1
  %48 = add i8 %47, 1
  store i8 %48, i8* %nonfirst, align 1
  %49 = icmp ne i8 %47, 0
  br i1 %49, label %51, label %50

; <label>:50                                      ; preds = %46
  call void @show_onechar(i8 signext 10)
  br label %51

; <label>:51                                      ; preds = %50, %46, %43
  br label %52

; <label>:52                                      ; preds = %55, %51
  %53 = load i8** %s, align 4
  %54 = load i8* %53
  call void @show_onechar(i8 signext %54)
  br label %55

; <label>:55                                      ; preds = %52
  %56 = load i8** %s, align 4
  %57 = getelementptr inbounds i8* %56, i32 1
  store i8* %57, i8** %s, align 4
  %58 = load i8* %56
  %59 = sext i8 %58 to i32
  %60 = icmp ne i32 %59, 10
  br i1 %60, label %52, label %61

; <label>:61                                      ; preds = %55
  br label %5

; <label>:62                                      ; preds = %42, %5
  ret void
}

define void @pspeak(i32 %m, i32 %skip) nounwind {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %s = alloca i8*, align 4
  %nonfirst = alloca i8, align 1
  %numst = alloca i8*, align 4
  %save = alloca i8, align 1
  %msg = alloca %struct.text*, align 4
  %tbuf = alloca i8*, align 4
  store i32 %m, i32* %1, align 4
  store i32 %skip, i32* %2, align 4
  %3 = load i32* %1, align 4
  %4 = getelementptr inbounds [101 x %struct.text]* @ptext, i32 0, i32 %3
  store %struct.text* %4, %struct.text** %msg, align 4
  %5 = load %struct.text** %msg, align 4
  %6 = getelementptr inbounds %struct.text* %5, i32 0, i32 1
  %7 = load i32* %6, align 4
  %8 = add nsw i32 %7, 1
  %9 = call i8* @tbuf_malloc(i32 %8)
  store i8* %9, i8** %tbuf, align 4
  %10 = icmp eq i8* %9, null
  br i1 %10, label %11, label %12

; <label>:11                                      ; preds = %0
  call void @err(i32 1, i8* null)
  br label %12

; <label>:12                                      ; preds = %11, %0
  %13 = load i8** %tbuf, align 4
  %14 = load %struct.text** %msg, align 4
  %15 = getelementptr inbounds %struct.text* %14, i32 0, i32 0
  %16 = load i8** %15, align 4
  %17 = load %struct.text** %msg, align 4
  %18 = getelementptr inbounds %struct.text* %17, i32 0, i32 1
  %19 = load i32* %18, align 4
  %20 = add nsw i32 %19, 1
  call void @not_memcpy(i8* %13, i8* %16, i32 %20)
  %21 = load i8** %tbuf, align 4
  store i8* %21, i8** %s, align 4
  store i8 0, i8* %nonfirst, align 1
  br label %22

; <label>:22                                      ; preds = %105, %64, %12
  %23 = load i8** %s, align 4
  %24 = load i8** %tbuf, align 4
  %25 = ptrtoint i8* %23 to i32
  %26 = ptrtoint i8* %24 to i32
  %27 = sub i32 %25, %26
  %28 = load %struct.text** %msg, align 4
  %29 = getelementptr inbounds %struct.text* %28, i32 0, i32 1
  %30 = load i32* %29, align 4
  %31 = icmp slt i32 %27, %30
  br i1 %31, label %32, label %106

; <label>:32                                      ; preds = %22
  %33 = load i8** %s, align 4
  store i8* %33, i8** %numst, align 4
  br label %34

; <label>:34                                      ; preds = %40, %32
  %35 = load i8** %s, align 4
  %36 = load i8* %35
  %37 = sext i8 %36 to i32
  %38 = icmp ne i32 %37, 9
  br i1 %38, label %39, label %43

; <label>:39                                      ; preds = %34
  br label %40

; <label>:40                                      ; preds = %39
  %41 = load i8** %s, align 4
  %42 = getelementptr inbounds i8* %41, i32 1
  store i8* %42, i8** %s, align 4
  br label %34

; <label>:43                                      ; preds = %34
  %44 = load i8** %s, align 4
  %45 = load i8* %44
  store i8 %45, i8* %save, align 1
  %46 = load i8** %s, align 4
  %47 = getelementptr inbounds i8* %46, i32 1
  store i8* %47, i8** %s, align 4
  store i8 0, i8* %46
  %48 = load i8** %numst, align 4
  %49 = call i32 @not_atoi(i8* %48)
  %50 = load i32* %2, align 4
  %51 = mul nsw i32 100, %50
  %52 = icmp ne i32 %49, %51
  br i1 %52, label %53, label %65

; <label>:53                                      ; preds = %43
  %54 = load i32* %2, align 4
  %55 = icmp sge i32 %54, 0
  br i1 %55, label %56, label %65

; <label>:56                                      ; preds = %53
  br label %57

; <label>:57                                      ; preds = %63, %56
  %58 = load i8** %s, align 4
  %59 = getelementptr inbounds i8* %58, i32 1
  store i8* %59, i8** %s, align 4
  %60 = load i8* %58
  %61 = sext i8 %60 to i32
  %62 = icmp ne i32 %61, 10
  br i1 %62, label %63, label %64

; <label>:63                                      ; preds = %57
  br label %57

; <label>:64                                      ; preds = %57
  br label %22

; <label>:65                                      ; preds = %53, %43
  %66 = load i8** %s, align 4
  %67 = load i8* %66
  %68 = sext i8 %67 to i32
  %69 = icmp eq i32 %68, 62
  br i1 %69, label %70, label %83

; <label>:70                                      ; preds = %65
  %71 = load i8** %s, align 4
  %72 = getelementptr inbounds i8* %71, i32 1
  %73 = load i8* %72
  %74 = sext i8 %73 to i32
  %75 = icmp eq i32 %74, 36
  br i1 %75, label %76, label %83

; <label>:76                                      ; preds = %70
  %77 = load i8** %s, align 4
  %78 = getelementptr inbounds i8* %77, i32 2
  %79 = load i8* %78
  %80 = sext i8 %79 to i32
  %81 = icmp eq i32 %80, 60
  br i1 %81, label %82, label %83

; <label>:82                                      ; preds = %76
  br label %106

; <label>:83                                      ; preds = %76, %70, %65
  %84 = load i32* @blklin, align 4
  %85 = icmp ne i32 %84, 0
  br i1 %85, label %86, label %91

; <label>:86                                      ; preds = %83
  %87 = load i8* %nonfirst, align 1
  %88 = add i8 %87, 1
  store i8 %88, i8* %nonfirst, align 1
  %89 = icmp ne i8 %87, 0
  br i1 %89, label %91, label %90

; <label>:90                                      ; preds = %86
  call void @show_onechar(i8 signext 10)
  br label %91

; <label>:91                                      ; preds = %90, %86, %83
  br label %92

; <label>:92                                      ; preds = %95, %91
  %93 = load i8** %s, align 4
  %94 = load i8* %93
  call void @show_onechar(i8 signext %94)
  br label %95

; <label>:95                                      ; preds = %92
  %96 = load i8** %s, align 4
  %97 = getelementptr inbounds i8* %96, i32 1
  store i8* %97, i8** %s, align 4
  %98 = load i8* %96
  %99 = sext i8 %98 to i32
  %100 = icmp ne i32 %99, 10
  br i1 %100, label %92, label %101

; <label>:101                                     ; preds = %95
  %102 = load i32* %2, align 4
  %103 = icmp slt i32 %102, 0
  br i1 %103, label %104, label %105

; <label>:104                                     ; preds = %101
  br label %106

; <label>:105                                     ; preds = %101
  br label %22

; <label>:106                                     ; preds = %104, %82, %22
  ret void
}

declare i8* @tbuf_malloc(i32)

declare void @not_memcpy(i8*, i8*, i32)
