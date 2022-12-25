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
println("[result] ",libfib.fib(35));
println("[time  ] ",tm.elapsedMSec()," ms");
tm.stamp();
println("[result] ",libfib.qfib(35));
println("[time  ] ",tm.elapsedMSec()," ms");
libfib.close();
println("[result] ",libfib.fib(35));
println("[result] ",libfib.qfib(35));
libfib.close();

var speed_test=func(){
    var d=dylib.dlopen("libfib."~(os.platform()=="windows"?"dll":"so"));
    println("[dylib ] ",d);
    var fd=d.quick_fib;
    var vec_call=dylib.dlcall;
    var invoke=dylib.limitcall(1);
    var tm=maketimestamp();

    for(var t=0;t<10;t+=1){
        tm.stamp();
        for(var i=0;i<5e5;i+=1){
            invoke(fd,40);
        }
        println("[time  ] limited call: ",int(5e5/tm.elapsedMSec())," call/ms");
        tm.stamp();
        for(var i=0;i<5e5;i+=1){
            vec_call(fd,40);
        }
        println("[time  ] dynamic call: ",int(5e5/tm.elapsedMSec())," call/ms");
    }
}

speed_test();