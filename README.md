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

( Operator    | } )

( ReserveWord | var )

( Identifier  | stopEngine )

( Operator    | = )

( ReserveWord | func )

( Operator    | ( )

( Operator    | ) )

( Operator    | { )

( Identifier  | props )

( Operator    | . )

( Identifier  | getNode )

( Operator    | ( )

( String      | "/" )

( Operator    | , )

( Number      | 1 )

( Operator    | ) )

( Operator    | . )

( Identifier  | setValue )

( Operator    | ( )

( String      | "/controls/engines/engine/started" )

( Operator    | , )

( Number      | 0 )

( Operator    | ) )

( Operator    | ; )

( Identifier  | props )

( Operator    | . )

( Identifier  | getNode )

( Operator    | ( )

( String      | "/" )

( Operator    | , )

( Number      | 1 )

( Operator    | ) )

( Operator    | . )

( Identifier  | setValue )

( Operator    | ( )

( String      | "/fdm/jsbsim/external_reactions/engine/magnitude" )

( Operator    | , )

( Number      | 0 )

( Operator    | ) )

( Operator    | ; )

( Identifier  | props )

( Operator    | . )

( Identifier  | getNode )

( Operator    | ( )

( String      | "/" )

( Operator    | , )

( Number      | 1 )

( Operator    | ) )

( Operator    | . )

( Identifier  | setValue )

( Operator    | ( )

( String      | "/systems/electrical/e-tron/switch/bat-fwd-eng" )

( Operator    | , )

( Number      | 0 )

( Operator    | ) )

( Operator    | ; )

( Identifier  | props )

( Operator    | . )

( Identifier  | getNode )

( Operator    | ( )

( String      | "/" )

( Operator    | , )

( Number      | 1 )

( Operator    | ) )

( Operator    | . )

( Identifier  | setValue )

( Operator    | ( )

( String      | "/systems/electrical/e-tron/switch/bat-bwd-eng" )

( Operator    | , )

( Number      | 0 )

( Operator    | ) )

( Operator    | ; )

( Identifier  | engineTimer )

( Operator    | . )

( Identifier  | stop )

( Operator    | ( )

( Operator    | ) )

( Operator    | ; )

( ReserveWord | print )

( Operator    | ( )

( String      | "Engine stopped" )

( Operator    | ) )

( Operator    | ; )

( Operator    | } )

This is what it outputs.

# Now with var_stack and function_stack

By printing the two stacks you can see how vars and functions change!

[long long int]a:1

[double]b:3.14

[string]c:"hello world!"

function:swap

function:__main

function:hello

# The structure of basical elements is...

class var;

class nasal_hash;//forward declaration

struct nasal_list_unit;

class nasal_list;

struct nasal_hash_unit;

class nasal_hash;

struct token_unit;

struct parameter;

class func;

class token_list;

#include "nasal_lexer.h"

main();
