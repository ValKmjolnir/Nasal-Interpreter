var os_time=func(){
    return "[\e[33;1m"~os.time()~"\e[0m] ";
}
var err_hd=func(){
    return "[\e[91;1merror\e[0m] ";
}
var info_hd=func(){
    return "[\e[96;1minfo\e[0m] ";
}
var modified_hd=func(){
    return "[\e[92;1mmodified\e[0m] ";
}
var usage=func(){
    println(os_time(),info_hd(),"\e[1musage: nasal watchdog.nas <filename> [\"argv\"]\e[0m");
}

var argv=runtime.argv();
if(size(argv)<1){
    println(os_time(),err_hd(),"\e[1mneed correct file path to watch\e[0m");
    usage();
    exit(-1);
}
var filename=argv[0];
if(!io.exists(filename)){
    println(os_time(),err_hd(),"\e[1mfile <",filename,"> does not exist\e[0m");
    usage();
    exit(-1);
}

var args=[];
if(size(argv)==2){
    println(os_time(),info_hd(),"\e[1mwith argument(s) ",argv[1],"\e[0m");
    args=split(" ",argv[1]);
}

var modified_time=fstat(filename).st_mtime;
println(os_time(),info_hd(),"\e[1mwatching ",filename," ..\e[0m");
while(1){
    unix.sleep(1);
    if(!io.exists(filename)){
        println(os_time(),err_hd(),"\e[1mfile <",filename,"> does not exist\e[0m");
        break;
    }
    var latest_modified_time=fstat(filename).st_mtime;
    if(latest_modified_time!=modified_time){
        modified_time=latest_modified_time;
        println(os_time(),modified_hd(),"\e[1m",filename,"\e[0m");
        var cmd=(os.platform()=="windows"?"":"./")~"nasal "~filename;
        foreach(var i;args)
            cmd~=" "~i;
        println(os_time(),info_hd(),"\e[1mexecuting command \"",cmd,"\"\e[0m");
        var ret=system(cmd);
        if(ret!=0){
            println(os_time(),err_hd(),"\e[1mprocess returned ",ret,"\e[0m");
        }else{
            println(os_time(),info_hd(),"\e[1mprocess returned ",ret,"\e[0m");
        }
    }
}