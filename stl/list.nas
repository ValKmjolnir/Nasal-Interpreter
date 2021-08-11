# lib list.nas
# valkmjolnir 2021/3/31
var list=func()
{
    var (begin,end)=(nil,nil);
    return
    {
        push_back:func(elem)
        {
            var tmp={elem:elem,prev:nil,next:nil};
            if(end!=nil)
            {
                end.next=tmp;
                tmp.prev=end;
                end=tmp;
            }
            else
                begin=end=tmp;
            return;
        },
        push_front:func(elem)
        {
            var tmp={elem:elem,prev:nil,next:nil};
            if(begin!=nil)
            {
                begin.prev=tmp;
                tmp.next=begin;
                begin=tmp;
            }
            else
                begin=end=tmp;
            return;
        },
        pop_back:func()
        {
            if(end!=nil)
                end=end.prev;
            if(end==nil)
                begin=nil;
            else
                end.next=nil;
            return;
        },
        pop_front:func()
        {
            if(begin!=nil)
                begin=begin.next;
            if(begin==nil)
                end=nil;
            else
                begin.prev=nil;
            return;
        },
        front:func()
        {
            if(begin!=nil)
                return begin.elem;
            return nil;
        },
        back:func()
        {
            if(end!=nil)
                return end.elem;
            return nil;
        },
    };
}