
var process=nil;
var task={};
var event={};

var add_event=func(name,interval,function){
    event[name]=coroutine.create(func{
        unix.sleep(interval);
        println("[event] ",name);
        function();
    });
}

var add_task=func(name,interval,function){
    
    task[name]=coroutine.create(func{
        while(1){
            unix.sleep(interval);
            println("[task] ",name);
            function();
            coroutine.yield();
        }
    });
}

var remove_task=func(name){
    if(contains(task,name))
        delete(task,name);
}

var remove_event=func(name){
    if(contains(event,name))
        delete(event,name);
}

var maketimer=func(interval,function){
    var name="nasal-timer-";
    var res={
        start:func{
            if(me.isRunning)
                return;
            me.isRunning=1;
            if(me.singleShot){
                add_event(name,interval,function);
            }else{
                add_task(name,interval,function);
            }
        },
        stop:func{
            if(me.isRunning){
                remove_task(name);
                me.isRunning=0;
            }
        },
        restart:func(itv){
            interval=itv;
            me.stop();
            me.start();
        },
        singleShot:0,
        isRunning:0,
        simulatedTime:0
    };
    name~=id(res);
    return res;
}

var simulation=func(){
    var running=1;
    while(running){
        running=0;
        foreach(var i;keys(task)){
            if(coroutine.resume(task[i])!=nil){
                running=1;
            }else{
                remove_task(i);
            }
        }
        foreach(var i;keys(event)){
            if(coroutine.resume(event[i])!=nil){
                running=1;
            }else{
                remove_event(i);
            }
        }
    }
}
