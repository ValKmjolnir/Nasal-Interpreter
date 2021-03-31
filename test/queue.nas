# lib queue.nas
# valkmjolnir 2021/3/31
var queue=func()
{
    var _={begin:nil,end:nil};
    return
    {
        push:func(elem)
        {
            var new_node=
            {
                elem:elem,
                next:nil
            };
            if(_.begin==nil)
                _.begin=_.end=new_node;
            else
            {
                _.end.next=new_node;
                _.end=new_node;
            }
            return;
        },
        pop:func()
        {
            if(_.begin!=nil)
                _.begin=_.begin.next;
            if(_.begin==nil)
                _.end=nil;
        },
        front:func()
        {
            if(_.begin!=nil)
                return _.begin.elem;
            return nil;
        },
        clear:func()
        {
            _.begin=_.end=nil;
        },
        empty:func()
        {
            return _.begin==nil;
        }
    };
}
