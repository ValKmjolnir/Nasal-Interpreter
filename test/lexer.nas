import("lib.nas");

var s=io.fin(input());
var len=size(s);
var ptr=0;
var cnt=0;
var token=[];

var jump_note=func()
{
    while(ptr<len and chr(s[ptr])!='\n')
        ptr+=1;
    ptr+=1;
    return;
}
var generate_id=func()
{
    var tmp="";
    while(ptr<len)
    {
        if('a'<=chr(s[ptr]) and chr(s[ptr])<='z'
        or 'A'<=chr(s[ptr]) and chr(s[ptr])<='Z'
        or chr(s[ptr])=='_'
        or '0'<=chr(s[ptr]) and chr(s[ptr])<='9')
            tmp~=chr(s[ptr]);
        else
            break;
        ptr+=1;
    }
    return tmp;
}
var generate_str=func()
{
    var tok_str="";
    var mark=chr(s[ptr]);
    ptr+=1;
    while(ptr<len and chr(s[ptr])!=mark)
    {
        if(chr(s[ptr])=='\\')
        {
            ptr+=1;
            if(chr(s[ptr])=='a') tok_str~='\a';
            elsif(chr(s[ptr])=='b') tok_str~='\b';
            elsif(chr(s[ptr])=='f') tok_str~='\f';
            elsif(chr(s[ptr])=='n') tok_str~='\n';
            elsif(chr(s[ptr])=='r') tok_str~='\r';
            elsif(chr(s[ptr])=='t') tok_str~='\t';
            elsif(chr(s[ptr])=='v') tok_str~='\v';
            elsif(chr(s[ptr])=='?') tok_str~='?';
            elsif(chr(s[ptr])=='0') tok_str~='\0';
            elsif(chr(s[ptr])=='\\') tok_str~='\\';
            elsif(chr(s[ptr])=='\'') tok_str~='\'';
            elsif(chr(s[ptr])=='\"') tok_str~='\"';
            else tok_str~=chr(s[ptr]);
        }
        else
            tok_str~=chr(s[ptr]);
        ptr+=1;
    }
    if(ptr>=len)
        print("read eof when generating string.\n");
    ptr+=1;
    return tok_str;
}
var generate_number=func()
{
    var number=chr(s[ptr]);
    ptr+=1;
    if(chr(s[ptr])=='x')
    {
        ptr+=1;
        while(ptr<len and ('a'<=chr(s[ptr]) and chr(s[ptr])<='f' or '0'<=chr(s[ptr]) and chr(s[ptr])<='9'))
        {
            number~=chr(s[ptr]);
            ptr+=1;
        }
        return num(number);
    }
    elsif(chr(s[ptr])=='o')
    {
        ptr+=1;
        while(ptr<len and ('0'<=chr(s[ptr]) and chr(s[ptr])<='7'))
        {
            number~=chr(s[ptr]);
            ptr+=1;
        }
        return num(number);
    }
    while(ptr<len and ('0'<=chr(s[ptr]) and chr(s[ptr])<='9'))
    {
        number~=chr(s[ptr]);
        ptr+=1;
    }
    if(chr(s[ptr])=='.')
        number~=chr(s[ptr]);
    else
        return num(number);
    ptr+=1;
    while(ptr<len and ('0'<=chr(s[ptr]) and chr(s[ptr])<='9'))
    {
        number~=chr(s[ptr]);
        ptr+=1;
    }
    if(chr(s[ptr])=='e' or chr(s[ptr])=='E')
        number~=chr(s[ptr]);
    else
        return num(number);
    ptr+=1;
    if(chr(s[ptr])=='-' or chr(s[ptr])=='+')
    {
        number~=chr(s[ptr]);
        ptr+=1;
    }
    while(ptr<len and ('0'<=chr(s[ptr]) and chr(s[ptr])<='9'))
    {
        number~=chr(s[ptr]);
        ptr+=1;
    }
    return num(number);
}
var generate_operator=func()
{
    var tmp="";
    if(chr(s[ptr])=='+' or chr(s[ptr])=='-' or chr(s[ptr])=='~' or chr(s[ptr])=='/' or chr(s[ptr])=='*' or chr(s[ptr])=='>' or chr(s[ptr])=='<' or chr(s[ptr])=='!' or chr(s[ptr])=='=')
    {
        tmp=chr(s[ptr]);
        ptr+=1;
        if(ptr<len and chr(s[ptr])=='=')
        {
            tmp~=chr(s[ptr]);
            ptr+=1;
        }
        return tmp;
    }
    elsif(chr(s[ptr])=='.')
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
    elsif(chr(s[ptr])!=' ' and chr(s[ptr])!='\t' and chr(s[ptr])!='\n' and chr(s[ptr])!='\r' and chr(s[ptr])[0]>0)
        tmp=chr(s[ptr]);
    ptr+=1;
    return tmp;
}

while(ptr<len)
{
    if(chr(s[ptr])=='#')
        jump_note();
    elsif('a'<=chr(s[ptr]) and chr(s[ptr])<='z' or 'A'<=chr(s[ptr]) and chr(s[ptr])<='Z' or chr(s[ptr])=='_')
        append(token,generate_id());
    elsif(chr(s[ptr])=='\'' or chr(s[ptr])=='\"')
        append(token,generate_str());
    elsif('0'<=chr(s[ptr]) and chr(s[ptr])<='9')
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
    print("(",cnt," | ",i,")\n");
    cnt+=1;
}
