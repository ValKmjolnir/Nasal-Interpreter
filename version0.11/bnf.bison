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
    |call.id
    |call.call
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
    |var id;
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
