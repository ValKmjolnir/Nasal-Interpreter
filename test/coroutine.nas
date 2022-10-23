# coroutine.nas by ValKmjolnir
# 2022/5/19
import.stl.process_bar;

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
    if(res[0]==nil or res[1]==nil or res[0][0]!=res[1][0])
        die("different coroutines don't share the same local scope");
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
    for(var i=0;i<16;i+=1){
        var res=coroutine.resume(co);
        if(res==nil or res[0]!=x or res[1]!=i)
            die("coroutine should have the ability to get upvalues");
    }
}();

# test crash in coroutines
var co=coroutine.create(func{
    var b=func(){b()}
    coroutine.yield(b);
    b();
    coroutine.yield(0);
});

println("coroutine yield: ",coroutine.resume(co));
println("coroutine state:\e[32m ",coroutine.status(co),"\e[0m");
println("coroutine error: ",coroutine.resume(co));
println("coroutine state:\e[91m ",coroutine.status(co),"\e[0m");
println("coroutine yield: ",coroutine.resume(co));
println("coroutine state:\e[91m ",coroutine.status(co),"\e[0m");

var co=coroutine.create(func{
    var a=1;
    var b=func(){
        b();
    }
    coroutine.yield(b);
    coroutine.yield(b());
});

println("coroutine yield: ",coroutine.resume(co));
println("coroutine state:\e[32m ",coroutine.status(co),"\e[0m");
println("coroutine error: ",coroutine.resume(co));
println("coroutine state:\e[91m ",coroutine.status(co),"\e[0m");
println("coroutine yield: ",coroutine.resume(co));
println("coroutine state:\e[91m ",coroutine.status(co),"\e[0m");
println("ok");

# pressure test
var productor=func(){
    for(var i=0;;i+=1)
        coroutine.yield(i);
}
var total=1000; # ms
var co=coroutine.create(productor);
var tm=maketimestamp();

if(os.platform()=="windows"){
    system("chcp 65001");
    system("color");
}
var counter=0;
var bar=process_bar.high_resolution_bar(40);
var consumer=func(){
    counter+=1;
    for(var i=0;i<5;i+=1)
        coroutine.resume(co);
    var rate=(tm.elapsedMSec()+1)/total;
    print(bar.bar(rate)," ",rate*100,"%     \r");
}

tm.stamp();
while(tm.elapsedMSec()<total)
    consumer();
println("\nexecute ",counter," tasks during ",total," ms, avg ",counter/total," tasks/ms.");
