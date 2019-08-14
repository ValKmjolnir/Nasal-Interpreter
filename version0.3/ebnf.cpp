/*
<>术语符号
|并列选项，只能选一个
*/

//basic elements
<identifier> ::= IDENTIFIER
<number> ::= NUMBER
<string> ::= STRING

//scalar
<identifier> ::= <identifier>|<array_search>|<hash_search>
<identifiers> ::= <identifier> <,> <identifier>
<identifiers> ::= <identifiers> <,> <identifier>
<scalar> ::= <number>|<string>|<array>|<hash>|<use_function>
<scalars> ::= <scalar> <,> <scalar>
<scalars> ::= <scalars> <,> <scalar>

//array
<array_search> ::= <identifier> <[> <number> <]>
<array> ::= <[> <scalar>|<scalars>|<identifier>|<identifiers> <]>

//hash
<hash_search> ::= <identifier> <.> <identifier>|<use_function>
<hash_member> ::= <identifier> <:> <identifier>|<scalar>|<function>
<hash_members> ::= <hash_member> <,> <hash_member>
<hash_members> ::= <hash_members> <,> <hash_member>
<hash> ::= <{> <hash_member>|<hash_members> <}>

//function
<function> ::= <func> <(> <identifier>|<identifiers> <)> <{> <statement>|<statements> <}>
<use_function> ::= <use_identifier> <(> <scalar>|<scalars>|<identifier>|<identifiers> <)>

//definition & assignment
<definition> ::= <var> <identifier> <=> <scalar>|<identifier>|<function> <;>
<assignment> ::= <identifier> <=>|<+=>|<-=>|<*=>|</=>|<~=> <scalar>|<identifier> <;>

//calculation
<add> ::= <scalar>|<identifier> <+> <scalar>|<identifier>
<sub> ::= <scalar>|<identifier> <-> <scalar>|<identifier>
<mul> ::= <scalar>|<identifier> <*> <scalar>|<identifier>
<div> ::= <scalar>|<identifier> </> <scalar>|<identifier>
<link> ::= <scalar>|<identifier> <~> <scalar>|<identifier>
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
<condition> ::= <scalar>|<identifier>
<condition> ::= <scalar>|<identifier> <cmp_operator> <scalar>|<identifier>
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
<func_return> ::= <return> <scalar>|<identifier> <;>
<loop_continue> ::= <continue> <;>
<loop_break> ::= <break> <;>
<print_function> ::= <print> <(> <scalar>|<scalars>|<identifier>|<identifiers> <)> <;>
<statement> ::= <definition>|<assignment>|<loop>|<choose>|<use_function>|<func_return>|<loop_continue>|<loop_break>|<print_function>
<statements> ::= <statement> <statement>
<statements> ::= <statements> <statement>
