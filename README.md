# Nasal Script Language

[Nasal](http://wiki.flightgear.org/Nasal_scripting_language) is a script language that used in [FlightGear](https://www.flightgear.org/).

The interpreter is totally rewritten by ValKmjolnir using C++(standard c++11) without reusing the code in Andy Ross's nasal interpreter(https://github.com/andyross/nasal). But we really appreciate that Andy created this amazing programming language and his interpreter project.

The interpreter is still in development(now it works well --2021/2/15). We really need your support!

Also,i am a member of [FGPRC](https://www.fgprc.org/), welcome to join us!

(2021/5/4) Now this project uses MIT license.Edit it if you want, use this project to learn or create more interesting things(But don't forget me XD).

# Why Writing Nasal Interpreter

Nasal is a script language first used in Flightgear, created by Andy Ross(https://github.com/andyross).

But in last summer holiday, members in FGPRC told me that it is hard to debug with nasal-console in Flightgear, especially when checking syntax errors.

So i tried to write a new interpreter to help them checking syntax error and even, runtime error.

I wrote the lexer, parser and runtimebytecode virtual machine(there was an ast-interpreter,but i deleted it after version4.0) to help checking errors.

They found it much easier to check syntax and runtime errors before copying nasal-codes in nasal-console in Flightgear to test.

Also, you could use this language to write some interesting programs and run them without the lib of Flightgear.

You could add your own built-in functions to change this interpreter to a useful tool in your own projects(such as a script in your own game).

# How to Compile

Better choose the latest update of the interpreter.

MUST USE -O2 ! pragma gcc optimize(2) seems useless when using g++

> g++ -std=c++11 -O2 main.cpp -o main.exe

# Parser

LL(k) parser.

```javascript

(var a,b,c)=[{b:nil},[1,2],func{return 0;}];

(a.b,b[0],c)=(1,2,3);

```

have the same first set,so LL(1) is useless for this language.

Maybe in the future i can refactor it to LL(1) with special checks.

# Abstract Syntax Tree

## Version 1.2

The ast has been completed in this version.

## Version 2.0

A completed ast-interpreter with unfinished lib functions.

## Version 3.0

The ast is refactored and is now easier to read and maintain.

Ast-interpreter uses new techniques so it can run codes more efficiently.

Now you can add your own functions as builtin-functions in this interpreter!

I decide to save the ast interpreter after releasing v4.0. Because it took me a long time to think and write...

## Version 5.0

I change my mind.AST interpreter leaves me too much things to do.

If i continue saving this interpreter,it will be harder for me to make the bytecode vm become more efficient.

# Byte Code Interpreter

## Version 4.0

I have just finished the first version of byte-code-interpreter.

This interpreter is still in test.After this test,i will release version 4.0!

Now i am trying to search hidden bugs in this interpreter.Hope you could help me! :)

There's an example of byte code below:

```javascript
for(var i=0;i<4000000;i+=1);
```

```asm
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

## Version 5.0

I decide to optimize bytecode vm in this version.

Because it takes more than 1.5s to count i from 0 to 4000000-1.This is not efficient at all!

2021/1/23 update: Now it can count from 0 to 4000000-1 in 1.5s.

## Version 6.0

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

```asm
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

# How to Use Nasal to Program

## basic value type

Nasal has 6 value types.Number,string,vector,hash,function,nil.

Number has 3 formats.Dec,hex and oct;

String has 3 formats.But the third one is often used to declare a character.

Vector has unlimited length and can store all types of values.

Hash is a hashmap that stores values with strings/identifiers as the key.

Function is also a value type in nasal.

```javascript
var spc=nil;

var a=1;
var a=2.71828;
var a=2.147e16;
var a=1e-10;
var a=0x7fffffff;
var a=0xAA55;
var a=0o170001;

var b='str';
var b="another string";
var b=`c`;

var c=[];
var c=[0,nil,{},[],func(){return 0;}];
append(c,0,1,2);

var d={
    member1:nil,
    member2:'str',
    'member3':'member\'s name can also be a string constant',
    "member4":"also this",
    function:func()
    {
        var a=me.member2~me.member3;
        return a;
    }
};

var f=func(x,y,z)
{
    return nil;
}
var f=func
{
    return 1024;
}
var f=func(x,y,z,default_parameter1=1,default_parameter2=2)
{
    return x+y+z+default_parameter1+default_parameter2;
}
var f=func(x,y,z,dynamic_parameter...)
{
    var sum=0;
    foreach(var i;dynamic_parameter)
        sum+=i;
    return sum+x+y+z;
}
```

## operators

```javascript
1+2;
1-2;
1*2;
1/2;
'str1'~'str2';
(1+2)*(3+4)

1+1 and 0;
1+2*3 or 0;
1<0;
1>0;
1<=0;
1>=0;
1==0;
1!=0;

-1;
!0;

a=b=c=d=1;
a+=1;
a-=1;
a*=1;
a/=1;
a~='string';
```

## definition

```javascript
var a=1;
var (a,b,c)=[0,1,2];
var (a,b,c)=(0,1,2);
(var a,b,c)=[0,1,2];
(var a,b,c)=(0,1,2);
```

## multi-assignment

```javascript
(a,b[0],c.d)=[0,1,2];
(a,b[1],c.e)=(0,1,2);
```

## conditional expression

```javascript
if(1)
{
    ;
}
elsif(2)
{
    ;
}
else if(3)
{
    ;
}
else
{
    ;
}
```

## loop

```javascript
while(condition)
    continue;

for(var i=0;i<10;i+=1)
    break;

forindex(var i;elem)
    print(elem[i]);

foreach(var i;elem)
    print(i);
```

## subvec

Use index to search one element in the string will get the ascii number of this character.If you want to get the character,use built-in function chr().

```javascript
a[-1,1,0:2,0:,:3,:,nil:8,3:nil,nil:nil];
"hello world"[0];
```

## special function call

This is of great use but is not very efficient(because hashmap use string as the key to compare).

```javascript
a(x:0,y:1,z:2);
```

## closure

Use closure to OOP.

```javascript
var f=func()
{
    var a=1;
    return func(){return a;};
}
print(f()());

var student=func(name,age)
{
    var val={
        name:name,
        age:age
    };
    return {
        print_info:func(){println(val.name,' ',val.age);},
        set_age:   func(age){val.age=age;},
        get_age:   func(){return val.age;},
        set_name:  func(name){val.name=name;},
        get_name:  func(){return val.name;}
    };
}
```

## built-in functions

Must import lib.nas or has these functions' definitions inside your code.

Also you could add builtin functions of your own(written in C/C++) to help you calculate things more quickly.(Advanced usage)

Check built-in functions in lib.nas!

If you want to add your own built-in functions,define the function in nasal_builtin.h.

Definition:

```C++
nasal_val* builtin_chr(std::vector<nasal_val*>&,nasal_gc&);
```

Then complete this function using C++:

```C++
nasal_val* builtin_print(std::vector<nasal_val*>& local_scope,nasal_gc& gc)
{
    // get arguments by using builtin_find
    // find value with index begin from 1
    // because local_scope[0] is reserved for value 'me'
    nasal_val* vector_value=local_scope[1];
    // main process
    // also check number of arguments and type here
    // if get a type error,use builtin_err and return nullptr
    for(auto i:vec_addr->ptr.vec->elems)
        switch(i->type)
        {
            case vm_nil:  std::cout<<"nil";            break;
            case vm_num:  std::cout<<i->ptr.num;       break;
            case vm_str:  std::cout<<*i->ptr.str;      break;
            case vm_vec:  i->ptr.vec->print();         break;
            case vm_hash: i->ptr.hash->print();        break;
            case vm_func: std::cout<<"func(...){...}"; break;
        }
    // if a nasal value is not in use,use gc::del_reference to delete it
    // generate return value,use gc::gc_alloc(type) to make a new value
    // or use reserved reference gc.nil_addr/gc.one_addr/gc.zero_addr
    return gc.nil_addr;
}
```

After that, write the built-in function's name(in nasal) and the function's pointer in this table:

```C++
struct FUNC_TABLE
{
    std::string name;
    nasal_val* (*func)(std::vector<nasal_val*>&,nasal_gc&);
} builtin_func[]=
{
    {"__builtin_std_cout",builtin_print},
    {"",                  NULL         }
};
```

At last,warp the '__builtin_std_cout' in a nasal file:

```javascript
var print=func(elements...)
{
    __builtin_std_cout(elements);
    return nil;
};
```

In version 5.0,if you don't warp built-in function in a normal nasal function,this built-in function may cause a fault when searching arguments,which will cause SIGSEGV segmentation error(maybe).

Use import("") to get the nasal file including your built-in functions,then you could use it.