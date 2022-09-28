import.stl.process_bar;
import.module.libkey;

var cpu_stat=func(){
    var cpu=split("\n",io.fin("/proc/stat"))[0];
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
    var cpu0=cpu_stat();
    unix.sleep(0.5);
    var cpu1=cpu_stat();
    var t0=cpu0.user+cpu0.nice+cpu0.system+cpu0.idle+cpu0.iowait+cpu0.irq+cpu0.softirq;
    var t1=cpu1.user+cpu1.nice+cpu1.system+cpu1.idle+cpu1.iowait+cpu1.irq+cpu1.softirq;
    var interval=cpu1.idle-cpu0.idle;
    return t0==t1?0:(1-interval/(t1-t0))*100;
}

var mem_occupation=func(){
    var meminfo=split("\n",io.fin("/proc/meminfo"));
    var mem_res={};
    forindex(var i;meminfo){
        var tmp=split(" ",meminfo[i])[0:1];
        tmp[0]=substr(tmp[0],0,size(tmp[0])-1);
        mem_res[tmp[0]]=num(tmp[1]);
    }
    return mem_res;
}
func(){
    if(os.platform()=="windows"){
        println("haven't supported yet.");
        return;
    }
    print("\ec");
    while(1){
        var mem=mem_occupation();
        var mem_occ=(mem.MemTotal-mem.MemFree)/mem.MemTotal*100;
        var cpu_occ=cpu_occupation();
        var key=libkey.nonblock();
        var bar=process_bar.high_resolution_bar(25);
        if(key!=nil and chr(key)=="q")
            break;
        println("\e[1;1H\e[1m Memory total(GB)    : \e[0m\e[36m",mem.MemTotal/1024/1024,"\e[0m");
        println("\e[2;1H\e[1m Memory free(GB)     : \e[0m\e[36m",mem.MemFree/1024/1024,"\e[0m");
        println("\e[3;1H\e[1m Memory occupation(%): \e[0m",mem_occ>60?"\e[91m":"\e[32m",bar.bar(mem_occ/100)~" ",mem_occ,"\e[0m         ");
        println("\e[4;1H\e[1m CPU occupation(%)   : \e[0m",cpu_occ>90?"\e[91m":"\e[32m",bar.bar(cpu_occ/100)~" ",cpu_occ,"\e[0m         ");
        println("\e[5;1H Press 'q' to quit.");
    }
}();