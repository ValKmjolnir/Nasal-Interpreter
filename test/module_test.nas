var libfib=func(){
    var (dd,fib,qfib)=(nil,nil,nil);
    var invoke=dylib.limitcall(1);
    return {
        open:func(){
            if(dd==nil){
                dd=dylib.dlopen("libfib."~(os.platform()=="windows"?"dll":"so"));
                fib=dylib.dlsym(dd,"fib");
                qfib=dylib.dlsym(dd,"quick_fib");
            }else{
                println("[info  ] already loaded.");
            }
        },
        close:func(){
            if(dd==nil){
                println("[error ] already closed.");
                return;
            }
            dylib.dlclose(dd);
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
    var fd=dylib.dlsym(d,"quick_fib");
    var vec_call=dylib.dlcall;
    var invoke=dylib.limitcall(1);

    var tm=maketimestamp();
    tm.stamp();
    for(var i=0;i<1e7;i+=1)
        invoke(fd,40);
    println("[time  ] limited call: ",tm.elapsedMSec()," ms");
    tm.stamp();
    for(var i=0;i<1e7;i+=1)
        vec_call(fd,40);
    println("[time  ] dynamic call: ",tm.elapsedMSec()," ms");
}

speed_test();