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
