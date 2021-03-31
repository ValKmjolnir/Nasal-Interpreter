import("lib.nas");

var tok_type=
{
    tok_null:0,
    tok_num:1,
    tok_str:2,
    tok_id:3,
    tok_for:4,
    tok_forindex:5,
    tok_foreach:6,
    tok_while:7,
    tok_var:8,
    tok_func:9,
    tok_break:10,
    tok_continue:11,
    tok_ret:12,
    tok_if:13,
    tok_elsif:14,
    tok_else:15,
    tok_nil:16,
    tok_lcurve:17,
    tok_rcurve:18,
    tok_lbracket:19,
    tok_rbracket:20,
    tok_lbrace:21,
    tok_rbrace:22,
    tok_semi:23,
    tok_and:24,
    tok_or:25,
    tok_comma:26,
    tok_dot:27,
    tok_ellipsis:28,
    tok_quesmark:29,
    tok_colon:30,
    tok_add:31,
    tok_sub:32,
    tok_mult:33,
    tok_div:34,
    tok_link:35,
    tok_not:36,
    tok_eq:37,
    tok_addeq:38,
    tok_subeq:39,
    tok_multeq:40,
    tok_diveq:41,
    tok_lnkeq:42,
    tok_cmpeq:43,
    tok_neq:44,
    tok_less:45,
    tok_leq:46,
    tok_grt:47,
    tok_geq:48,
	tok_eof:49
};

