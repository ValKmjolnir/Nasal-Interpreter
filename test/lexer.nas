import("lib.nas");

var s=io.fin("a.nas");
s=split('',s);
var len=size(s);
var ptr=0;

var jump_note=func()
{
    while(ptr<len and s[ptr]!='\n')
        ptr+=1;
    ptr+=1;
    return;
}

var generate_id=func()
{
    var tmp="";
    while(ptr<len)
    {
        if('a'<=s[ptr] and s[ptr]<='z'
        or 'A'<=s[ptr] and s[ptr]<='Z'
        or s[ptr]=='_'
        or '0'<=s[ptr] and s[ptr]<='9')
            tmp~=s[ptr];
        else
            break;
        ptr+=1;
    }
    return tmp;
}

var generate_str=func()
{
    var tok_str="";
    var mark=s[ptr];
    ptr+=1;
    while(ptr<len and s[ptr]!=mark)
    {
        if(s[ptr]=='\\')
        {
            ptr+=1;
            if(s[ptr]=='a') tok_str~='\a';
            elsif(s[ptr]=='b') tok_str~='\b';
            elsif(s[ptr]=='f') tok_str~='\f';
            elsif(s[ptr]=='n') tok_str~='\n';
            elsif(s[ptr]=='r') tok_str~='\r';
            elsif(s[ptr]=='t') tok_str~='\t';
            elsif(s[ptr]=='v') tok_str~='\v';
            elsif(s[ptr]=='?') tok_str~='?';
            elsif(s[ptr]=='0') tok_str~='\0';
            elsif(s[ptr]=='\\') tok_str~='\\';
            elsif(s[ptr]=='\'') tok_str~='\'';
            elsif(s[ptr]=='\"') tok_str~='\"';
            else tok_str~=s[ptr];
        }
        else
            tok_str~=s[ptr];
        ptr+=1;
    }
    if(ptr>=len)
        print("read eof when generating string.");
    ptr+=1;
    return tok_str;
}

var generate_number=func()
{
    var number=s[ptr];
    ptr+=1;
    if(s[ptr]=='x')
    {
        ptr+=1;
        while(ptr<len and ('a'<=s[ptr] and s[ptr]<='f' or '0'<=s[ptr] and s[ptr]<='9'))
        {
            number~=s[ptr];
            ptr+=1;
        }
        return num(number);
    }
    elsif(s[ptr]=='o')
    {
        ptr+=1;
        while(ptr<len and ('0'<=s[ptr] and s[ptr]<='7'))
        {
            number~=s[ptr];
            ptr+=1;
        }
        return num(number);
    }

    while(ptr<len and ('0'<=s[ptr] and s[ptr]<='9'))
    {
        number~=s[ptr];
        ptr+=1;
    }
    if(s[ptr]=='.')
        number~=s[ptr];
    else
        return num(number);
    ptr+=1;
    while(ptr<len and ('0'<=s[ptr] and s[ptr]<='9'))
    {
        number~=s[ptr];
        ptr+=1;
    }
    if(s[ptr]=='e' or s[ptr]=='E')
        number~=s[ptr];
    else
        return num(number);
    ptr+=1;
    if(s[ptr]=='-' or s[ptr]=='+')
    {
        number~=s[ptr];
        ptr+=1;
    }
    while(ptr<len and ('0'<=s[ptr] and s[ptr]<='9'))
    {
        number~=s[ptr];
        ptr+=1;
    }
    return num(number);
}

var generate_operator=func()
{
    var tmp="";
    if(s[ptr]=='+' or s[ptr]=='-' or s[ptr]=='~' or s[ptr]=='/' or s[ptr]=='*' or s[ptr]=='>' or s[ptr]=='<' or s[ptr]=='!' or s[ptr]=='=')
    {
        tmp=s[ptr];
        ptr+=1;
        if(ptr<len and s[ptr]=='=')
        {
            tmp~=s[ptr];
            ptr+=1;
        }
        return tmp;
    }
    elsif(s[ptr]=='.')
    {
        if(ptr+2<len and s[ptr+1]=='.' and s[ptr+2]=='.')
        {
            tmp='...';
            ptr+=3;
        }
        else
        {
            tmp='.';
            ptr+=1;
        }
        return tmp;
    }
    elsif(s[ptr]!=' ' and s[ptr]!='\t' and s[ptr]!='\n' and s[ptr]!='\r' and s[ptr][0]>0)
        tmp=s[ptr];
    ptr+=1;
    return tmp;
}
var cnt=0;
var token=[];
while(ptr<len)
{
    if(s[ptr]=='#')
        jump_note();
    elsif('a'<=s[ptr] and s[ptr]<='z' or 'A'<=s[ptr] and s[ptr]<='Z' or s[ptr]=='_')
        append(token,generate_id());
    elsif(s[ptr]=='\'' or s[ptr]=='\"')
        append(token,generate_str());
    elsif('0'<=s[ptr] and s[ptr]<='9')
        append(token,generate_number());
    else
    {
        var tmp=generate_operator();
        if(size(tmp))
            append(token,tmp);
    }
    if(ptr>=len)
        break;
}
foreach(var i;token)
{
    print("(",cnt," | ",i,")");
    cnt+=1;
}