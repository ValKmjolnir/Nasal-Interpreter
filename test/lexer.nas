import("lib.nas");

var lexer=func(file)
{
    var (ptr,token)=(0,[]);
    var s=io.fin(file);
    var len=size(s);
    return
    {
        jmp_note:func()
        {
            while(ptr<len and chr(s[ptr])!='\n')
                ptr+=1;
            ptr+=1;
        },
        id_gen:func()
        {
            var tmp="";
            while(ptr<len)
            {
                var c=s[ptr];
                if(('a'[0]<=c and c<='z'[0])
                or ('A'[0]<=c and c<='Z'[0])
                or ('0'[0]<=c and c<='9'[0])
                or c=='_'[0])
                    tmp~=chr(c);
                else
                    break;
                ptr+=1;
            }
            append(token,tmp);
        },
        str_gen:func()
        {
            var str="";
            var mark=chr(s[ptr]);
            ptr+=1;
            while(ptr<len and chr(s[ptr])!=mark)
            {
                if(chr(s[ptr])=='\\')
                {
                    ptr+=1;
                    var c=chr(s[ptr]);
                    if   (c=='a' ) str~='\a';
                    elsif(c=='b' ) str~='\b';
                    elsif(c=='f' ) str~='\f';
                    elsif(c=='n' ) str~='\n';
                    elsif(c=='r' ) str~='\r';
                    elsif(c=='t' ) str~='\t';
                    elsif(c=='v' ) str~='\v';
                    elsif(c=='?' ) str~='\?';
                    elsif(c=='0' ) str~='\0';
                    elsif(c=='\\') str~='\\';
                    elsif(c=='\'') str~='\'';
                    elsif(c=='\"') str~='\"';
                    else           str~=c;
                }
                else
                    str~=chr(s[ptr]);
                ptr+=1;
            }
            if(ptr>=len)
                print("read eof when generating string.\n");
            ptr+=1;
            append(token,str);
        },
        num_gen:func()
        {
            var number=chr(s[ptr]);
            ptr+=1;
            if(ptr<len and chr(s[ptr])=='x')
            {
                ptr+=1;
                while(ptr<len and
                ('a'[0]<=s[ptr] and s[ptr]<='f'[0]
                or '0'[0]<=s[ptr] and s[ptr]<='9'[0]))
                {
                    number~=chr(s[ptr]);
                    ptr+=1;
                }
                append(token,num(number));
                return;
            }
            elsif(ptr<len and chr(s[ptr])=='o')
            {
                ptr+=1;
                while(ptr<len and ('0'[0]<=s[ptr] and s[ptr]<='7'[0]))
                {
                    number~=chr(s[ptr]);
                    ptr+=1;
                }
                append(token,num(number));
                return;
            }
            while(ptr<len and ('0'[0]<=s[ptr] and s[ptr]<='9'[0]))
            {
                number~=chr(s[ptr]);
                ptr+=1;
            }
            if(ptr<len and chr(s[ptr])=='.')
            {
                number~=chr(s[ptr]);
                ptr+=1;
                while(ptr<len and ('0'[0]<=s[ptr] and s[ptr]<='9'[0]))
                {
                    number~=chr(s[ptr]);
                    ptr+=1;
                }
            }
            if(chr(s[ptr])=='e' or chr(s[ptr])=='E')
            {
                number~=chr(s[ptr]);
                ptr+=1;
                if(chr(s[ptr])=='-' or chr(s[ptr])=='+')
                {
                    number~=chr(s[ptr]);
                    ptr+=1;
                }
                while(ptr<len and ('0'[0]<=s[ptr] and s[ptr]<='9'[0]))
                {
                    number~=chr(s[ptr]);
                    ptr+=1;
                }
            }
            var last_c=chr(number[-1]);
            if(last_c=='.' or last_c=='e' or last_c=='E' or last_c=='-' or last_c=='+')
                println("error number: ",number);
            append(token,num(number));
        },
        opr_gen:func()
        {
            var c=chr(s[ptr]);
            if(c=='+' or c=='-' or c=='~' or c=='/' or c=='*' or c=='>' or c=='<' or c=='!' or c=='=')
            {
                var tmp=c;
                ptr+=1;
                if(ptr<len and chr(s[ptr])=='=')
                {
                    tmp~=chr(s[ptr]);
                    ptr+=1;
                }
                append(token,tmp);
                return;
            }
            elsif(c=='.')
            {
                if(ptr+2<len and chr(s[ptr+1])=='.' and chr(s[ptr+2])=='.')
                {
                    append(token,"...");
                    ptr+=3;
                }
                else
                {
                    append(token,".");
                    ptr+=1;
                }
                return;
            }
            elsif(c!=' ' and c!='\t' and c!='\n' and c!='\r' and s[ptr]>0)
                append(token,c);
            ptr+=1;
            return;
        },
        compile:func()
        {
            while(ptr<len)
            {
                var c=s[ptr];
                if(c=='#'[0])
                    me.jmp_note();
                elsif('a'[0]<=c and c<='z'[0]
                    or 'A'[0]<=c and c<='Z'[0]
                    or c=='_'[0])
                    me.id_gen();
                elsif(c=='\''[0] or c=='\"'[0])
                    me.str_gen();
                elsif('0'[0]<=c and c<='9'[0])
                    me.num_gen();
                else
                    me.opr_gen();
            }
            return;
        },
        get_token:func(){return token;}
    };
}

var lex=lexer("test/props.nas");
lex.compile();
foreach(var tok;lex.get_token())
    print(tok,' ');
print('\n');