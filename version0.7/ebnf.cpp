LR(1)
<scalar> ::=
		<number>|<string>
		<identifier> <=> <scalar>|<identifier>|<list>|<hash>
<scalars> ::=
		<scalar>|<list>|<hash>|<function>|<calculation> <,> <identifier>|<identifiers>|<scalar>|<scalars>|<list>|<hash>|<function>|<calcualtion>
<identifier> ::=
		<identifier>
		<identifier> <[> <scalar>|<identifier>|<calculation> <]>
		<identifier> <[> <scalar>|<identifier>|<calculation> <:> <scalar>|<identifier>|<calculation> <]>
		<identifier> <.> <identifier>
		<identifier> <(><)>
		<identifier> <(> <scalar>|<scalars>|<identifier>|<identifiers>|<list>|<hash>|<function>|<calculation> <)>
<identifiers> ::=
		<identifier> <,> <identifier>|<identifiers>
<list> ::=
		<[><]>
		<[> <scalars>|<identifiers>|<list>|<hash> <]>
<hash> ::=
		<{><}>
		<{> <hash_member>|<hash_members> <}>
<hash_member> ::=
		<identifier> <:> <scalar>|<identifier>|<list>|<hash>|<function>
<hash_members> ::=
		<hash_member> <,> <hash_member>|<hash_members>
<definition> ::=
		<var> <identifier> <=> <scalar>|<identifier>|<list>|<hash> <;>
		<var> <identifier> <=> <function>
		<var> <(> <identifiers> <)> <=> <(> <identifiers> <)> <;>
		<var> <(> <identifiers> <)> <=> <scalar>|<identifier>|<list>|<hash> <;>
		<(>	<var> <identifiers> <)> <=> <(> <identifiers> <)> <;>
		<(>	<var> <identifiers> <)> <=> <scalar>|<identifier>|<list>|<hash> <;>
<assignment> ::=
		<!var> <identifier> <operator> <scalar>|<identifier>|<list>|<hash> <;>
		<!var> <(> <identifiers> <)> <operator> <(> <identifiers> <)> <;>
		<!var> <(> <identifiers> <)> <operator> <scalar>|<identifier>|<list>|<hash> <;>
<function> ::=
		<func> <{><}>
		<func> <{> <statement> <}>
		<func> <(><)> <{><}>
		<func> <(><)> <{> <statement> <}>
		<func> <(> <identifier>|<identifiers> <)> <{><}>
		<func> <(> <identifier>|<identifiers> <)> <{> <statement> <}>
<calculation> ::=
		<scalar>|<identifier>|<calculation> <+>|<->|<*>|</>|<~>|<<>|<<=>|<>>|<>=>|<==>|<!=>|<and>|<or> <scalar>|<identifier>|<calculation>
		<(> <scalar>|<identifier>|<calculation> <)> <+>|<->|<*>|</>|<~>|<<>|<<=>|<>>|<>=>|<==>|<!=>|<and>|<or> <scalar>|<identifier>|<calculation>
		<scalar>|<identifier>|<calculation> <+>|<->|<*>|</>|<~>|<<>|<<=>|<>>|<>=>|<==>|<!=>|<and>|<or> <(> <scalar>|<identifier>|<calculation> <)>
		<(> <scalar>|<identifier>|<calculation> <)> <+>|<->|<*>|</>|<~>|<<>|<<=>|<>>|<>=>|<==>|<!=>|<and>|<or> <(> <scalar>|<identifier>|<calculation> <)>
<loop>
<choose>
<statement>
<statements>

