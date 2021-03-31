# lib stack.nas
# valkmjolnir 2021/3/31
var stack=func()
{
    var _={next:nil};
    return
    {
        push:func(elem)
        {
            _.next={elem:elem,next:_.next};
            return;
        },
        pop:func()
        {
            var tmp=_.next;
            if(tmp!=nil)
                _.next=tmp.next;
            return;
        },
        top:func()
        {
            var tmp=_.next;
            if(tmp!=nil)
                return tmp.elem;
            return nil;
        },
        clear:func()
        {
            _.next=nil;
        },
        empty:func()
        {
            return _.next==nil;
        }
    };
}
