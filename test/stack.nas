# lib stack.nas
var block_alloc=func()
{
    return {elem:nil,next:nil};
}
var new_stack=func()
{
    return {next:nil};
}
var stack_push=func(stack,elem)
{
    var tmp=stack.next;
    stack.next=block_alloc();
    stack.next.elem=elem;
    stack.next.next=tmp;
}
var stack_pop=func(stack)
{
    var tmp=stack.next;
    if(tmp!=nil)
        stack.next=tmp.next;
    return;
}
var stack_top=func(stack)
{
    var tmp=stack.next;
    if(tmp!=nil)
        return tmp.elem;
    return nil;
}
var stack_empty=func(stack)
{
    return stack.next==nil;
}