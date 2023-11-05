# stack.nas
# valkmjolnir 2021/3/31
var stack = func() {
    var vec = [];
    return {
        push: func(elem) {
            append(vec, elem);
        },
        pop: func() {
            return pop(vec);
        },
        top: func() {
            if (size(vec)!=0) {
                return vec[-1];
            }
        },
        clear: func() {
            vec = [];
        },
        empty: func() {
            return size(vec)==0;
        }
    };
}
