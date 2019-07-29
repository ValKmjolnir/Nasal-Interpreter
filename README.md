# Nasal-Interpreter

# Nasal

Nasal is a script language that used in FlightGear.

There is a Nasal console in FlightGear but sometimes it is not so easy for every developer to use.

So this is an interpreter for Nasal written by C++.

The interpreter is still in development.Anyone who interested in this could also join us!

# Now complete all types

var: int   float   double   std::string   char   long long int

NasalList: array in nasal []

  append
  
  setsize
  
  subvec
  
  pop
  
  sort

NasalHash: Hash in nasal {}

  append
  
  delete
  
  contains

  keys
  
  # Now complete Lexical Analysis!
  
  What it looks like?
  
( - , ---- )

( 标识符 , U )

( - , ---- )

( 标识符 , V )

( " , ---- )

( , , ---- )

( Number , 1 )

( ) , ---- )

( ; , ---- )

( Reserve Word , var )

( 标识符 , bwdANode )

( = , ---- )

( 标识符 , props.getNode )

( ( , ---- )

( " , ---- )

( / , ---- )

( 标识符 , systems )

This is what it outputs.
