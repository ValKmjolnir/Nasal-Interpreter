/*
<>术语符号
|并列选项，只能选一个
*/

//basic elements
<identifier> ::= IDENTIFIER
<number> ::= NUMBER
<string> ::= STRING

//scalar
<use_identifier> ::= <identifier>|<array_search>|<hash_search>
<scalar> ::= <number>|<string>|<array>|<hash>|<use_identifier>|<use_function>
<scalars> ::= <scalar> <,> <scalar>
<scalars> ::= <scalars> <,> <scalar>

//array
<array_search> ::= <use_identifier> <[> <number> <]>
<array> ::= <[> <scalar>|<scalars> <]>

//hash
<hash_search> ::= <use_identifier> <.> <use_identifier>|<use_function>
<hash_member> ::= <identifier> <:> <scalar>|<function>
<hash_members> ::= <hash_member> <,> <hash_member>
<hash_members> ::= <hash_members> <,> <hash_member>
<hash> ::= <{> <hash_member>|<hash_members> <}>

//function
<parameter> ::= <scalar>
<parameters> ::= <scalars>
<function> ::= <func> <(> <parameter>|<parameters> <)> <{> <statement>|<statements> <}>
<use_function> ::= <use_identifier> <(> <parameter>|<parameters> <)>

//definition & assignment
<definition> ::= <var> <identifier> <=> <scalar>|<function> <;>
<assignment> ::= <identifier> <=>|<+=>|<-=>|<*=>|</=>|<~=> <scalar> <;>

//calculation
<add> ::= <scalar> <+> <scalar>
<sub> ::= <scalar> <-> <scalar>
<mul> ::= <scalar> <*> <scalar>
<div> ::= <scalar> </> <scalar>
<link> ::= <scalar> <~> <scalar>
<calc> ::= <add>|<sub>|<mul>|<div>
<calcs> ::= <(> <calc>|<calcs> <)>
<calcs> ::= <calc> <+>|<->|<*>|</>|<~> <calc>
<calcs> ::= <calcs> <+>|<->|<*>|</>|<~> <calc>
<calcs> ::= <calc> <+>|<->|<*>|</>|<~> <calcs>

//loop
<loop_condition> ::= <statement>|<statements> <;> <condition>|<conditions> <;> <statement>|<statements>
<loop> ::= <for> <(> <loop_condition> <)> <statement>|<{> <statement>|<statements> <}>
<loop> ::= <while> <(> <condition>|<conditions> <)> <statement>|<{> <statement>|<statements> <}>

//if else
<condition_link_operator> ::= <and>|<or>|<!>
<cmp_operator> ::= <==>|<>>|<<>|<<=>|<>=>|<!=>|<condition_link_operator>
<condition> ::= <scalar>
<condition> ::= <scalar> <cmp_operator> <scalar>
<conditions> ::= <(> <condition> <)> <condition_link_operator> <(> <condition> <)>
<conditions> ::= <(> <conditions> <)> <condition_link_operator> <(> <condition> <)>
<conditions> ::= <(> <condition> <)> <condition_link_operator> <(> <conditions> <)>
<conditions> ::= <conditions> <condition_link_operator> <(> <condition> <)>
<conditions> ::= <conditions> <condition_link_operator> <(> <conditions> <)>
//(e==1) && (e==2)
//((e==1) && (e==2)) && (e==3)
//(e==1) && ((e==2) && (e==3))
//(e==1) && (e==2) && (e==3) && ((e==4) && (e==5))
<_if> ::= <if> <(> <condition>|<conditions> <)> <statement>
<_if> ::= <if> <(> <condition>|<conditions> <)> <{> <statement>|<statements> <}>
<_else> ::= <else> <statement>
<_else> ::= <else> <{> <statement>|<statements> <}>
<else_if> ::= <else> <if> <(> <condition>|<conditions> <)> <statement>
<else_if> ::= <else> <if> <(> <condition>|<conditions> <)> <{> <statement>|<statements> <}>
<else_ifs> ::= <else_if> <else_if>
<else_ifs> ::= <else_ifs> <else_if>
<choose> ::= <_if>
<choose> ::= <_if> <_else>
<choose> ::= <_if> <else_if>
<choose> ::= <_if> <else_ifs>
<choose> ::= <_if> <else_if> <_else>
<choose> ::= <_if> <else_ifs> <_else>

//statement
<func_return> ::= <return> <scalar>
<loop_continue> ::= <continue>
<loop_break> ::= <break>
<statement> ::= <definition>|<assignment>|<loop>|<choose>|<use_function>|<func_return>|<loop_continue>|<loop_break> <;>
<statements> ::= <statement> <statement>
<statements> ::= <statements> <statement>
