#lib json.nas
import("lib.nas");
var json={
    text:'',
    line:1,
    size:0,
    ptr:0,
    get:nil,
    check:nil,
    next:nil,
    match:nil,
    hash_gen:nil,
    vec_gen:nil,
    member:nil,
    parse:nil,
    token:{content:'',type:''},
    content:[],
};

json.get=func(filename)
{
    me.line=1;
    me.ptr=0;
    me.content=[];
    me.token={content:'',type:''};
    me.text=io.fin(filename);
    if(!size(me.text))
        die("cannot open "~filename);
    me.text=split('',me.text);
    me.size=size(me.text);
    return;
}

json.check=func(ptr)
{
    var str=me.text[ptr];
    return (str=='{' or str=='}' or str=='[' or str==']' or str==',' or str==':' or str=='\"' or ('0'<=str and str<='9'));
}

json.next=func()
{
    while(me.ptr<me.size and !json.check(me.ptr))
    {
        if(me.text[me.ptr]=='\n')
            me.line+=1;
        me.ptr+=1;
    }
    if(me.ptr>=me.size)
    {
        me.token.content="";
        me.token.type="eof";
        return;
    }
    
    if(me.text[me.ptr]=='{')
    {
        me.token.content='{';
        me.token.type="left brace";
    }
    elsif(me.text[me.ptr]=='}')
    {
        me.token.content='}';
        me.token.type="right brace";
    }
    elsif(me.text[me.ptr]=='[')
    {
        me.token.content='[';
        me.token.type="left bracket";
    }
    elsif(me.text[me.ptr]==']')
    {
        me.token.content=']';
        me.token.type="right bracket";
    }
    elsif(me.text[me.ptr]==',')
    {
        me.token.content=',';
        me.token.type="comma";
    }
    elsif(me.text[me.ptr]==':')
    {
        me.token.content=':';
        me.token.type="colon";
    }
    elsif(me.text[me.ptr]=='\"')
    {
        var s="";
        me.ptr+=1;
        while(me.ptr<me.size and me.text[me.ptr]!='\"')
        {
            s~=me.text[me.ptr];
            me.ptr+=1;
        }
        me.token.content=s;
        me.token.type="string";
    }
    elsif('0'<=me.text[me.ptr] and me.text[me.ptr]<='9')
    {
        var s=me.text[me.ptr];
        me.ptr+=1;
        while(me.ptr<me.size and (('0'<=me.text[me.ptr] and me.text[me.ptr]<='9') or me.text[me.ptr]=='.'))
        {
            s~=me.text[me.ptr];
            me.ptr+=1;
        }
        me.ptr-=1;
        me.token.content=num(s);
        me.token.type="number";
    }
    me.ptr+=1;
    return;
}

json.match=func(type)
{
    if(me.token.type!=type)
        print("line ",me.line,": expect ",type," but get ",me.token.content,".");
    me.next();
    return;
}

json.hash_gen=func()
{
    var hash={};
    me.match("left brace");
    me.member(hash);
    while(me.token.type=="comma")
    {
        me.match("comma");
        me.member(hash);
    }
    me.match("right brace");
    return hash;
}

json.vec_gen=func()
{
    var vec=[];
    me.match("left bracket");
    if(me.token.type=="left brace")
        append(vec,me.hash_gen());
    elsif(me.token.type=="left bracket")
        append(vec,me.vec_gen());
    elsif(me.token.type=="string" or me.token.type=="number")
    {
        append(vec,me.token.content);
        me.next();
    }
    while(me.token.type=="comma")
    {
        me.match("comma");
        if(me.token.type=="left brace")
            append(vec,me.hash_gen());
        elsif(me.token.type=="left bracket")
            append(vec,me.vec_gen());
        elsif(me.token.type=="string" or me.token.type=="number")
        {
            append(vec,me.token.content);
            me.next();
        }
    }
    me.match("right bracket");
    return vec;
}

json.member=func(hash)
{
    var name=me.token.content;
    me.match("string");
    me.match("colon");
    if(me.token.type=="left brace")
        hash[name]=me.hash_gen();
    elsif(me.token.type=="left bracket")
        hash[name]=me.vec_gen();
    elsif(me.token.type=="string" or me.token.type=="number")
    {
        hash[name]=me.token.content;
        me.next();
    }
    return;
}

json.parse=func()
{
    me.content={};
    me.next();
    me.match("left brace");
    me.member(me.content);
    while(me.token.type=="comma")
    {
        me.match("comma");
        me.member(me.content);
    }
    me.match("right brace");
    return;
}