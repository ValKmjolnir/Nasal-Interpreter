import.stl.process_bar;
import.module.libkey;

var is_windows_platform=os.platform()=="windows";
var is_macos_platform=os.platform()=="macOS";

if(is_windows_platform){
    system("chcp 65001");
}

var cpu_stat=func(){
    if(is_windows_platform or is_macos_platform)
        return nil;
    var cpu=split("\n",io.readfile("/proc/stat"))[0];
    cpu=split(" ",cpu);
    cpu={
        name:cpu[0],
        user:cpu[1],
        nice:cpu[2],
        system:cpu[3],
        idle:cpu[4],
        iowait:cpu[5],
        irq:cpu[6],
        softirq:cpu[7],
    };
    return cpu;
}

var cpu_occupation=func(){
    var first_in=1;
    while(1){
        var cpu0=cpu_stat();
        if(first_in){
            unix.sleep(0.1);
            first_in=0;
        }else{
            for(var i=0;i<10;i+=1){
                unix.sleep(0.1);
                coroutine.yield(nil);
            }
        }
        var cpu1=cpu_stat();
        if(is_windows_platform or is_macos_platform){
            coroutine.yield(0);
            continue;
        }
        var t0=cpu0.user+cpu0.nice+cpu0.system+cpu0.idle+cpu0.iowait+cpu0.irq+cpu0.softirq;
        var t1=cpu1.user+cpu1.nice+cpu1.system+cpu1.idle+cpu1.iowait+cpu1.irq+cpu1.softirq;
        var interval=cpu1.idle-cpu0.idle;
        coroutine.yield(t0==t1?0:(1-interval/(t1-t0))*100);
    }
}

var mem_occupation=func(){
    if(is_windows_platform or is_macos_platform)
        return {MemTotal:math.inf,MemFree:math.inf};
    var meminfo=split("\n",io.readfile("/proc/meminfo"));
    var mem_res={};
    forindex(var i;meminfo){
        var tmp=split(" ",meminfo[i])[0:1];
        tmp[0]=substr(tmp[0],0,size(tmp[0])-1);
        mem_res[tmp[0]]=num(tmp[1]);
    }
    return mem_res;
}

var random_generator=func(){
    var rise=[" ","▁","▂","▃","▄","▅","▆","▇","█"];
    var total=0;
    var statistics=[];
    setsize(statistics,70);
    while(1){
        for(var i=0;i<10;i+=1){
            total+=1;
            var u=rand()*rand()*(rand()>0.5?-1:1);
            statistics[int(size(statistics)/2+u*size(statistics)/2)]+=1;
        }
        var s=["","",""];
        foreach(var st;statistics){
            var max_rate=100/size(statistics);
            var rate=st/total*100;
            for(var i=size(s)-1;i>=0;i-=1){
                if(rate>=max_rate){
                    s[i]~="█";
                    rate-=max_rate;
                }else{
                    s[i]~=rise[rate/max_rate*size(rise)];
                    rate=0;
                }
            }
        }
        var tmp="";
        for(var i=0;i<size(statistics);i+=1){
            tmp~="-";
        }
        println("\e[16;1H \e[32m|",s[0],"|\e[0m");
        println("\e[17;1H \e[32m|",s[1],"|\e[0m");
        println("\e[18;1H \e[32m|",s[2],"|\e[0m");
        println("\e[19;1H \e[32m+"~tmp~"+\e[0m");
        coroutine.yield();
    }
}

