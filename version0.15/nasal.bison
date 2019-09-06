%%

program
	:statement program
	;

scalar
	:number
	|string
	|calculation
	;
call
	:call_list
	|call_hash
	|call_function
	;
id_list
	:id ',' id
	|id ',' id_list
	;
scalar_list
	:scalar ',' scalar
	|scalar ',' call
	|scalar ',' function
	|scalar ',' id_list
	|scalar ',' list
	|scalar ',' hash
	|call ',' scalar
	|call ',' call
	|call ',' function
	|call ',' id_list
	|call ',' list
	|call ',' hash
	|function ',' scalar
	|function ',' call
	|function ',' function
	|function ',' id_list
	|function ',' list
	|function ',' hash
	|scalar ',' scalar_list
	|call ',' scalar_list
	|function ',' scalar_list
	|id ',' scalar_list
	|list ',' scalar_list
	|hash ',' scalar_list
	;

subvec
	:'['id':'']'
	|'['id':'id']'
	|'['id':'scalar']'
	|'['id':'call']'
	|'['scalar':'']'
	|'['scalar':'id']'
	|'['scalar':'scalar']'
	|'['scalar':'call']'
	|'['call':'']'
	|'['call':'id']'
	|'['call':'scalar']'
	|'['call':'call']'
	;
call_list
	:id'['id']'
	|id'['scalar']'
	|id'['call']'
	|id subvec
	|call'['id']'
	|call'['scalar']'
	|call'['call']'
	|call subvec
	;
call_hash
	:id '.' id
	|id '.' call
	|call '.' id
	|call '.' call
	;
call_function
	:id '('')'
	|id '(' id ')'
	|id '(' scalar ')'
	|id '(' dynamic_id ')'
	|id '(' call ')'
	|id '(' function ')'
	|id '(' id_list ')'
	|id '(' scalar_list ')'
	;
list
	:'['']'
	|'['id']'
	|'['scalar']'
	|'['call']'
	|'['list']'
	|'['hash']'
	|'['id_list']'
	|'['scalar_list']'
	;
hash_member
    :id':'id
    |id':'scalar
    |id':'call
    |id':'function
    |id':'list
    |id':'hash
    ;
hash_members
    :hash_member,hash_member
    |hash_member,hash_members
    ;
hash
	:'{''}'
	|'{'hash_member'}'
	|'{'hash_members'}'
	;
operator
	:'+'
	|'-'
	|'*'
	|'/'
	|'~'
	|'=='
	|'!='
	|'<'
	|'<='
	|'>'
	|'>='
	|and
	|or
	;

calculation
	:id operator id
	|id operator scalar
	|id operator call
	|scalar operator id
	|scalar operator scalar
	|scalar operator call
	|call operator id
	|call operator scalar
	|call operator call
	|'('id')'
	|'('scalar')'
	|'('call')'
	|'!' id
	|'!' scalar
	|'!' call
	|'-' id
	|'-' scalar
	|'-' call
	|'+' id
	|'+' scalar
	|'+' call
	;

definition
	:var id '=' id ';'
	|var id '=' scalar ';'
	|var id '=' call ';'
	|var id '=' function
	|var id '=' list ';'
	|var id '=' hash ';'
	;

assign_operator
	:'+='
	|'-='
	|'*='
	|'/='
	|'~='
	|'='
	;
pre_assignment
	:id assign_operator id
	|id assign_operator scalar
	|id assign_operator call
	|call assign_operator id
	|call assign_operator scalar
	|call assign_operator call
	;
assignment
	:pre_assignment ';'
	|id '=' function
	|call '=' function
	;

function
	:func'{''}'
	|func'{'statement'}'
	|func'('')''{''}'
	|func'('')''{'statement'}'
	|func'('id')''{''}'
	|func'('id')''{'statement'}'
	|func'('scalar')''{''}'
	|func'('scalar')''{'statement'}'
	|func'('call')''{''}'
	|func'('call')''{'statement'}'
	|func'('list')''{''}'
	|func'('list')''{'statement}'
	|func'('hash')''{''}'
	|func'('hash')''{'statement'}'
	|func'('function')''{''}'
	|func'('function')''{'statement'}'
	|func'('dynamic_id')''{''}'
	|func'('dynamic_id')''{'statement'}'
	|func'('id_list')''{''}'
	|func'('id_list')''{'statement'}'
	;

choose
	:if'('id')''{''}'
	|if'('scalar')''{''}'
	|if'('call')''{'statement'}'
	|if'('id')''{'statement'}'
	|if'('scalar')''{'statement'}'
	|if'('call')''{'statement'}'
	|else if'('id')''{''}'
	|else if'('scalar')''{''}'
	|else if'('call')''{'statement'}'
	|else if'('id')''{'statement'}'
	|else if'('scalar')''{'statement'}'
	|else if'('call')''{'statement'}'
	|elsif'('id')''{''}'
	|elsif'('scalar')''{''}'
	|elsif'('call')''{'statement'}'
	|elsif'('id')''{'statement'}'
	|elsif'('scalar')''{'statement'}'
	|elsif'('call')''{'statement'}'
	|else'{''}'
	|else'{'statement'}'
	;

loop
	:while'('id')''{''}'
	|while'('scalar')''{''}'
	|while'('call')''{''}'
	|while'('id')''{'statement'}'
	|while'('scalar')''{'statement'}'
	|while'('call')''{'statement'}'
	|foreach'('statement id')''{''}'
	|foreach'('statement scalar')''{''}'
	|foreach'('statement call')''{''}'
	|foreach'('statement list')''{''}'
	|foreach'('statement id')''{'statement'}'
	|foreach'('statement scalar')''{'statement'}'
	|foreach'('statement call')''{'statement'}'
	|foreach'('statement list')''{'statement'}'
	|forindex'('statement id')''{''}'
	|forindex'('statement scalar')''{''}'
	|forindex'('statement call')''{''}'
	|forindex'('statement list')''{''}'
	|forindex'('statement id')''{'statement'}'
	|forindex'('statement scalar')''{'statement'}'
	|forindex'('statement call')''{'statement'}'
	|forindex'('statement list')''{'statement'}'
	|for'('statement pre_assignment')''{''}'
	|for'('statement pre_assignment')''{'statement'}'
	;

statement
	:definition
	|assignment
	|loop
	|choose
	|return_scalar
	|continue ';'
	|break ';'
	|call_function';'
	|';'
	;
