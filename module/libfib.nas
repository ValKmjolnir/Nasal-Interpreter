
var libfib=func(){
    var dl=dylib.dlopen("libfib."~(os.platform()=="windows"?"dll":"so"));
    var fib=dl.fib;
    var qfib=dl.quick_fib;
    var create_ghost=dl.create_ghost;
    var set_ghost=dl.set_ghost;
    var print_ghost=dl.print_ghost;
    var zero_call=dylib.limitcall(0);
    var call=dylib.limitcall(1);
    var test_call=dylib.limitcall(2);
    var res={
        fib: func(x) {return call(fib,x)},
        qfib: func(x) {return call(qfib,x)},
        create_ghost: func() {return zero_call(create_ghost)},
        set_ghost: func(object, x) {return test_call(set_ghost, object, x)},
        print_ghost: func(object) {return call(print_ghost, object)}
    };

    res.test_ghost=func() {
        var ghost=res.create_ghost();
        res.print_ghost(nil); # err
        res.print_ghost(ghost); # random
        res.set_ghost(nil, 114); # err
        res.set_ghost(ghost, 114); # success
        res.print_ghost(ghost); # 114
    }

    return res;
}();