import("lib.nas");

var s=io.fin("lexer.nas");
s=split('',s);
var len=size(s);
var ptr=0;

var cmp=func(s1,s2)
{
    var size1=size(s1);
    var size2=size(s2);
    if(size1!=size2)
        return size1<size2;
    for(var i=0;i<size1;i+=1)
        if(s1[i]>s2[i])
            return 0;
    return 1;
}

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
        if('a'[0]<=s[ptr][0] and s[ptr][0]<='z'[0]
        or 'A'[0]<=s[ptr][0] and s[ptr][0]<='Z'[0]
        or s[ptr]=='_'
        or '0'[0]<=s[ptr][0] and s[ptr][0]<='9'[0])
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
    {
        print("read eof when generating string.");
        break;
    }
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
        while(ptr<len and ('a'[0]<=s[ptr][0] and s[ptr][0]<='f'[0] or '0'[0]<=s[ptr][0] and s[ptr][0]<='9'[0]))
        {
            number~=s[ptr];
            ptr+=1;
        }
        return num(number);
    }
    elsif(s[ptr]=='o')
    {
        ptr+=1;
        while(ptr<len and ('0'[0]<=s[ptr][0] and s[ptr][0]<='7'[0]))
        {
            number~=s[ptr];
            ptr+=1;
        }
        return num(number);
    }

    while(ptr<len and ('0'[0]<=s[ptr][0] and s[ptr][0]<='9'[0]))
    {
        number~=s[ptr];
        ptr+=1;
    }
    if(s[ptr]=='.')
        number~=s[ptr];
    else
        return num(number);
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
while(ptr<len)
{
    if(s[ptr]=='#')
        jump_note();
    elsif('a'[0]<=s[ptr][0] and s[ptr][0]<='z'[0] or 'A'[0]<=s[ptr][0] and s[ptr][0]<='Z'[0] or s[ptr]=='_')
        print("(",cnt+=1," | ",generate_id(),")");
    elsif(s[ptr]=='\'' or s[ptr]=='\"')
        print("(",cnt+=1," | \"",generate_str(),"\")");
    elsif('0'[0]<=s[ptr][0] and s[ptr][0]<='9'[0])
        print("(",cnt+=1," | ",generate_number(),")");
    else
    {
        var tmp=generate_operator();
        if(size(tmp))
            print("(",cnt+=1," | ",tmp,")");
    }
    if(ptr>=len)
        break;
}