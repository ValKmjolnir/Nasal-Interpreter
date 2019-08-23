

data type = <scalar> <identifier> <calculation> <list> <hash> <function>


<scalar> ::= NUMBER|STRING
		<!= <scalar>|<identifier>|<calculation>> <+>|<-> <scalar>
<scalars> ::=
		<scalar>|<scalars>|<list>|<hash>|<calculation> <,> <scalar>|<identifier>|<list>|<hash>|<calculation>
		<identifier>|<identifiers> <,> <scalar>|<list>|<hash>|<calculation>
<identifier> ::=
		<!= <scalar>|<identifier>|<calculation>> <+>|<-> <identifier>
		<id> <!= <[>|<.>|<(>>
<identifiers> ::=
		<identifier>|<identifiers> <,> <identifier>
<calculation> ::=
		<scalar>|<identifier>|<calculation> <+>|<->|<*>|</>|<~>|<<>|<<=>|<>>|<>=>|<==>|<!=>|<and>|<or> <scalar>|<identifier>|<calculation>
		<(> <scalar>|<identifier>|<calculation> <)> <+>|<->|<*>|</>|<~>|<<>|<<=>|<>>|<>=>|<==>|<!=>|<and>|<or> <scalar>|<identifier>|<calculation>
		<scalar>|<identifier>|<calculation> <+>|<->|<*>|</>|<~>|<<>|<<=>|<>>|<>=>|<==>|<!=>|<and>|<or> <(> <scalar>|<identifier>|<calculation> <)>
		<(> <scalar>|<identifier>|<calculation> <)> <+>|<->|<*>|</>|<~>|<<>|<<=>|<>>|<>=>|<==>|<!=>|<and>|<or> <(> <scalar>|<identifier>|<calculation> <)>
		<!= <scalar>|<identifier>|<calculation>> <+>|<-> <calculation>
<unknown_type_id> ::=
		IDENTIFIER
		<unknown_type_id> <.> <identifier>
		<unknown_type_id> <[> <scalar>|<identifier>|<calcualtion> <]>
		<unknown_type_id> <(> <scalar>|<scalars>|<identifier>|<identifiers>|<calculation> <)>
<list> ::=
		<[><]>
		<[> <scalar>|<scalars>|<identifier>|<identifiers>|<list>|<hash>|<calculation> <]>
<hash> ::=
		<{><}>
		<{> <hashmember>|<hashmembers> <}>
<hashmember> ::=
		<identifier> <:> <scalar>|<identifier>|<list>|<hash>|<function>|<calculation>
<hashmembers> ::=
		<hashmember>|<hashmembers> <,> <hashmember>
<function> ::=
		<func> <{><}>
		<func> <{> <statement> <}>
		<func> <(><)> <{><}>
		<func> <(><)> <{> <statement> <}>
		<func> <(> <identifier>|<identifiers> <)> <{><}>
		<func> <(> <identifier>|<identifiers> <)> <{> <statement> <}>
<definition> ::=
		<var> <identifier> <=> <scalar>|<identifier>|<list>|<hash>|<calculation> <;>
		<var> <identifier> <=> <function>
<assigntment> ::=
		<!=var> <identifier> <=> <scalar>|<identifier>|<list>|<hash>|<calculation> <;>
<statement> ::=
		<!=<for>> <definition>|<assignment>
		<return> <scalar>|<identifier>|<list>|<hash>|<calculation> <;>
		<continue>|<break> <;>
		<choose>
		<loop>
<choose>
		<if> <(> <scalar>|<identifier>|<calculation> <)>
		<elsif> <(> <scalar>|<identifier>|<calculation> <)>
		<else> <if> <(> <scalar>|<identifier>|<calculation> <)>
		<else>
<loop> ::=
		<while> <(> <scalar>|<identifier>|<calculation> <)> <{><}>
		<while> <(> <scalar>|<identifier>|<calculation> <)> <{> <statement> <}>
		<while> <(> <scalar>|<identifier>|<calculation> <)> <statement>
