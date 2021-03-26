# Nasal Script Language

[Nasal](http://wiki.flightgear.org/Nasal_scripting_language) is a script language that used in [FlightGear](https://www.flightgear.org/).

The interpreter is still in development(now it works well --2021/2/15). We really need your support!

Also,i am a member of [FGPRC](https://www.fgprc.org/), welcome to join us!

# Why Writing Nasal Interpreter

Nasal is a script language first used in Flightgear.

But in last summer holiday, members in FGPRC told me that it is hard to debug with nasal-console in Flightgear, especially when checking syntax errors.

So i tried to write a new interpreter to help them checking syntax error and even, runtime error.

I wrote the lexer, parser and runtime(bytecode virtual machine/ast-runtime virtual machine) to help checking errors.

They found it easier for them to check errors before copying nasal-codes in nasal-console in Flightgear to test.

# How to Compile

> g++ -std=c++11 main.cpp -o main.exe

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
0x00000001: load   0x00000000 (i)
0x00000002: call   0x00000000 (i)
0x00000003: pnum   0x00000001 (4e+006)
0x00000004: less   0x00000000
0x00000005: jf     0x0000000b
0x00000006: pone   0x00000000
0x00000007: mcall  0x00000000 (i)
0x00000008: addeq  0x00000000
0x00000009: pop    0x00000000
0x0000000a: jmp    0x00000002
0x0000000b: nop    0x00000000
```

## Version 5.0

I decide to optimize bytecode vm in this version.

Because it takes more than 1.5s to count i from 0 to 4000000-1.This is not efficient at all!

2021/1/23 update: Now it can count from 0 to 4000000-1 in 1.5s.

# How to Use Nasal to Program

## basic value type

nasal has 6 value types.Number,string,vector,hash,function,nil.

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

This is of great use but is not very efficient.

```javascript
a(x:0,y:1,z:2);
```

## closure
```javascript
var f=func()
{
    var a=1;
    return func(){return a;};
}
print(f()());
```

## built-in functions

Must import lib.nas or has these functions' definitions inside your code.

Also you could add builtin functions of your own(written in C/C++) to help you calculate things more quickly.(Advanced usage)

Check built-in functions in lib.nas!

If you want to add your own built-in functions,define the function in nasal_builtin.h.

Definition:

```C++
nasal_val* builtin_chr(nasal_val*,nasal_gc&);
```

Then complete this function using C++:

```C++
nasal_val* builtin_print(nasal_val* local_scope_addr,nasal_gc& gc)
{
    // get arguments by using in_builtin_find
    nasal_val* vector_value_addr=in_builtin_find("elements");
    // main process
    // also check type here,if get a type error,use builtin_error_occurred and return nullptr
    nasal_vec& ref_vec=vector_value_addr->get_vector();
    int size=ref_vec.size();
    for(int i=0;i<size;++i)
    {
        nasal_val* tmp=ref_vec[i];
        switch(tmp->get_type())
        {
            case vm_nil:  std::cout<<"nil";             break;
            case vm_num:  std::cout<<tmp->get_number(); break;
            case vm_str:  std::cout<<tmp->get_string(); break;
            case vm_vec:  tmp->get_vector().print();    break;
            case vm_hash: tmp->get_hash().print();      break;
            case vm_func: std::cout<<"func(...){...}";  break;
        }
    }
    // if a nasal value is not in use,use gc::del_reference to delete it
    // if get a new reference of a nasal value,use gc::add_reference
    // generate return value,use gc::gc_alloc(type) to make a new value
    return gc.gc_alloc(vm_nil);
}
```

After that, write the built-in function's name(in nasal) and the function's pointer in this table:

```C++
struct FUNC_TABLE
{
    std::string func_name;
    nasal_val* (*func_pointer)(nasal_val* x,nasal_gc& gc);
} builtin_func_table[]=
{
    {"__builtin_std_cout",builtin_print},
    {"",NULL}
};
```

At last,warp the 'nasal_call_builtin_std_cout' in a nasal file:

```javascript
var print=func(elements...)
{
    nasal_call_builtin_std_cout(elements);
    return nil;
};
```

In version 5.0,if you don't warp built-in function in a normal nasal function,this built-in function may cause a fault when searching arguments,which will cause SIGSEGV segmentation error(maybe).

Use import("") to get the nasal file including your biult-in functions,then you could use it.