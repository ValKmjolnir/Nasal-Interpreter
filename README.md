# Nasal Script Language

## Introduction

[Nasal](http://wiki.flightgear.org/Nasal_scripting_language)
is an ECMAscript-like programming language that used in [FlightGear](https://www.flightgear.org/).

The interpreter is totally rewritten by [ValKmjolnir](https://github.com/ValKmjolnir) using C++(`-std=c++11`)
without reusing the code in [Andy Ross's nasal interpreter](<https://github.com/andyross/nasal>).
But we really appreciate that Andy created this amazing programming language and his interpreter project.

Now this project uses MIT license (2021/5/4).
Edit it if you want,
use this project to learn or create more interesting things
(But don't forget me XD).

## Why Writing Nasal Interpreter

Nasal is a script language first used in Flightgear,
created by [Andy Ross](<https://github.com/andyross>).
But in 2019 summer holiday,
members in [FGPRC](https://www.fgprc.org/) told me that it is hard to debug with nasal-console in Flightgear,
especially when checking syntax errors.
So i tried to write a new interpreter to help them checking syntax error and even, runtime error.

I wrote the lexer,
parser and
bytecode virtual machine(there was an ast-interpreter,
but i deleted it after version4.0) to help checking errors.
We found it much easier to check syntax and runtime
errors before copying nasal-codes in nasal-console in Flightgear to test.

Also, you could use this language to write some
interesting programs and run them without the lib of Flightgear.
You could add your own built-in functions to change
this interpreter to a useful tool in your own projects (such as a script in your own game).

## How to Compile

Better choose the latest update of the interpreter.
Download the source code and build it!
It's quite easy to build this interpreter.

MUST USE `-O2/-O3` if want to optimize the interpreter!

Also remember to use g++ or clang++.(mingw-w64 in Windows)

> [cpp compiler] -std=c++11 -O3 main.cpp -o nasal.exe -fno-exceptions

Or use this in linux/macOS/Unix

> [cpp compiler] -std=c++11 -O3 main.cpp -o nasal -fno-exceptions

## How to Use?

Input this command to run scripts directly:

> ./nasal filename

Use these commands to get version of interpreter:

> ./nasal -v | --version

Use these commands to get help(see more debug commands in help):

> ./nasal -h | --help

If your system is Windows and you want to output unicode,please use this command before running nasal interpreter:

> chcp 65001

The interpreter's interactive mode will do this automatically,so you don't need to run this command if you use the interactive interpreter.

## Parser

LL(k) parser.

```javascript
(var a,b,c)=[{b:nil},[1,2],func return 0;];
(a.b,b[0],c)=(1,2,3);
```

These two expressions have the same first set,so LL(1) is useless for this language.

Maybe in the future i can refactor it to LL(1) with special checks.

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

### version 1.0(last update 2019/10/14)

First fully functional version of nasal_parser.

Before version 1.0,i tried many times to create a correct parser.

Finally i learned LL(1) and LL(k) and wrote a parser for math formulas in version 0.16(last update 2019/9/14).

In version 0.17(2019/9/15) 0.18(2019/9/18) 0.19(2019/10/1)i was playing the parser happily and after that i wrote version 1.0.

This project began at 2019/8/31.

## Abstract Syntax Tree

### Version 1.2(last update 2019/10/31)

The ast has been completed in this version.

### Version 2.0(last update 2020/8/31)

A completed ast-interpreter with unfinished lib functions.

### Version 3.0(last update 2020/10/23)

The ast is refactored and is now easier to read and maintain.

Ast-interpreter uses new techniques so it can run codes more efficiently.

Now you can add your own functions as builtin-functions in this interpreter!

I decide to save the ast interpreter after releasing v4.0. Because it took me a long time to think and write...

### Version 5.0(last update 2021/3/7)

I change my mind.
AST interpreter leaves me too much things to do.

If i continue saving this interpreter,
it will be harder for me to make the bytecode vm become more efficient.

## Byte Code Virtual Machine

### Version 4.0 (last update 2020/12/17)

I have just finished the first version of byte-code-interpreter.

This interpreter is still in test.After this test,i will release version 4.0!

Now i am trying to search hidden bugs in this interpreter.Hope you could help me! :)

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

### Version 5.0 (last update 2021/3/7)

I decide to optimize bytecode vm in this version.

Because it takes more than 1.5s to count i from 0 to 4000000-1.This is not efficient at all!

2021/1/23 update: Now it can count from 0 to 4000000-1 in 1.5s.

### Version 6.0 (last update 2021/6/1)

Use loadg loadl callg calll mcallg mcalll to avoid branches.

Delete type vm_scop.

Use const vm_num to avoid frequently new & delete.

Change garbage collector from reference-counting to mark-sweep.

Vapp and newf operand use .num to reduce the size of exec_code.

2021/4/3 update: Now it can count from 0 to 4000000-1 in 0.8s.

2021/4/19 update: Now it can count from 0 to 4e6-1 in 0.4s.

In this update i changed global and local scope from unordered_map to vector.

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

### Version 6.5 (last update 2021/6/24)

2021/5/31 update:

Now gc can collect garbage correctly without re-collecting,
which will cause fatal error.

Add builtin_alloc to avoid mark-sweep when running a built-in function,
which will mark useful items as useless garbage to collect.

Better use setsize and assignment to get a big array,
append is very slow in this situation.

2021/6/3 update:

Fixed a bug that gc still re-collects garbage,
this time i use three mark states to make sure garbage is ready to be collected.

Change callf to callfv and callfh.
And callfv fetches arguments from val_stack directly instead of using vm_vec,
a not very efficient way.

Better use callfv instead of callfh,
callfh will fetch a vm_hash from stack and parse it,
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
codegen will generate byte code by nasal_codegen::call_gen() instead of nasal_codegen::mcall_gen(),
and the last child of the ast will be generated by nasal_codegen::mcall_gen().
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
mcall/mcallv/mcallh operands' using frequency will reduce,
call/callv/callh/callfv/callfh at the opposite.

And because of the new structure of mcall,
addr_stack, a stack used to store the memory address,
is deleted from nasal_vm,
and now nasal_vm use nasal_val** mem_addr to store the memory address.
This will not cause fatal errors because the memory address is used __immediately__ after getting it.

### version 7.0 (last update 2021/10/8)

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
std::vector global is deleted,
now the global values are all stored on stack(from val_stack+0 to val_stack+intg-1).

2021/6/29 update:

Add some instructions that execute const values:
op_addc,op_subc,op_mulc,op_divc,op_lnkc,op_addeqc,op_subeqc,op_muleqc,op_diveqc,op_lnkeqc.

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
Now the first identifier that called in assignment will use op_load to assign,
instead of op_meq,op_pop.

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

### version 8.0 (latest)

2021/10/8 update:

In this version vm_nil and vm_num now is not managed by nasal_gc,
this will decrease the usage of gc::alloc and increase the efficiency of execution.

New value type is added: vm_obj.
This type is reserved for user to define their own value types.
Related API will be added in the future.

Fully functional closure:
Add new operands that get and set upvalues.
Delete an old operand 'op_offset'.

2021/10/13 update:

The format of output information of bytecodes changes to this:

```x86asm
0x0000017c: jmp    0x181
0x0000017d: calll  0x1
0x0000017e: calll  0x1
0x0000017f: callfv 0x1
0x00000180: ret    
0x00000181: newf   0x185
0x00000182: intl   0x2
0x00000183: para   0x29 ("f")
0x00000184: jmp    0x19d
0x00000185: newf   0x189
0x00000186: intl   0x2
0x00000187: para   0x1d ("x")
0x00000188: jmp    0x19c
0x00000189: calll  0x1
0x0000018a: lessc  0x12 (2.000000)
0x0000018b: jf     0x18e
0x0000018c: calll  0x1
0x0000018d: ret    
0x0000018e: upval  0x0[0x1]
0x0000018f: upval  0x0[0x1]
0x00000190: callfv 0x1
0x00000191: calll  0x1
0x00000192: subc   0x13 (1.000000)
0x00000193: callfv 0x1
0x00000194: upval  0x0[0x1]
0x00000195: upval  0x0[0x1]
0x00000196: callfv 0x1
0x00000197: calll  0x1
0x00000198: subc   0x12 (2.000000)
0x00000199: callfv 0x1
0x0000019a: add    
0x0000019b: ret    
```

## Benchmark

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

## Simple Tutorial

Nasal is really easy to learn.
Reading this tutorial will not takes you over 15 minutes.
You could totally use it after reading this simple tutorial:

### __Basic Value Type__

`vm_none` is error type.
This type is used to interrupt the execution of virtual machine and will not be created by user program.

`vm_nil` is a null type. It means nothing.

```javascript
var spc=nil;
```

`vm_num` has 3 formats. Dec, hex and oct. Using IEEE754 double to store.

```javascript
var n=1;
var n=2.71828;
var n=2.147e16;
var n=1e-10;
var n=0x7fffffff;
var n=0xAA55;
var n=0o170001;
```

`vm_str` has 3 formats. But the third one is often used to declare a character.

```javascript
var s='str';
var s="another string";
var s=`c`;
```

`vm_vec` has unlimited length and can store all types of values.

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

`vm_hash` is a hashmap that stores values with strings/identifiers as the key.

```javascript
var hash={
    member1:nil,
    member2:'str',
    'member3':'member\'s name can also be a string constant',
    "member4":"also this",
    function:func(){
        var a=me.member2~me.member3;
        return a;
    }
};
```

`vm_func` is a function type (in fact it is lambda).

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

`vm_obj` is a special type that stores user data.
This means you could use other complex C/C++ data types in nasal.
This type is used when you are trying to add a new data structure into nasal,
so this type is often created by native-function that programmed in C/C++ by library developers.
You could see how to write your own native-functions below.

```javascript
var my_new_obj=func(){
    return __builtin_my_obj();
}
var obj=my_new_obj();
```

### __Operators__

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

### __Definition__

```javascript
var a=1;
var (a,b,c)=[0,1,2];
var (a,b,c)=(0,1,2);
(var a,b,c)=[0,1,2];
(var a,b,c)=(0,1,2);
```

### __Multi-Assignment__

```javascript
(a,b[0],c.d)=[0,1,2];
(a,b[1],c.e)=(0,1,2);
(a,b)=(b,a);
```

### __Conditional Expression__

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

### __Loop__

While loop and for loop is simalar to C/C++.

```javascript
while(condition)
    continue;

for(var i=0;i<10;i+=1)
    break;
```

Nasal has another two kinds of loops that iterates through a vector:

`forindex` will get the index of a vector.

```javascript
forindex(var i;elem)
    print(elem[i]);
```

`foreach` will get the element of a vector.

```javascript
foreach(var i;elem)
    print(i);
```

### __Subvec__

Use index to search one element in the string will get the ascii number of this character.
If you want to get the character,use built-in function chr().

```javascript
a[-1,1,0:2,0:,:3,:,nil:8,3:nil,nil:nil];
"hello world"[0];
```

### __Special Function Call__

This is of great use but is not very efficient
(because hashmap use string as the key to compare).

```javascript
f(x:0,y:nil,z:[]);
```

### __Lambda__

Also functions have this kind of use:

```javascript
func(x,y){return x+y}(0,1);
func(x){return 1/(1+math.exp(-x));}(0.5);
```

There's an interesting test file 'y-combinator.nas',
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

### __Closure__

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

### __Trait__

Also there's another way to OOP,that is `trait`.

When a hash has a member named `parents` and the value type is vector,
then when you are trying to find a member that is not in this hash,
virtual machine will search the member is parents.
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
```

First virtual machine cannot find member `set` in hash `a`, but in `a.parents` there's a hash `trait` has the member `set`, so we get the `set`.
variable `me` points to hash `a`, so we change the `a.val`.
And `get` has the same process.

```javascript
var a=class.new();
a.set(114514);
println(a.get());
```

### __Native Functions(This is for library developers)__

You could add builtin functions of your own
(written in C/C++) to help you calculate things more quickly.
(Advanced usage)

Check built-in functions in lib.nas!
You could use this file as the example to learn.

If you want to add your own built-in functions,
define the function in nasal_builtin.h. (or any other place, but remember to compile it)

Definition:

```C++
nasal_ref builtin_chr(std::vector<nasal_ref>&,nasal_gc&);
```

Then complete this function using C++:

```C++
nasal_ref builtin_print(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    // find value with index begin from 1
    // because local_scope[0] is reserved for value 'me'
    nasal_ref vec=local[1];
    // main process
    // also check number of arguments and type here
    // if get an error,use builtin_err
    for(auto i:vec.vec()->elems)
        switch(i.type)
        {
            case vm_none: std::cout<<"undefined";      break;
            case vm_nil:  std::cout<<"nil";            break;
            case vm_num:  std::cout<<i.num();          break;
            case vm_str:  std::cout<<*i.str();         break;
            case vm_vec:  i.vec()->print();            break;
            case vm_hash: i.hash()->print();           break;
            case vm_func: std::cout<<"func(...){...}"; break;
            case vm_obj:  std::cout<<"<object>";       break;
        }
    std::cout<<std::flush;
    // generate return value,use gc::alloc(type) to make a new value
    // or use reserved reference gc.nil/gc.one/gc.zero
    return gc.nil;
}
```

After that, register the built-in function's name(in nasal) and the function's pointer in this table:

```C++
struct func
{
    const char* name;
    nasal_ref (*func)(std::vector<nasal_ref>&,nasal_gc&);
} builtin_func[]=
{
    {"__builtin_print",builtin_print},
    {nullptr,          nullptr      }
};
```

At last,warp the `__builtin_print` in a nasal file:

```javascript
var print=func(elems...){
    return __builtin_print(elems);
};
```

In fact the arguments that `__builtin_print` uses is not necessary.
So writting it like this is also right:

```javascript
var print=func(elems...){
    return __builtin_print;
};
```

If you don't warp built-in function in a normal nasal function,
this built-in function may cause a fault when searching arguments,
which will segmentation error.

Use `import(".nas")` to get the nasal file including your built-in functions,
then you could use it.

version 6.5 update:

Use nasal_gc::builtin_alloc in builtin function if this function uses alloc more than one time.

When running a builtin function,alloc will run more than one time,
this may cause mark-sweep in gc::alloc.

The value got before will be collected,but stil in use in this builtin function,
this is a fatal error.

So use builtin_alloc in builtin functions like this:

```C++
nasal_ref builtin_keys(std::vector<nasal_ref>& local,nasal_gc& gc)
{
    nasal_ref hash=local[1];
    if(hash.type!=vm_hash)
    {
        builtin_err("keys","\"hash\" must be hash");
        return nasal_ref(vm_none);
    }

    // push vector into local scope to avoid being sweeped
    local.push_back(gc.alloc(vm_vec));
    std::vector<nasal_ref>& vec=local.back().vec()->elems;
    for(auto& iter:hash.hash()->elems)
    {
        nasal_ref str=gc.builtin_alloc(vm_str);
        *str.str()=iter.first;
        vec.push_back(str);
    }
    return local.back();
}
```

## Difference Between Andy's Nasal Interpreter and This Interpreter

This interpreter uses more strict syntax to make sure it is easier for you to program and debug.

In Andy's interpreter:

```javascript
foreach(i;[0,1,2,3])
    print(i)
```

This program can run normally with output 0 1 2 3.
But take a look at the iterator 'i',
this symbol is defined in foreach without using keyword 'var'.
I think this design will make programmers filling confused.
This is ambiguous that programmers maybe difficult to find the 'i' is defined here.
Without 'var',programmers may think this 'i' is defined anywhere else.

So in this new interpreter i use a more strict syntax to force users to use 'var' to define iterator of forindex and foreach.
If you forget to add the keyword 'var',
and you haven't defined this symbol before,
you will get this:

```javascript
[code] <test.nas> line 1: undefined symbol "i".
[codegen] in <test.nas>: error(s) occurred,stop.
```

Also there's another difference.
In Andy's interpreter:

```javascript
var a=func {print(b);}
var b=1;
a();
```

This program runs normally with output 1.
But in this new interpreter, it will get:

```javascript
[code] <test.nas> line 1: undefined symbol "b".
[codegen] in <test.nas>: error(s) occurred,stop.
```

(outdated)This difference is caused by different kinds of ways of lexical analysis.
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

In this new interpreter,
function doesn't put dynamic arguments into vector 'arg' automatically.
So if you use 'arg' without definition,
you'll get an error of 'undefined symbol'.

## Trace Back Info

Now when the interpreter crashes,
it will print trace back information:

```javascript
func()
{
    println("hello");
    die("error occurred this line");
    return;
}();
```

Function 'die' is used to throw error and crash.

```javascript
hello
[vm] error: error occurred this line
[vm] error at 0x0000009b: native function error.
trace back:
        0x0000009b: callb  0x22 <__builtin_die> (lib.nas line 85)
        0x00000182: callfv 0x1 (a.nas line 6)
        0x00000186: callfv 0x0 (a.nas line 8)
vm stack(limit 10):
        null |
        func | <0x8b0f50> func{entry=0x9b}
        func | <0x8b1db0> func{entry=0x17c}
        num  | 57.295780
        num  | 1852.000000
        num  | 1.943800
        num  | 0.000540
        num  | 39.370100
        num  | 3.280800
        num  | 0.453600
```

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

And the trace back info:

```javascript
[vm] stack overflow
trace back:
        0x0000000f: callfv 0x1 (a.nas line 5)
        0x0000000f: 4090 same call(s) ...    
        0x00000007: callfv 0x1 (a.nas line 2)
        0x00000013: callfv 0x1 (a.nas line 3)
vm stack(limit 10):
        func | <0xc511e0> func{entry=0xd}    
        ...  | 9 same value(s)
```

Error will be thrown if there's a fatal error when executing:

```javascript
func(){
    return 0;
}()[1];
```

And the trace back info:

```javascript
[vm] error at 0x00000008: callv: must call a vector/hash/string
trace back:
        0x00000008: callv  0x0 (a.nas line 3)
vm stack(limit 10):
        num  | 0.000000
```

Use command `-d` or `--detail` the trace back info will be this:

```javascript
hello world
[vm] error: exception test
[vm] native function error.
trace back:
        0x0000008f: callb  0x22 <__builtin_die> (<lib.nas> line 20)
        0x00000214: callfv 0x1 (<test/exception.nas> line 16)      
        0x00000248: callfv 0x0 (<test/exception.nas> line 39)      
vm stack(limit 10):
        null |
        func | <0x23bc3f0> func{entry=0x8f}
        func | <0x23bdc50> func{entry=0x20e}
mcall address: 0x24a4b88
global:
[0]     func | <0x23d3960> func{entry=0x5}
[1]     func | <0x23bb8b0> func{entry=0xc}
[2]     func | <0x23bb950> func{entry=0x14}
[3]     func | <0x23bb9f0> func{entry=0x1c}
[4]     func | <0x23bba90> func{entry=0x23}
[5]     func | <0x23bbb30> func{entry=0x29}
[6]     func | <0x23bbbd0> func{entry=0x30}
[7]     func | <0x23bbc70> func{entry=0x38}
[8]     func | <0x23bbd10> func{entry=0x40}
[9]     func | <0x23bbdb0> func{entry=0x47}
[10]    func | <0x23bbe50> func{entry=0x4e}
[11]    func | <0x23bbef0> func{entry=0x55}
[12]    func | <0x23bbf90> func{entry=0x5c}
[13]    func | <0x23bc030> func{entry=0x63}
[14]    func | <0x23bc0d0> func{entry=0x6a}
[15]    func | <0x23bc170> func{entry=0x72}
[16]    func | <0x23bc210> func{entry=0x7a}
[17]    func | <0x23bc2b0> func{entry=0x81}
[18]    func | <0x23bc350> func{entry=0x88}
[19]    func | <0x23bc3f0> func{entry=0x8f}
[20]    func | <0x23bc490> func{entry=0x96}
[21]    func | <0x23bc530> func{entry=0x9f}
[22]    func | <0x23bc5d0> func{entry=0xa7}
[23]    func | <0x23bc670> func{entry=0xaf}
[24]    func | <0x23bc710> func{entry=0xb7}
[25]    func | <0x23bc7b0> func{entry=0xbf}
[26]    func | <0x23bc850> func{entry=0xc6}
[27]    func | <0x23bc8f0> func{entry=0xcd}
[28]    hash | <0x248ae70> {14 member}
[29]    hash | <0x248aed0> {9 member}
[30]    hash | <0x248af30> {12 member}
[31]    num  | 0.017453
[32]    num  | 0.592500
[33]    num  | 0.304800
[34]    num  | 3.785400
[35]    num  | 0.025400
[36]    num  | 2.204600
[37]    num  | 1.687800
[38]    num  | 0.514400
[39]    num  | 0.264200
[40]    num  | 0.453600
[41]    num  | 3.280800
[42]    num  | 39.370100
[43]    num  | 0.000540
[44]    num  | 1.943800
[45]    num  | 1852.000000
[46]    num  | 57.295780
[47]    hash | <0x248af90> {3 member}
[48]    func | <0x23bdcf0> func{entry=0x21e}
[49]    func | <0x23bdd90> func{entry=0x22d}
[50]    func | <0x23bde30> func{entry=0x237}
local:
[0]     nil  |
[1]     str  | <0x249abd0> exception test
```
