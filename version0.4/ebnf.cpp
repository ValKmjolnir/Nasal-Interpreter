//
//definition:
//var e=1;
//var e=e;
//var e=e[0];
//var e=e[i];
//var e=e.e;
//var e=e[func()];
//var e=func();
//	identifier
//	scalar
//	call_func -> identifier    func_name()
//	array_search -> identifier e[0] e[e]
//	hash_search -> identifier  e.e

<1><scalar> ::=
	<number> //NUMBER
	<string> //STRING
	<(> <!> <identifier>|<scalar> <)>//first
	<!> <identifier>|<scalar>//then
	<(> <identifier>|<scalar> <+>|<->|<*>|</>|<~>|<and>|<or>|<==>|<!=>|<<>|<<=>|<>>|<>=> <identifier>|<scalar> <)> //first
	<identifier>|<scalar> <+>|<->|<*>|</>|<~>|<and>|<or>|<==>|<!=>|<<>|<<=>|<>>|<>=> <identifier>|<scalar> //then
<2><scalars> ::=
	<scalar>|<scalars>         <,> <scalar>|<identifier>
	<identifier>|<identifiers> <,> <scalar>
<3><identifier> ::=
	<identifier> // IDENTIFIER
	<identifier> <[> <identifier>|<scalar> <]> //__array_search
	<identifier> <.> <identifier> //__hash_search
	<identifier> <(> <identifier>|<identifiers>|<scalar>|<scalars> <)> //__call_func
	<identifier> <(><)> //__call_func_null
<4><identifiers> ::=
	<identifier>|<identifiers>  <,> <identifier>
<5><hash_member> ::=
	<identifier> <:> <identifier>|<scalar>|<__func>
	<identifier> <:> <func> <(><)> <{><}>
	<identifier> <:> <func> <(> <identifier>|<identifiers> <)> <{><}>
	<identifier> <:> <func> <(><)> <{> <statement>|<statements> <}>
	<identifier> <:> <func> <(> <identifier>|<identifiers> <)> <{> <statement>|<statements> <}>
<6><hash_members> ::=
	<hash_member>|<hash_members> <,> <hash_member>
<7><definition> ::=
	<var> <identifier> <=> <identifier>|<scalar> <;>
	<var> <identifier> <=> <[><]> <;>
	<var> <identifier> <=> <{><}> <;>
	<var> <identifier> <=> <[> <scalar>|<scalars>|<identifier>|<identifiers> <]> <;>
	<var> <identifier> <=> <{> <hash_member>|<hash_members> <}> <;>
<8><assignment> ::=
	<identifier> <=> <identifier>|<scalar> <;>
	<identifier> <=> <[><]> <;>
	<identifier> <=> <{><}> <;>
	<identifier> <=> <[> <scalar>|<scalars>|<identifier>|<identifiers> <]> <;>
	<identifier> <=> <{> <hash_member>|<hash_members> <}> <;>
<9><__func> ::=
	<var> <identifier> <=> <func> <(><)> <{><}>
	<var> <identifier> <=> <func> <(> <identifier>|<identifiers> <)> <{><}>
	<var> <identifier> <=> <func> <(><)> <{> <statement>|<statements> <}>
	<var> <identifier> <=> <func> <(> <identifier>|<identifiers> <)> <{> <statement>|<statements> <}>
<10><loop> ::=
	<...>
<11><choose> ::=
	<...>
<12><statement> ::=
	<definition>|<assignment>|<__func>|<loop>|<choose>
	<identifier>|<continue>|<break> <;>
	<return> <identifier>|<scalar> <;>
<13><statements> ::=
	<statement>|<statements> <statement>
