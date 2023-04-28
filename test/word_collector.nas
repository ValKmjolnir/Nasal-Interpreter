import.stl.sort;

var to_lower=func(s){
    var tmp="";
    for(var i=0;i<size(s);i+=1){
        var c=s[i];
        if('a'[0]<=c and c<='z'[0])
            tmp~=chr(c);
        elsif('A'[0]<=c and c<='Z'[0])
            tmp~=chr(c-'A'[0]+'a'[0]);
        else
            tmp~=chr(c);
    }
    return tmp;
}

var spliter=func(content){
    var token={};
    var len=size(content);
    var s="";
    for(var i=0;i<len;i+=1){
        var n=content[i];
        var c=chr(n);
        if(('a'[0]<=n and n<='z'[0]) or ('A'[0]<=n and n<='Z'[0])){
            s~=c;
        }elsif(c=='.' or c==',' or c=='-' or c=='\'' or c=='\"' or c=='!' or c=='?'){
            if(size(s)) {
                token[to_lower(s)]+=1;
            }
            token[c]+=1;
            s="";
        }elsif(size(s)){
            if(s[0]!="-"[0] and s[0]!="'"[0] and s[-1]!="-"[0] and s[-1]!="'"[0])
                token[to_lower(s)]+=1;
            s="";
        }
    }
    return token;
}

func(argv){
    if(size(argv)<1){
        println("no input files.");
        exit(-1);
    }
    var all_exists=1;
    foreach(var f;argv){
        if(!io.exists(f)){
            println("cannot open file <",f,">");
            all_exists=0;
        }
    }
    if(!all_exists){
        exit(-1);
    }
    var file_content="";
    foreach(var f;argv)
        file_content~=io.readfile(f)~" ";
    var vec=keys(spliter(file_content));
    sort(vec,func(a,b){return cmp(a,b)<=0;});
    println(vec);
}(runtime.argv());