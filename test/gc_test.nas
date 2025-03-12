use std.runtime;
use std.os;

var test_func = func(test_processes...) {
    var test_process_total = maketimestamp();
    test_process_total.stamp();

    var time_stamp = maketimestamp();
    var info = runtime.gc.info();
    var gc_total = info.total;
    var duration = 0;
    foreach (var t; test_processes) {
        var name = t[0];
        var f = t[1];
        print("[", os.time(), "] testing ", name, " : ");
        time_stamp.stamp();
        f();
        duration = time_stamp.elapsedMSec();
        info = runtime.gc.info();
        println(duration, " ms, gc ",
            (info.total-gc_total)*100/duration, "%, ",
            1000/duration, " count/sec"
        );
        gc_total = info.total;
    }

    println("[", os.time(), "] test time: ",
        test_process_total.elapsedMSec(), " ms");
    
    info = runtime.gc.info();
    println("##-gc----------------");
    println("total    : ", info.total, " ms");
    println("average  : ", info.average, " ms");
    println("max gc   : ", info.max_gc, " ms");
    println("max mark : ", info.max_mark, " ms");
    println("max sweep: ", info.max_sweep, " ms");
    println("---------------------");
}

var MAX_ITER_NUM = 2e5;

var append_vec = func {
    var res = [];
    for(var i=0; i<MAX_ITER_NUM; i+=1) {
        append(res, [1, 2, 3, 4]);
    }
}

var append_hash = func {
    var res = [];
    for(var i=0; i<MAX_ITER_NUM; i+=1) {
        append(res, {a:1, b:2, c:3, d:4});
    }
}

var append_func = func {
    var res = [];
    for(var i=0; i<MAX_ITER_NUM; i+=1) {
        append(res, func {
            println(arg);
        });
    }
}

var append_vec_in_vec = func {
    var res = [];
    for(var i=0; i<MAX_ITER_NUM; i+=1) {
        append(res, [[], [], [], []]);
    }
}

var append_hash_in_vec = func {
    var res = [];
    for(var i=0; i<MAX_ITER_NUM; i+=1) {
        append(res, {a:{}, b:{}, c:{}, d:{}});
    }
}

var append_vec_in_hash = func {
    var res = [];
    for(var i=0; i<MAX_ITER_NUM; i+=1) {
        append(res, {a:[], b:[], c:[], d:[]});
    }
}

var append_hash_in_hash = func {
    var res = [];
    for(var i=0; i<MAX_ITER_NUM; i+=1) {
        append(res, {a:{}, b:{}, c:{}, d:{}});
    }
}

for (var i = 0; i < 10; i += 1) {
    test_func(
        ["vec", append_vec],
        ["hash", append_hash],
        ["func", append_func],
        ["vec<vec>", append_vec_in_vec],
        ["vec<hash>", append_hash_in_vec],
        ["hash<str, vec>", append_vec_in_hash],
        ["hash<str, hash>", append_hash_in_hash]
    );
}