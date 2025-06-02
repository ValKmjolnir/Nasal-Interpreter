use std.os;
use std.dylib;
use module.libfib;

println(keys(libfib));
libfib.test_ghost();

println("[keys  ] ", keys(libfib));
var tm = maketimestamp();
tm.stamp();
println("[result] ", libfib.fib(35));
println("[time  ] ", tm.elapsedMSec()," ms");
tm.stamp();
println("[result] ", libfib.qfib(35));
println("[time  ] ", tm.elapsedMSec()," ms");
println();

func() {
    for (var i = 0; i<256; i += 1) {
        var tmp = []; # try trigger gc here to test delete ghost
    }
    println();
}();

var speed_test = func() {
    var dd = dylib.dlopen("libfib");
    println("[dylib ] ", dd);
    var fd = dd.quick_fib;
    var vec_call = dylib.dlcall;
    var invoke = dylib.limitcall(1);
    var tm = maketimestamp();

    for (var t=0; t<10; t+=1) {
        tm.stamp();
        for (var i=0; i<5e5; i+=1) {
            invoke(fd, 40);
        }
        println("[time  ] limited call: ", int(5e5/tm.elapsedMSec()), " call/ms");
        tm.stamp();
        for (var i=0; i<5e5; i+=1) {
            vec_call(fd, 40);
        }
        println("[time  ] dynamic call: ", int(5e5/tm.elapsedMSec()), " call/ms");
    }
}

speed_test();