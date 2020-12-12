# Nasal Interpreter

[![nasal_new_logo](pic/nasal.png?raw=true)](http://wiki.flightgear.org/File:Nasallogo3.png)

# Nasal script language

[Nasal](http://wiki.flightgear.org/Nasal_scripting_language) is a script language that used in [FlightGear](https://www.flightgear.org/).

There is a Nasal console in FlightGear but sometimes it is not so easy for every developer to use.

So this is an interpreter for Nasal written by C++.

The interpreter is still in development.We really need your support!

# Why Writing Nasal Interpreter

Nasal is a script language first used in Flightgear.

But in last summer holiday, members in FGPRC told me that it is hard to debug with nasal-console in Flightgear, especially when checking syntax error.

So i tried to write a new interpreter to help them checking syntax error and even, runtime error.

I wrote the lexer, parser and runtime(nasal virtual machine/ast-runtime virtual machine) to help checking errors.

They found it easier for them to check errors before copying nasal-codes in nasal-console in Flightgear to test.

# How to Compile

> g++ -std=c++11 main.cpp -o main.exe
  
# Lexical Analysis

The flow chart of lexer is here:

[![nasal_lexer.png](pic/nasal_lexer.png?raw=true)](https://github.com/ValKmjolnir/Nasal-Interpreter/blob/master/pic/nasal_lexer.png)

This picture seems ugly. I will re-draw it later(maybe 1000 years later).

# Parser

## Version 3.0

I refactored parser and make it easier to maintain.

The EBNF is also refactored.

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

# Byte Code Interpreter

## Version 4.0

I have just finished the first version of byte-code-interpreter.

This interpreter is still in test.After this test,i will release version 4.0!

Now i am trying to search hidden bugs in this interpreter.Hope you could help me! :)

There's an example of byte code below:

```javascript
var (a,b,c)=(1,2,3);
```

```asm
.number 1
.number 2
.number 3
.symbol a
.symbol b
.symbol c
0x00000000: pone   0x00000000
0x00000001: load   0x00000000  (a)
0x00000002: pnum   0x00000001  (2)
0x00000003: load   0x00000001  (b)
0x00000004: pnum   0x00000002  (3)
0x00000005: load   0x00000002  (c)
0x00000006: nop    0x00000000
```