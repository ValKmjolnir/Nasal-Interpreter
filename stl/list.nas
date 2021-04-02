# lib list.nas
# valkmjolnir 2021/3/31
var list=func()
{
    var _={begin:nil,end:nil};
    return
    {
        push_back:func(elem)
        {
            var tmp={elem:elem,prev:nil,next:nil};
            if(_.end!=nil)
            {
                _.end.next=tmp;
                tmp.prev=_.end;
                _.end=tmp;
            }
            else
            {
                _.begin=tmp;
                _.end=tmp;
            }
            return;
        },
        push_front:func(elem)
        {
            var tmp={elem:elem,prev:nil,next:nil};
            if(_.begin!=nil)
            {
                _.begin.prev=tmp;
                tmp.next=_.begin;
                _.begin=tmp;
            }
            else
            {
                _.begin=tmp;
                _.end=tmp;
            }
            return;
        },
        pop_back:func()
        {
            if(_.end!=nil)
                _.end=_.end.prev;
            if(_.end==nil)
                _.begin=nil;
            else
                _.end.next=nil;
            return;
        },
        pop_front:func()
        {
            if(_.begin!=nil)
                _.begin=_.begin.next;
            if(_.begin==nil)
                _.end=nil;
            else
                _.begin.prev=nil;
            return;
        },
        front:func()
        {
            if(_.begin!=nil)
                return _.begin.elem;
            return nil;
        },
        back:func()
        {
            if(_.end!=nil)
                return _.end.elem;
            return nil;
        },
    };
}