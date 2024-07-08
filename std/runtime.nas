# runtime.nas
# 2023 by ValKmjolnir
# runtime gives us some functions that we could manage it manually.

# command line arguments
var argv = func() {
    return globals.arg;
}

var _gc_extend = func(type) {
    return __gcextd;
}

var gc = {
    extend: func(type, times = 1) {
        if (times<=0) {
            return nil;
        }
        if (times>16) {
            times = 16;
        }

        for(var i = 0; i<times; i+=1) {
            _gc_extend(type);
        }
        return nil;
    },
    info: func() {return __gcinfo;}
};

var windows = {
    set_utf8_output: func() {
        return __set_utf8_output;
    }
};

var version = func() {
    return __version;
}

var major_version = func() {
    return split(".", version())[0];
}

var minor_version = func() {
    return split(".", version())[1];
}
