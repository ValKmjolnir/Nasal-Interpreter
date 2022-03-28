import("lib.nas");

var md5=func(str){
    var lib=dylib.dlopen("./module/libmd5"~(os.platform()=="windows"?".dll":".so"));
    var res=dylib.dlcall(dylib.dlsym(lib,"nas_md5"),str);
    dylib.dlclose(lib);
    return res;
}