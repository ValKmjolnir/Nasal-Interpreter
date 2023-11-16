# queue.nas
# valkmjolnir 2021/3/31
var new = func() {
    var (begin, end) = (nil, nil);
    return{
        push: func(elem) {
            var new_node={
                elem:elem,
                next:nil
            };
            if (begin==nil)
                begin=end=new_node;
            else {
                end.next=new_node;
                end=new_node;
            }
        },
        pop: func() {
            if (begin!=nil)
                begin=begin.next;
            if (begin==nil)
                end=nil;
        },
        front: func() {
            if (begin!=nil)
                return begin.elem;
        },
        clear: func() {
            begin=end=nil;
        },
        empty: func() {
            return begin==nil;
        }
    };
}
