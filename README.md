# __Nasal Scripting Language__

![GitHub code size](https://img.shields.io/github/languages/code-size/ValKmjolnir/Nasal-Interpreter?style=flat-square&logo=github)
![GitHub release(latest by date)](https://img.shields.io/github/v/release/ValKmjolnir/Nasal-Interpreter?style=flat-square&logo=github)
![in dev](https://img.shields.io/badge/dev-v10.1-blue?style=flat-square&logo=github)
[![license](https://img.shields.io/badge/license-MIT-green?style=flat-square&logo=github)](./LICENSE)

> This document is also available in: [__中文__](./doc/README_zh.md) | [__English__](./README.md)

## __Contents__

* [__Introduction__](#introduction)
* [__Compile__](#how-to-compile)
* [__Usage__](#how-to-use)
* [__Tutorial__](#tutorial)
* [__Release Notes__](./doc/dev.md#release-notes)
* [__Development History__](./doc/dev.md)
* [__Benchmark__](./doc/benchmark.md)
* [__Difference__](#difference-between-andys-and-this-interpreter)
* [__Trace Back Info__](#trace-back-info)
* [__Debugger__](#debugger)

__Contact us if having great ideas to share!__

* __E-mail__: __lhk101lhk101@qq.com__

## __Introduction__

__[Nasal](http://wiki.flightgear.org/Nasal_scripting_language)__
is an ECMAscript-like language that used in [FlightGear](https://www.flightgear.org/).
The designer is [Andy Ross](https://github.com/andyross).

This interpreter is totally rewritten by [ValKmjolnir](https://github.com/ValKmjolnir) using `C++`(`-std=c++14`)
without reusing the code in [Andy Ross's nasal interpreter](https://github.com/andyross/nasal).
But we really appreciate that Andy created this amazing programming language and his interpreter project.

This project uses __MIT license__ (2021/5/4).

__Why writing this nasal interpreter?__
In 2019 summer holiday,
members in [FGPRC](https://www.fgprc.org/) told me that it is hard to debug with nasal-console in Flightgear,
especially when checking syntax errors.
So i wrote a new interpreter to help them checking syntax error and even, runtime error.

I wrote the lexer, parser and
bytecode virtual machine to help checking errors.
We found it much easier to check syntax and runtime
errors.

You could also use this language to write some
interesting programs and run them without the lib of Flightgear.
You could add your own modules to make
the interpreter a useful tool in your own projects.

## __How to Compile__

![windows](https://img.shields.io/badge/Microsoft-Windows-green?style=flat-square&logo=windows)
![macOS](https://img.shields.io/badge/Apple%20Inc.-MacOS-green?style=flat-square&logo=apple)
![linux](https://img.shields.io/badge/GNU-Linux-green?style=flat-square&logo=GNU)

![g++](https://img.shields.io/badge/GNU-g++-A42E2B?style=flat-square&logo=GNU)
![clang++](https://img.shields.io/badge/LLVM-clang++-262D3A?style=flat-square&logo=LLVM)
![vs](https://img.shields.io/badge/Visual_Studio-MSVC-5C2D91?style=flat-square&logo=visualstudio)

Better download the latest update source of the interpreter and build it! It's quite easy to build this interpreter.
__CAUTION__: If want to use the release zip/tar.gz file to build the interpreter, please read the [__Release Notes__](./doc/dev.md#release-notes) to make sure this release file has no fatal bugs.

Use g++(`MinGW-w64`) or MSVC(`Visual Studio`) on `Windows` .

Use g++/clang++ on `Linux/macOS/Unix` platform (we suggest `clang`).

On `Windows (MinGW-w64)`:

> mingw32-make nasal.exe

You could create project in `Visual Studio` by this way: [__CLICK__](./doc/vs.md).

On `Linux/macOS/Unix`:

> make nasal

You could choose which compiler you want to use:

> make nasal CXX=...

If you think `-O3` isn't that safe and stable, you could choose:

> make stable-release

## __How to Use__

First we should learn how to write and run a program using this language,
click to see the [__tutorial__](#tutorial).

Use this command to get help:

> ./nasal -h

If your system is __`Windows`__ and you want to output unicode,please use this command before running nasal interpreter:

> chcp 65001

or you could write this in your nasal code:

```javascript
if(os.platform()=="windows")
    system("chcp 65001");
```

## __Tutorial__

Nasal is really __easy__ to learn.
Reading this tutorial will not takes you over 15 minutes.
__If you have learnt C/C++/Javascript before, this will take less time.__
You could totally use it after reading this simple tutorial:

<details><summary> Basic value type </summary>

__`vm_none`__ is error type.
This type is used to interrupt the execution of virtual machine and will not be created by user program.

__`vm_nil`__ is a null type. It means nothing.

```javascript
var spc=nil;
```

__`vm_num`__ has 3 formats: `dec`, `hex` and `oct`. Using IEEE754 `double` to store.

```javascript
# this language use '#' to write notes
var n=2.71828;    # dec
var n=2.147e16;   # dec
var n=1e-10;      # dec
var n=0xAA55;     # hex
var n=0o170001;   # oct
```

__`vm_str`__ has 3 formats. The third one is used to declare a character.

```javascript
var s='str';
var s="another string";
var s=`c`;
# some special characters is allowed in this language:
'\a'; '\b'; '\e'; '\f';
'\n'; '\r'; '\t'; '\v';
'\0'; '\\'; '\?'; '\'';
'\"';
```

__`vm_vec`__ has unlimited length and can store all types of values.

```javascript
var vec=[];
var vec=[0,nil,{},[],func(){return 0}];
append(vec,0,1,2);
```

__`vm_hash`__ is a hashmap(or like a dict in `python`) that stores values with strings/identifiers as the key.

```javascript
var hash={
    member1:nil,
    member2:"str",
    "member3":"member\'s name can also be a string constant",
    funct:func(){
        return me.member2~me.member3;
    }
};
```

__`vm_func`__ is a function type (in fact it is lambda).

```javascript
var f=func(x,y,z){return nil;}
var f=func{return 114514;}
var f=func(x,y,z,deft=1){
    return x+y+z+deft;
}
var f=func(args...){
    var sum=0;
    foreach(var i;args)
        sum+=i;
    return sum;
}
```

__`vm_upval`__ is used to store upvalues, used in __`nasal_vm`__ to make sure closure runs correctly.

__`vm_obj`__ is used to store other complex C/C++ data types.
This type is often created by native-function of nasal. If want to define your own data type, see how to add native-functions by editing this project.

</details>

<details><summary> Operators </summary>

Nasal has basic math operators `+` `-` `*` `/` and a special operator `~` that links two strings together.

```javascript
1+2-(1+3)*(2+4)/(16-9);
'str1'~'str2';
```

For conditional expressions, operators `==` `!=` `<` `>` `<=` `>=` are used to compare two values.
`and` `or` have the same function as C/C++ `&&` `||`, link comparations together.

```javascript
1+1 and 0;
1<0 or 1>0;
1<=0 and 1>=0;
1==0 or 1!=0;
```

Unary operators `-` `!` have the same function as C/C++.

```javascript
-1;
!0;
```

Operators `=` `+=` `-=` `*=` `/=` `~=` are used in assignment expressions.

```javascript
a=b=c=d=1;
a+=1; a-=1; a*=1; a/=1;
a~='string';
```

</details>

<details><summary> Definition </summary>

```javascript
var a=1;
var (a,b,c)=[0,1,2];
var (a,b,c)=(0,1,2);
(var a,b,c)=[0,1,2];
(var a,b,c)=(0,1,2);
```

</details>

<details><summary> Multi-assignment </summary>

The last one is often used to swap two variables.

```javascript
(a,b[0],c.d)=[0,1,2];
(a,b[1],c.e)=(0,1,2);
(a,b)=(b,a);
```

</details>

<details><summary> Conditional expression </summary>

In nasal there's a new key word `elsif`.
It has the same functions as `else if`.

```javascript
if(1){
    ;
}elsif(2){
    ;
}else if(3){
    ;
}else{
    ;
}
```

</details>

<details><summary> Loop </summary>

While loop and for loop is simalar to C/C++.

```javascript
while(condition)
    continue;
for(var i=0;i<10;i+=1)
    break;
```

Nasal has another two kinds of loops that iterates through a vector:

`forindex` will get the index of a vector. Index will be `0` to `size(elem)-1`.

```javascript
forindex(var i;elem)
    print(elem[i]);
```

`foreach` will get the element of a vector. Element will be `elem[0]` to `elem[size(elem)-1]`.

```javascript
foreach(var i;elem)
    print(i);
```

</details>

<details><summary> Subvec </summary>

Nasal provides this special syntax to help user generate a new vector by getting values by one index or getting values by indexes in a range from an old vector.
If there's only one index in the bracket, then we will get the value directly.
Use index to search one element in the string will get the __ascii number__ of this character.
If you want to get the character, use built-in function `chr()`.

```javascript
a[0];
a[-1,1,0:2,0:,:3,:,nil:8,3:nil,nil:nil];
"hello world"[0];
```

</details>

<details><summary> Special function call </summary>

This is of great use but is not very efficient
(because hashmap use string as the key to compare).

```javascript
f(x:0,y:nil,z:[]);
```

</details>

<details><summary> Lambda </summary>

Also functions have this kind of use:

```javascript
func(x,y){return x+y}(0,1);
func(x){return 1/(1+math.exp(-x));}(0.5);
```

There's an interesting test file `y-combinator.nas`,
try it for fun:

```javascript
var fib=func(f){
    return f(f);
}(
    func(f){
        return func(x){
            if(x<2) return x;
            return f(f)(x-1)+f(f)(x-2);
        }
    }
);
```

</details>

<details><summary> Closure </summary>

Closure means you could get the variable that is not in the local scope of a function that you called.
Here is an example, result is `1`:

```javascript
var f=func(){
    var a=1;
    return func(){return a;};
}
print(f()());
```

Using closure makes it easier to OOP.

```javascript
var student=func(n,a){
    var (name,age)=(n,a);
    return {
        print_info:func() {println(name,' ',age);},
        set_age:   func(a){age=a;},
        get_age:   func() {return age;},
        set_name:  func(n){name=n;},
        get_name:  func() {return name;}
    };
}
```

</details>

<details><summary> Trait </summary>

Also there's another way to OOP, that is `trait`.

When a hash has a member named `parents` and the value type is vector,
then when you are trying to find a member that is not in this hash,
virtual machine will search the member in `parents`.
If there is a hash that has the member, you will get the member's value.

Using this mechanism, we could OOP like this, the result is `114514`:

```javascript
var trait={
    get:func{return me.val;},
    set:func(x){me.val=x;}
};

var class={
    new:func(){
        return {
            val:nil,
            parents:[trait]
        };
    }
};
var a=class.new();
a.set(114514);
println(a.get());
```

First virtual machine cannot find member `set` in hash `a`, but in `a.parents` there's a hash `trait` has the member `set`, so we get the `set`.
variable `me` points to hash `a`, so we change the `a.val`.
And `get` has the same process.

And we must remind you that if you do this:

```javascript
var trait={
    get:func{return me.val;},
    set:func(x){me.val=x;}
};

var class={
    new:func(){
        return {
            val:nil,
            parents:[trait]
        };
    }
};
var a=class.new();
var b=class.new();
a.set(114);
b.set(514);
println(a.get());
println(b.get());

var c=a.get;
var d=b.get;

println(c());
println(c());
println(d());
println(d());
```

You will get this result now:

```bash
114
514
514
514
514
514
```

Because `a.get` will set `me=a` in the `trait.get`. Then `b.get` do the `me=b`. So in fact c is `b.get` too after running `var d=b.get`.
If you want to use this trick to make the program running more efficiently, you must know this special mechanism.

</details>

<details><summary> Native functions and module import </summary>

This part shows how we add native functions in this nasal interpreter.
If you are interested in this part, this may help you.
And...

__CAUTION:__ If you want to add your own functions __without__ changing the source code of the interpreter, see the __`module`__ after this part.

If you really want to change source code, check built-in functions in `lib.nas` and see the example below.

Definition:

```C++
nas_ref builtin_print(nas_ref*,nasal_gc&);
// you could also use a macro to define one.
nas_native(builtin_print);
```

Then complete this function using C++:

```C++
nas_ref builtin_print(nas_ref* local,nasal_gc& gc)
{
    // find value with index begin from 1
    // because local[0] is reserved for value 'me'
    nas_ref vec=local[1];
    // main process
    // also check number of arguments and type here
    // if get an error,use nas_err
    for(auto& i:vec.vec().elems)
        switch(i.type)
        {
            case vm_none: std::cout<<"undefined";   break;
            case vm_nil:  std::cout<<"nil";         break;
            case vm_num:  std::cout<<i.num();       break;
            case vm_str:  std::cout<<i.str();       break;
            case vm_vec:  i.vec().print();          break;
            case vm_hash: i.hash().print();         break;
            case vm_func: std::cout<<"func(..){..}";break;
            case vm_obj:  std::cout<<"<object>";    break;
        }
    std::cout<<std::flush;
    // generate return value,
    // use gc::alloc(type) to make a new value
    // or use reserved reference nil/one/zero
    return nil;
}
```

After that, register the built-in function's name(in nasal) and the function's pointer in this table:

```C++
struct func
{
    const char* name;
    nas_ref (*func)(nas_ref*,nasal_gc&);
} builtin[]=
{
    {"__print",builtin_print},
    {nullptr,  nullptr      }
};
```

At last,warp the `__print` in a nasal file:

```javascript
var print=func(elems...){
    return __print(elems);
};
```

In fact the arguments that `__print` uses are not necessary.
So writting it like this is also right:

```javascript
var print=func(elems...){
    return __print;
};
```

If you don't warp built-in function in a normal nasal function,
this built-in function may cause a fault when searching arguments,
which will cause __segmentation error__.

Use `import("filename.nas")` to get the nasal file including your built-in functions, then you could use it.
Also there's another way of importing nasal files, the two way of importing have the same function:

```javascript
import.dirname.dirname.filename;
import("./dirname/dirname/filename.nas");
```

When running a builtin function, alloc will run more than one time, this may cause mark-sweep in `gc::alloc`.
The value got before will be collected, but stil in use in this builtin function, this will cause a fatal error.

So use `gc::temp` in builtin functions to temprorarily store the gc-managed value that you want to return later. Like this:

```C++
nas_ref builtin_keys(nas_ref* local,nasal_gc& gc)
{
    nas_ref hash=local[1];
    if(hash.type!=vm_hash)
        return nas_err("keys","\"hash\" must be hash");
    // avoid being sweeped
    nas_ref res=gc.temp=gc.alloc(vm_vec);
    auto& vec=res.vec().elems;
    for(auto& iter:hash.hash().elems)
        vec.push_back(gc.newstr(iter.first));
    gc.temp=nil;
    return res;
}
```

</details>

<details><summary> Modules(for lib developers) </summary>

If there is only one way to add your own functions into nasal,
that is really inconvenient.

Luckily, we have developed some useful native-functions to help you add modules that created by you.

After 2021/12/3, there are some new functions added to `lib.nas`:

```javascript
var dylib=
{
    dlopen:  func(libname){return __dlopen;},
    dlsym:   func(lib,sym){return __dlsym; },
    dlclose: func(lib){return __dlclose;   },
    dlcall:  func(funcptr,args...){return __dlcall}
};
```

Aha, as you could see, these functions are used to load dynamic libraries into the nasal runtime and execute.
Let's see how they work.

First, write a cpp file that you want to generate the dynamic lib, take the `fib.cpp` as the example(example codes are in `./module`):

```C++
// add header file nasal.h to get api
#include "nasal.h"
double fibonaci(double x){
    if(x<=2)
        return x;
    return fibonaci(x-1)+fibonaci(x-2);
}
// remember to use extern "C",
// so you could search the symbol quickly
extern "C" nas_ref fib(std::vector<nas_ref>& args,nasal_gc& gc){
    // the arguments are generated into a vm_vec: args
    // get values from the vector that must be used here
    nas_ref num=args[0];
    // if you want your function safer, try this
    // nas_err will print the error info on screen
    // and return vm_null for runtime to interrupt
    if(num.type!=vm_num)
        return nas_err("extern_fib","\"num\" must be number");
    // ok, you must know that vm_num now is not managed by gc
    // if want to return a gc object, use gc.alloc(type)
    // usage of gc is the same as adding a native function
    return {vm_num,fibonaci(num.tonum())};
}
```

Next, compile this `fib.cpp` into dynamic lib.

Linux(`.so`):

`clang++ -c -O3 fib.cpp -fPIC -o fib.o`

`clang++ -shared -o libfib.so fib.o`

Mac(`.so` & `.dylib`): same as Linux.

Windows(`.dll`):

`g++ -c -O3 fib.cpp -fPIC -o fib.o`

`g++ -shared -o libfib.dll fib.o`

Then we write a test nasal file to run this fib function, using `os.platform()` we could write a program that runs on three different OS:

```javascript
var dlhandle=dylib.dlopen("libfib."~(os.platform()=="windows"?"dll":"so"));
var fib=dylib.dlsym(dlhandle,"fib");
for(var i=1;i<30;i+=1)
    println(dylib.dlcall(fib,i));
dylib.dlclose(dlhandle);
```

`dylib.dlopen` is used to load dynamic library.

`dylib.dlsym` is used to get the function address.

`dylib.dlcall` is used to call the function, the first argument is the function address, make sure this argument is vm_obj and type=obj_extern.

`dylib.dlclose` is used to unload the library, at the moment that you call the function, all the function addresses that got from it are invalid.

If get this, Congratulations!

```bash
./nasal a.nas
1
2 
3 
5 
8 
13
21
34
55
89
144
233
377
610
987
1597
2584
4181
6765
10946
17711
28657
46368
75025
121393
196418
317811
514229
832040
```

</details>

## __Difference Between Andy's and This Interpreter__

<details><summary>Must use `var` to define variables</summary> 

This interpreter uses more strict syntax to make sure it is easier for you to program and debug.

In Andy's interpreter:

```javascript
foreach(i;[0,1,2,3])
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
1 | foreach(i;[0,1,2,3])
  |         ^ undefined symbol "i"

code: undefined symbol "i"
 --> test.nas:2:11
  | 
2 |     print(i)
  |           ^ undefined symbol "i"
```

</details>

<details><summary>Default dynamic arguments not supported</summary>

In this interpreter,
function doesn't put dynamic args into vector `arg` by default.
So if you use `arg` without definition,
you'll get an error of `undefined symbol`.

```javascript
var f=func(){
    println(arg)
}
f(1,2,3);
```

Compilation result:

```javascript
code: undefined symbol "arg"
 --> test.nas:2:15
  | 
2 |     println(arg)
  |               ^ undefined symbol "arg"
```

</details>

## __Trace Back Info__

When interpreter crashes,
it will print trace back information:

<details><summary>Native function `die`</summary>

Function `die` is used to throw error and crash immediately.

```javascript
func()
{
    println("hello");
    die("error occurred this line");
    return;
}();
```

```javascript
hello
[vm] error: error occurred this line
[vm] native function error.
trace back:
  0x000000ac      40 00 00 00 25      callb  0x25 <__die@0x41afc0> (lib.nas:131)
  0x000004f6      3e 00 00 00 01      callfv 0x1 (a.nas:4)
  0x000004fa      3e 00 00 00 00      callfv 0x0 (a.nas:6)
vm stack (0x7fffcd21bc68 <sp+80>, limit 10, total 12):
  0x0000005b    | null |
  ...
  0x00000057    | str  | <0x138ff60> error occurred t...
  ...
  0x00000052    | nil  |
```

</details>

<details><summary>Stack overflow crash info</summary>

Here is an example of stack overflow:

```javascript
func(f){
    return f(f);
}(
    func(f){
        f(f);
    }
)();
```

```javascript
[vm] stack overflow
trace back:
  0x000004fb      3e 00 00 00 01      callfv 0x1 (a.nas:5)
  0x000004fb      1349 same call(s)
  0x000004f3      3e 00 00 00 01      callfv 0x1 (a.nas:2)
  0x000004ff      3e 00 00 00 01      callfv 0x1 (a.nas:3)
vm stack (0x7fffd3781d58 <sp+80>, limit 10, total 8108):
  0x00001ffb    | func | <0x15f8d90> entry:0x4f9
  0x00001ffa    | func | <0x15f8d90> entry:0x4f9
  0x00001ff9    | pc   | 0x4fb
  ...
  0x00001ff2    | addr | 0x7fffd37a16e8
```

</details>

<details><summary>Normal vm error crash info</summary>

Error will be thrown if there's a fatal error when executing:

```javascript
func(){
    return 0;
}()[1];
```

```javascript
[vm] callv: must call a vector/hash/string
trace back:
  0x000004f4      3b 00 00 00 00      callv  0x0 (a.nas:3)
vm stack (0x7fffff539c28 <sp+80>, limit 10, total 1):
  0x00000050    | num  | 0
```

</details>

<details><summary>Detailed crash info</summary>

Use command __`-d`__ or __`--detail`__ the trace back info will show more details:

```javascript
hello
[vm] error: error occurred this line
[vm] error: native function error
trace back (main)
  0x000000b0      40 00 00 00 2b      callb   0x2b <__die@0x41c380> (lib.nas:131)
  0x00000553      3e 00 00 00 01      callfv  0x1 (test.nas:4)
  0x00000557      3e 00 00 00 00      callfv  0x0 (test.nas:6)
vm stack (0x7fffe0ffed90 <sp+63>, limit 10, total 12)
  0x0000004a    | null |
  0x00000049    | pc   | 0x553
  0x00000048    | addr | 0x7fffe0ffeda0
  ...
  0x00000041    | nil  |
registers (main)
  [ pc     ]    | pc   | 0xb0
  [ global ]    | addr | 0x7fffe0ffe9a0
  [ localr ]    | addr | 0x7fffe0ffedf0
  [ memr   ]    | addr | 0x0
  [ canary ]    | addr | 0x7fffe1002990
  [ top    ]    | addr | 0x7fffe0ffee40
  [ funcr  ]    | func | <0x677cd0> entry:0xb0
  [ upvalr ]    | nil  |
global (0x7fffe0ffe9a0 <sp+0>)
  0x00000000    | func | <0x65fb00> entry:0x5
  0x00000001    | func | <0x65fb20> entry:0xd
  ...
  0x0000003d    | func | <0x66bf00> entry:0x51f
  0x0000003e    | hash | <0x65ffa0> {5 val}
local (0x7fffe0ffedf0 <sp+45>)
  0x00000000    | nil  |
  0x00000001    | str  | <0x6cb630> error occurred t...
```

</details>

## __Debugger__

We added a debugger in `v8.0`.
Use command `./nasal -dbg xxx.nas` to use the debugger,
and the debugger will print this:

<details><summary>Click to unfold</summary>

```javascript
[debug] nasal debug mode
input 'h' to get help

source code:
--> var fib=func(x)
    {
        if(x<2) return x;
        return fib(x-1)+fib(x-2);
    }
    for(var i=0;i<31;i+=1)
        print(fib(i),'\n');

next bytecode:
--> 0x00000000      01 00 00 00 41      intg    0x41 (test/fib.nas:0)
    0x00000001      0b 00 00 00 05      newf    0x5 (lib.nas:6)
    0x00000002      02 00 00 00 02      intl    0x2 (lib.nas:6)
    0x00000003      0f 00 00 00 00      dyn     0x0 ("elems") (lib.nas:6)
    0x00000004      32 00 00 00 07      jmp     0x7 (lib.nas:6)
    0x00000005      40 00 00 00 00      callb   0x0 <__print@0x419c80> (lib.nas:7)
    0x00000006      4a 00 00 00 00      ret     0x0 (lib.nas:7)
    0x00000007      03 00 00 00 00      loadg   0x0 (lib.nas:6)
vm stack (0x7fffd0259138 <sp+65>, limit 10, total 0)
>>
```

</details>

If want help, input `h` to get help.

When running the debugger, you could see what is on stack.
This will help you debugging or learning how the vm works:

<details><summary>Click to unfold</summary>

```javascript
source code:
    var fib=func(x)
    {
-->     if(x<2) return x;
        return fib(x-1)+fib(x-2);
    }
    for(var i=0;i<31;i+=1)
        print(fib(i),'\n');

next bytecode:
    0x00000548      0c 00 00 00 aa      happ    0xaa ("running") (lib.nas:503)
    0x00000549      03 00 00 00 3e      loadg   0x3e (lib.nas:498)
    0x0000054a      0b 00 00 05 4e      newf    0x54e (test/fib.nas:1)
    0x0000054b      02 00 00 00 02      intl    0x2 (test/fib.nas:1)
    0x0000054c      0d 00 00 00 1b      para    0x1b ("x") (test/fib.nas:1)
    0x0000054d      32 00 00 05 5d      jmp     0x55d (test/fib.nas:1)
--> 0x0000054e      39 00 00 00 01      calll   0x1 (test/fib.nas:3)
    0x0000054f      2d 00 00 00 03      lessc   0x3 (2) (test/fib.nas:3)
vm stack (0x7fffd0259138 <sp+65>, limit 10, total 7)
  0x00000047    | pc   | 0x566
  0x00000046    | addr | 0x0
  0x00000045    | nil  |
  0x00000044    | num  | 0
  0x00000043    | nil  |
  0x00000042    | nil  |
  0x00000041    | func | <0x88d2f0> entry:0x5
>>
```

</details>
