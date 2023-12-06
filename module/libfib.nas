use std.dylib;
use std.os;

var _dl = dylib.dlopen("libfib."~(os.platform()=="windows"?"dll":"so"));

var _fib = _dl.fib;

var _qfib = _dl.quick_fib;

var _create_ghost = _dl.create_ghost;

var _set_ghost = _dl.set_ghost;

var _print_ghost = _dl.print_ghost;

var _zero_call = dylib.limitcall(0);

var _call = dylib.limitcall(1);

var _test_call = dylib.limitcall(2);

var fib = func(x) {
    return _call(_fib, x)
}

var qfib = func(x) {
    return _call(_qfib, x)
}

var create_ghost = func() {
    return _zero_call(_create_ghost)
}

var set_ghost = func(object, x) {
    return _test_call(_set_ghost, object, x)
}

var print_ghost = func(object) {
    return _call(_print_ghost, object)
}

var test_ghost = func() {
    var ghost = create_ghost();
    print_ghost(nil); # err
    print("\n");
    print_ghost(ghost); # random
    print("\n");
    set_ghost(nil, 114); # err
    print("\n");
    set_ghost(ghost, 114); # success
    print("\n");
    for(var i = 0; i<256; i+=1) {
        var temp = []; # try to trigger gc
    }
    print("\n");
    print_ghost(ghost); # 114
    print("\n");
}
