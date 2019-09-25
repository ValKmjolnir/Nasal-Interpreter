# Nasal Interpreter

# Nasal

Nasal is a script language that used in FlightGear.

There is a Nasal console in FlightGear but sometimes it is not so easy for every developer to use.

So this is an interpreter for Nasal written by C++.

The interpreter is still in development.Anyone who interested in this could also join us!


# Now complete add and sub in abstract syntax tree

And add a command 'ast' to see the AST

and command 'run' can give you the result
  
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

And in future i will make it to support more flexible grammar.
