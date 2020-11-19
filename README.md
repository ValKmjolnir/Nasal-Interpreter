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

# Parser

## In __version 2.0__
```javascript
(var a,b,c)=(1,2,3);
var (r,g,b)=color;
```
These two types of statements are both definition_expr.

```javascript
(a,b,c)=(1,2,3);
(a,b)=(b,a);
```
This type of statement is multi_assignment.

And to check if an expr in '(' ')' is multi_scalar or multi_id.

i used bool nasal_parse::check_multi_scalar() and bool nasal_parse::check_multi_assignment().

## In __version 3.0__

I refactored parser and make it easier to maintain.

the EBNF is also refactored.

# Abstract Syntax Tree

In __version 1.2__ the ast has been completed.

In __version 3.0__ the ast is refactored and is now easier to read and maintain.

# Version 2.0

a completed ast-interpreter with unfinished lib functions.

# Version 3.0

ast-interpreter uses new techniques so it can run codes more efficiently.

byte-code-interpreter is in progress(i need a lot of time to learn that :( ).
