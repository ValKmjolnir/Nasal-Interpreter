# lib stack.nas
# valkmjolnir 2021/3/31
var stack=func()
{
    var next=nil;
    return
    {
        push:func(elem)
        {
            next={elem:elem,next:next};
            return;
        },
        pop:func()
        {
            var tmp=next;
            if(tmp!=nil)
                next=tmp.next;
            return;
        },
        top:func()
        {
            var tmp=next;
            if(tmp!=nil)
                return tmp.elem;
            return nil;
        },
        clear:func()
        {
            next=nil;
        },
        empty:func()
        {
            return next==nil;
        }
    };
}
