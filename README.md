# __Nasal Scripting Language__

```C++
       __                _
    /\ \ \__ _ ___  __ _| |
   /  \/ / _` / __|/ _` | |
  / /\  / (_| \__ \ (_| | |
  \_\ \/ \__,_|___/\__,_|_|
```

![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/ValKmjolnir/Nasal-Interpreter?style=flat-square&logo=github)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/ValKmjolnir/Nasal-Interpreter?style=flat-square&logo=github)
![in dev](https://img.shields.io/badge/dev-v10.0-blue?style=flat-square&logo=github)
[![license](https://img.shields.io/badge/license-MIT-green?style=flat-square&logo=github)](./LICENSE)

> This document is also available in: [__中文__](./doc/README_zh.md) | [__English__](./README.md)

## __Contents__

* [__Introduction__](#introduction)
* [__Compile__](#how-to-compile)
* [__Usage__](#how-to-use)
* [__Tutorial__](#tutorial)
  * [basic value type](#basic-value-type)
  * [operators](#operators)
  * [definition](#definition)
  * [multi-assignment](#multi-assignment)
  * [conditional expression](#conditional-expression)
  * [loop](#loop)
  * [subvec](#subvec)
  * [special function call](#special-function-call)
  * [lambda](#lambda)
  * [closure](#closure)
  * [trait](#trait)
  * [native functions](#native-functions)
  * [modules](#modulesfor-library-developers)
* [__Release Notes__](#release-notes)
  * [v8.0](#version-80-release)
* [__Parser__](#parser)
  * [v1.0](#version-10-parser-last-update-20191014)
* [__Abstract Syntax Tree__](#abstract-syntax-tree)
  * [v1.2](#version-12-ast-last-update-20191031)
  * [v2.0](#version-20-ast-last-update-2020831)
  * [v3.0](#version-30-ast-last-update-20201023)
  * [v5.0](#version-50-ast-last-update-202137)
* [__Bytecode VM__](#bytecode-virtual-machine)
  * [v4.0](#version-40-vm-last-update-20201217)
  * [v5.0](#version-50-vm-last-update-202137)
  * [v6.0](#version-60-vm-last-update-202161)
  * [v6.5](#version-65-vm-last-update-2021624)
  * [v7.0](#version-70-vm-last-update-2021108)
  * [v8.0](#version-80-vm-last-update-2022212)
  * [v9.0](#version-90-vm-last-update-2022518)
  * [v10.0](#version-100-vm-latest)
* [__Benchmark__](#benchmark)
  * [v6.5 (i5-8250U windows 10)](#version-65-i5-8250u-windows10-2021619)
  * [v6.5 (i5-8250U ubuntu-WSL)](#version-70-i5-8250u-ubuntu-wsl-on-windows10-2021629)
  * [v8.0 (R9-5900HX ubuntu-WSL)](#version-80-r9-5900hx-ubuntu-wsl-2022123)
  * [v9.0 (R9-5900HX ubuntu-WSL)](#version-90-r9-5900hx-ubuntu-wsl-2022213)
* [__Difference__](#difference-between-andys-and-this-interpreter)
  * [strict definition](#1-must-use-var-to-define-variables)
  * [(outdated)use after definition](#2-now-supported-couldnt-use-variables-before-definitions)
  * [default dynamic arguments](#3-default-dynamic-arguments-not-supported)
* [__Trace Back Info__](#trace-back-info)
  * [native function 'die'](#1-native-function-die)
  * [stack overflow](#2-stack-overflow-crash-info)
  * [runtime error](#3-normal-vm-error-crash-info)
  * [detailed crash info](#4-detailed-crash-info)
* [__Debugger__](#debugger)

__Contact us if having great ideas to share!__

* __E-mail__: __lhk101lhk101@qq.com__

* __QQ__: __896693328__

## __Introduction__

__[Nasal](http://wiki.flightgear.org/Nasal_scripting_language)__
is an ECMAscript-like programming language that used in __[FlightGear](https://www.flightgear.org/)__.
This language is designed by __[Andy Ross](https://github.com/andyross)__.

The interpreter is totally rewritten by __[ValKmjolnir](https://github.com/ValKmjolnir)__ using `C++`(`-std=c++11`)
without reusing the code in __[Andy Ross's nasal interpreter](<https://github.com/andyross/nasal>)__.
But we really appreciate that Andy created this amazing programming language and his interpreter project.

Now this project uses __MIT license__ (2021/5/4).
Edit it if you want,
use this project to learn or create more interesting things
(But don't forget me XD).

__Why writing this nasal interpreter?__
In 2019 summer holiday,
members in __[FGPRC](https://www.fgprc.org/)__ told me that it is hard to debug with nasal-console in Flightgear,
especially when checking syntax errors.
So i tried to write a new interpreter to help them checking syntax error and even, runtime error.

I wrote the lexer,
parser and
bytecode virtual machine(there was an ast-interpreter,
but deleted after v4.0) to help checking errors.
We found it much easier to check syntax and runtime
errors before copying nasal-codes in nasal-console in Flightgear to test.

Also, you could use this language to write some
interesting programs and run them without the lib of Flightgear.
You could add your own modules to make
this interpreter a useful tool in your own projects (such as a script in a game just as Flightgear does).

## __How to Compile__

![windows](https://img.shields.io/badge/Microsoft-Windows-green?style=flat-square&logo=windows)
![macOS](https://img.shields.io/badge/Apple%20Inc.-MacOS-green?style=flat-square&logo=apple)
![linux](https://img.shields.io/badge/GNU-Linux-green?style=flat-square&logo=GNU)

Better choose the latest update of the interpreter.
Download the source and build it! It's quite easy to build this interpreter.

__CAUTION__: If want to use the release zip/tar.gz file to build the interpreter, please read the [__Release Notes__](#release-notes) below to make sure this release file has no fatal bugs. There are some tips to fix the release manually.

![g++](https://img.shields.io/badge/GNU-g++-A42E2B?style=flat-square&logo=GNU)
![clang++](https://img.shields.io/badge/LLVM-clang++-262D3A?style=flat-square&logo=LLVM)
![vs](https://img.shields.io/badge/Visual_Studio-MSVC-5C2D91?style=flat-square&logo=visualstudio)

Use g++(`MinGW-w64`) or MSVC(`Visual Studio`) on __`Windows`__ platform. Download MinGW-w64 [__HERE__](https://www.mingw-w64.org/downloads/)(Visual Studio also has this), and use g++/clang++ on __`linux/macOS/Unix`__ platform (we suggest `clang`).

We could build the interpreter using `makefile`.

`mingw32-make` is __`Windows(MinGW-w64)`__ platform's `make`:

> mingw32-make nasal.exe
>
> mingw32-make.exe nasal.exe

on __`linux/macOS/Unix`__:

> make nasal

You could choose which compiler you want to use:

> make nasal CXX=clang++
>
> make nasal CXX=g++
>
> make nasal CXX=...

If you think `-O3` isn't that safe and stable, you could choose:

> make stable-release
>
> mingw32-make stable-release-mingw

## __How to Use__

First we should learn how to write and run a program using this language,
click to see the [__tutorial__](#tutorial).

Input this command to run scripts __directly__:

> ./nasal filename

Use these commands to get version of interpreter:

> ./nasal -v | --version

Use these commands to get help(see more debug commands in help):

> ./nasal -h | --help

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

### __basic value type__

__`vm_none`__ is error type.
This type is used to interrupt the execution of virtual machine and will not be created by user program.

__`vm_nil`__ is a null type. It means nothing.

```javascript
var spc=nil;
```

__`vm_num`__ has 3 formats: `dec`, `hex` and `oct`. Using IEEE754 double to store.

```javascript
# this language use '#' to write notes
var n=1;          # dec
var n=2.71828;    # dec
var n=2.147e16;   # dec
var n=1e-10;      # dec
var n=0x7fffffff; # hex
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
var vec=[
    0,
    nil,
    {},
    [],
    func(){return 0;}
];
append(vec,0,1,2);
```

__`vm_hash`__ is a hashmap(or like a dict in `python`) that stores values with strings/identifiers as the key.

```javascript
var hash={
    member1:nil,
    member2:"str",
    'member3':'member\'s name can also be a string constant',
    function:func(){
        var a=me.member2~me.member3;
        return a;
    }
};
```

__`vm_func`__ is a function type (in fact it is lambda).

```javascript
var f=func(x,y,z){
    return nil;
}
var f=func{
    return 1024;
}
var f=func(x,y,z,default1=1,default2=2){
    return x+y+z+default1+default2;
}
var f=func(args...){
    var sum=0;
    foreach(var i;args)
        sum+=i;
    return sum;
}
```

__`vm_upval`__ is a special type that used to store upvalues.
This type is only used in `nasal_vm` to make sure closure runs correctly.

__`vm_obj`__ is a special type that stores user data.
This means you could use other complex C/C++ data types in nasal.
This type is used when you are trying to add a new data structure into nasal,
so this type is often created by native-function that programmed in C/C++ by library developers.
You could see how to write your own native-functions below.

```javascript
var new_obj=func(){
    return __my_obj();
}
var obj=new_obj();
```

### __operators__

Nasal has basic math operators `+` `-` `*` `/` and a special operator `~` that links two strings together.

```javascript
1+2-1*2/1;
'str1'~'str2';
(1+2)*(3+4)
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
a+=1;
a-=1;
a*=1;
a/=1;
a~='string';
```

### __definition__

```javascript
var a=1;
var (a,b,c)=[0,1,2];
var (a,b,c)=(0,1,2);
(var a,b,c)=[0,1,2];
(var a,b,c)=(0,1,2);
```

### __multi-assignment__

The last one is often used to swap two variables.

```javascript
(a,b[0],c.d)=[0,1,2];
(a,b[1],c.e)=(0,1,2);
(a,b)=(b,a);
```

### __conditional expression__

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

### __loop__

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

### __subvec__

Nasal provides this special syntax to help user generate a new vector by getting values by one index or getting values by indexes in a range from an old vector.
If there's only one index in the bracket, then we will get the value directly.
Use index to search one element in the string will get the __ascii number__ of this character.
If you want to get the character, use built-in function `chr()`.

```javascript
a[0];
a[-1,1,0:2,0:,:3,:,nil:8,3:nil,nil:nil];
"hello world"[0];
```

### __special function call__

This is of great use but is not very efficient
(because hashmap use string as the key to compare).

```javascript
f(x:0,y:nil,z:[]);
```

### __lambda__

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

### __closure__

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

### __trait__

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

### __native functions__

This part shows how we add native functions in this nasal interpreter.
If you are interested in this part, this may help you.
And...

__CAUTION:__ If you want to add your own functions __without__ changing the source code of the interpreter, see the __`module`__ after this part.

If you really want to change source code, check built-in functions in `lib.nas` and see the example below.

Definition:

```C++
nasal_ref builtin_print(nasal_ref*,nasal_gc&);
// you could also use a macro to define one.
nas_native(builtin_print);
```

Then complete this function using C++:

```C++
nasal_ref builtin_print(nasal_ref* local,nasal_gc& gc)
{
    // find value with index begin from 1
    // because local[0] is reserved for value 'me'
    nasal_ref vec=local[1];
    // main process
    // also check number of arguments and type here
    // if get an error,use builtin_err
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
    nasal_ref (*func)(nasal_ref*,nasal_gc&);
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
nasal_ref builtin_keys(nasal_ref* local,nasal_gc& gc)
{
    nasal_ref hash=local[1];
    if(hash.type!=vm_hash)
        return builtin_err("keys","\"hash\" must be hash");
    // avoid being sweeped
    nasal_ref res=gc.temp=gc.alloc(vm_vec);
    auto& vec=res.vec().elems;
    for(auto& iter:hash.hash().elems)
        vec.push_back(gc.newstr(iter.first));
    gc.temp=nil;
    return res;
}
```

### __modules(for library developers)__

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
extern "C" nasal_ref fib(std::vector<nasal_ref>& args,nasal_gc& gc){
    // the arguments are generated into a vm_vec: args
    // get values from the vector that must be used here
    nasal_ref num=args[0];
    // if you want your function safer, try this
    // builtin_err will print the error info on screen
    // and return vm_null for runtime to interrupt
    if(num.type!=vm_num)
        return builtin_err("extern_fib","\"num\" must be number");
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
import("lib.nas");
var dlhandle=dylib.dlopen("./module/libfib."~(os.platform()=="windows"?"dll":"so"));
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

## __Release Notes__

### __version 8.0 release__

I made a __big mistake__ in `v8.0` release:

in __`nasal_dbg.h:215`__: `auto canary=gc.stack+STACK_MAX_DEPTH-1;`

this will cause incorrect `stackoverflow` error.
please change it to:

`canary=gc.stack+STACK_MAX_DEPTH-1;`

If do not change this line, only the debugger runs abnormally. this bug is fixed in `v9.0`.

Another bug is that in `nasal_err.h:class nasal_err`, we should add a constructor for this class:

```C++
    nasal_err():error(0){}
```

This bug is fixed in `v9.0`. So we suggest that do not use `v8.0`.

## __Parser__

`LL(1)` parser with special check.

```javascript
(var a,b,c)=[{b:nil},[1,2],func return 0;];
(a.b,b[0],c)=(1,2,3);
```

These two expressions have the same first set,so `LL(1)` is useless for this language. We add some special checks in it.

Problems mentioned above have been solved for a long time, but recently i found a new problem here:

```javascript
var f=func(x,y,z){return x+y+z}
(a,b,c)=(0,1,2);
```

This will be recognized as this:

```javascript
var f=func(x,y,z){return x+y+z}(a,b,c)
=(0,1,2);
```

and causes fatal syntax error.
And i tried this program in flightgear nasal console.
It also found this is a syntax error.
I think this is a serious design fault.
To avoid this syntax error, change program like this, just add a semicolon:

```javascript
var f=func(x,y,z){return x+y+z};
                               ^ here
(a,b,c)=(0,1,2);
```

### version 1.0 parser (last update 2019/10/14)

First fully functional version of nasal_parser.

Before version 1.0,i tried many times to create a correct parser.

Finally i learned `LL(1)` and `LL(k)` and wrote a parser for math formulas in version 0.16(last update 2019/9/14).

In version 0.17(2019/9/15) 0.18(2019/9/18) 0.19(2019/10/1)i was playing the parser happily and after that i wrote version 1.0.

__This project began at 2019/7/25__.

## __Abstract Syntax Tree__

### version 1.2 ast (last update 2019/10/31)

The ast has been completed in this version.

### version 2.0 ast (last update 2020/8/31)

A completed ast-interpreter with unfinished lib functions.

### version 3.0 ast (last update 2020/10/23)

The ast is refactored and is now easier to read and maintain.

Ast-interpreter uses new techniques so it can run codes more efficiently.

Now you can add your own functions as builtin-functions in this interpreter!

I decide to save the ast interpreter after releasing v4.0. Because it took me a long time to think and write...

### version 5.0 ast (last update 2021/3/7)

I change my mind.
AST interpreter leaves me too much things to do.

If i continue saving this interpreter,
it will be harder for me to make the bytecode vm become more efficient.

## __Bytecode Virtual Machine__

### version 4.0 vm (last update 2020/12/17)

I have just finished the first version of bytecode-interpreter.

This interpreter is still in test.
After this test, i will release version 4.0!

Now i am trying to search hidden bugs in this interpreter.
Hope you could help me! :)

There's an example of byte code below:

```javascript
for(var i=0;i<4000000;i+=1);
```

```x86asm
.number 0
.number 4e+006
.number 1
.symbol i
0x00000000: pzero  0x00000000
0x00000001: loadg  0x00000000 (i)
0x00000002: callg  0x00000000 (i)
0x00000003: pnum   0x00000001 (4e+006)
0x00000004: less   0x00000000
0x00000005: jf     0x0000000b
0x00000006: pone   0x00000000
0x00000007: mcallg 0x00000000 (i)
0x00000008: addeq  0x00000000
0x00000009: pop    0x00000000
0x0000000a: jmp    0x00000002
0x0000000b: nop    0x00000000
```

### version 5.0 vm (last update 2021/3/7)

I decide to optimize bytecode vm in this version.

Because it takes more than 1.5s to count i from `0` to `4000000-1`.This is not efficient at all!

2021/1/23 update: Now it can count from `0` to `4000000-1` in 1.5s.

### version 6.0 vm (last update 2021/6/1)

Use `loadg`/`loadl`/`callg`/`calll`/`mcallg`/`mcalll` to avoid branches.

Delete type `vm_scop`.

Use const `vm_num` to avoid frequently new & delete.

Change garbage collector from reference-counting to mark-sweep.

`vapp` and `newf` operand use .num to reduce the size of `exec_code`.

2021/4/3 update: Now it can count from `0` to `4e6-1` in 0.8s.

2021/4/19 update: Now it can count from `0` to `4e6-1` in 0.4s.

In this update i changed global and local scope from `unordered_map` to `vector`.

So the bytecode generator changed a lot.

```javascript
for(var i=0;i<4000000;i+=1);
```

```x86asm
.number 4e+006
0x00000000: intg   0x00000001
0x00000001: pzero  0x00000000
0x00000002: loadg  0x00000000
0x00000003: callg  0x00000000
0x00000004: pnum   0x00000000 (4e+006)
0x00000005: less   0x00000000
0x00000006: jf     0x0000000c
0x00000007: pone   0x00000000
0x00000008: mcallg 0x00000000
0x00000009: addeq  0x00000000
0x0000000a: pop    0x00000000
0x0000000b: jmp    0x00000003
0x0000000c: nop    0x00000000
```

### version 6.5 vm (last update 2021/6/24)

2021/5/31 update:

Now gc can collect garbage correctly without re-collecting,
which will cause fatal error.

Add `builtin_alloc` to avoid mark-sweep when running a built-in function,
which will mark useful items as useless garbage to collect.

Better use setsize and assignment to get a big array,
`append` is very slow in this situation.

2021/6/3 update:

Fixed a bug that gc still re-collects garbage,
this time i use three mark states to make sure garbage is ready to be collected.

Change `callf` to `callfv` and `callfh`.
And `callfv` fetches arguments from `val_stack` directly instead of using `vm_vec`,
a not very efficient way.

Better use `callfv` instead of `callfh`,
`callfh` will fetch a `vm_hash` from stack and parse it,
making this process slow.

```javascript
var f=func(x,y){return x+y;}
f(1024,2048);
```

```x86asm
.number 1024
.number 2048
.symbol x   
.symbol y
0x00000000: intg   0x00000001
0x00000001: newf   0x00000007
0x00000002: intl   0x00000003
0x00000003: offset 0x00000001
0x00000004: para   0x00000000 (x)
0x00000005: para   0x00000001 (y)
0x00000006: jmp    0x0000000b
0x00000007: calll  0x00000001
0x00000008: calll  0x00000002
0x00000009: add    0x00000000
0x0000000a: ret    0x00000000
0x0000000b: loadg  0x00000000
0x0000000c: callg  0x00000000
0x0000000d: pnum   0x00000000 (1024)
0x0000000e: pnum   0x00000001 (2048)
0x0000000f: callfv 0x00000002
0x00000010: pop    0x00000000
0x00000011: nop    0x00000000
```

2021/6/21 update: Now gc will not collect nullptr.
And the function of assignment is complete,
now these kinds of assignment is allowed:

```javascript
var f=func()
{
    var _=[{_:0},{_:1}];
    return func(x)
    {
        return _[x];
    }
}
var m=f();
m(0)._=m(1)._=10;

[0,1,2][1:2][0]=0;
```

In the old version,
parser will check this left-value and tells that these kinds of left-value are not allowed(bad lvalue).

But now it can work.
And you could see its use by reading the code above.
To make sure this assignment works correctly,
codegen will generate byte code by `nasal_codegen::call_gen()` instead of `nasal_codegen::mcall_gen()`,
and the last child of the ast will be generated by `nasal_codegen::mcall_gen()`.
So the bytecode is totally different now:

```x86asm
.number 10
.number 2
.symbol _
.symbol x
0x00000000: intg   0x00000002
0x00000001: newf   0x00000005
0x00000002: intl   0x00000002
0x00000003: offset 0x00000001
0x00000004: jmp    0x00000017
0x00000005: newh   0x00000000
0x00000006: pzero  0x00000000
0x00000007: happ   0x00000000 (_)
0x00000008: newh   0x00000000
0x00000009: pone   0x00000000
0x0000000a: happ   0x00000000 (_)
0x0000000b: newv   0x00000002
0x0000000c: loadl  0x00000001
0x0000000d: newf   0x00000012
0x0000000e: intl   0x00000003
0x0000000f: offset 0x00000002
0x00000010: para   0x00000001 (x)
0x00000011: jmp    0x00000016
0x00000012: calll  0x00000001
0x00000013: calll  0x00000002
0x00000014: callv  0x00000000
0x00000015: ret    0x00000000
0x00000016: ret    0x00000000
0x00000017: loadg  0x00000000
0x00000018: callg  0x00000000
0x00000019: callfv 0x00000000
0x0000001a: loadg  0x00000001
0x0000001b: pnum   0x00000000 (10.000000)
0x0000001c: callg  0x00000001
0x0000001d: pone   0x00000000
0x0000001e: callfv 0x00000001
0x0000001f: mcallh 0x00000000 (_)
0x00000020: meq    0x00000000
0x00000021: callg  0x00000001
0x00000022: pzero  0x00000000
0x00000023: callfv 0x00000001
0x00000024: mcallh 0x00000000 (_)
0x00000025: meq    0x00000000
0x00000026: pop    0x00000000
0x00000027: pzero  0x00000000
0x00000028: pzero  0x00000000
0x00000029: pone   0x00000000
0x0000002a: pnum   0x00000001 (2.000000)
0x0000002b: newv   0x00000003
0x0000002c: slcbeg 0x00000000
0x0000002d: pone   0x00000000
0x0000002e: pnum   0x00000001 (2.000000)
0x0000002f: slc2   0x00000000
0x00000030: slcend 0x00000000
0x00000031: pzero  0x00000000
0x00000032: mcallv 0x00000000
0x00000033: meq    0x00000000
0x00000034: pop    0x00000000
0x00000035: nop    0x00000000
```

As you could see from the bytecode above,
`mcall`/`mcallv`/`mcallh` operands' using frequency will reduce,
`call`/`callv`/`callh`/`callfv`/`callfh` at the opposite.

And because of the new structure of `mcall`,
`addr_stack`, a stack used to store the memory address,
is deleted from `nasal_vm`,
and now `nasal_vm` use `nasal_val** mem_addr` to store the memory address.
This will not cause fatal errors because the memory address is used __immediately__ after getting it.

### version 7.0 vm (last update 2021/10/8)

2021/6/26 update:

Instruction dispatch is changed from call-threading to computed-goto(with inline function).
After changing the way of instruction dispatch,
there is a great improvement in nasal_vm.
Now vm can run test/bigloop and test/pi in 0.2s!
And vm runs test/fib in 0.8s on linux.
You could see the time use data below,
in Test data section.

This version uses g++ extension "labels as values",
which is also supported by clang++.
(But i don't know if MSVC supports this)

There is also a change in nasal_gc:
`std::vector` global is deleted,
now the global values are all stored on stack(from `val_stack+0` to `val_stack+intg-1`).

2021/6/29 update:

Add some instructions that execute const values:
`op_addc`,`op_subc`,`op_mulc`,`op_divc`,`op_lnkc`,`op_addeqc`,`op_subeqc`,`op_muleqc`,`op_diveqc`,`op_lnkeqc`.

Now the bytecode of test/bigloop.nas seems like this:

```x86asm
.number 4e+006
.number 1
0x00000000: intg   0x00000001
0x00000001: pzero  0x00000000
0x00000002: loadg  0x00000000
0x00000003: callg  0x00000000
0x00000004: pnum   0x00000000 (4000000)
0x00000005: less   0x00000000
0x00000006: jf     0x0000000b
0x00000007: mcallg 0x00000000
0x00000008: addeqc 0x00000001 (1)
0x00000009: pop    0x00000000
0x0000000a: jmp    0x00000003
0x0000000b: nop    0x00000000
```

And this test file runs in 0.1s after this update.
Most of the calculations are accelerated.

Also, assignment bytecode has changed a lot.
Now the first identifier that called in assignment will use `op_load` to assign,
instead of `op_meq`,`op_pop`.

```javascript
var (a,b)=(1,2);
a=b=0;
```

```x86asm
.number 2
0x00000000: intg   0x00000002
0x00000001: pone   0x00000000
0x00000002: loadg  0x00000000
0x00000003: pnum   0x00000000 (2)
0x00000004: loadg  0x00000001
0x00000005: pzero  0x00000000
0x00000006: mcallg 0x00000001
0x00000007: meq    0x00000000 (b=2 use meq,pop->a)
0x00000008: loadg  0x00000000 (a=b use loadg)
0x00000009: nop    0x00000000
```

### version 8.0 vm (last update 2022/2/12)

2021/10/8 update:

In this version vm_nil and vm_num now is not managed by    `nasal_gc`,
this will decrease the usage of `gc::alloc` and increase the efficiency of execution.

New value type is added: `vm_obj`.
This type is reserved for user to define their own value types.
Related API will be added in the future.

Fully functional closure:
Add new operands that get and set upvalues.
Delete an old operand `op_offset`.

2021/10/13 update:

The format of output information of bytecodes changes to this:

```x86asm
0x000002f2: newf   0x2f6
0x000002f3: intl   0x2
0x000002f4: para   0x3e ("x")
0x000002f5: jmp    0x309
0x000002f6: calll  0x1
0x000002f7: lessc  0x0 (2)
0x000002f8: jf     0x2fb
0x000002f9: calll  0x1
0x000002fa: ret
0x000002fb: upval  0x0[0x1]
0x000002fc: upval  0x0[0x1]
0x000002fd: callfv 0x1
0x000002fe: calll  0x1
0x000002ff: subc   0x1d (1)
0x00000300: callfv 0x1
0x00000301: upval  0x0[0x1]
0x00000302: upval  0x0[0x1]
0x00000303: callfv 0x1
0x00000304: calll  0x1
0x00000305: subc   0x0 (2)
0x00000306: callfv 0x1
0x00000307: add
0x00000308: ret
0x00000309: ret
0x0000030a: callfv 0x1
0x0000030b: loadg  0x32
```

2022/1/22 update:

Delete `op_pone` and `op_pzero`.
Both of them are meaningless and will be replaced by `op_pnum`.

### version 9.0 vm (last update 2022/5/18)

2022/2/12 update:

Local values now are __stored on stack__.
So function calling will be faster than before.
Because in v8.0 when calling a function,
new `vm_vec` will be allocated by `nasal_gc`, this makes gc doing mark-sweep too many times and spends a quite lot of time.
In test file `test/bf.nas`, it takes too much time to test the file because this file has too many function calls(see test data below in table `version 8.0 (R9-5900HX ubuntu-WSL 2022/1/23)`).

Upvalue now is generated when creating first new function in the local scope, using `vm_vec`.
And after that when creating new functions, they share the same upvalue, and the upvalue will synchronize with the local scope each time creating a new function.

2022/3/27 update:

In this month's updates we change upvalue from `vm_vec` to `vm_upval`,
a special gc-managed object,
which has almost the same structure of that upvalue object in another programming language __`Lua`__.

Today we change the output format of bytecode.
New output format looks like `objdump`:

```x86asm
  0x0000029b:       0a 00 00 00 00        newh

func <0x29c>:
  0x0000029c:       0b 00 00 02 a0        newf    0x2a0
  0x0000029d:       02 00 00 00 02        intl    0x2
  0x0000029e:       0d 00 00 00 66        para    0x66 ("libname")
  0x0000029f:       32 00 00 02 a2        jmp     0x2a2
  0x000002a0:       40 00 00 00 42        callb   0x42 <__dlopen@0x41dc40>
  0x000002a1:       4a 00 00 00 00        ret
<0x29c>;

  0x000002a2:       0c 00 00 00 67        happ    0x67 ("dlopen")

func <0x2a3>:
  0x000002a3:       0b 00 00 02 a8        newf    0x2a8
  0x000002a4:       02 00 00 00 03        intl    0x3
  0x000002a5:       0d 00 00 00 68        para    0x68 ("lib")
  0x000002a6:       0d 00 00 00 69        para    0x69 ("sym")
  0x000002a7:       32 00 00 02 aa        jmp     0x2aa
  0x000002a8:       40 00 00 00 43        callb   0x43 <__dlsym@0x41df00>
  0x000002a9:       4a 00 00 00 00        ret
<0x2a3>;

  0x000002aa:       0c 00 00 00 6a        happ    0x6a ("dlsym")
```

### version 10.0 vm (latest)

2022/5/19 update:

Now we add coroutine in this runtime:

```javascript
var coroutine={
    create: func(function){return __cocreate;},
    resume: func(co)      {return __coresume;},
    yield:  func(args...) {return __coyield; },
    status: func(co)      {return __costatus;},
    running:func()        {return __corun;   }
};
```

`coroutine.create` is used to create a new coroutine object using a function.
But this coroutine will not run immediately.

`coroutine.resume` is used to continue running a coroutine.

`coroutine.yield` is used to interrupt the running of a coroutine and throw some values.
These values will be accepted and returned by `coroutine.resume`.
And `coroutine.yield` it self returns `vm_nil` in the coroutine function.

`coroutine.status` is used to see the status of a coroutine.
There are 3 types of status:`suspended` means waiting for running,`running` means is running,`dead` means finished running.

`coroutine.running` is used to judge if there is a coroutine running now.

__CAUTION:__ coroutine should not be created or running inside another coroutine.

__We will explain how resume and yield work here:__

When `op_callb` is called, the stack frame is like this:

```C++
+----------------------------+(main stack)
| old pc(vm_ret)             | <- top[0]
+----------------------------+
| old localr(vm_addr)        | <- top[-1]
+----------------------------+
| old upvalr(vm_upval)       | <- top[-2]
+----------------------------+
| local scope(nasal_ref)     |
| ...                        |
+----------------------------+ <- local pointer stored in localr
| old funcr(vm_func)         | <- old function stored in funcr
+----------------------------+
```

In `op_callb`'s progress, next step the stack frame is:

```C++
+----------------------------+(main stack)
| nil(vm_nil)                | <- push nil
+----------------------------+
| old pc(vm_ret)             |
+----------------------------+
| old localr(vm_addr)        |
+----------------------------+
| old upvalr(vm_upval)       |
+----------------------------+
| local scope(nasal_ref)     |
| ...                        |
+----------------------------+ <- local pointer stored in localr
| old funcr(vm_func)         | <- old function stored in funcr
+----------------------------+
```

Then we call `resume`, this function will change stack.
As we can see, coroutine stack already has some values on it,
but if we first enter it, the stack top will be `vm_ret`, and the return `pc` is `0`.

So for safe running, `resume` will return `gc.top[0]`.
`op_callb` will do `top[0]=resume()`, so the value does not change.

```C++
+----------------------------+(coroutine stack)
| pc:0(vm_ret)               | <- now gc.top[0]
+----------------------------+
```

When we call `yield`, the function will do like this.
And we find that `op_callb` has put the `nil` at the top.
but where is the returned `local[1]` sent?

```C++
+----------------------------+(coroutine stack)
| nil(vm_nil)                | <- push nil
+----------------------------+
| old pc(vm_ret)             |
+----------------------------+
| old localr(vm_addr)        |
+----------------------------+
| old upvalr(vm_upval)       |
+----------------------------+
| local scope(nasal_ref)     |
| ...                        |
+----------------------------+ <- local pointer stored in localr
| old funcr(vm_func)         | <- old function stored in funcr
+----------------------------+
```

When `builtin_coyield` is finished, the stack is set to main stack,
and the returned `local[1]` in fact is set to the top of the main stack by `op_callb`:

```C++
+----------------------------+(main stack)
| return_value(nasal_ref)    |
+----------------------------+
| old pc(vm_ret)             |
+----------------------------+
| old localr(vm_addr)        |
+----------------------------+
| old upvalr(vm_upval)       |
+----------------------------+
| local scope(nasal_ref)     |
| ...                        |
+----------------------------+ <- local pointer stored in localr
| old funcr(vm_func)         | <- old function stored in funcr
+----------------------------+
```

so the main progress feels the value on the top is the returned value of `resume`.
but in fact the `resume`'s returned value is set on coroutine stack.
so we conclude this:

```C++
resume (main->coroutine) return coroutine.top[0]. coroutine.top[0] = coroutine.top[0];
yield  (coroutine->main) return a vector.         main.top[0]      = vector;
```

## Benchmark

![benchmark](./pic/benchmark.png)

### version 6.5 (i5-8250U windows10 2021/6/19)

running time and gc time:

|file|call gc|total time|gc time|
|:----|:----|:----|:----|
|pi.nas|12000049|0.593s|0.222s|
|fib.nas|10573747|2.838s|0.187s|
|bp.nas|4419829|1.99s|0.18s|
|bigloop.nas|4000000|0.419s|0.039s|
|mandelbrot.nas|1044630|0.433s|0.041s|
|life.nas|817112|8.557s|0.199s|
|ascii-art.nas|45612|0.48s|0.027s|
|calc.nas|8089|0.068s|0.006s|
|quick_sort.nas|2768|0.107s|0s|
|bfs.nas|2471|1.763s|0.003s|

operands calling frequency:

|file|1st|2nd|3rd|4th|5th|
|:----|:----|:----|:----|:----|:----|
|pi.nas|callg|pop|mcallg|pnum|pone|
|fib.nas|calll|pnum|callg|less|jf|
|bp.nas|calll|callg|pop|callv|addeq|
|bigloop.nas|pnum|less|jf|callg|pone|
|mandelbrot.nas|callg|mult|loadg|pnum|pop|
|life.nas|calll|callv|pnum|jf|callg|
|ascii-art.nas|calll|pop|mcalll|callg|callb|
|calc.nas|calll|pop|pstr|mcalll|jmp|
|quick_sort.nas|calll|pop|jt|jf|less|
|bfs.nas|calll|pop|callv|mcalll|jf|

operands calling total times:

|file|1st|2nd|3rd|4th|5th|
|:----|:----|:----|:----|:----|:----|
|pi.nas|6000004|6000003|6000000|4000005|4000002|
|fib.nas|17622792|10573704|7049218|7049155|7049155|
|bp.nas|7081480|4227268|2764676|2617112|2065441|
|bigloop.nas|4000001|4000001|4000001|4000001|4000000|
|mandelbrot.nas|1519632|563856|290641|286795|284844|
|life.nas|2114371|974244|536413|534794|489743|
|ascii-art.nas|37906|22736|22402|18315|18292|
|calc.nas|191|124|109|99|87|
|quick_sort.nas|16226|5561|4144|3524|2833|
|bfs.nas|24707|16297|14606|14269|8672|

### version 7.0 (i5-8250U ubuntu-WSL on windows10 2021/6/29)

running time:

|file|total time|info|
|:----|:----|:----|
|pi.nas|0.15625s|great improvement|
|fib.nas|0.75s|great improvement|
|bp.nas|0.4218s(7162 epoch)|good improvement|
|bigloop.nas|0.09375s|great improvement|
|mandelbrot.nas|0.0312s|great improvement|
|life.nas|8.80s(windows) 1.25(ubuntu WSL)|little improvement|
|ascii-art.nas|0.015s|little improvement|
|calc.nas|0.0468s|little improvement|
|quick_sort.nas|0s|great improvement|
|bfs.nas|0.0156s|great improvement|

### version 8.0 (R9-5900HX ubuntu-WSL 2022/1/23)

running time:

|file|total time|info|
|:----|:----|:----|
|bf.nas|1100.19s||
|mandel.nas|28.98s||
|life.nas|0.56s|0.857s(windows)|
|ycombinator.nas|0.64s||
|fib.nas|0.28s||
|bfs.nas|0.156s|random result|
|pi.nas|0.0625s||
|bigloop.nas|0.047s||
|calc.nas|0.03125s|changed test file|
|mandelbrot.nas|0.0156s||
|ascii-art.nas|0s||
|quick_sort.nas|0s||

### version 9.0 (R9-5900HX ubuntu-WSL 2022/2/13)

running time:

|file|total time|info|
|:----|:----|:----|
|bf.nas|276.55s|great improvement|
|mandel.nas|28.16s||
|ycombinator.nas|0.59s||
|life.nas|0.2s|0.649s(windows)|
|fib.nas|0.234s|little improvement|
|bfs.nas|0.14s|random result|
|pi.nas|0.0625s||
|bigloop.nas|0.047s||
|calc.nas|0.0469s|changed test file|
|quick_sort.nas|0.016s|changed test file:100->1e4|
|mandelbrot.nas|0.0156s||
|ascii-art.nas|0s||

`bf.nas` is a very interesting test file that there is a brainfuck interpreter written in nasal.
And we use this bf interpreter to draw a mandelbrot set.

In 2022/2/17 update we added `\e` into the lexer. And the `bfcolored.nas` uses this special ASCII code. Here is the result:

![mandelbrot](./pic/mandelbrot.png)

## __Difference Between Andy's and This Interpreter__

### 1. must use `var` to define variables

This interpreter uses more strict syntax to make sure it is easier for you to program and debug.

In Andy's interpreter:

```javascript
import("lib.nas");
foreach(i;[0,1,2,3])
    print(i)
```

This program can run normally with output 0 1 2 3.
But take a look at the iterator 'i',
this symbol is defined in foreach without using keyword 'var'.
I think this design will make programmers filling confused.
This is ambiguous that programmers maybe difficult to find the 'i' is defined here.
Without 'var', programmers may think this 'i' is defined anywhere else.

So in this new interpreter i use a more strict syntax to force users to use 'var' to define iterator of forindex and foreach.
If you forget to add the keyword 'var',
and you haven't defined this symbol before,
you will get this:

```javascript
[code] test.nas:2 undefined symbol "i".
foreach(i;[0,1,2,3])
[code] test.nas:3 undefined symbol "i".
    print(i)
```

### 2. (now supported) couldn't use variables before definitions

(__Outdated__: this is now supported) Also there's another difference.
In Andy's interpreter:

```javascript
var a=func {print(b);}
var b=1;
a();
```

This program runs normally with output 1.
But in this new interpreter, it will get:

```javascript
[code] test.nas:1 undefined symbol "b".
var a=func {print(b);}
```

This difference is caused by different kinds of ways of lexical analysis.
In most script language interpreters,
they use dynamic analysis to check if this symbol is defined yet.
However, this kind of analysis is at the cost of lower efficiency.
To make sure the interpreter runs at higher efficiency,
i choose static analysis to manage the memory space of each symbol.
By this way, runtime will never need to check if a symbol exists or not.
But this causes a difference.
You will get an error of 'undefined symbol',
instead of nothing happening in most script language interpreters.

This change is __controversial__ among FGPRC's members.
So maybe in the future i will use dynamic analysis again to cater to the habits of senior programmers.

(2021/8/3 update) __Now i use scanning ast twice to reload symbols.
So this difference does not exist from this update.__
But a new difference is that if you call a variable before defining it,
you'll get nil instead of 'undefined error'.

### 3. default dynamic arguments not supported

In this new interpreter,
function doesn't put dynamic arguments into vector `arg` automatically.
So if you use `arg` without definition,
you'll get an error of `undefined symbol`.

## __Trace Back Info__

When the interpreter crashes,
it will print trace back information:

### 1. native function `die`

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
        0x000000ac:     40 00 00 00 25      callb  0x25 <__die@0x41afc0> (lib.nas:131)
        0x000004f6:     3e 00 00 00 01      callfv 0x1 (a.nas:4)
        0x000004fa:     3e 00 00 00 00      callfv 0x0 (a.nas:6)
vm stack(0x7fffcd21bc68<sp+80>, limit 10, total 12):
  0x0000005b    | null |
  0x0000005a    | pc   | 0x4f6
  0x00000059    | addr | 0x7fffcd21bc78
  0x00000058    | nil  |
  0x00000057    | str  | <0x138ff60> error occurred t...
  0x00000056    | nil  |
  0x00000055    | func | <0x13445b0> entry:0x4f0
  0x00000054    | pc   | 0x4fa
  0x00000053    | addr | 0x0
  0x00000052    | nil  |
```

### 2. stack overflow crash info

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
        0x000004fb:     3e 00 00 00 01      callfv 0x1 (a.nas:5)
        0x000004fb:     1349 same call(s)
        0x000004f3:     3e 00 00 00 01      callfv 0x1 (a.nas:2)
        0x000004ff:     3e 00 00 00 01      callfv 0x1 (a.nas:3)
vm stack(0x7fffd3781d58<sp+80>, limit 10, total 8108):
  0x00001ffb    | func | <0x15f8d90> entry:0x4f9
  0x00001ffa    | func | <0x15f8d90> entry:0x4f9
  0x00001ff9    | pc   | 0x4fb
  0x00001ff8    | addr | 0x7fffd37a1748
  0x00001ff7    | nil  |
  0x00001ff6    | func | <0x15f8d90> entry:0x4f9
  0x00001ff5    | nil  |
  0x00001ff4    | func | <0x15f8d90> entry:0x4f9
  0x00001ff3    | pc   | 0x4fb
  0x00001ff2    | addr | 0x7fffd37a16e8
```

### 3. normal vm error crash info

Error will be thrown if there's a fatal error when executing:

```javascript
func(){
    return 0;
}()[1];
```

```javascript
[vm] callv: must call a vector/hash/string
trace back:
        0x000004f4:     3b 00 00 00 00      callv  0x0 (a.nas:3)
vm stack(0x7fffff539c28<sp+80>, limit 10, total 1):
  0x00000050    | num  | 0
```

### 4. detailed crash info

Use command __`-d`__ or __`--detail`__ the trace back info will show more details:

```javascript
hello
[vm] error: error occurred this line
[vm] native function error.
trace back:
        0x000000ac:     40 00 00 00 25      callb  0x25 <__die@0x41afc0> (lib.nas:131)
        0x000004f6:     3e 00 00 00 01      callfv 0x1 (a.nas:4)
        0x000004fa:     3e 00 00 00 00      callfv 0x0 (a.nas:6)
vm stack(0x7ffff42f3d08<sp+80>, limit 10, total 12):
  0x0000005b    | null |
  0x0000005a    | pc   | 0x4f6
  0x00000059    | addr | 0x7ffff42f3d18
  0x00000058    | nil  |
  0x00000057    | str  | <0x1932480> error occurred t...
  0x00000056    | nil  |
  0x00000055    | func | <0x18e6ad0> entry:0x4f0
  0x00000054    | pc   | 0x4fa
  0x00000053    | addr | 0x0
  0x00000052    | nil  |
registers(main):
  [ pc     ]    | pc   | 0xac
  [ global ]    | addr | 0x7ffff42f3808
  [ localr ]    | addr | 0x7ffff42f3d68
  [ memr   ]    | addr | 0x0
  [ funcr  ]    | func | <0x18fbe50> entry:0xac
  [ upvalr ]    | nil  |
  [ canary ]    | addr | 0x7ffff43137f8
  [ top    ]    | addr | 0x7ffff42f3db8
global(0x7ffff42f3808<sp+0>):
  0x00000000    | func | <0x18d62d0> entry:0x5
  0x00000001    | func | <0x18d7e40> entry:0xc
  ...
  0x00000031    | func | <0x18f6ad0> entry:0x237
  0x00000032    | hash | <0x191f780> {14 val}
  0x00000033    | func | <0x18df660> entry:0x29b
  0x00000034    | hash | <0x191f7a0> {9 val}
  0x00000035    | hash | <0x191f7c0> {18 val}
  ...
  0x00000039    | hash | <0x191f840> {1 val}
  0x0000003a    | num  | 0.0174533
  ...
  0x00000049    | num  | 57.2958
  0x0000004a    | func | <0x18e6490> entry:0x489
  ...
  0x0000004e    | func | <0x18e6710> entry:0x4c2
  0x0000004f    | hash | <0x191f8b0> {5 val}
local(0x7ffff42f3d68<sp+86>):
  0x00000000    | nil  |
  0x00000001    | str  | <0x1932480> error occurred t...
```

## __Debugger__

In nasal `v8.0` we added a debugger.
Now we could see both source code and bytecode when testing program.

Use command `./nasal -dbg xxx.nas` to use the debugger,
and the debugger will print this:

```javascript
[debug] nasal debug mode
input 'h' to get help

source code:
-->     import("lib.nas");
        var fib=func(x)
        {
                if(x<2) return x;
                return fib(x-1)+fib(x-2);
        }
        for(var i=0;i<31;i+=1)
                print(fib(i),'\n');
next bytecode:
-->     0x00000000:     01 00 00 00 4f      intg   0x4f (a.nas:0)
        0x00000001:     0b 00 00 00 05      newf   0x5 (lib.nas:5)
        0x00000002:     02 00 00 00 02      intl   0x2 (lib.nas:5)
        0x00000003:     0d 00 00 00 00      para   0x0 ("filename") (lib.nas:5)
        0x00000004:     32 00 00 00 07      jmp    0x7 (lib.nas:5)
        0x00000005:     40 00 00 00 24      callb  0x24 <__import@0x419b20> (lib.nas:6)
        0x00000006:     4a 00 00 00 00      ret    0x0 (lib.nas:6)
        0x00000007:     03 00 00 00 00      loadg  0x0 (lib.nas:5)
vm stack(0x7fffe05e3190<sp+79>, limit 5, total 0)
>>
```

If want help, input `h` to get help.

```bash
>> h
```

When running the debugger, you could see what is on stack.
This will help you debugging or learning how the vm works:

```javascript
source code:
        import("lib.nas");
        var fib=func(x)
        {
-->             if(x<2) return x;
                return fib(x-1)+fib(x-2);
        }
        for(var i=0;i<31;i+=1)
                print(fib(i),'\n');
next bytecode:
        0x00000458:     4a 00 00 00 00      ret    0x0 (lib.nas:463)
        0x00000459:     03 00 00 00 4c      loadg  0x4c (lib.nas:463)
        0x0000045a:     0b 00 00 04 5e      newf   0x45e (a.nas:2)
        0x0000045b:     02 00 00 00 02      intl   0x2 (a.nas:2)
        0x0000045c:     0d 00 00 00 1c      para   0x1c ("x") (a.nas:2)
        0x0000045d:     32 00 00 04 6d      jmp    0x46d (a.nas:2)
-->     0x0000045e:     39 00 00 00 01      calll  0x1 (a.nas:4)
        0x0000045f:     2d 00 00 00 02      lessc  0x2 (2) (a.nas:4)
vm stack(0x7fffe05e3190<sp+79>, limit 5, total 6):
  0x00000054    | pc   | 0x476
  0x00000053    | addr | 0x0
  0x00000052    | num  | 0
  0x00000051    | nil  |
  0x00000050    | nil  |
>>
```
