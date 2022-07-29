var libfib=func(){
    var (dd,fib,qfib)=(nil,nil,nil);
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
                return dylib.dlcall(fib,x);
            println("[error ] cannot call fib.");
            return nil;
        },
        qfib:func(x){
            if(qfib!=nil)
                return dylib.dlcall(qfib,x);
            println("[error ] cannot call qfib.");
            return nil;
        }
    }
}();

println("[keys  ] ",keys(libfib));
libfib.open();
libfib.open();
println("[result] ",libfib.fib(40));
println("[result] ",libfib.qfib(40));
libfib.close();
println("[result] ",libfib.fib(40));
println("[result] ",libfib.qfib(40));
libfib.close();