# list.nas
# valkmjolnir 2021/3/31

var new = func() {
    var (begin, end, len) = (nil, nil, 0);
    return {
        push_back: func(elem) {
            var tmp = {
                elem: elem,
                prev: nil,
                next: nil
            };
            if (end!=nil) {
                end.next = tmp;
                tmp.prev = end;
                end = tmp;
            } else {
                begin = end = tmp;
            }
            len += 1;
        },
        push_front: func(elem) {
            var tmp = {
                elem: elem,
                prev: nil,
                next: nil
            };
            if (begin!=nil) {
                begin.prev = tmp;
                tmp.next = begin;
                begin = tmp;
            } else {
                begin = end = tmp;
            }
            len += 1;
        },
        pop_back: func() {
            if (end!=nil) {
                end = end.prev;
            }
            if (end==nil) {
                begin = nil;
            } else {
                end.next = nil;
            }
            if (len) {
                len -= 1;
            }
        },
        pop_front: func() {
            if (begin!=nil) {
                begin = begin.next;
            }
            if (begin==nil) {
                end = nil;
            } else {
                begin.prev = nil;
            }
            if (len) {
                len -= 1;
            }
        },
        front: func() {
            if (begin!=nil) {
                return begin.elem;
            }
        },
        back: func() {
            if (end!=nil) {
                return end.elem;
            }
        },
        length: func() {
            return len;
        }
    };
}