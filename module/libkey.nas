
var lib=dylib.dlopen("libkey"~(os.platform()=="windows"?".dll":".so"));
var kb=lib.nas_kbhit;
var gt=lib.nas_getch;
var nb=lib.nas_noblock;
var call=dylib.limitcall(0);

var kbhit = func(){return call(kb);}
var getch = func(){return call(gt);}
var nonblock = func(){return call(nb);}
