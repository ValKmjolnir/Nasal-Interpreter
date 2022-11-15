var libfib=func(){
    var dl=dylib.dlopen("libfib."~(os.platform()=="windows"?"dll":"so"));
    var fib=dl.fib;
    var qfib=dl.quick_fib;
    var call=dylib.limitcall(1);
    return {
        fib: func(x){return call(fib,x)},
        qfib:func(x){return call(qfib,x)}
    };
}();