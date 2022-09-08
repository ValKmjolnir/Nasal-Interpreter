import.stl.sort;
var argv=runtime.argv();

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
    exit(-1);;
}

var to_lower=func(c){
    if('a'[0]<=c[0] and c[0]<='z'[0])
        return c;
    elsif('A'[0]<=c[0] and c[0]<='Z'[0])
        return chr(c[0]-'A'[0]+'a'[0]);
}

var file_content="";
foreach(var f;argv)
    file_content~=io.fin(f)~" ";
var token={};
var len=size(file_content);
var s="";
for(var i=0;i<len;i+=1){
    var n=file_content[i];
    var c=chr(n);
    if(('a'[0]<=n and n<='z'[0]) or ('A'[0]<=n and n<='Z'[0]) or c=='\''[0] or c=='-'[0]){
        s~=to_lower(c);
    }elsif(size(s)){
        token[s]+=1;
        s="";
    }
}

var vec=keys(token);
sort(vec,0,size(vec)-1,func(a,b){return cmp(a,b)<=0;});
println(vec);