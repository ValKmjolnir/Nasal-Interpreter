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
<2><scalars> ::=
	<scalar>|<scalars>         <,> <scalar>|<identifier>
	<identifier>|<identifiers> <,> <scalar>
<3><identifier> ::=
	<identifier> // IDENTIFIER
	<identifier> <[> <identifier>|<scalar> <]> //__array_search
	<identifier> <.> <identifier> //__hash_search
	<identifier> <(> <identifier>|<identifiers>|<scalar>|<scalars> <)> //__call_func
<4><identifiers> ::=
	<identifier>  <,> <identifier>
	<identifiers> <,> <identifier>
<5><hash_member> ::=
	<identifier> <:> <identifier>|<scalar>
<6><hash_members> ::=
	<hash_member>|<hash_members> <,> <hash_member>
<7><definition> ::=
	<var> <identifier> <=> <identifier> <;>
	<var> <identifier> <=> <[><]> <;>
	<var> <identifier> <=> <{><}> <;>
	<var> <identifier> <=> <[> <scalar>|<scalars>|<identifier>|<identifiers> <]> <;>
	<var> <identifier> <=> <{> <hash_members> <}> <;>
<8><assignment> ::=
	<identifier> <=> <identifier> <;>
<9><def_func> ::=
	<var> <identifier> <=> <func> <(><)> <{><}>
	<var> <identifier> <=> <func> <(> <identifier>|<identifiers> <)> <{><}>
	<var> <identifier> <=> <func> <(><)> <{> <statements> <}>
	<var> <identifier> <=> <func> <(> <identifier>|<identifiers> <)> <{> <statements> <}>
<10><loop> ::=
	<...>
<11><choose> ::=
	<...>
<12><statement> ::=
	<definition>
	<assignment>
	<def_func>
	<identifier> <;>
	<return> <identifier>|<scalar> <;>
	<continue> <;>
	<break> <;>
	<loop>
	<choose>
<13><statements> ::=
	<statement>|<statements> <statement>
