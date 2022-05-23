
var task={};
var event={};

var add_event=func(name,interval,function){
    event[name]=coroutine.create(func{
        var timestamp=maketimestamp();
        timestamp.stamp();
        while(timestamp.elapsedMSec()<interval*1000)
            coroutine.yield();
        timestamp.stamp();
        println("[\e[32m",name,"\e[0m]  type:\e[33mevent\e[0m  interval:\e[34m",interval,"\e[0m");
        function();
    });
}

var add_task=func(name,interval,function){
    task[name]=coroutine.create(func{
        var counter=0;
        var timestamp=maketimestamp();
        while(1){
            counter+=1;
            timestamp.stamp();
            while(timestamp.elapsedMSec()<interval*1000)
                coroutine.yield();
            timestamp.stamp();
            println("[\e[32m",name,"\e[0m]  type:\e[34mtask\e[0m  interval:\e[34m",interval,"\e[0m  invoke-time:\e[96m",counter,"\e[0m");
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


var maketimer_multi_coroutine_test=func(coroutine_size){
    var task_vec=[];
    setsize(task_vec,coroutine_size);
    forindex(var i;task_vec)
        task_vec[i]=func{};
    task_vec[coroutine_size-1]=func(){
        println("\e[101m",coroutine_size," tasks invoked.\e[0m");
    }
    var event_vec=[];
    setsize(event_vec,coroutine_size);
    forindex(var i;event_vec)
        event_vec[i]=func{};
    event_vec[coroutine_size-1]=func(){
        println("\e[101m",coroutine_size," events invoked.\e[0m");
    }

    var timer=[];
    setsize(timer,coroutine_size*2);
    forindex(var i;task_vec){
        timer[i]=maketimer((i+1)/5,task_vec[i]);
        timer[i].start();
    }
    forindex(var i;event_vec){
        timer[i+coroutine_size-1]=maketimer((i+1)/5,event_vec[i]);
        timer[i+coroutine_size-1].singleShot=1;
        timer[i+coroutine_size-1].start();
    }
    simulation();
}