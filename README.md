# __Nasal - Modern Interpreter__

<img src="./doc/pic/header.png" style="width:800px"></img>

![GitHub code size](https://img.shields.io/github/languages/code-size/ValKmjolnir/Nasal-Interpreter?style=flat-square&logo=github)
![GitHub release(latest by date)](https://img.shields.io/github/v/release/ValKmjolnir/Nasal-Interpreter?style=flat-square&logo=github)
![in dev](https://img.shields.io/badge/dev-v11.2-blue?style=flat-square&logo=github)
[![license](https://img.shields.io/badge/license-GPLv2-green?style=flat-square&logo=github)](./LICENSE)

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
* [__REPL__](#repl)

__Contact us if having great ideas to share!__

* __E-mail__: __lhk101lhk101@qq.com__(ValKmjolnir) __1467329765@qq.com__(Sidi762)

## __Introduction__

[Nasal](http://wiki.flightgear.org/Nasal_scripting_language)
is an ECMAscript-like language used in [FlightGear](https://www.flightgear.org/).
The designer is [Andy Ross](https://github.com/andyross).

This interpreter is totally rewritten by [ValKmjolnir](https://github.com/ValKmjolnir) using `C++`(`-std=c++17`)
without reusing the code in [Andy Ross's nasal interpreter](https://github.com/andyross/nasal).
But we really appreciate that Andy created this amazing programming language.

This project uses __MIT license__ (2019/7 ~ 2021/5/4 ~ 2023/5), __GPL v2 license__ (since 2023/6).

### __Why writing this nasal interpreter?__

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

## __How to Compile__

![windows](https://img.shields.io/badge/Microsoft-Windows-green?style=flat-square&logo=windows)
![macOS](https://img.shields.io/badge/Apple%20Inc.-MacOS-green?style=flat-square&logo=apple)
![linux](https://img.shields.io/badge/GNU-Linux-green?style=flat-square&logo=GNU)
![g++](https://img.shields.io/badge/GNU-g++-A42E2B?style=flat-square&logo=GNU)
![clang++](https://img.shields.io/badge/LLVM-clang++-262D3A?style=flat-square&logo=LLVM)
![vs](https://img.shields.io/badge/Visual_Studio-MSVC-5C2D91?style=flat-square&logo=visualstudio)

Better download the latest update source of the interpreter and build it! It's quite easy to build this interpreter, what you need are only two things: C++ compiler and the `make`. There is no third-party library used in this project.

__CAUTION__: If want to use the release zip/tar.gz file to build the interpreter, please read the [__Release Notes__](./doc/dev.md#release-notes) to make sure this release file has no fatal bugs.

### __`Windows (MinGW-w64)`__

Make sure your MinGW thread model is `posix thread model`, otherwise it may not have the thread library.

> mkdir build
>
> mingw32-make nasal.exe -j4

### __`Windows (Visual Studio)`__

This project gives a [__CMakelists.txt__](./CMakeLists.txt) for you to create project in `Visual Studio`.

### __`Linux/macOS/Unix`__

> mkdir build
>
> make -j4

You could choose which compiler you want to use:

> make nasal CXX=...

## __How to Use__

![usage](./doc/gif/help.gif)

If your system is `Windows` and you want to output unicode, you could write this in nasal code:

```javascript
if (os.platform()=="windows") {
    system("chcp 65001");
}
```

## __Tutorial__

Nasal is really __easy__ to learn.
Reading this tutorial will not takes you over 15 minutes.
__If you have learnt C/C++/Javascript before, this will take less time.__
You could totally use it after reading this simple tutorial:

<details><summary> Basic type </summary>

__`none`__ is error type used to interrupt the execution.
This type is not created by user program.

__`nil`__ is a null type. Just like `null`.

```javascript
var spc = nil;
```

__`num`__ has 3 formats: `dec`, `hex` and `oct`. Using IEEE754 `double` to store.

```javascript
# this language use '#' to write notes
var n = 2.71828;    # dec
var n = 2.147e16;   # dec
var n = 1e-10;      # dec
var n = 0xAA55;     # hex
var n = 0o170001;   # oct

# caution: true and false also useful in nasal now
var n = true;       # in fact n is now 1.0
var n = false;      # in face n is now 0.0
```

__`str`__ has 3 formats. The third one is used to declare a character.

```javascript
var s = 'str';
var s = "another string";
var s = `c`;
# some special characters is allowed in this language:
'\a'; '\b'; '\e'; '\f';
'\n'; '\r'; '\t'; '\v';
'\0'; '\\'; '\?'; '\'';
'\"';
```

__`vec`__ has unlimited length and can store all types of values.

```javascript
var vec = [];
var vec = [0, nil, {}, [], func(){return 0}];
append(vec, 0, 1, 2);
```

__`hash`__ is a hashmap (or like a `dict` in `python`) that stores values with strings/identifiers as the key.

```javascript
var hash = {
    member1: nil,
    member2: "str",
    "member3": "member\'s name can also be a string constant",
    funct: func() {
        return me.member2~me.member3;
    }
};
```

__`func`__ is a function type (in fact it is `lambda`).

```javascript
var f = func(x, y, z) {
    return nil;
}
# function could be declared without parameters and `(`, `)`
var f = func {
    return 114514;
}
var f = func(x, y, z, deft = 1) {
    return x+y+z+deft;
}
var f = func(args...) {
    var sum = 0;
    foreach(var i; args) {
        sum += i;
    }
    return sum;
}
```

__`upval`__ is used to store upvalues, used in __`vm`__ to make sure closure runs correctly.

__`ghost`__ is used to store other complex `C/C++` data types.
This type is created by native-function of nasal. If want to define a new data type, see how to add native-functions by editing code.

</details>

<details><summary> Operators </summary>

Nasal has basic math operators `+` `-` `*` `/` and a special operator `~` that joints strings.

```javascript
1+2-(1+3)*(2+4)/(16-9);
"str1"~"str2";
```

For conditional expressions, operators `==` `!=` `<` `>` `<=` `>=` are used to compare two values.
`and` `or` have the same function as C/C++ `&&` `||`.

```javascript
1+1 and (1<0 or 1>0);
1<=0 and 1>=0;
1==0 or 1!=0;
```

Unary operators `-` `!` have the same function as C/C++.

```javascript
-1;
!0;
```

Bitwise operators `~` `|` `&` `^` have the same function as C/C++.

```javascript
# these operators will:
# 1. convert f64 to i32 (static_cast<int32_t>)
# 2. do the bitwise function

~0x80000000; # not 2147483647
0x8|0x1;     # or
0x1&0x2;     # and
0x8^0x1;     # xor
```

Operators `=` `+=` `-=` `*=` `/=` `~=` `^=` `&=` `|=` are used in assignment expressions.

```javascript
a = b = c = d = 1;
a += 1;
a -= 1;
a *= 1;
a /= 1;
a ~= "string";

a ^= 0xff;
a &= 0xca;
a |= 0xba;
```

</details>

<details><summary> Definition </summary>

As follows.

```javascript
var a = 1;             # define single variable
var (a, b, c) = [0, 1, 2]; # define multiple variables from a vector
var (a, b, c) = (0, 1, 2); # define multiple variables from a tuple
```

Nasal has many special global symbols:

```javascript
globals; # hashmap including all global symbols and their values
arg;     # in global scope, arg is the command line arguments
         # in local scope, arg is the dynamic arguments of this function call
```

For example:

```javascript
var a = 1;
println(globals); # will print {a:1}
```

```javascript
# nasal a b c
println(arg); # will print ["a", "b", "c"]

func() {
    println(arg);
}(1, 2, 3);   # will print [1, 2, 3]
```

</details>

<details><summary> Multi-assignment </summary>

The last one is often used to swap two variables.

```javascript
(a, b[0], c.d) = [0, 1, 2];
(a, b[1], c.e) = (0, 1, 2);
(a, b) = (b, a);
```

</details>

<details><summary> Conditional expression </summary>

In nasal there's a new key word `elsif`.
It has the same functions as `else if`.

```javascript
if (1) {
    ;
} elsif (2) {
    ;
} else if (3) {
    ;
} else {
    ;
}
```

</details>

<details><summary> Loop </summary>

While loop and for loop is simalar to C/C++.

```javascript
while(condition) {
    continue;
}
for(var i = 0; i<10; i += 1) {
    break;
}
```

Nasal has another two kinds of loops that iterates through a vector:

`forindex` will get the index of a vector. Index will be `0` to `size(elem)-1`.

```javascript
forindex(var i; elem) {
    print(elem[i]);
}
```

`foreach` will get the element of a vector. Element will be `elem[0]` to `elem[size(elem)-1]`.

```javascript
foreach(var i; elem) {
    print(i);
}
```

</details>

<details><summary> Subvec </summary>

Nasal provides this special syntax to help user generate a new vector by getting values by one index or getting values by indexes in a range from an old vector.
If there's only one index in the bracket, then we will get the value directly.
Use index to search one element in the string will get the __ascii number__ of this character.
If you want to get the character, use built-in function `chr()`.

```javascript
a[0];
a[-1, 1, 0:2, 0:, :3, :, nil:8, 3:nil, nil:nil];
"hello world"[0];
```

</details>

<details><summary> Special function call </summary>

This is not very efficient,
because hashmap use string as the key to compare.

But if it really useful, the efficientcy may not be so important...

```javascript
f(x:0, y:nil, z:[]);
```

</details>

<details><summary> Lambda </summary>

Also functions have this kind of use:

```javascript
func(x, y) {
    return x+y
}(0, 1);
func(x) {
    return 1/(1+math.exp(-x));
}(0.5);
```

There's an interesting test file `y-combinator.nas`,
try it for fun:

```javascript
var fib = func(f) {
    return f(f);
}(
    func(f) {
        return func(x) {
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
var f = func() {
    var a = 1;
    return func() {return a;};
}
print(f()());
```

Using closure makes it easier to OOP.

```javascript
var student = func(n, a) {
    var (name, age) = (n, a);
    return {
        print_info: func()  {println(name, ' ', age);},
        set_age:    func(a) {age = a;},
        get_age:    func()  {return age;},
        set_name:   func(n) {name = n;},
        get_name:   func()  {return name;}
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
var trait = {
    get: func {return me.val;},
    set: func(x) {me.val = x;}
};

var class = {
    new: func() {
        return {
            val: nil,
            parents: [trait]
        };
    }
};
var a = class.new();
a.set(114514);
println(a.get());
```

First virtual machine cannot find member `set` in hash `a`, but in `a.parents` there's a hash `trait` has the member `set`, so we get the `set`.
variable `me` points to hash `a`, so we change the `a.val`.
And `get` has the same process.

And we must remind you that if you do this:

```javascript
var trait = {
    get: func {return me.val;},
    set: func(x) {me.val = x;}
};

var class = {
    new: func() {
        return {
            val: nil,
            parents: [trait]
        };
    }
};
var a = class.new();
var b = class.new();
a.set(114);
b.set(514);
println(a.get());
println(b.get());

var c = a.get;
var d = b.get;

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

This part shows how we add native functions in this interpreter.
If you are interested in this part, this may help you.
And...

__CAUTION:__ If you want to add your own functions __without__ changing the source code, see the __`module`__ after this part.

If you really want to change source code, check built-in functions in `lib.nas` and see the example below.

Definition:

```C++
// you could also use a macro to define one.
var builtin_print(context*, gc*);
```

Then complete this function using C++:

```C++
var builtin_print(context* ctx, gc* ngc) {
    // find value with index begin from 1
    // because local[0] is reserved for value 'me'
    for(auto& i : ctx->localr[1].vec().elems) {
        std::cout << i;
    }
    std::cout << std::flush;
    // generate return value,
    // use ngc::alloc(type) to make a new value
    // or use reserved reference nil/one/zero
    return nil;
}
```

When running a builtin function, alloc will run more than one time, this may cause mark-sweep in `gc::alloc`.
The value got before will be collected, but stil in use in this builtin function, this will cause a fatal error.

So use `gc::temp` in builtin functions to temprorarily store the gc-managed value that you want to return later. Like this:

```C++
var builtin_keys(context* ctx, gc* ngc) {
    auto hash = ctx->localr[1];
    if (hash.type!=vm_hash && hash.type!=vm_map) {
        return nas_err("keys", "\"hash\" must be hash");
    }
    // use gc.temp to store the gc-managed-value, to avoid being sweeped
    auto res = ngc->temp = ngc->alloc(vm_vec);
    auto& vec = res.vec().elems;
    if (hash.type==vm_hash) {
        for(const auto& iter : hash.hash().elems) {
            vec.push_back(ngc->newstr(iter.first));
        }
    } else {
        for(const auto& iter : hash.map().mapper) {
            vec.push_back(ngc->newstr(iter.first));
        }
    }
    ngc->temp = nil;
    return res;
}
```

After that, register the built-in function's name(in nasal) and the function's pointer in this table:

```C++
nasal_builtin_table builtin[] = {
    {"__print", builtin_print},
    {nullptr,  nullptr}
};
```

At last,warp the `__print` in a nasal file:

```javascript
var print = func(elems...) {
    return __print(elems);
};
```

In fact the arguments that `__print` uses are not necessary.
So writting it like this is also right:

```javascript
var print = func(elems...) {
    return __print;
};
```

If you don't warp built-in function in a normal nasal function,
this native function may cause __segmentation fault__ when searching arguments.

Use `import("filename.nas")` to get the nasal file including your built-in functions, then you could use it.
Also there's another way of importing nasal files, the two way of importing have the same function:

```javascript
use dirname.dirname.filename;
import("./dirname/dirname/filename.nas");
```

</details>

<details><summary> Modules (for lib developers) </summary>

If there is only one way to add your own functions into nasal,
that is really inconvenient.

Luckily, we have developed some useful native-functions to help you add modules that created by you.

Functions used to load dynamic libraries are added to `std/dylib.nas`:

```javascript
var dlopen = func(libname) {
    ...
}

var dlclose = func(lib) {
    ...
}

var dlcall = func(ptr, args...) {
    ...
}

var limitcall = func(arg_size = 0) {
    ...
}
```

As you could see, these functions are used to load dynamic libraries into the nasal runtime and execute.
Let's see how they work.

First, write a cpp file that you want to generate the dynamic lib, take the `fib.cpp` as the example(example codes are in `./module`):

```C++
// add header file nasal.h to get api
#include "nasal.h"
double fibonaci(double x) {
    if (x<=2) {
        return x;
    }
    return fibonaci(x-1)+fibonaci(x-2);
}
// module functions' parameter list example
var fib(var* args, usize size, gc* ngc) {
    if (!size) {
        return nas_err("fib", "lack arguments");
    }
    // the arguments are generated into a vm_vec: args
    // get values from the vector that must be used here
    var num = args[0];
    // if you want your function safer, try this
    // nas_err will print the error info on screen
    // and return vm_null for runtime to interrupt
    if(num.type!=vm_num) {
        return nas_err("extern_fib", "\"num\" must be number");
    }
    // ok, you must know that vm_num now is not managed by gc
    // if want to return a gc object, use ngc->alloc(type)
    // usage of gc is the same as adding a native function
    return var::num(fibonaci(num.tonum()));
}

// then put function name and address into this table
// make sure the end of the table is {nullptr,nullptr}
module_func_info func_tbl[] = {
    {"fib", fib},
    {nullptr, nullptr}
};

// must write this function, this will help nasal to
// get the function pointer by name
// the reason why using this way to get function pointer
// is because `var` has constructors, which is not compatiable in C
// so "extern "C" var fib" may get compilation warnings
extern "C" module_func_info* get() {
    return func_tbl;
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

Then we write a test nasal file to run this fib function, using `os.platform()` we could write a cross-platform program:

```javascript
use std.dylib;
var dlhandle = dylib.dlopen("libfib."~(os.platform()=="windows"?"dll":"so"));
var fib = dlhandle.fib;
for(var i = 1; i<30; i += 1)
    println(dylib.dlcall(fib, i));
dylib.dlclose(dlhandle.lib);
```

`dylib.dlopen` is used to load dynamic library and get the function address.

`dylib.dlcall` is used to call the function, the first argument is the function address, make sure this argument is `vm_obj` and `type=obj_extern`.

`dylib.dlclose` is used to unload the library, at the moment that you call the function, all the function addresses that got from it are invalid.

`dylib.limitcall` is used to get `dlcall` function that has limited parameter size, this function will prove the performance of your code because it does not use `vm_vec` to store the arguments, instead it uses local scope to store them, so this could avoid frequently garbage collecting. And the code above could also be written like this:

```javascript
use std.dylib;
var dlhandle = dylib.dlopen("libfib."~(os.platform()=="windows"?"dll":"so"));
var fib = dlhandle.fib;
var invoke = dylib.limitcall(1); # this means the called function has only one parameter
for(var i = 1; i<30; i += 1)
    println(invoke(fib, i));
dylib.dlclose(dlhandle.lib);
```

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

<details><summary> Ghost Type (for lib developers) </summary>

It's quite easy to create a new ghost by yourself now.
Look at the example below:

```c++
const auto ghost_for_test = "ghost_for_test";

// declare destructor for ghost type
void ghost_for_test_destructor(void* ptr) {
    std::cout << "ghost_for_test::destructor (0x";
    std::cout << std::hex << reinterpret_cast<u64>(ptr) << std::dec << ") {\n";
    delete static_cast<u32*>(ptr);
    std::cout << "    delete 0x" << std::hex;
    std::cout << reinterpret_cast<u64>(ptr) << std::dec << ";\n";
    std::cout << "}\n";
}

var create_new_ghost(var* args, usize size, gc* ngc) {
    var res = ngc->alloc(vm_obj);
    // create ghost type
    res.ghost().set(ghost_for_test, ghost_for_test_destructor, new u32);
    return res;
}

var set_new_ghost(var* args, usize size, gc* ngc) {
    var res = args[0];
    if (!res.object_check(ghost_for_test)) {
        std::cout << "set_new_ghost: not ghost for test type.\n";
        return nil;
    }
    f64 num = args[1].num();
    *(reinterpret_cast<u32*>(res.ghost().pointer)) = static_cast<u32>(num);
    std::cout << "set_new_ghost: successfully set ghost = " << num << "\n";
    return nil;
}

var print_new_ghost(var* args, usize size, gc* ngc) {
    var res = args[0];
    // check ghost type by the type name
    if (!res.object_check(ghost_for_test)) {
        std::cout << "print_new_ghost: not ghost for test type.\n";
        return nil;
    }
    std::cout << "print_new_ghost: " << res.ghost() << " result = "
              << *((u32*)res.ghost().pointer) << "\n";
    return nil;
}
```

We use this function to create a new ghost type:

`void nas_ghost::set(const std::string&, nasal::nas_ghost::destructor, void*);`

`const std::string&` is the name of the ghost type.

`nasal::nas_ghost::destructor` is the pointer of the destructor of the ghost type.

`void*` is the pointer of the ghost type instance.

And we use this function to check if value is the correct ghost type:

`bool var::object_check(const std::string&);`

The parameter is the name of the ghost type.

</details>

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
--> var fib=func(x)
    {
        if(x<2) return x;
        return fib(x-1)+fib(x-2);
    }
    for(var i=0;i<31;i+=1)
        print(fib(i),'\n');


next bytecode:
    0x000848     4a 00 00 01     callfv  0x1(std/lib.nas:427)
    0x000849     3d 00 00 00     pop     0x0(std/lib.nas:427)
    0x00084a     07 00 00 00     pnil    0x0(std/lib.nas:423)
    0x00084b     56 00 00 00     ret     0x0(std/lib.nas:423)
    0x00084c     03 00 00 5e     loadg   0x5e(std/lib.nas:423)
--> 0x00084d     0b 00 08 51     newf    0x851(test/fib.nas:1)
    0x00084e     02 00 00 03     intl    0x3(test/fib.nas:1)
    0x00084f     0d 00 00 08     para    0x8 (x)(test/fib.nas:1)

stack (0x55ccd0a1b9d0, limit 10, total 0)
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
    0x000850     3e 00 08 60     jmp     0x860(test/fib.nas:1)
--> 0x000851     45 00 00 01     calll   0x1(test/fib.nas:3)
    0x000852     39 00 00 07     lessc   0x7 (2)(test/fib.nas:3)
    0x000853     40 00 08 56     jf      0x856(test/fib.nas:3)
    0x000854     45 00 00 01     calll   0x1(test/fib.nas:3)
    0x000855     56 00 00 00     ret     0x0(test/fib.nas:3)
    0x000856     44 00 00 5f     callg   0x5f(test/fib.nas:4)
    0x000857     45 00 00 01     calll   0x1(test/fib.nas:4)

stack (0x55ccd0a1b9d0, limit 10, total 8)
  0x000007    | pc   | 0x869
  0x000006    | addr | 0x0
  0x000005    | nil  |
  0x000004    | nil  |
  0x000003    | num  | 0
  0x000002    | nil  |
  0x000001    | nil  |
  0x000000    | func | <0x55ccd0a58fa0> entry:0x487
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
