import.stl.padding;
import.stl.file;
import.stl.sort;

var source=find_all_files_with_extension(".","cpp","h");
sort(source,func(a,b){return cmp(a,b)<0});

var lib=find_all_files_with_extension("./stl","nas");
sort(lib,func(a,b){return cmp(a,b)<0});

var testfile=find_all_files_with_extension("./test","nas");
sort(testfile,func(a,b){return cmp(a,b)<0});

var module=find_all_files_with_extension("./module","cpp","nas");
sort(module,func(a,b){return cmp(a,b)<0});

var longest=func(vec...){
    var len=0;
    foreach(var v;vec)
        foreach(var f;v)
            len=size(f)>len?size(f):len;
    return len;
}
var padding_length=longest(source,lib,testfile,module);

var blank=func(s){
    if(!size(s)){
        return 1;
    }
    var space=[" "[0],"\n"[0],"\t"[0],"\r"[0]];
    for(var i=0;i<size(s);i+=1){
        var flag=0;
        foreach(var j;space){
            if(s[i]==j){
                flag=1;
            }
        }
        if(!flag){
            return 0;
        }
    }
    return 1;
}

var count=func(s,c){
    var cnt=0;
    foreach(var i;split(c,s))
        cnt+=!blank(i);
    return cnt;
}

var column=func(number){
    number=number>=1000?substr(str(number/1000),0,3)~'k':str(number);
    return leftpad(number,6);
}

var calc=func(codetype,files,path=""){
    println(codetype);
    var (bytes,ctx,line,semi,line_cnt,semi_cnt)=(0,"",0,0,0,0);
    forindex(var i;files){
        var s=io.exists(path~files[i])?io.readfile(path~files[i]):"";
        (line_cnt,semi_cnt)=(count(s,'\n'),count(s,';'));
        println(rightpad(files[i],padding_length),'|',
            column(line_cnt),' line |',
            column(semi_cnt),' semi |',
            leftpad(str(int(size(s)/1024)),4),' kb | ',
            md5(s),' |');
        bytes+=size(s);
        ctx~=s;
        line+=line_cnt;
        semi+=semi_cnt;
    }
    println(rightpad("total:",padding_length),'|',
        column(line),' line |',
        column(semi),' semi |',
        leftpad(str(int(bytes/1024)),4),' kb | ',
        md5(ctx),' |\n');
    return int(bytes/1024);
}

var all=calc("source code:",source)
    +calc("lib:",lib,"stl/")
    +calc("test file:",testfile,"test/")
    +calc("module:",module,"module/");
println(rightpad("total:",padding_length),'|',leftpad(str(all),6),' kb   |');