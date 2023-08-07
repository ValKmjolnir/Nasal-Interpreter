# test if globals run correctly

println(globals);
println(keys(globals));
foreach(var i; keys(globals)) {
    println("var ", i, " = ", typeof(globals[i]), ";");
}

var test_func = nil;
globals.test_func = func() {
    println("succeed!");
}
println();
println(globals.test_func);
globals.test_func();

var f = func() {
    println(arg);
    func() {println(arg);}(114, 514, 1919, 810);
    println(arg);
}

f(1, 2, 3);

var a = func(arg, b) {
    println(arg, " ", b);
}
var b = func(a) {
    println(a, " ", arg);
}

a(1, 2, 3, 4); # 1 2
b(1, 2, 3, 4); # 1 [2 3 4]

# command line arguments
println(arg);
println(globals.arg);
println(typeof(globals));