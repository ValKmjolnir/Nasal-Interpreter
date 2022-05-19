# coroutine.nas by ValKmjolnir
# 2022/5/19
var fib=func(){
    var (a,b)=(1,1);
    if(coroutine.running()){
        coroutine.yield(a);
        coroutine.yield(b);
    }
    while(1){
        (a,b)=(b,a+b);
        if(coroutine.running())
            coroutine.yield(b);
        else
            break;
    }
    return;
}

var co=[coroutine.create(fib),coroutine.create(fib)];
for(var i=0;i<45;i+=1){
    var res=[coroutine.resume(co[0]),coroutine.resume(co[1])];
    println('coroutine[0]:',res[0]==nil?nil:res[0][0],'\ncoroutine[1]:',res[1]==nil?nil:res[1][0]);
}

fib();