var lexer=func(filename)
{
    var _={s:io.fin(filename),len:0,ptr:0,token:[]};
    _.len=size(_.s);
    return
    {
        jmp_note:func()
        {
            while(_.ptr<_.len and chr(_.s[_.ptr])!='\n')
                _.ptr+=1;
            _.ptr+=1;
        },
        idgen:func()
        {
            var tmp="";
            while(_.ptr<_.len)
            {
                if('a'<=chr(_.s[_.ptr]) and chr(_.s[_.ptr])<='z'
                or 'A'<=chr(_.s[_.ptr]) and chr(_.s[_.ptr])<='Z'
                or '0'<=chr(_.s[_.ptr]) and chr(_.s[_.ptr])<='9'
                or chr(_.s[_.ptr])=='_')
                    tmp~=chr(_.s[_.ptr]);
                else
                    break;
                _.ptr+=1;
            }
            append(_.token,tmp);
        },
        strgen:func()
        {
            var str="";
            var mark=chr(_.s[_.ptr]);
            _.ptr+=1;
            while(_.ptr<_.len and chr(_.s[_.ptr])!=mark)
            {
                if(chr(_.s[_.ptr])=='\\')
                {
                    _.ptr+=1;
                    if   (chr(_.s[_.ptr])=='a' ) str~='\a';
                    elsif(chr(_.s[_.ptr])=='b' ) str~='\b';
                    elsif(chr(_.s[_.ptr])=='f' ) str~='\f';
                    elsif(chr(_.s[_.ptr])=='n' ) str~='\n';
                    elsif(chr(_.s[_.ptr])=='r' ) str~='\r';
                    elsif(chr(_.s[_.ptr])=='t' ) str~='\t';
                    elsif(chr(_.s[_.ptr])=='v' ) str~='\v';
                    elsif(chr(_.s[_.ptr])=='?' ) str~='?';
                    elsif(chr(_.s[_.ptr])=='0' ) str~='\0';
                    elsif(chr(_.s[_.ptr])=='\\') str~='\\';
                    elsif(chr(_.s[_.ptr])=='\'') str~='\'';
                    elsif(chr(_.s[_.ptr])=='\"') str~='\"';
                    else str~=chr(_.s[_.ptr]);
                }
                else
                    str~=chr(_.s[_.ptr]);
                _.ptr+=1;
            }
            if(_.ptr>=_.len)
                print("read eof when generating string.\n");
            _.ptr+=1;
            append(_.token,[tok_type.tok_str,str]);
        },
        numgen:func()
        {
            var number=chr(_.s[_.ptr]);
            _.ptr+=1;
            if(chr(_.s[_.ptr])=='x')
            {
                _.ptr+=1;
                while(_.ptr<_.len and
                ('a'<=chr(_.s[_.ptr]) and chr(_.s[_.ptr])<='f'
                or '0'<=chr(_.s[_.ptr]) and chr(_.s[_.ptr])<='9'))
                {
                    number~=chr(_.s[_.ptr]);
                    _.ptr+=1;
                }
                append(_.token,[tok_type.tok_num,num(number)]);
                return;
            }
            elsif(chr(_.s[_.ptr])=='o')
            {
                _.ptr+=1;
                while(_.ptr<_.len and ('0'<=chr(_.s[_.ptr]) and chr(_.s[_.ptr])<='7'))
                {
                    number~=chr(_.s[_.ptr]);
                    _.ptr+=1;
                }
                append(_.token,[tok_type.tok_num,num(number)]);
                return;
            }
            while(_.ptr<_.len and ('0'<=chr(_.s[_.ptr]) and chr(_.s[_.ptr])<='9'))
            {
                number~=chr(_.s[_.ptr]);
                _.ptr+=1;
            }
            if(chr(_.s[_.ptr])=='.')
                number~=chr(_.s[_.ptr]);
            else
            {
                append(_.token,[tok_type.tok_num,num(number)]);
                return;
            }
            _.ptr+=1;
            while(_.ptr<_.len and ('0'<=chr(_.s[_.ptr]) and chr(_.s[_.ptr])<='9'))
            {
                number~=chr(_.s[_.ptr]);
                _.ptr+=1;
            }
            if(chr(_.s[_.ptr])=='e' or chr(_.s[_.ptr])=='E')
                number~=chr(_.s[_.ptr]);
            else
            {
                append(_.token,[tok_type.tok_num,num(number)]);
                return;
            }
            _.ptr+=1;
            if(chr(_.s[_.ptr])=='-' or chr(_.s[_.ptr])=='+')
            {
                number~=chr(_.s[_.ptr]);
                _.ptr+=1;
            }
            while(_.ptr<_.len and ('0'<=chr(_.s[_.ptr]) and chr(_.s[_.ptr])<='9'))
            {
                number~=chr(_.s[_.ptr]);
                _.ptr+=1;
            }
            append(_.token,[tok_type.tok_num,num(number)]);
        },
        oprgen:func()
        {
            var tmp="";
            if(chr(_.s[_.ptr])=='+'
            or chr(_.s[_.ptr])=='-'
            or chr(_.s[_.ptr])=='~'
            or chr(_.s[_.ptr])=='/'
            or chr(_.s[_.ptr])=='*'
            or chr(_.s[_.ptr])=='>'
            or chr(_.s[_.ptr])=='<'
            or chr(_.s[_.ptr])=='!'
            or chr(_.s[_.ptr])=='=')
            {
                tmp=chr(_.s[_.ptr]);
                _.ptr+=1;
                if(_.ptr<_.len and chr(_.s[_.ptr])=='=')
                {
                    tmp~=chr(_.s[_.ptr]);
                    _.ptr+=1;
                }
                append(_.token,tmp);
                return;
            }
            elsif(chr(_.s[_.ptr])=='.')
            {
                if(_.ptr+2<_.len and _.s[_.ptr+1]=='.' and _.s[_.ptr+2]=='.')
                {
                    tmp='...';
                    _.ptr+=3;
                }
                else
                {
                    tmp='.';
                    _.ptr+=1;
                }
                append(_.token,tmp);
                return;
            }
            elsif(chr(_.s[_.ptr])!=' '
            and chr(_.s[_.ptr])!='\t'
            and chr(_.s[_.ptr])!='\n'
            and chr(_.s[_.ptr])!='\r'
            and chr(_.s[_.ptr])[0]>0)
                tmp=chr(_.s[_.ptr]);
            _.ptr+=1;
            if(size(tmp))
                append(_.token,tmp);
        },
        main:func()
        {
            while(_.ptr<_.len)
            {
                if(chr(_.s[_.ptr])=='#')
                    me.jmp_note();
                elsif('a'<=chr(_.s[_.ptr]) and chr(_.s[_.ptr])<='z'
                or 'A'<=chr(_.s[_.ptr]) and chr(_.s[_.ptr])<='Z'
                or chr(_.s[_.ptr])=='_')
                    me.idgen();
                elsif(chr(_.s[_.ptr])=='\'' or chr(_.s[_.ptr])=='\"')
                    me.strgen();
                elsif('0'<=chr(_.s[_.ptr]) and chr(_.s[_.ptr])<='9')
                    me.numgen();
                else
                    me.oprgen();
                if(_.ptr>=_.len)
                    break;
            }
            return;
        },
        get_token:func(){return _.token;}
    };
}

var parse=func(token)
{
    var _={error:0,size:0,token:token,ast:nil};
    _.size=size(token);
    return
    {
        die:func(info)
        {
            _.error+=1;
            println(info);
        }
    };
}

var ast=func()
{
    var _={type:nil,child:[]};
    return
    {
        set_type:func(type){_.type=type;},
        get_type:func(){return _.type;},
        add_child:func(child){append(_.child,child);},
        get_child:func(){return _.child;}
    };
}

var nasal_lexer=lexer(input());
nasal_lexer.main();
println(nasal_lexer.get_token());
var nasal_parse=parse(nasal_lexer.get_token());