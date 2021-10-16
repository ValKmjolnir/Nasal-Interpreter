# lib stack.nas
# valkmjolnir 2021/3/31
var stack=func()
{
    var next=nil;
    return{
        push:func(elem){
            next={elem:elem,next:next};
        },
        pop:func(){
            if(next!=nil)
                next=next.next;
        },
        top:func(){
            if(next!=nil)
                return next.elem;
        },
        clear:func(){
            next=nil;
        },
        empty:func(){
            return next==nil;
        }
    };
}
