# Nasal-- Interpreter

# Nasal

Nasal is a script language that used in FlightGear.

There is a Nasal console in FlightGear but sometimes it is not so easy for every developer to use.

So this is an interpreter for Nasal written by C++.

The interpreter is still in development.Anyone who interested in this could also join us!

# Nasal--

For some reasons i will first try to make a interpreter for nasal--(???)

Maybe.Because i haven't take the lesson of 'principle of compiler'(what a shame...)

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

# Now the parser can...

recognize these things...

print("a",e);

var e=1;

var e="str";

var e=[];

var e={};

var e=e[0];

var e=e.e;

var e=e(0);

var e=e();

var e=e[e()];

var e=[1,e,1,e,1];

var e=[e,e,e,e];

var e={e:1,e:2,e:3};

var e=e[e.e];

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

class var_stack;

var_stack nasal_var_stack;

class func_stack;

func_stack nasal_func_stack;

class token_list;

token_list nasal_lexer;

class parse;

parse nasal_parser;

#include "nasal_lexer.h"

main();