func(){
    var limited_loop=(size(runtime.argv())!=0 and !math.isnan(num(runtime.argv()[0])));
    if(limited_loop){
        limited_loop=num(runtime.argv()[0]);
    }else{
        limited_loop=-1;
    }

    var rise=[" ","▁","▂","▃","▄","▅","▆","▇","█"];

    var cpu_occupation_log=[];
    var mem_occupation_log=[];
    setsize(cpu_occupation_log,70);
    setsize(mem_occupation_log,70);

    var co=coroutine.create(cpu_occupation);
    var rd=coroutine.create(random_generator);

    var bar=process_bar.high_resolution_bar(48);
    print("\ec");
    while(limited_loop!=0){
        limited_loop=limited_loop<0?limited_loop:limited_loop-1;
        var mem=mem_occupation();
        var mem_occ=(mem.MemTotal-mem.MemFree)/mem.MemTotal*100;
        if(math.isnan(mem_occ) or mem_occ<0 or mem_occ>100){
            mem_occ=0;
        }
        var cpu_occ=nil;
        while((cpu_occ=coroutine.resume(co)[0])==nil){
            var key=libkey.nonblock();
            coroutine.resume(rd);
            if(key!=nil and chr(key)=="q")
                return;
        }

        if(is_windows_platform or is_macos_platform){
            # sorry this is not real data
            cpu_occ=rand()*10;
            mem_occ=rand()*10+40;
        }

        for(var i=0;i<size(cpu_occupation_log)-1;i+=1){
            cpu_occupation_log[i]=cpu_occupation_log[i+1];
            mem_occupation_log[i]=mem_occupation_log[i+1];
        }
        cpu_occupation_log[-1]=cpu_occ;
        mem_occupation_log[-1]=mem_occ;

        println("\e[1;1H\e[1m Operating system    : \e[0m",is_windows_platform?"\e[31m":"\e[36m",os.platform()," ",os.arch(),"\e[0m");
        println("\e[2;1H\e[1m Current time        : \e[0m\e[36m",os.time(),"\e[0m");
        println("\e[3;1H\e[1m Memory total(GB)    : \e[0m\e[36m",mem.MemTotal/1024/1024,"\e[0m");
        println("\e[4;1H\e[1m Memory free(GB)     : \e[0m\e[36m",mem.MemFree/1024/1024,"\e[0m");
        println("\e[5;1H\e[1m Memory occupation(%): \e[0m",mem_occ>60?"\e[91m":"\e[32m",bar.bar(mem_occ/100)~" ",mem_occ,"\e[0m         ");
        println("\e[6;1H\e[1m CPU occupation(%)   : \e[0m",cpu_occ>90?"\e[91m":"\e[32m",bar.bar(cpu_occ/100)~" ",cpu_occ,"\e[0m         ");

        var tmp="";
        for(var i=0;i<70;i+=1){
            tmp~="-";
        }

        var s=["","",""];
        foreach(var occ;cpu_occupation_log){
            var max_rate=50/size(s);
            var rate=occ;
            for(var i=size(s)-1;i>=0;i-=1){
                if(rate>=max_rate){
                    s[i]~="█";
                    rate-=max_rate;
                }else{
                    s[i]~=rise[rate/max_rate*size(rise)];
                    rate=0;
                }
            }
        }
        println("\e[7;1H \e[32m+"~tmp~"+\e[0m");
        println("\e[8;1H \e[32m|",s[0],"|\e[0m");
        println("\e[9;1H \e[32m|",s[1],"|\e[0m");
        println("\e[10;1H \e[32m|",s[2],"|\e[0m");
        println("\e[11;1H \e[32m+"~tmp~"+\e[0m");

        var s=["","",""];
        foreach(var occ;mem_occupation_log){
            var max_rate=100/size(s);
            var rate=occ;
            for(var i=size(s)-1;i>=0;i-=1){
                if(rate>=max_rate){
                    s[i]~="█";
                    rate-=max_rate;
                }else{
                    s[i]~=rise[rate/max_rate*size(rise)];
                    rate=0;
                }
            }
        }
        println("\e[12;1H \e[32m|",s[0],"|\e[0m");
        println("\e[13;1H \e[32m|",s[1],"|\e[0m");
        println("\e[14;1H \e[32m|",s[2],"|\e[0m");
        println("\e[15;1H \e[32m+"~tmp~"+\e[0m");

        println("\e[20;1H Press 'q' to quit.");
    }
}();