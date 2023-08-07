# runtime.nas
# 2023 by ValKmjolnir
# runtime gives us some functions that we could manage it manually.

# command line arguments
var argv = func() {
    return globals.arg;
}

var gc = {
    extend: func(type) {return __gcextd;},
    info: func() {return __gcinfo;}
};
