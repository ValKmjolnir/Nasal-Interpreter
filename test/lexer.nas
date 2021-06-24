import("lib.nas");

var lexer=func(file)
{
    var _={s:io.fin(file),len:0,ptr:0,token:[]};
    _.len=size(_.s);
    return
    {
        jmp_note:func()
        {
            while(_.ptr<_.len and chr(_.s[_.ptr])!='\n')
                _.ptr+=1;
            _.ptr+=1;
        },
        id_gen:func()
        {
            var tmp="";
            while(_.ptr<_.len)
            {
                var c=_.s[_.ptr];
                if(('a'[0]<=c and c<='z'[0])
                or ('A'[0]<=c and c<='Z'[0])
                or ('0'[0]<=c and c<='9'[0])
                or c=='_'[0])
                    tmp~=chr(c);
                else
                    break;
                _.ptr+=1;
            }
            append(_.token,tmp);
        },
        str_gen:func()
        {
            var str="";
            var mark=chr(_.s[_.ptr]);
            _.ptr+=1;
            while(_.ptr<_.len and chr(_.s[_.ptr])!=mark)
            {
                if(chr(_.s[_.ptr])=='\\')
                {
                    _.ptr+=1;
                    var c=chr(_.s[_.ptr]);
                    if   (c=='a' ) str~='\a';
                    elsif(c=='b' ) str~='\b';
                    elsif(c=='f' ) str~='\f';
                    elsif(c=='n' ) str~='\n';
                    elsif(c=='r' ) str~='\r';
                    elsif(c=='t' ) str~='\t';
                    elsif(c=='v' ) str~='\v';
                    elsif(c=='?' ) str~='?';
                    elsif(c=='0' ) str~='\0';
                    elsif(c=='\\') str~='\\';
                    elsif(c=='\'') str~='\'';
                    elsif(c=='\"') str~='\"';
                    else           str~=c;
                }
                else
                    str~=chr(_.s[_.ptr]);
                _.ptr+=1;
            }
            if(_.ptr>=_.len)
                print("read eof when generating string.\n");
            _.ptr+=1;
            append(_.token,str);
        },
        num_gen:func()
        {
            var number=chr(_.s[_.ptr]);
            _.ptr+=1;
            if(_.ptr<_.len and chr(_.s[_.ptr])=='x')
            {
                _.ptr+=1;
                while(_.ptr<_.len and
                ('a'[0]<=_.s[_.ptr] and _.s[_.ptr]<='f'[0]
                or '0'[0]<=_.s[_.ptr] and _.s[_.ptr]<='9'[0]))
                {
                    number~=chr(_.s[_.ptr]);
                    _.ptr+=1;
                }
                append(_.token,num(number));
                return;
            }
            elsif(_.ptr<_.len and chr(_.s[_.ptr])=='o')
            {
                _.ptr+=1;
                while(_.ptr<_.len and ('0'[0]<=_.s[_.ptr] and _.s[_.ptr]<='7'[0]))
                {
                    number~=chr(_.s[_.ptr]);
                    _.ptr+=1;
                }
                append(_.token,num(number));
                return;
            }
            while(_.ptr<_.len and ('0'[0]<=_.s[_.ptr] and _.s[_.ptr]<='9'[0]))
            {
                number~=chr(_.s[_.ptr]);
                _.ptr+=1;
            }
            if(_.ptr<_.len and chr(_.s[_.ptr])=='.')
            {
                number~=chr(_.s[_.ptr]);
                _.ptr+=1;
                while(_.ptr<_.len and ('0'[0]<=_.s[_.ptr] and _.s[_.ptr]<='9'[0]))
                {
                    number~=chr(_.s[_.ptr]);
                    _.ptr+=1;
                }
            }
            if(chr(_.s[_.ptr])=='e' or chr(_.s[_.ptr])=='E')
            {
                number~=chr(_.s[_.ptr]);
                _.ptr+=1;
                if(chr(_.s[_.ptr])=='-' or chr(_.s[_.ptr])=='+')
                {
                    number~=chr(_.s[_.ptr]);
                    _.ptr+=1;
                }
                while(_.ptr<_.len and ('0'[0]<=_.s[_.ptr] and _.s[_.ptr]<='9'[0]))
                {
                    number~=chr(_.s[_.ptr]);
                    _.ptr+=1;
                }
            }
            var last_c=chr(number[-1]);
            if(last_c=='.' or last_c=='e' or last_c=='E' or last_c=='-' or last_c=='+')
                println("error number: ",number);
            append(_.token,num(number));
        },
        opr_gen:func()
        {
            var c=chr(_.s[_.ptr]);
            if(c=='+' or c=='-' or c=='~' or c=='/' or c=='*' or c=='>' or c=='<' or c=='!' or c=='=')
            {
                var tmp=c;
                _.ptr+=1;
                if(_.ptr<_.len and chr(_.s[_.ptr])=='=')
                {
                    tmp~=chr(_.s[_.ptr]);
                    _.ptr+=1;
                }
                append(_.token,tmp);
                return;
            }
            elsif(c=='.')
            {
                if(_.ptr+2<_.len and chr(_.s[_.ptr+1])=='.' and chr(_.s[_.ptr+2])=='.')
                {
                    append(_.token,"...");
                    _.ptr+=3;
                }
                else
                {
                    append(_.token,".");
                    _.ptr+=1;
                }
                return;
            }
            elsif(c!=' ' and c!='\t' and c!='\n' and c!='\r' and _.s[_.ptr]>0)
                append(_.token,c);
            _.ptr+=1;
            return;
        },
        main:func()
        {
            while(_.ptr<_.len)
            {
                var c=_.s[_.ptr];
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
        get_token:func(){return _.token;}
    };
}

var nasal_lexer=lexer("test/lexer.nas");
nasal_lexer.main();
foreach(var tok;nasal_lexer.get_token())
    print(tok,' ');
println();