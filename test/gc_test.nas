use std.runtime;

var test_func = func(test_processes...) {
    var test_process_total = maketimestamp();
    test_process_total.stamp();

    var time_stamp = maketimestamp();
    var info = runtime.gc.info();
    var gc_total = info.total;
    var duration = 0;
    foreach(var f; test_processes) {
        println("[", os.time(), "] testing ", id(f));
        time_stamp.stamp();
        f();
        duration = time_stamp.elapsedMSec();
        info = runtime.gc.info();
        println("[", os.time(), "] ", duration, " ms, gc ",
            (info.total-gc_total)*100/duration, "%, ",
            int(1000/duration), " cps");
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

var append_vec = func {
    var res = [];
    for(var i=0; i<1e6; i+=1) {
        append(res, [1]);
    }
}

var append_hash = func {
    var res = [];
    for(var i=0; i<1e6; i+=1) {
        append(res, {a:1, b:2});
    }
}

var append_func = func {
    var res = [];
    for(var i=0; i<1e6; i+=1) {
        append(res, func {
            println(arg);
        });
    }
}

var append_vec_in_vec = func {
    var res = [];
    for(var i=0; i<1e6; i+=1) {
        append(res, [[]]);
    }
}

var append_vec_in_hash = func {
    for(var i=0; i<1e6; i+=1) {
        append([], {a:[], b:[]});
    }
}

for(var i=0; i<10; i+=1) {
    test_func(
        append_vec,
        append_hash,
        append_func,
        append_vec_in_vec,
        append_vec_in_hash
    );
}