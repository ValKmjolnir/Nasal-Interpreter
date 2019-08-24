

data type = <scalar> <identifier> <calculation> <list> <hash> <function>

<1><unknown_type_id> ::=
		IDENTIFIER
		<!= <scalar>|<identifier>|<calculation>> <+>|<-> <unknown_type_id>
		
<1><scalar> ::= NUMBER|STRING
		<!= <scalar>|<identifier>|<calculation>> <+>|<-> <scalar>
<1><scalars> ::=
		<scalar>|<scalars>|<list>|<hash>|<calculation>|<function> <,> <scalar>|<identifier>|<list>|<hash>|<calculation>|<function> <!=operator>
		<identifier>|<identifiers> <,> <scalar>|<list>|<hash>|<calculation>|<function> <!=operator>
<1><identifier> ::=
		IDENTIFIER <!= <[>|<.>|<(>>
		<unknown_type_id>|<identifier> <.> <identifier>
		<unknown_type_id> <[> <scalar>|<identifier>|<calcualtion> <]> <!= <[>|<.>|<(>>
		<unknown_type_id> <(> <scalar>|<scalars>|<identifier>|<identifiers>|<calculation>|<list>|<hash> <)> <!= <[>|<.>|<(>>
		<unknown_type_id> <(><)> <!= <[>|<.>|<(>>
		<unknown_type_id> <.> <.> <.>
<1><identifiers> ::=
		<identifier>|<identifiers> <,> <identifier>
<1><calculation> ::=
		<scalar>|<identifier>|<calculation> <+>|<->|<*>|</>|<~>|<<>|<<=>|<>>|<>=>|<==>|<!=>|<and>|<or> <scalar>|<identifier>|<calculation>
		<(> <scalar>|<identifier>|<calculation> <)> <+>|<->|<*>|</>|<~>|<<>|<<=>|<>>|<>=>|<==>|<!=>|<and>|<or> <scalar>|<identifier>|<calculation>
		<scalar>|<identifier>|<calculation> <+>|<->|<*>|</>|<~>|<<>|<<=>|<>>|<>=>|<==>|<!=>|<and>|<or> <(> <scalar>|<identifier>|<calculation> <)>
		<(> <scalar>|<identifier>|<calculation> <)> <+>|<->|<*>|</>|<~>|<<>|<<=>|<>>|<>=>|<==>|<!=>|<and>|<or> <(> <scalar>|<identifier>|<calculation> <)>
		<!= <scalar>|<identifier>|<calculation>> <+>|<-> <calculation>
<1><list> ::=
		<[><]>
		<!=<identifier>|<unknown_type_id>><[> <scalar>|<scalars>|<identifier>|<identifiers>|<list>|<hash>|<calculation> <]>
<1><hash> ::=
		<{><}>
		<{> <hashmember>|<hashmembers> <}>
<1><hashmember> ::=
		<identifier> <:> <scalar>|<identifier>|<list>|<hash>|<function>|<calculation>
<1><hashmembers> ::=
		<hashmember>|<hashmembers> <,> <hashmember>
<1><function> ::=
		<func> <{><}>
		<func> <{> <statement> <}>
		<func> <(><)> <{><}>
		<func> <(><)> <{> <statement> <}>
		<func> <(> <identifier>|<identifiers> <)> <{><}>
		<func> <(> <identifier>|<identifiers> <)> <{> <statement> <}>
<1><definition> ::=
		<var> <identifier> <=> <scalar>|<identifier>|<list>|<hash>|<calculation> <;>
		<var> <identifier> <=> <function>
<1><assigntment> ::=
		<!=var> <identifier> <=> <scalar>|<identifier>|<list>|<hash>|<calculation> <;>
<statement> ::=
		<!=<for> <(>> <definition>|<assignment>
		<return> <scalar>|<identifier>|<list>|<hash>|<calculation> <;>
		<return> <;>
		<!=return> <continue>|<break>|<identifier> <;>
		<choose>
		<loop>
<statements> ::=
		<statement>|<statements> <statement>|<;>
<1><choose>
		<if> <(> <scalar>|<identifier>|<calculation> <)>
		<elsif> <(> <scalar>|<identifier>|<calculation> <)>
		<else> <if> <(> <scalar>|<identifier>|<calculation> <)>
		<else>
<loop> ::=
		<while> <(> <scalar>|<identifier>|<calculation> <)> <{><}>
		<while> <(> <scalar>|<identifier>|<calculation> <)> <{> <statement> <}>
		<while> <(> <scalar>|<identifier>|<calculation> <)> <statement>
