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
<hash_search> ::= <use_identifier> <[> <string> <]>
<hash_member> ::= <identifier> <:> <scalar>
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
<assignment> ::= <identifier> <=> <scalar> <;>

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
<statement> ::= <definition>|<assignment>|<loop>|<choose>|<use_function>|<func_return> <;>
<statements> ::= <statement> <statement>
<statements> ::= <statements> <statement>
