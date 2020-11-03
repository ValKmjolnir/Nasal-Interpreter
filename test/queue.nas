# lib queue.nas
var block_alloc=func()
{
    return {elem:nil,next:nil};
}
var new_queue=func()
{
    return {next:nil};
}
var queue_push=func(queue,elem)
{
    var tmp=queue;
    while(tmp.next!=nil)
        tmp=tmp.next;
    tmp.next=block_alloc();
    tmp.next.elem=elem;
}
var queue_pop=func(queue)
{
    var tmp=queue.next;
    if(tmp!=nil)
        queue.next=tmp.next;
    return;
}
var queue_front=func(queue)
{
    var tmp=queue.next;
    if(tmp!=nil)
        return tmp.elem;
    return nil;
}
var queue_empty=func(queue)
{
    return queue.next==nil;
}