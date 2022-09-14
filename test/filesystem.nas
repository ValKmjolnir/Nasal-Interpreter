var files=func(path){
    if(!io.exists(path))
        return [];
    var dd=unix.opendir(path);
    var res=[];
    while(var n=unix.readdir(dd))
        append(res,n);
    unix.closedir(dd);
    return res;
}
var prt=func(s,path){
    var vec=files(path);
    var last=size(vec)-1;
    forindex(var i;vec){
        var f=vec[i];
        if(f=="." or f=="..")
            continue;
        foreach(var j;s)
            print("\e[34m",j,"\e[0m");
        if(unix.isdir(path~"/"~f)){
            println("\e[34m",i==last?" └─":" ├─","\e[0m\e[33m[",f,"]\e[36m>\e[0m");
            append(s,i==last?"   ":" │ ");
            prt(s,path~"/"~f);
            pop(s);
        }elsif(unix.isfile(path~"/"~f)){
            println("\e[34m",i==last?" └─":" ├─","\e[0m\e[32m",f,"\e[0m");
        }else{
            println("\e[34m",i==last?" └─":" ├─","\e[0m\e[91m",f,"\e[0m");
        }
    }
}

# enable unicode
if(os.platform()=="windows")
    system("chcp 65001");
println("\e[33m[",unix.getcwd(),"]\e[36m>\e[0m");
prt([""],".");