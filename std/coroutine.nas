# coroutine.nas
# 2023 by ValKmjolnir

# in fact it is not multi-threaded, maybe in the future i could make it

var create = func(function) {
    return __cocreate;
}

var resume = func(co, args...) {
    return __coresume;
}

var yield = func(args...) {
    return __coyield;
}

var status = func(co) {
    return __costatus;
}

var running = func() {
    return __corun;
}
