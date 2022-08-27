var os_time=func(){
    return "[\e[33m"~os.time()~"\e[0m] ";
}
var err_hd=func(){
    return "[\e[91merror\e[0m] ";
}
var watching_hd=func(){
    return "[\e[96mwatching\e[0m] ";
}
var modified_hd=func(){
    return "[\e[92mmodified\e[0m] ";
}
var usage=func(){
    println(os_time(),"[\e[92musage\e[0m] nasal watchdog.nas <filename>");
}

var argv=runtime.argv();
if(size(argv)!=1){
    println(os_time(),err_hd(),"need correct file path to watch");
    usage();
    exit(-1);
}
var filename=argv[0];
if(!io.exists(filename)){
    println(os_time(),err_hd(),"file <",filename,"> does not exist");
    usage();
    exit(-1);
}

var modified_time=fstat(filename).st_mtime;
println(os_time(),watching_hd(),filename," ..");
while(1){
    unix.sleep(1);
    if(!io.exists(filename)){
        println(os_time(),err_hd(),"file <",filename,"> does not exist");
        break;
    }
    var latest_modified_time=fstat(filename).st_mtime;
    if(latest_modified_time!=modified_time){
        modified_time=latest_modified_time;
        println(os_time(),modified_hd(),filename);
        var ret=system((os.platform()=="windows"?"":"./")~"nasal "~filename);
        if(ret!=0){
            println(os_time(),err_hd(),"process returned value ",ret);
        }
    }
}