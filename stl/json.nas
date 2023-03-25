# lib json.nas
# 2021 ValKmjolnir
var JSON=func() {

    var (
        j_eof,
        j_lbrace,
        j_rbrace,
        j_lbrkt,
        j_rbrkt,
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

    var init=func() {
        text='';
        line=1;
        text_size=0;
        ptr=0;
        token={content:'',type:''};
    }

    var isnum=func(c) {
        return '0'<=c and c<='9';
    }

    var isid=func(c) {
        var tmp=c[0];
        return ('a'[0]<=tmp and tmp<='z'[0]) or
        ('A'[0]<=tmp and tmp<='Z'[0]) or
        c=='_';
    }

    var check=func() {
        var c=char(text[ptr]);
        return (
            c=='{' or c=='}' or
            c=='[' or c==']' or
            c==',' or c==':' or
            c=='\"' or c=='\'' or
            isnum(c) or isid(c)
        );
    }

    var get=func(str) {
        init();
        if(!size(str)) {
            println("JSON.parse: empty string");
            str="[]";
        }
        text=str;
        text_size=size(text);
        return;
    }

    var next=func() {
        while(ptr<text_size and !check()) {
            if(char(text[ptr])=='\n')
                line+=1;
            ptr+=1;
        }
        if(ptr>=text_size) {
            token.content="eof";
            token.type=j_eof;
            return;
        }
        
        var c=char(text[ptr]);
        if(c=='{') {
            token.content='{';
            token.type=j_lbrace;
        } elsif(c=='}') {
            token.content='}';
            token.type=j_rbrace;
        } elsif(c=='[') {
            token.content='[';
            token.type=j_lbrkt;
        } elsif(c==']') {
            token.content=']';
            token.type=j_rbrkt;
        } elsif(c==',') {
            token.content=',';
            token.type=j_comma;
        } elsif(c==':') {
            token.content=':';
            token.type=j_colon;
        } elsif(c=='\"' or c=='\'') {
            var strbegin=c;
            var s="";
            ptr+=1;
            while(ptr<text_size and char(text[ptr])!=strbegin) {
                s~=char(text[ptr]);
                ptr+=1;
                if(char(text[ptr-1])=="\\" and ptr<text_size) {
                    s~=char(text[ptr]);
                    ptr+=1;
                }
            }
            token.content=s;
            token.type=j_str;
        } elsif(isnum(c)) {
            var s=c;
            ptr+=1;
            while(ptr<text_size and ((isnum(char(text[ptr])) or char(text[ptr])=='.'))) {
                s~=char(text[ptr]);
                ptr+=1;
            }
            ptr-=1;
            token.content=num(s);
            token.type=j_num;
        } elsif(isid(c)) {
            var s=c;
            ptr+=1;
            while(ptr<text_size and (isid(char(text[ptr])) or isnum(char(text[ptr])))) {
                s~=char(text[ptr]);
                ptr+=1;
            }
            ptr-=1;
            token.content=s;
            token.type=j_id;
        }
        ptr+=1;
        return;
    }

    var match=func(type) {
        if(token.type!=type)
            println("JSON.parse: line ",line,": expect ",j_content[type]," but get `",token.content,"`.");
        next();
        return;
    }

    var member=func(hash) {
        var name=token.content;
        if(token.type==j_rbrace) {
            return;
        }
        if(token.type==j_str) {
            match(j_str);
        } else {
            match(j_id);
        }
        match(j_colon);
        if(token.type==j_lbrace) {
            hash[name]=hash_gen();
        } elsif(token.type==j_lbrkt) {
            hash[name]=vec_gen();
        } elsif(token.type==j_str or token.type==j_num) {
            hash[name]=token.content;
            next();
        }
        return;
    }

    var hash_gen=func() {
        var hash={};
        match(j_lbrace);
        member(hash);
        while(token.type==j_comma) {
            match(j_comma);
            member(hash);
        }
        match(j_rbrace);
        return hash;
    }

    var vec_gen=func() {
        var vec=[];
        match(j_lbrkt);
        if(token.type==j_lbrace) {
            append(vec,hash_gen());
        } elsif(token.type==j_lbrkt) {
            append(vec,vec_gen());
        } elsif(token.type==j_str or token.type==j_num) {
            append(vec,token.content);
            next();
        }
        while(token.type==j_comma) {
            match(j_comma);
            if(token.type==j_lbrace) {
                append(vec,hash_gen());
            } elsif(token.type==j_lbrkt) {
                append(vec,vec_gen());
            } elsif(token.type==j_str or token.type==j_num) {
                append(vec,token.content);
                next();
            }
        }
        match(j_rbrkt);
        return vec;
    }

    return {
        parse:func(str) {
            if(typeof(str)!="str") {
                println("JSON.parse: must use string");
                return [];
            }
            get(str);
            next();

            if(token.type==j_lbrkt) {
                var res=vec_gen();
            } else {
                var res=hash_gen();
            }

            init();
            return res;
        }
    };
}();

JSON.stringify=func(object) {
    if(typeof(object)!="hash" and typeof(object)!="vec") {
        println("JSON.stringify: must use hashmap or vector");
        return "[]";
    }

    var s="";
    var gen=func(elem) {
        var t=typeof(elem);
        if(t=="num") {
            s~=str(elem);
        } elsif(t=="str") {
            s~='"'~elem~'"';
        } elsif(t=="vec") {
            vgen(elem);
        } elsif(t=="hash") {
            hgen(elem);
        } else {
            s~='"undefined"';
        }
    }

    var vgen=func(v) {
        s~="[";
        var vsize=size(v);
        for(var i=0;i<vsize;i+=1) {
            gen(v[i]);
            if(i!=vsize-1) {
                s~=",";
            }
        }
        s~="]";
    }

    var hgen=func(h) {
        s~="{";
        var k=keys(h);
        var vsize=size(k);
        for(var i=0;i<vsize;i+=1) {
            s~=k[i]~":";
            gen(h[k[i]]);
            if(i!=vsize-1) {
                s~=",";
            }
        }
        s~="}";
    }

    if(typeof(object)=="vec") {
        vgen(object);
    } else {
        hgen(object);
    }
    return s;
}