/*
<>术语符号
[]最多出现一次
{}可以出现任意次，包括0次
()分组
|并列选项，只能选一个
*/

<long long int> ::= <long long int>
<double> ::= <double>
<char> ::= <char>
<string> ::= <string>
<array> ::= <array>
<hash> ::= <hash>
<key> ::= <identifier>
<number> ::= <long long int>|<double>
<class_function> ::= <identifier> [<(> { <,> <identifier> } <)>] <{> { <statement> <;> } <}>
<initial_value> ::= <number>|<char>|<string>
<array_initial_value> ::= <[> { <,> <number>|<char>|<string>|<array>|<hash> } <]>
<hash_initial_value> ::= <{> { <string_as_name> <:> <number>|<char>|<string>|<array>|<hash> } <}>
<class_initial_value> ::= <{> { <string_as_name> <:> <number>|<char>|<string>|<array>|<hash>|<class_function> } <}>

<definition_var> ::= <var> <identifier> <=> <initial_value>|<array_initial_value>|<hash_initial_value> <;>
<definition_class> ::= <var> <identifier> <=> <class_initial_value> <;>

<change_var> ::= <identifier> <=> <initial_value>|<array_initial_value>|<hash_initial_value> <;>
<not_global_var> ::= <identifier> <=> <initial_value>|<array_initial_value>|<hash_initial_value> <;>
<change_array> ::= <array> <[> <long long int> <]> <=> <initial_value>|<array_initial_value>|<hash_initial_value> <;>
<change_hash> ::= <hash> <[> <key> <]> <=> <initial_value>|<array_initial_value>|<hash_initial_value> <;>

<append> ::= <(><array> { <,> <number>|<char>|<string>|<array>|<hash> } <)> <;>
<setsize> ::= <(> <array> <,> <long long int> <)> <;>
<subvec> ::= <(> <array> <,> <long long int> <,> <long long int> <)> <;>
<pop> ::= <(> <array> <)> <;>
<sort> ::= <(> <array> <,> <func> <(> <identifier> <,> <identifier> <)> (<identifier> <-> <identifier>)|(<cmp> <(> <identifier> <,> <identifier> <)>) <)> <;>
<contains> ::= <(> <hash> <,> <key> <)> <;>
<delete> ::= <(> <hash> <,> <key> <)> <;>
<keys> ::= <(> <hash> <)> <;>

<choose> ::= [ <if> ] <(> <condition> <)> (<statement> <;>)|(<{> { <statement> <;> } <}>)
			 { (<else> <if>)|<elsif> }|[ <else> ] <(> <condition> <)> (<statement> <;>)|(<{> { <statement> <;> } <}>)
<loop> ::= <for> <(> <var> <identifier> <=> <number> <;> <condition> <;> <identifier> <=> <identifier> <+> <number> <)>
<{>
	{ <statement> <;> }
<}>
<loop> ::= <while> <(> <condition> <)>
<{>
	{ <statement> <;> }
<}>
<loop> ::= <foreach>|<forindex> <(> <identifier> <,> <array> <)>
<{>
	{ <statement> <;> }
<}>

<function_definition> ::= <var> <identifier> <func> [<(> { <identifier> } <)>]
<{>
	{ <statement> <;> }
<}>
<function_use> ::= <identifier> <(> { ({<,> <number>|<char>|<string>|<array>|<hash>})|({<,> <identifier> <:> <number>|<char>|<string>|<array>|<hash>}) } <)>
<function_reload> ::= <var> <identifier> <func> [<(> { <identifier> } <)>]
<{>
	{ <if> <(> <typeof> <(> <identifier> <)> <==> <typename_string> <)> ([<{> {<statement> <;>} <}>])|(<statement> <;>) }
	<die> <(> <string> <)> <;>
<}>
<function_dynamic_spawn> ::= <var> <identifier> <func> [<(> { <identifier> } <)>]
<{>
	{ <statement> <;> }
	<return> <func> [<(> {<,> <identifier>} <)>]
	<{>
		{ <statement> <;> }
	<}>
<}>

<array_print> ::= <array> <[> <number> <]>|[ <.> <key>|<array_print> ]
<hash_print> ::= <hash> <.> <key>|<array_print>
<print> ::= <(> { <,> <identifier>|<string>|<array_print>|<hash_print> } <)> <;>

<error_occur> ::= <die> <(> <string> <)> <;>

<multiple_declare> ::= <(> <var> { <,> <identifier> [<[> <number>|<string> <]>] } <)> <=> <(> { <number>|<char>|<string>|<array>|<hash> } <)> <;>
<multiple_declare> ::= <var> <(> { <,> <identifier> [<[> <number>|<string> <]>] } <)> <=> <(> { <number>|<char>|<string>|<array>|<hash> } <)> <;>

<swap_multiple_var> ::= <(> { <,> <identifier> [<[> <number>|<string> <]>] } <)> <=> <(> { <,> <identifier> [<[> <number>|<string> <]>] } <)> <;>
<var_from_array> ::= <var> <(> { <,> <identifier> [<[> <number>|<string> <]>] } <)> <=> <array> <;>
<sub_array> ::= <var> <identifier> <=> <identifier> <[> { <,> <number> }|(<number> <:> [ <number> ])|<number> <]> <;>


<language type> nasal--
<long long int> ::= <long long int>
<double> ::= <double>
<string> ::= <string>
<array> ::= <array>
<hash> ::= <hash>
<number> ::= <long long int>|<double>
<use_void_function> ::= <identifier> <(> {<identifier>} <)> <;>
<use_return_function> ::= <identifier> <(> {<identifier>} <)>
<definition> ::= <var> <identifier> <=> <number>|<string>|<array>|<hash>|<identifier>|<use_return_function> <;>
<assignment> ::= <identifier> <=> <number>|<string>|<array>|<hash>|<identifier>|<use_function> <;>
<function> ::= <var> <identifier> <=> <func> <(> {<identifier>} <)> <{> {<statement> <;>} <}>
<loop> ::= <for> <(> <var> <identifier> <=> <number> <;> <condition> <;> <identifier> <=> <identifier> <+> <number> <)>
<{>
	{ <statement> <;> }
<}>
<loop> ::= <while> <(> <condition> <)>
<{>
	{ <statement> <;> }
<}>
<choose> ::= [ <if> ] <(> <condition> <)> (<statement> <;>)|(<{> { <statement> <;> } <}>)
			 { (<else> <if>)|<elsif> }|[ <else> ] <(> <condition> <)> (<statement> <;>)|(<{> { <statement> <;> } <}>)

