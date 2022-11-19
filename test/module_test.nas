var libfib=func(){
    var (dd,fib,qfib)=(nil,nil,nil);
    var invoke=dylib.limitcall(1);
    return {
        open:func(){
            if(dd==nil){
                dd=dylib.dlopen("libfib."~(os.platform()=="windows"?"dll":"so"));
                fib=dd.fib;
                qfib=dd.quick_fib;
            }else{
                println("[info  ] already loaded.");
            }
        },
        close:func(){
            if(dd==nil){
                println("[error ] already closed.");
                return;
            }
            dylib.dlclose(dd.lib);
            (dd,fib,qfib)=(nil,nil,nil);
        },
        fib:func(x){
            if(fib!=nil)
                return invoke(fib,x);
            println("[error ] cannot call fib.");
            return nil;
        },
        qfib:func(x){
            if(qfib!=nil)
                return invoke(qfib,x);
            println("[error ] cannot call qfib.");
            return nil;
        }
    }
}();

println("[keys  ] ",keys(libfib));
libfib.open();
libfib.open();
var tm=maketimestamp();
tm.stamp();
println("[result] ",libfib.fib(40));
println("[time  ] ",tm.elapsedMSec()," ms");
tm.stamp();
println("[result] ",libfib.qfib(40));
println("[time  ] ",tm.elapsedMSec()," ms");
libfib.close();
println("[result] ",libfib.fib(40));
println("[result] ",libfib.qfib(40));
libfib.close();

var speed_test=func(){
    var d=dylib.dlopen("libfib."~(os.platform()=="windows"?"dll":"so"));
    println("[dylib ] ",d);
    var fd=d.quick_fib;
    var vec_call=dylib.dlcall;
    var invoke=dylib.limitcall(1);
    var tm=maketimestamp();
    var duration=0;

    for(var t=0;t<10;t+=1){
        tm.stamp();
        for(var i=0;i<5e6;i+=1)
            invoke(fd,40);
        duration=tm.elapsedMSec();
        println("[time  ] limited call: ",duration," ms avg ",5e6/duration," call/ms");
        tm.stamp();
        for(var i=0;i<5e6;i+=1)
            vec_call(fd,40);
        duration=tm.elapsedMSec();
        println("[time  ] dynamic call: ",duration," ms avg ",5e6/duration," call/ms");
    }
}

speed_test();