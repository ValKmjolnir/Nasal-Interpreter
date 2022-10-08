# lib json.nas
# 2021 ValKmjolnir
var JSON=func(){

    var (
        j_eof,
        j_lbrace,
        j_rbrace,
        j_lbracket,
        j_rbracket,
        j_comma,
        j_colon,
        j_str,
        j_num,
        j_id
    )=(0,1,2,3,4,5,6,7,8,9);
    var j_content=[
        "eof",
        "`{`",
        "`}`",
        "`[`",
        "`]`",
        "`,`",
        "`:`",
        "string",
        "number",
        "identifier"
    ];

    var text='';
    var line=1;
    var text_size=0;
    var ptr=0;
    var token={content:'',type:''};
    var content={};
    var init=func(){
        text='';
        line=1;
        text_size=0;
        ptr=0;
        content={};
        token={content:'',type:''};
    }

    var isnum=func(c){
        return '0'<=c and c<='9';
    }
    var isid=func(c){
        var tmp=c[0];
        return ('a'[0]<=tmp and tmp<='z'[0]) or
        ('A'[0]<=tmp and tmp<='Z'[0]) or
        c=='_';
    }
    var check=func(){
        var c=text[ptr];
        return (
            c=='{' or c=='}' or
            c=='[' or c==']' or
            c==',' or c==':' or
            c=='\"' or c=='\'' or
            isnum(c) or isid(c)
        );
    }

    var get=func(str){
        init();
        if(!size(str))
            die("empty string");
        text=split('',str);
        text_size=size(text);
        return;
    }
    var next=func(){
        while(ptr<text_size and !check()){
            if(text[ptr]=='\n')
                line+=1;
            ptr+=1;
        }
        if(ptr>=text_size){
            token.content="eof";
            token.type=j_eof;
            return;
        }
        
        var c=text[ptr];
        if(c=='{'){
            token.content='{';
            token.type=j_lbrace;
        }elsif(c=='}'){
            token.content='}';
            token.type=j_rbrace;
        }elsif(c=='['){
            token.content='[';
            token.type=j_lbracket;
        }elsif(c==']'){
            token.content=']';
            token.type=j_rbracket;
        }elsif(c==','){
            token.content=',';
            token.type=j_comma;
        }elsif(c==':'){
            token.content=':';
            token.type=j_colon;
        }elsif(c=='\"' or c=='\''){
            var strbegin=c;
            var s="";
            ptr+=1;
            while(ptr<text_size and text[ptr]!=strbegin){
                s~=text[ptr];
                ptr+=1;
            }
            token.content=s;
            token.type=j_str;
        }elsif(isnum(c)){
            var s=c;
            ptr+=1;
            while(ptr<text_size and ((isnum(text[ptr]) or text[ptr]=='.'))){
                s~=text[ptr];
                ptr+=1;
            }
            ptr-=1;
            token.content=num(s);
            token.type=j_num;
        }elsif(isid(c)){
            var s=c;
            ptr+=1;
            while(ptr<text_size and (isid(text[ptr]) or isnum(text[ptr]))){
                s~=text[ptr];
                ptr+=1;
            }
            ptr-=1;
            token.content=s;
            token.type=j_id;
        }
        ptr+=1;
        return;
    }

    var match=func(type){
        if(token.type!=type)
            print("line ",line,": expect ",j_content[type]," but get `",token.content,"`.\n");
        next();
        return;
    }

    var hash_gen=func(){
        var hash={};
        match(j_lbrace);
        member(hash);
        while(token.type==j_comma){
            match(j_comma);
            member(hash);
        }
        match(j_rbrace);
        return hash;
    }

    var vec_gen=func(){
        var vec=[];
        match(j_lbracket);
        if(token.type==j_lbrace){
            append(vec,hash_gen());
        }elsif(token.type==j_lbracket){
            append(vec,vec_gen());
        }elsif(token.type==j_str or token.type==j_num){
            append(vec,token.content);
            next();
        }
        while(token.type==j_comma){
            match(j_comma);
            if(token.type==j_lbrace){
                append(vec,me.hash_gen());
            }elsif(token.type==j_lbracket){
                append(vec,vec_gen());
            }elsif(token.type==j_str or token.type==j_num){
                append(vec,token.content);
                next();
            }
        }
        match(j_rbracket);
        return vec;
    }

    var member=func(hash){
        var name=token.content;
        if(token.type==j_rbrace){
            return;
        }
        if(token.type==j_str){
            match(j_str);
        }else{
            match(j_id);
        }
        match(j_colon);
        if(token.type==j_lbrace){
            hash[name]=hash_gen();
        }elsif(token.type==j_lbracket){
            hash[name]=vec_gen();
        }elsif(token.type==j_str or token.type==j_num){
            hash[name]=token.content;
            next();
        }
        return;
    }
    return {
        parse:func(str){
            if(typeof(str)!="str")
                die("JSON.parse: must use string");
            get(str);
            next();

            match(j_lbrace);
            member(content);
            while(token.type==j_comma){
                match(j_comma);
                member(content);
            }
            match(j_rbrace);

            var res=content;
            init();
            return res;
        },
        stringify:func(hash){
            if(typeof(hash)!="hash")
                die("JSON.stringify: must use hashmap");
            var s="";
            var gen=func(elem){
                var t=typeof(elem);
                if(t=="num"){
                    s~=elem;
                }elsif(t=="str"){
                    s~='"'~elem~'"';
                }elsif(t=="vec"){
                    vgen(elem);
                }elsif(t=="hash"){
                    hgen(elem);
                }else{
                    s~='"undefined"';
                }
            }
            var vgen=func(v){
                s~="[";
                var vsize=size(v);
                for(var i=0;i<vsize;i+=1){
                    gen(v[i]);
                    if(i!=vsize-1)
                        s~=",";
                }
                s~="]";
            }
            var hgen=func(h){
                s~="{";
                var k=keys(h);
                var vsize=size(k);
                for(var i=0;i<vsize;i+=1){
                    s~=k[i]~":";
                    gen(h[k[i]]);
                    if(i!=vsize-1)
                        s~=",";
                }
                s~="}";
            }
            hgen(hash);
            return s;
        }
    };
}();
