use std.dylib;

var (
    kbhit,
    getch,
    nonblock
) = func {
    var lib = dylib.dlopen("libkey"~(os.platform()=="windows"? ".dll":".so"));
    var kb = lib.nas_kbhit;
    var gt = lib.nas_getch;
    var nb = lib.nas_noblock;
    var call = dylib.limitcall(0);
    return [
        func(){return call(kb);},
        func(){return call(gt);},
        func(){return call(nb);}
    ];
}();
