import("lib.nas");

var fd=io.open("test/filesystem.nas");
while((var line=io.readln(fd))!=nil)
    println(line);
io.close(fd);
println(io.stat("test/filesystem.nas"));

var dd=unix.opendir("test");
while(var name=unix.readdir(dd))
    println(name);
unix.closedir(dd);

var files=func(dir){
    var dd=unix.opendir(dir);
    var res=[];
    while(var n=unix.readdir(dd))
        append(res,n);
    unix.closedir(dd);
    return res;
}
var prt=func(s,path){
    foreach(var i;files(path)){
        print(s,i);
        if(unix.isdir(path~'/'~i)){
            print(' <dir>\n');
            if(i!='.' and i!='..')
                prt(s~' |',path~'/'~i);
        }
        elsif(unix.isfile(path~'/'~i))
            print(" <file>\n");
        else
            print(' <unknown>\n');
    }
}
prt('',".");