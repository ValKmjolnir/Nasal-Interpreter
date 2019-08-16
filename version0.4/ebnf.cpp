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
<5><definition> ::=
	<var> <identifier> <=> <identifier> <;>
	<var> <identifier> <=> <call_function> <;>
	<var> <identifier> <=> <[><]> <;>
	<var> <identifier> <=> <{><}> <;>
	<var> <identifier> <=> <[> <scalar>|<scalars>|<identifier>|<identifiers> <]> <;>
	<var> <identifier> <=> <{> <hash_members> <}> <;>
<6><assignment> ::=
	<identifier> <=> <identifier> <;>
<7><def_func> ::=
	<var> <identifier> <=> <func> <(><)> <{><}>
	<var> <identifier> <=> <func> <(> <identifier>|<identifiers> <)> <{><}>
	<var> <identifier> <=> <func> <(><)> <{> <statements> <}>
	<var> <identifier> <=> <func> <(> <identifier>|<identifiers> <)> <{> <statements> <}>
<8><statement> ::=
	<definition>
	<assignment>
	<def_func>
	<identifier> <;>
<9><statements> ::=
	<statement>|<statements> <statement>
