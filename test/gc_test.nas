use std.runtime;
use std.os;

var test_func = func(test_processes...) {
    var test_process_total = maketimestamp();
    test_process_total.stamp();

    var time_stamp = maketimestamp();
    var begin_info = runtime.gc.info();
    var gc_total_begin = begin_info.total;

    foreach (var f; test_processes) {
        f();
        print(".");
    }

    var end_info = runtime.gc.info();
    var gc_total_end = end_info.total;
    var duration = time_stamp.elapsedMSec();
    println(" ", duration, " ms,\tgc ",
        int((gc_total_end-gc_total_begin)*100/duration), "%,\t",
        int(1000/(duration/size(test_processes))*10)/10, " test(s)/sec"
    );

    var info = runtime.gc.info();
    println("+##-gc----------------------");
    println("| avg gc cycle : ", int(1000 / info.average), " exec/sec");
    println("| avg mark     : ", int(1000 / info.avg_mark), " exec/sec");
    println("| avg sweep    : ", int(1000 / info.avg_sweep), " exec/sec");
    println("| mark count   : ", info.mark_count);
    println("| sweep count  : ", info.sweep_count);
    println("| max mark     : ", info.max_mark, " ms");
    println("| max sweep    : ", info.max_sweep, " ms");
    println("+---------------------------");
}

var MAX_ITER_NUM = 0.5e5;

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
        append(res, [{}, {}, {}, {}]);
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

var append_hash_vec_hash = func {
    var res = [];
    for(var i=0; i<MAX_ITER_NUM; i+=1) {
        append(res, {a:[{}], b:[{}], c:[{}], d:[{}]});
    }
}

var append_tree = func {
    var res = [];
    for(var i=0; i<MAX_ITER_NUM; i+=1) {
        append(res, {
            a: {b: {c:[]}},
            d: {e: {}},
            j: {k: {l:{m:[{a:{b:{c:[{}]}}}]}}}
        });
    }
}

for (var i = 0; i < 10; i += 1) {
    test_func(
        append_vec,
        append_hash,
        append_func,
        append_vec_in_vec,
        append_hash_in_vec,
        append_vec_in_hash,
        append_hash_in_hash,

        append_vec,
        append_hash,
        append_func,
        append_vec_in_vec,
        append_hash_in_vec,
        append_vec_in_hash,
        append_hash_in_hash,

        append_hash_vec_hash,
        append_hash_vec_hash,
        append_hash_vec_hash,

        append_tree,
        append_tree,
        append_tree,

        append_hash_vec_hash,
        append_hash_vec_hash,
        append_hash_vec_hash,

        append_tree,
        append_tree,
        append_tree,
        
        append_hash_vec_hash,
        append_hash_vec_hash,
        append_hash_vec_hash,

        append_tree,
        append_tree,
        append_tree,
        
        append_hash_vec_hash,
        append_hash_vec_hash,
        append_hash_vec_hash,

        append_tree,
        append_tree,
        append_tree
    );
}