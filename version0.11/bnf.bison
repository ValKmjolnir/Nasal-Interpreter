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
    |scalar,id
    |scalar,scalar
    |scalar,call
    |call,id
    |call,scalar
    |call,call
    |id,datas
    |scalar,datas
    |call,datas
    ;
hash_member
    :id:id
    |id:scalar
    |id:call
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
    |call.id
    |call.call
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
    |(id)
    |(scalar)
    |(call)
    |! id
    |! scalar
    |! call
    |+-id
    |+-scalar
    |+-call
    ;
definition
    :var id = id;
    |var id = scalar;
    |var id = call;
    |var id = list;
    |var id = hash;
    |var id = function
assignment
    :id = id;
    |id = scalar;
    |id = call;
    |id = function
    |id = list;
    |id = hash;
    |call = id;
    |call = scalar;
    |call = call;
    |call = function
    |call = list;
    |call = hash;
    ;
statement
    :return id;
    |return scalar;
    |return call;
    |return function;
    |return list;
    |return hash;
    |continue;
    |break;
    |definition
    |assignment
    |loop
    |choose
    ;