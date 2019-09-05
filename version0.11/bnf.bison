%%
id
    :IDENTIFIER
    ;
scalar
    :number
    |string
    |char
    |calculation
    ;
call
    :call_list
    |call_hash
    |call_function
    ;
datas
    :id,id
    |id,scalar
    |id,call
    |id,function
    |id,list
    |id,hash
    |scalar,id
    |scalar,scalar
    |scalar,call
    |scalar,function
    |scalar,list
    |scalar,hash
    |call,id
    |call,scalar
    |call,call
    |call,function
    |call,list
    |call,hash
    |function,id
    |function,scalar
    |function,call
    |function,function
    |function,list
    |function,hash
    |list,id
    |list,scalar
    |list,call
    |list,function
    |list,list
    |list,hash
    |hash,id
    |hash,scalar
    |hash,call
    |hash,function
    |hash,list
    |hash,hash
    |list,datas
    |hash,datas
    |id,datas
    |scalar,datas
    |call,datas
    |function,datas
    ;
hash_member
    :id:id
    |id:scalar
    |id:call
    |id:function
    ;
hash_members
    :hash_member,hash_member
    |hash_member,hash_members
    ;
list
    :[]
    |[id]
    |[scalar]
    |[call]
    |[function]
    |[datas]
    ;
hash
    :{}
    |{hash_member}
    |{hash_members}
    ;
call_list
    :id list
    |call list
    |id[hash_member]
    |call[hash_member]
    |id[id:]
    |id[scalar:]
    |id[call:]
    |call[id:]
    |call[scalar:]
    |call[call:]
    ;
call_hash
    :id.id
    |id.call
    |id.scalar
    |call.id
    |call.call
    |call.scalar
    ;
call_function
    :id()
    |id(id)
    |id(scalar)
    |id(call)
    |id(list)
    |id(hash)
    |id(function)
    |id(datas)

calculation
    :id two_operator id
    |id two_operator scalar
    |id two_operator call
    |scalar two_operator id
    |scalar two_operator scalar
    |scalar two_operator call
    |call two_operator id
    |call two_operator scalar
    |call two_operator call
    |(id)
    |(scalar)
    |(call)
    |! id
    |! scalar
    |! call
    |__add0 -id
    |__add0 -scalar
    |__add0 -call
    |__add0 +id
    |__add0 +scalar
    |__add0 +call
    ;
two_operator
    :+
    |-
    |*
    |/
    |~
    |==
    |>
    |>=
    |<
    |<=
    |!=
    |and
    |or
    ;
assignment_operator
    :+=
    |-=
    |*=
    |/=
    |~=
    ;

function
	:func{}
	|func{statement}
	|func{statements}
	|func(){}
	|func(){statement}
	|func(){statements}
	|func(id){}
	|func(id){statement}
	|func(id){statements}
	|func(scalar){}
	|func(scalar){statement}
	|func(scalar){statements}
	|func(call){}
	|func(call){statement}
	|func(call){statements}
	|func(list){}
	|func(list){statement}
	|func(list){statements}
	|func(hash){}
	|func(hash){statement}
	|func(hash){statements}
	|func(function){}
	|func(function){statement}
	|func(function){statements}
	|func(dynamic_id){}
	|func(dynamic_id){statement}
	|func(dynamic_id){statements}
	|func(datas){}
	|func(datas){statement}
	|func(datas){statements}

choose
	:if(id){}
	|if(scalar){}
	|if(call){statement}
	|if(id){statement}
	|if(scalar){statement}
	|if(call){statement}
	|if(id){statements}
	|if(scalar){statements}
	|if(call){statements}
	|else if(id){}
	|else if(scalar){}
	|else if(call){statement}
	|else if(id){statement}
	|else if(scalar){statement}
	|else if(call){statement}
	|else if(id){statements}
	|else if(scalar){statements}
	|else if(call){statements}
	|elsif(id){}
	|elsif(scalar){}
	|elsif(call){statement}
	|elsif(id){statement}
	|elsif(scalar){statement}
	|elsif(call){statement}
	|elsif(id){statements}
	|elsif(scalar){statements}
	|elsif(call){statements}
	|else{}
	|else{statement}
	|else{statements}

loop
	:while(id){}
	|while(scalar){}
	|while(call){}
	|while(id){statement}
	|while(scalar){statement}
	|while(call){statement}
	|while(id){statements}
	|while(scalar){statements}
	|while(call){statements}
	|forindex|foreach(statement id){}
	|forindex|foreach(statement scalar){}
	|forindex|foreach(statement call){}
	|forindex|foreach(statement list){}
	|forindex|foreach(statement id){statement}
	|forindex|foreach(statement scalar){statement}
	|forindex|foreach(statement call){statement}
	|forindex|foreach(statement list){statement}
	|forindex|foreach(statement id){statements}
	|forindex|foreach(statement scalar){statements}
	|forindex|foreach(statement call){statements}
	|forindex|foreach(statement list){statements}
	|for(statement pre_assignment){}
	|for(statement pre_assignment){statement}
	|for(statement pre_assignment){statements}

definition
    :var id = id;
    |var id = scalar;
    |var id = call;
    |var id = list;
    |var id = hash;
    |var id = function;
    |var id;
pre_assignment
    :id = id
    |id = scalar
    |id = call
    |id = function
    |id = list
    |id = hash
    |call = id
    |call = scalar
    |call = call
    |call = function
    |call = list
    |call = hash
    |id assignment_operator id
    |id assignment_operator scalar
    |id assignment_operator call
    |call assignment_operator id
    |call assignment_operator scalar
    |call assignment_operator call
    ;
assignment
	:pre_assignment ;
	;
return_scalar
	:return id;
    |return scalar;
    |return call;
    |return function;
    |return list;
    |return hash;
    |return;
    ;
statement
    :continue;
    |break;
    |return_scalar
    |definition
    |assignment
    |loop
    |choose
    ;
