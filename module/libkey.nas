var libkey=func(){
    var lib=dylib.dlopen("libkey"~(os.platform()=="windows"?".dll":".so"));
    var kb=lib.nas_kbhit;
    var gt=lib.nas_getch;
    var nb=lib.nas_noblock;
    var call=dylib.limitcall(0);
    return {
        kbhit:func(){return call(kb);},
        getch:func(){return call(gt);},
        nonblock:func(){return call(nb);}
    }
}();