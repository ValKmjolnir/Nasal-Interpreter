var a = func(x, y, z) {
    for (var i = 0; i < 20; i += 1) {
        var cl = caller(i);
        if (cl == nil) {
            return;
        }
        print("[", i, "]\t", cl[1], "\t -> called from ", cl[2], ":", cl[3], "\n");
    }
}

var b = func(x, y) {
    a(1, 2, 3);
}

var c = func(x) b(1, 2);
var d = func c(1);
var e = func d();
var f = func e();
var g = func f();
var h = func g();
var i = func h();
var j = func i();

j();