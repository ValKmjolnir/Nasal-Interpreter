# coroutine.nas by ValKmjolnir
# 2022/5/19
use std.coroutine;
use std.process_bar;
use std.padding;
use std.os;
use std.runtime;

if (os.platform() == "windows") {
    runtime.windows.set_utf8_output();
    system("color");
}

var fib = func() {
    var (a, b) = (1, 1);
    coroutine.yield(a);
    coroutine.yield(b);
    while (1) {
        (a, b) = (b, a + b);
        coroutine.yield(b);
    }
    return;
}
var co = [coroutine.create(fib), coroutine.create(fib)];
for (var i = 0; i < 45; i += 1) {
    var res = [coroutine.resume(co[0]), coroutine.resume(co[1])];
    if (res[0] == nil or res[1] == nil or res[0][0] != res[1][0])
        die("different coroutines don't share the same local scope");
}

# test if coroutine can get upvalues
func() {
    var x = 1;
    var co = coroutine.create(func() {
        for (var j = 0; j < 128; j += 1) {
            coroutine.yield(x, i, j);
            x += 1;
        }
    });
    for (var i = 0; i < 16; i += 1) {
        var res = coroutine.resume(co);
        if (res == nil or res[0] != x or res[1] != i)
            die("coroutine should have the ability to get upvalues");
    }
}();

# test coroutine.resume passing arguments to coroutine
func {
    var co = coroutine.create(func() {
        var (a, b) = coroutine.yield(a + b);
        println("[0] coroutine.yield get ", a, " ", b);
        (a, b) = coroutine.yield(a + b);
        println("[0] coroutine.yield get ", a, " ", b);
        return "end";
    });

    for (var i = 0; i < 5; i += 1)
        println("[0] coroutine.resume get ", coroutine.resume(co, i, i + 1));
    print("\n");
}();

# test crash in coroutines
var co = coroutine.create(func {
    var b = func() { b() }
    coroutine.yield(b);
    b();
    coroutine.yield(0); # unreachable
});

println("[1] coroutine yield: ", coroutine.resume(co));
println("[1] coroutine state after yield:\e[32m ", coroutine.status(co), "\e[0m");
println("[1] coroutine stackoverflow error: ", coroutine.resume(co));
println("[1] coroutine state after error:\e[91m ", coroutine.status(co), "\e[0m");
println("[1] coroutine yield after error: ", coroutine.resume(co));
println("[1] coroutine state after error:\e[91m ", coroutine.status(co), "\e[0m");
print("\n");

var co = coroutine.create(func {
    var a = 1;
    var b = func() {
        b();
    }
    coroutine.yield(b);
    coroutine.yield(b());
});

println("[2] coroutine yield: ", coroutine.resume(co));
println("[2] coroutine state:\e[32m ", coroutine.status(co), "\e[0m");
println("[2] coroutine error: ", coroutine.resume(co));
println("[2] coroutine state:\e[91m ", coroutine.status(co), "\e[0m");
println("[2] coroutine yield: ", coroutine.resume(co));
println("[2] coroutine state:\e[91m ", coroutine.status(co), "\e[0m");
println("[2] ok\n");

# pressure test
for (var t = 0; t < 10; t += 1) {
    var productor = func() {
        while (1) {
            coroutine.yield(i);
        }
    }

    var co = coroutine.create(productor);
    var tm = maketimestamp();

    var counter = 0;
    var bar = process_bar.high_resolution_bar(40);
    var consumer = func() {
        counter += 1;
        for (var i = 0; i < t + 1; i += 1)
            coroutine.resume(co);
        if (counter - int(counter / 1000) * 1000 == 0) {
            var rate = counter / 2e5;
            print(" ", bar.bar(rate), " ",
                padding.leftpad(str(int(rate*100)),3), "% | ",
                str(1e3 * int(counter / tm.elapsedMSec())),
                " tasks/s         \r"
            );
        }
    }

    tm.stamp();
    for (var i = 0; i < 1e5; i += 1)
        consumer();
    println(" ", bar.bar(1), " 100% | ",
        str(int(1e3 * counter / tm.elapsedMSec())),
        " tasks/s         "
    );
}