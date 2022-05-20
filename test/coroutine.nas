# coroutine.nas by ValKmjolnir
# 2022/5/19
var fib=func(){
    var (a,b)=(1,1);
    coroutine.yield(a);
    coroutine.yield(b);
    while(1){
        (a,b)=(b,a+b);
        coroutine.yield(b);
    }
    return;
}

var co=[coroutine.create(fib),coroutine.create(fib)];
for(var i=0;i<45;i+=1){
    var res=[coroutine.resume(co[0]),coroutine.resume(co[1])];
    println('coroutine[0]:',res[0]==nil?nil:res[0][0],'\ncoroutine[1]:',res[1]==nil?nil:res[1][0]);
}

var productor=func(){
    for(var i=0;;i+=1)
        coroutine.yield(i);
}
var counter=0;
var consumer=func(){
    counter+=1;
    print('[',counter,']: ');
    for(var i=0;i<5;i+=1){
        print('[',i,']',coroutine.resume(co)[0],' ');
    }
    print('\n');
}

var co=coroutine.create(productor);
var tm=maketimestamp();
tm.stamp();
while(tm.elapsedMSec()<1000)
    consumer();

func(){
    var x=1;
    var co=coroutine.create(func(){
        for(var j=0;j<1024;j+=1){
            coroutine.yield(x,i,j);
            x+=1;
        }
    });
    for(var i=0;i<256;i+=1)
        println(coroutine.resume(co));
}();