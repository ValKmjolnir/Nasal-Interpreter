# Nasal Interpreter

[![nasal_new_logo](pic/nasal.png?raw=true)](http://wiki.flightgear.org/File:Nasallogo3.png)

# Nasal script language

[Nasal](http://wiki.flightgear.org/Nasal_scripting_language) is a script language that used in [FlightGear](https://www.flightgear.org/).

There is a Nasal console in FlightGear but sometimes it is not so easy for every developer to use.

So this is an interpreter for Nasal written by C++.

The interpreter is still in development.Anyone who interested in this could also join us!
  
# Lexical Analysis
  
What do it's outputs look like?

> ( identifier   | start )

> ( operator     | ; )

> ( reserve word | print )

> ( operator     | ( )

> ( string       | Engine started )

> ( operator     | ) )

This is what it outputs.

And the flow chart of lexer is here:

[![nasal_lexer.png](pic/nasal_lexer.png?raw=true)](https://github.com/ValKmjolnir/Nasal-Interpreter/blob/master/pic/nasal_lexer.png)

# Push down automata

After many times of failure,i finally try to use PDA to do the parse work.

But something occurred,so i finally didn't make it.However you can still see this LL(1) and LR(0) parser in __version 0.7 & 0.9__ .

# Parser

The parser can recognize some basic elements in resource program.

In __version 1.1__ you can use the fully-functional parser.

But there are still some differences such as:

```javascript
(var a,b,c)=(1,2,3);
var (r,g,b)=color;
(a,b)=(b,a);
```

etc. cannot be recognized.

And each statement must have a ';' after it or you will be informed with __'error: expect a ';''__ .

In __version 2.0__ these problems have been solved.

In __version 2.0__
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

__And to check if an expr in '(' ')' is multi_scalar or multi_id__

__i used bool nasal_parse::check_multi_scalar() and bool nasal_parse::check_multi_assignment()__

# Calculator

You can try a calculator in __version 0.17~0.19__ !

# Abstract Syntax Tree

In __version 1.2__ the ast has been completed.

# Version 2.0

a completed ast-interpreter with unfinished lib functions.

a new byte code interpreter is in progress(version 3.0).
