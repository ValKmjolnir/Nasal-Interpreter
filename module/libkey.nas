var libkey=func(){
    var lib=dylib.dlopen("libkey"~(os.platform()=="windows"?".dll":".so"));
    var kb=dylib.dlsym(lib,"nas_kbhit");
    var gt=dylib.dlsym(lib,"nas_getch");
    var nb=dylib.dlsym(lib,"nas_noblock");
    var call=dylib.dlcall;
    return {
        kbhit:func(){return call(kb);},
        getch:func(){return call(gt);},
        nonblock:func(){return call(nb);}
    }
}();