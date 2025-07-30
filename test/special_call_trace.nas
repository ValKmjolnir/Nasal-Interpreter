var f = func() {
    append({}, []);
}

var b = func(f...) {
    foreach(var i; f) {
        i();
    }
}

b(f);

func(a, b, c) {
    f()
}(0, 1, 2);