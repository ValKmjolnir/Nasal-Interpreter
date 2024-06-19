
var create = func(vec) {
    return __subprocess_create(vec);
}

var active = func(pid) {
    return __subprocess_active(pid);
}

var terminate = func(pid) {
    return __subprocess_terminate(pid);
}
