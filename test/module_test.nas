import("lib.nas");

var libfib=func(){
    var dl=dylib.dlopen("./module/libfib."~(os.platform()=="windows"?"dll":"so"));
    var fib=dylib.dlsym(dl,"fib");
    var qfib=dylib.dlsym(dl,"quick_fib");
    var call=dylib.dlcall;
    return
    {
        fib: func(x){return call(fib,x)},
        qfib:func(x){return call(qfib,x)}
    };
}();

println(libfib);
println(libfib.fib(29));
println(libfib.qfib(29));