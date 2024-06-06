# __Tutorial__

![mandelbrotset](../doc/pic/mandelbrotset.png)

Nasal is __easy__ to learn.
After reading this tutorial about 15 minutes,
You could totally use nasal.

## __Contents__

* [__Basic Type__](#basic-type)
* [__Operators__](#operators)
* [__Definition__](#definition)
* [__Multi-Assignment__](#multi-assignment)
* [__Conditional Expression__](#conditional-expression)
* [__Loop__](#loop)
* [__Subvec__](#subvec)
* [__Special function call__](#special-function-call)
* [__Lambda__](#lambda)
* [__Closure__](#closure)
* [__Trait__](#trait)
* [__Multi-Files/Modules Import__](#multi-filesmodules-import)
* [__Native Functions and Module Import__](#native-functions-and-module-import)
* [__C++ Modules (for lib developers)__](#c-modules-for-lib-developers)
* [__Ghost Type (for lib developers)__](#ghost-type-for-lib-developers)

## Basic Type

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

## Operators

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

Operator `??` is used to check left hand side value is `nil` or not, if not,
return the right hand side value:

```javascript
print(nil??"should get this string");
```

The example above will print `should get this string`.

Operator `?.` is used to get hash member if left hand side value is not `nil`.
And if left hand side value is not `nil` and not `hash`,
will cause error and exit.

```javascript
var a = nil;
print(a?.try_get); # nil

var a = {try_get: "congrats!"};
print(a?.try_get); # "congrats!"
```

## Definition

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

## Multi-assignment

The last one is often used to swap two variables.

```javascript
(a, b[0], c.d) = [0, 1, 2];
(a, b[1], c.e) = (0, 1, 2);
(a, b) = (b, a);
```

## Conditional expression

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

## Loop

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

## Subvec

Nasal provides this special syntax to help user generate a new vector by getting values by one index or getting values by indexes in a range from an old vector.
If there's only one index in the bracket, then we will get the value directly.
Use index to search one element in the string will get the __ascii number__ of this character.
If you want to get the character, use built-in function `chr()`.

```javascript
a[0];
a[-1, 1, 0:2, 0:, :3, :, nil:8, 3:nil, nil:nil];
"hello world"[0];
```

## Special function call

This is not very efficient,
because hashmap use string as the key to compare.

But if it really useful, the efficientcy may not be so important...

```javascript
f(x:0, y:nil, z:[]);
```

## Lambda

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

## Closure

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

## Trait

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

## Multi-Files/Modules Import</summary>

See more details in [namespace.md](./namespace.md)

## Native functions and module import

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

## C++ Modules (for lib developers)

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
NASAL_EXTERN module_func_info* get() {
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

## Ghost Type (for lib developers)

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
