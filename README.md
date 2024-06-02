# __Nasal - Modern Interpreter__

<img src="./doc/pic/header.png" style="width:600px"></img>

![GitHub release(latest by date)](https://img.shields.io/github/v/release/ValKmjolnir/Nasal-Interpreter?style=flat-square&logo=github)
[![license](https://img.shields.io/badge/license-GPLv2-green?style=flat-square&logo=github)](./LICENSE)
![downloads](https://img.shields.io/github/downloads/ValKmjolnir/Nasal-Interpreter/total.svg?style=flat-square&logo=github)
[![C/C++ CI](https://github.com/ValKmjolnir/Nasal-Interpreter/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/ValKmjolnir/Nasal-Interpreter/actions/workflows/c-cpp.yml)

> This document is also available in: [__中文__](./doc/README_zh.md) | [__English__](./README.md)

## __Contents__

* [__Introduction__](#introduction)
* [__Download__](#download)
* [__Compile__](#compile)
* [__Usage__](#how-to-use)
* [__Tutorial__](./doc/tutorial.md)
* [__Release Notes__](./doc/dev.md#release-notes)
* [__Development History__](./doc/dev.md)
* [__Benchmark__](./doc/benchmark.md)
* [__Difference__](#difference-between-andys-and-this-interpreter)
* [__Trace Back Info__](#trace-back-info)
* [__Debugger__](#debugger)
* [__REPL__](#repl)

__Contact us if having great ideas to share!__

* __lhk101lhk101@qq.com__ (ValKmjolnir)

* __sidi.liang@gmail.com__ (Sidi)

## __Introduction__

![star](https://img.shields.io/github/stars/ValKmjolnir/Nasal-Interpreter?style=flat-square&logo=github)
![fork](https://img.shields.io/github/forks/ValKmjolnir/Nasal-Interpreter?style=flat-square&logo=github)
![issue](https://img.shields.io/github/issues/ValKmjolnir/Nasal-Interpreter?style=flat-square&logo=github)
![pr](https://img.shields.io/github/issues-pr/ValKmjolnir/Nasal-Interpreter?style=flat-square&logo=github)

[Nasal](http://wiki.flightgear.org/Nasal_scripting_language)
is an ECMAscript-like language used in [FlightGear](https://www.flightgear.org/).
The designer is [Andy Ross](https://github.com/andyross).
This interpreter is rewritten by [ValKmjolnir](https://github.com/ValKmjolnir) using `C++`(`-std=c++17`).
We really appreciate that Andy created this amazing programming language: [Andy Ross's nasal interpreter](https://github.com/andyross/nasal).

Old version of this project uses __MIT license__ (2019/7 ~ 2021/5/4 ~ 2023/5). Now it uses __GPL v2 license__ (since 2023/6).

### __Why writing this Nasal interpreter?__

2019 summer,
members in [FGPRC](https://www.fgprc.org/) told me that it is hard to debug with nasal-console in Flightgear,
especially when checking syntax errors.
So i wrote a new interpreter to help checking syntax error and  runtime error.

I wrote the lexer, parser and
bytecode virtual machine to help checking errors.
We found it much easier to debug.

You could also use this language to write some
interesting programs and run them without the lib of Flightgear.
You could add your own modules to make
the interpreter a useful tool in your own projects.

## __Download__

Nightly build could be found here.
Windows nightly build is not supported yet,
please wait or just compile it by yourself, a Cmake file is given for Visual Studio to compile this project easily:

* [macOS-nightly-build](https://github.com/ValKmjolnir/Nasal-Interpreter/releases/tag/next_macOS)
* [linux-nightly-build](https://github.com/ValKmjolnir/Nasal-Interpreter/releases/tag/next_linux_x86_64)
* windows-nightly-build: [WIP]

## __Compile__

![g++](https://img.shields.io/badge/GNU-g++-A42E2B?style=flat-square&logo=GNU)
![clang++](https://img.shields.io/badge/LLVM-clang++-262D3A?style=flat-square&logo=LLVM)
![vs](https://img.shields.io/badge/Visual_Studio-MSVC-5C2D91?style=flat-square&logo=visualstudio)

Better download the latest update source of the interpreter and build it! It's quite easy to build this interpreter, what you need are only two things: C++ compiler and the `make`. There is no third-party library used in this project.

### __Windows (MinGW-w64)__

![windows](https://img.shields.io/badge/Microsoft-Windows-green?style=flat-square&logo=windows)

Make sure thread model is `posix thread model`, otherwise no thread library exists.

> mingw32-make nasal.exe -j4

### __Windows (Visual Studio)__

![windows](https://img.shields.io/badge/Microsoft-Windows-green?style=flat-square&logo=windows)

There is a [__CMakelists.txt__](./CMakeLists.txt) to create project.

### __Linux / macOS / Unix__

![linux](https://img.shields.io/badge/GNU-Linux-green?style=flat-square&logo=GNU) ![macOS](https://img.shields.io/badge/Apple%20Inc.-MacOS-green?style=flat-square&logo=apple)

> make -j

You could choose which compiler you want to use:

> make nasal CXX=... -j

## __How to Use__

![usage](./doc/gif/help.gif)

If your system is `Windows` and you want to output unicode, you could write this in nasal code:

```javascript
if (os.platform()=="windows") {
    system("chcp 65001");
}
```

Or use `std.runtime.windows.set_utf8_output()`:

```javascript
use std.runtime;

runtime.windows.set_utf8_output();
```

## __Difference Between Andy's and This Interpreter__

![error](./doc/gif/error.gif)

<details><summary>Must use `var` to define variables</summary> 

This interpreter uses more strict syntax to make sure it is easier for you to program and debug.
And flightgear's nasal interpreter also has the same rule.
So do not use variable without using `var` to declare it.

In Andy's interpreter:

```javascript
foreach(i; [0, 1, 2, 3])
    print(i)
```

This program can run normally.
But take a look at the iterator `i`,
it is defined in foreach without using keyword `var`.
I think this design will make programmers feeling confused that they maybe hard to find the `i` is defined here.
Without `var`, they may think this `i` is defined anywhere else.

So in this interpreter i use a more strict syntax to force users to use `var` to define iterator of forindex and foreach.
If you forget to add the keyword `var`, you will get this:

```javascript
code: undefined symbol "i"
 --> test.nas:1:9
  | 
1 | foreach(i; [0, 1, 2, 3])
  |         ^ undefined symbol "i"

code: undefined symbol "i"
 --> test.nas:2:11
  | 
2 |     print(i)
  |           ^ undefined symbol "i"
```

</details>

## __Trace Back Info__

![stackoverflow](./doc/gif/stackoverflow.gif)

When interpreter crashes,
it will print trace back information:

<details><summary>Native function `die`</summary>

Function `die` is used to throw error and crash immediately.

```javascript
func() {
    println("hello");
    die("error occurred this line");
    return;
}();
```

```javascript
hello
[vm] error: error occurred this line
[vm] error: error occurred in native function

call trace (main)
  call func@0x557513935710() {entry: 0x850}

trace back (main)
  0x000547     4c 00 00 16     callb   0x16 <__die@0x557512441780>(std/lib.nas:150)
  0x000856     4a 00 00 01     callfv  0x1(a.nas:3)
  0x00085a     4a 00 00 00     callfv  0x0(a.nas:5)

stack (0x5575138e8c40, limit 10, total 14)
  0x00000d    | null |
  0x00000c    | pc   | 0x856
  0x00000b    | addr | 0x5575138e8c50
  0x00000a    | nil  |
  0x000009    | nil  |
  0x000008    | str  | <0x5575138d9190> error occurred t...
  0x000007    | nil  |
  0x000006    | func | <0x5575139356f0> entry:0x850
  0x000005    | pc   | 0x85a
  0x000004    | addr | 0x0
```

</details>

<details><summary>Stack overflow</summary>

Here is an example of stack overflow:

```javascript
func(f) {
    return f(f);
}(
    func(f) {
        f(f);
    }
)();
```

```javascript
[vm] error: stack overflow

call trace (main)
  call func@0x564106058620(f) {entry: 0x859}
   --> 583 same call(s)
  call func@0x5641060586c0(f) {entry: 0x851}

trace back (main)
  0x000859     45 00 00 01     calll   0x1(a.nas:5)
  0x00085b     4a 00 00 01     callfv  0x1(a.nas:5)
  0x00085b     582 same call(s)
  0x000853     4a 00 00 01     callfv  0x1(a.nas:2)
  0x00085f     4a 00 00 01     callfv  0x1(a.nas:3)

stack (0x56410600be00, limit 10, total 4096)
  0x000fff    | func | <0x564106058600> entry:0x859
  0x000ffe    | pc   | 0x85b
  0x000ffd    | addr | 0x56410601bd20
  0x000ffc    | nil  |
  0x000ffb    | nil  |
  0x000ffa    | func | <0x564106058600> entry:0x859
  0x000ff9    | nil  |
  0x000ff8    | func | <0x564106058600> entry:0x859
  0x000ff7    | pc   | 0x85b
  0x000ff6    | addr | 0x56410601bcb0
```

</details>

<details><summary>Normal vm error crash info</summary>

Error will be thrown if there's a fatal error when executing:

```javascript
func() {
    return 0;
}()[1];
```

```javascript
[vm] error: must call a vector/hash/string but get number

trace back (main)
  0x000854     47 00 00 00     callv   0x0(a.nas:3)

stack (0x564993f462b0, limit 10, total 1)
  0x000000    | num  | 0
```

</details>

<details><summary>Detailed crash info</summary>

Use command __`-d`__ or __`--detail`__ the trace back info will show more details:

```javascript
hello
[vm] error: error occurred this line
[vm] error: error occurred in native function

call trace (main)
  call func@0x55dcb5b8fbf0() {entry: 0x850}

trace back (main)
  0x000547     4c 00 00 16     callb   0x16 <__die@0x55dcb3c41780>(std/lib.nas:150)
  0x000856     4a 00 00 01     callfv  0x1(a.nas:3)
  0x00085a     4a 00 00 00     callfv  0x0(a.nas:5)

stack (0x55dcb5b43120, limit 10, total 14)
  0x00000d    | null |
  0x00000c    | pc   | 0x856
  0x00000b    | addr | 0x55dcb5b43130
  0x00000a    | nil  |
  0x000009    | nil  |
  0x000008    | str  | <0x55dcb5b33670> error occurred t...
  0x000007    | nil  |
  0x000006    | func | <0x55dcb5b8fbd0> entry:0x850
  0x000005    | pc   | 0x85a
  0x000004    | addr | 0x0

registers (main)
  [pc    ]    | pc   | 0x547
  [global]    | addr | 0x55dcb5b53130
  [local ]    | addr | 0x55dcb5b43190
  [memr  ]    | addr | 0x0
  [canary]    | addr | 0x55dcb5b53110
  [top   ]    | addr | 0x55dcb5b431f0
  [funcr ]    | func | <0x55dcb5b65620> entry:0x547
  [upval ]    | nil  |

global (0x55dcb5b53130)
  0x000000    | nmspc| <0x55dcb5b33780> namespace [95 val]
  0x000001    | vec  | <0x55dcb5b64c20> [0 val]
  ...
  0x00005e    | func | <0x55dcb5b8fc70> entry:0x846

local (0x55dcb5b43190 <+7>)
  0x000000    | nil  |
  0x000001    | str  | <0x55dcb5b33670> error occurred t...
  0x000002    | nil  |
```

</details>

## __Debugger__

![dbg](./doc/gif/dbg.gif)

We added a debugger in `v8.0`.
Use command `./nasal -dbg xxx.nas` to use the debugger,
and the debugger will print this:

<details><summary>Click to unfold</summary>

```javascript
source code:
--> var fib = func(x) {
        if (x<2) return x;
        return fib(x-1)+fib(x-2);
    }
    for(var i=0;i<31;i+=1)
        print(fib(i),'\n');


next bytecode:
    0x0003a8    07:00 00 00 00 00 00 00 00     pnil    0x0 (std/lib.nas:413)
    0x0003a9    56:00 00 00 00 00 00 00 00     ret     0x0 (std/lib.nas:413)
    0x0003aa    03:00 00 00 00 00 00 00 56     loadg   0x56 (std/lib.nas:413)
--> 0x0003ab    0b:00 00 00 00 00 00 03 af     newf    0x3af (test/fib.nas:1)
    0x0003ac    02:00 00 00 00 00 00 00 03     intl    0x3 (test/fib.nas:1)
    0x0003ad    0d:00 00 00 00 00 00 00 22     para    0x22 (x) (test/fib.nas:1)
    0x0003ae    3e:00 00 00 00 00 00 03 be     jmp     0x3be (test/fib.nas:1)
    0x0003af    45:00 00 00 00 00 00 00 01     calll   0x1 (test/fib.nas:2)

vm stack (0x7fca7e9f1010, limit 16, total 0)
>>
```

</details>

If want help, input `h` to get help.

When running the debugger, you could see what is on stack.
This will help you debugging or learning how the vm works:

<details><summary>Click to unfold</summary>

```javascript
source code:
    var fib = func(x) {
-->     if (x<2) return x;
        return fib(x-1)+fib(x-2);
    }
    for(var i=0;i<31;i+=1)
        print(fib(i),'\n');


next bytecode:
    0x0003a8    07:00 00 00 00 00 00 00 00     pnil    0x0 (std/lib.nas:413)
    0x0003a9    56:00 00 00 00 00 00 00 00     ret     0x0 (std/lib.nas:413)
    0x0003aa    03:00 00 00 00 00 00 00 56     loadg   0x56 (std/lib.nas:413)
    0x0003ab    0b:00 00 00 00 00 00 03 af     newf    0x3af (test/fib.nas:1)
    0x0003ac    02:00 00 00 00 00 00 00 03     intl    0x3 (test/fib.nas:1)
    0x0003ad    0d:00 00 00 00 00 00 00 22     para    0x22 (x) (test/fib.nas:1)
    0x0003ae    3e:00 00 00 00 00 00 03 be     jmp     0x3be (test/fib.nas:1)
--> 0x0003af    45:00 00 00 00 00 00 00 01     calll   0x1 (test/fib.nas:2)

vm stack (0x7fca7e9f1010, limit 16, total 8)
  0x000007    | pc   | 0x3c7
  0x000006    | addr | 0x0
  0x000005    | nil  |
  0x000004    | nil  |
  0x000003    | num  | 0
  0x000002    | nil  |
  0x000001    | nil  |
  0x000000    | func | <0x5573f66ef5f0> func(elems...) {..}
>>
```

</details>

## REPL

We added experimental repl interpreter in v11.0.
Use this command to use the repl interpreter:

> nasal -r

Then enjoy!

```bash
[nasal-repl] Initializating enviroment...
[nasal-repl] Initialization complete.

Nasal REPL interpreter version 11.0 (Oct  7 2023 17:28:31)
.h, .help   | show help
.e, .exit   | quit the REPL
.q, .quit   | quit the REPL
.c, .clear  | clear the screen
.s, .source | show source code

>>>
```

Try import `std/json.nas`~

```bash
[nasal-repl] Initializating enviroment...
[nasal-repl] Initialization complete.

Nasal REPL interpreter version 11.1 (Nov  1 2023 23:37:30)
.h, .help   | show help
.e, .exit   | quit the REPL
.q, .quit   | quit the REPL
.c, .clear  | clear the screen
.s, .source | show source code

>>> use std.json;
{stringify:func(..) {..},parse:func(..) {..}}

>>> 
```
