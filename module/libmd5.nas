import("lib.nas");

var md5=func(){
    var lib=dylib.dlopen("./module/libmd5"~(os.platform()=="windows"?".dll":".so"));
    var sym=dylib.dlsym(lib,"nas_md5");
    var call=dylib.dlcall;
    return func(s){
        return call(sym,s);
    };
}();