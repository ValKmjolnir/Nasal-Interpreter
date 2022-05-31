# flightgear developer environments simulator (beta)
# ValKmjolnir 2022

println("------------------------------------------------------------");
println("FlightGear simulated-env for developers project, since 2019");
println("Developed by:");
println("Sidi Liang (FGPRC-0762)");
println("Haokun Lee (FGPRC-0818 aka ValKmjolnir)");
println("------------------------------------------------------------");
println("[\e[32m fg_env    \e[0m] init begin");
println("[\e[32m maketimer \e[0m] init tasks");
println("[\e[32m maketimer \e[0m] init events");
var fg_globals={
    task:{},
    event:{}
};

println("[\e[32m maketimer \e[0m] new func add_event(name,interval,function)");
var add_event=func(name,interval,function){
    fg_globals.event[name]=coroutine.create(func{
        var timestamp=maketimestamp();
        timestamp.stamp();
        while(timestamp.elapsedMSec()<interval*1000)
            coroutine.yield();
        timestamp.stamp();
        println("[\e[32m",name,"\e[0m]  type:\e[33mevent\e[0m  interval:\e[34m",interval,"\e[0m");
        function();
    });
}

println("[\e[32m maketimer \e[0m] new func add_task(name,interval,function)");
var add_task=func(name,interval,function){
    fg_globals.task[name]=coroutine.create(func{
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

println("[\e[32m maketimer \e[0m] new func remove_task(name)");
var remove_task=func(name){
    if(contains(fg_globals.task,name))
        delete(fg_globals.task,name);
}

println("[\e[32m maketimer \e[0m] new func remove_event(name)");
var remove_event=func(name){
    if(contains(fg_globals.event,name))
        delete(fg_globals.event,name);
}

println("[\e[32m maketimer \e[0m] new func maketimer(interval,function)");
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

println("[\e[32m maketimer \e[0m] test func simulation()");
var simulation=func(){
    var running=1;
    while(running){
        running=0;
        foreach(var i;keys(fg_globals.task)){
            if(coroutine.resume(fg_globals.task[i])!=nil){
                running=1;
            }else{
                remove_task(i);
            }
        }
        foreach(var i;keys(fg_globals.event)){
            if(coroutine.resume(fg_globals.event[i])!=nil){
                running=1;
            }else{
                remove_event(i);
            }
        }
    }
}

println("[\e[32m maketimer \e[0m] test func maketimer_multi_coroutine_test(size)");
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

println("[\e[32m geodinfo  \e[0m] init geodinfo(lat,lon)");
var geodinfo=func(lat,lon){
    return [nil,{
        names:["Road","Freeway"]
    }];
}

println("[\e[32m props     \e[0m] init props base");
var props=
{
    globals:nil,
    Node:nil,
    getNode:func(path,index)
    {
        path=split('/',path);
        var tmp=me.globals;
        var path_size=size(path);
        for(var i=0;i<path_size-1;i+=1)
            tmp=tmp.val[path[i]];
        if(path_size>0)
        {
            if(contains(tmp.val,path[i]~'['~index~']'))
                return tmp.val[path[i]~'['~index~']'];
            else
                return tmp.val[path[i]];
        }
        return tmp;
    }
};

println("[\e[32m props     \e[0m] init props.Node");
props.Node=
{
    new:func(values=nil)
    {
        var res={
            parents:[props.Node],
            val:{},
            type:'GHOST',
            parent:nil
        };
        if(typeof(values)=="hash")
            res.val=values;
        return res;
    },
    addChild:func(name)
    {
        if(!contains(me.val,name))
        {
            me.val[name]=props.Node.new();
            me.val[name].parent=me;
            return 1;
        }
        return 0;
    },
    addChildren:func(name,cnt=0)
    {
        for(var i=0;i<cnt;i+=1)
        {
            var label=name~'['~i~']';
            me.val[label]=props.Node.new();
            me.val[label].parent=me;
        }
        return;
    },
    setValue:func(path,val)
    {
        path=split('/',path);
        var tmp=me;
        foreach(var label;path)
            tmp=tmp.val[label];
        tmp.val=val;
        if(typeof(val)=='str')
        {
            if(val=='true' or val=='false')
                tmp.type='BOOL';
            else
                tmp.type='STRING';
        }
        elsif(typeof(val)=='num')
            tmp.type='DOUBLE';
        return;
    },
    setIntValue:func(num)
    {
        me.val=num;
        me.type='INT';
        return;
    },
    setBoolValue:func(state)
    {
        me.val=state;
        me.type='BOOL';
        return;
    },
    setDoubleValue:func(num)
    {
        me.val=num;
        me.type='DOUBLE';
        return;
    },
    getValue:func(){return me.val;},
    getName:func()
    {
        var val=me.parent.val;
        var key=keys(val);
        foreach(var k;key)
            if(val[k]==me)
                return k;
        return '';
    },
    getParent:func()
    {
        return me.parent;
    },
    getPath:func()
    {
        if(me.parent==nil) return '';
        return me.parent.getPath()~'/'~me.getName();
    },
    equals:func(node){return me==node;},
    debug:func(s='')
    {
        if(typeof(me.val)=='hash')
        {
            var key=keys(me.val);
            if(!size(key))
            {
                println("\e[91m{}\e[0m");
                return;
            }
            println('\e[91m{\e[0m');
            foreach(var k;key)
            {
                print(s~"   ","\e[34m",k,"\e[0m\e[95m:\e[0m");
                me.val[k].debug(s~"   ");
            }
            println(s,'\e[91m}\e[0m');
        }
        else
            println("\e[35m",me.val,"\e[0m\e[33m(\e[0m\e[96m",me.type,'\e[0m\e[33m)\e[0m');
        return;
    }
};

println("[\e[32m props     \e[0m] init props.globals");
props.globals=props.Node.new();
var c=['aircraft','ai','models','position','orientation','controls','sim'];
foreach(var i;c)
    props.getNode('/',1).addChild(i);
props.getNode('/ai',1).addChildren('ai',4);
props.getNode('/aircraft',1).setValue('/','IDG MD-11');
for(var i=0;i<4;i+=1)
    props.getNode('/ai/ai['~i~']',1).setBoolValue('true');
props.getNode('/models',1).addChildren('building',4);
for(var i=0;i<4;i+=1)
    props.getNode('/models/building['~i~']',1).setIntValue(i);
props.getNode('/',1).addChild('test');
props.getNode('/test',1).addChildren('in',4);
props.getNode('/test/in',0).setValue('/','true');
props.getNode('/test/in',1).setValue('/','false');
props.getNode('/test/in',2).setValue('/','welcome aboard,need help? use help->tutorial');
props.getNode('/test/in',3).setValue('/',2147483648);

props.getNode("/sim",1).addChild("messages");
props.getNode("/sim/messages",1).addChild("copilot");
props.getNode("/position",1).addChild("latitude-deg");
props.getNode("/position",1).addChild("longitude-deg");
props.getNode("/orientation",1).addChild("heading-deg");
props.getNode("/controls",1).addChild("flight");
props.getNode("/controls/flight",1).addChild("rudder");

props.getNode("/sim/messages/copilot",1).setValue('/',"nothing");
props.getNode("/position/latitude-deg",1).setValue('/',90);
props.getNode("/position/longitude-deg",1).setValue('/',90);
props.getNode("/orientation/heading-deg",1).setValue('/',90);
props.getNode("/controls/flight/rudder",1).setValue('/',0.114);
println("[\e[32m props     \e[0m] init done");
println("[\e[32m fg_env    \e[0m] init done");
println("------------------------------------------------------------");

props.globals.debug();