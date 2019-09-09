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
  
  What it looks like?
  
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
