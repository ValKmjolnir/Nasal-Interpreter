# Nasal Interpreter

# Nasal

Nasal is a script language that used in FlightGear.

There is a Nasal console in FlightGear but sometimes it is not so easy for every developer to use.

So this is an interpreter for Nasal written by C++.

The interpreter is still in development.Anyone who interested in this could also join us!
  
  # Now complete Lexical Analysis!
  
  What do it's outputs look like?
  
( Identifier  | engineTimer )

( Operator    | . )

( Identifier  | start )

( Operator    | ( )

( Operator    | ) )

( Operator    | ; )

( ReserveWord | print )

( Operator    | ( )

( String      | "Engine started" )

( Operator    | ) )

( Operator    | ; )

This is what it outputs.

# Push down automata

After many times of failure,i finally try to use PDA to do the parse work.

But something occurred,so i finally didn't make it.However you can still see this LL(1) and LR(0) parser in version 0.7 & 0.9

# Now complete Parser

The parser can recognize some basic elements in resource program.

In version 1.1 you can use the fully-functional parser.

But there are still some differences such as:

(var a,b,c)=(1,2,3);

var (r,g,b)=color;

(a,b)=(b,a);

etc. cannot be recognized.

And each statement must have a ';' after it or you will be informed with 'error: expect a ';''.

# Calculator

You can try a calculator in version 0.17~0.19 !

# Now complete Abstract Syntax Tree

In version 1.2 the ast has been completed.

But there are still some bugs inside.
