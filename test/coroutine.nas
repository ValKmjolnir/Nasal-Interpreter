# coroutine.nas by ValKmjolnir
# 2022/5/19
import("stl/process_bar.nas");
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
# different coroutines don't share the same local scope
var co=[coroutine.create(fib),coroutine.create(fib)];
for(var i=0;i<45;i+=1){
    var res=[coroutine.resume(co[0]),coroutine.resume(co[1])];
    println('co[0]: ',res[0]==nil?nil:res[0][0],'\nco[1]: ',res[1]==nil?nil:res[1][0]);
}

# test if coroutine can get upvalues
func(){
    var x=1;
    var co=coroutine.create(func(){
        for(var j=0;j<128;j+=1){
            coroutine.yield(x,i,j);
            x+=1;
        }
    });
    for(var i=0;i<16;i+=1)
        println(coroutine.resume(co));
}();

# pressure test
var productor=func(){
    for(var i=0;;i+=1)
        coroutine.yield(i);
}
var total=10000; # ms
var co=coroutine.create(productor);
var tm=maketimestamp();

var counter=0;
var bar=process_bar.bar("block","point","line",40);
var consumer=func(){
    counter+=1;
    for(var i=0;i<5;i+=1)
        coroutine.resume(co);
    var rate=(tm.elapsedMSec()+1)/total;
    print(bar.bar(rate)," ",rate*100,"%  \r");
}

tm.stamp();
while(tm.elapsedMSec()<total)
    consumer();
println("\nexecute ",counter," tasks during ",total," ms, avg ",counter/(total)," tasks/ms.")