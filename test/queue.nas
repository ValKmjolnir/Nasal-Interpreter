# lib queue.nas
# valkmjolnir 2021/3/3
var new_queue=func()
{
    return {begin:nil,end:nil};
}
var queue_push=func(queue_head,elem)
{
    var new_node=
    {
        elem:elem,
        next:nil
    };
    if(queue_head.begin==nil)
        queue_head.begin=queue_head.end=new_node;
    else
    {
        queue_head.end.next=new_node;
        queue_head.end=new_node;
    }
    return;
}
var queue_pop=func(queue_head)
{
    var t=queue_head.begin;
    queue_head.begin=queue_head.begin.next;
    if(queue_head.begin==nil)
        queue_head.end=nil;
    return t;
}
var queue_front=func(queue_head)
{
    if(queue_head.begin!=nil)
        return queue_head.begin.elem;
    return nil;
}
var queue_empty=func(queue_head)
{
    return queue_head.begin==nil;
}