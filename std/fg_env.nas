# flightgear developer environments simulator (beta)
# ValKmjolnir 2022
use std.runtime;
use std.coroutine;
use std.os;

println("-------------------------------------------------------------");
println(" FlightGear simulated-env for developers project, since 2019");
println(" Developed by:");
println(" Sidi762 (FGPRC-0762)");
println(" ValKmjolnir (FGPRC-0818)");
println("-------------------------------------------------------------");
println(" See help using command line argument: --fg-env-help");
println("-------------------------------------------------------------");

var fg_env_cli={
    "--fg-env-help":{
        info:"get help",
        trigger:0,
        f:func {
            if (me.trigger)
                return;
            println("-------------------------------------------------------------");
            println(" Help:");
            foreach(var i;keys(fg_env_cli))
                println(" ",i,": ",fg_env_cli[i].info);
            println("-------------------------------------------------------------");
            me.trigger=1;
        }
    },
    "--fg-env-debug":{
        info:"get property tree structure",
        trigger:0,
        f:func {
            if (me.trigger)
                return;
            props.globals.debug();
            me.trigger=1;
        }
    },
    "--fg-env-mktmtest":{
        info:"test maketimer",
        trigger:0,
        f:func {
            if (me.trigger)
                return;
            maketimer_multi_coroutine_test(32);
            me.trigger=1;
        }
    }
};

println("[\e[32m fg_env    \e[0m] [",os.time(),"] init begin");
println("[\e[32m maketimer \e[0m] [",os.time(),"] init tasks");
println("[\e[32m maketimer \e[0m] [",os.time(),"] init events");
var fg_globals={
    task:{},
    event:{}
};

println("[\e[32m maketimer \e[0m] [",os.time(),"] new func add_event(name,interval,function)");
var add_event = func(name,interval,function) {
    fg_globals.event[name]=coroutine.create(func {
        var timestamp=maketimestamp();
        timestamp.stamp();
        while(timestamp.elapsedMSec()<interval*1000)
            coroutine.yield();
        println("[\e[32m",name,"\e[0m] [",os.time(),"]  type:\e[33mevent\e[0m  interval:\e[34m",interval,"\e[0m");
        function();
    });
}

println("[\e[32m maketimer \e[0m] [",os.time(),"] new func add_task(name,interval,function)");
var add_task = func(name,interval,function) {
    fg_globals.task[name]=coroutine.create(func {
        var counter=0;
        var timestamp=maketimestamp();
        while(1) {
            counter+=1;
            timestamp.stamp();
            while(timestamp.elapsedMSec()<interval*1000)
                coroutine.yield();
            println("[\e[32m",name,"\e[0m] [",os.time(),"]  type:\e[34mtask\e[0m  interval:\e[34m",interval,"\e[0m  invoke-time:\e[96m",counter,"\e[0m");
            function();
            coroutine.yield();
        }
    });
}

println("[\e[32m maketimer \e[0m] [",os.time(),"] new func remove_task(name)");
var remove_task = func(name) {
    if (contains(fg_globals.task,name))
        delete(fg_globals.task,name);
}

println("[\e[32m maketimer \e[0m] [",os.time(),"] new func remove_event(name)");
var remove_event = func(name) {
    if (contains(fg_globals.event,name))
        delete(fg_globals.event,name);
}

println("[\e[32m maketimer \e[0m] [",os.time(),"] new func maketimer(interval,function)");
var maketimer = func(interval,function) {
    var name="nasal-timer-";
    var res={
        start:func {
            if (me.isRunning)
                return;
            me.isRunning=1;
            if (me.singleShot) {
                add_event(name,interval,function);
            } else {
                add_task(name,interval,function);
            }
        },
        stop:func {
            if (me.isRunning) {
                remove_task(name);
                me.isRunning=0;
            }
        },
        restart:func(itv) {
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

println("[\e[32m settimer  \e[0m] [",os.time(),"] new func settimer(function,interval,rt)");
var settimer = func() {
    var index=0;
    return func(function,interval,realtime=1) {
        var name="nasal-settimer-"~index;
        index+=1;
        add_task(name,interval,function);
    }
}();

println("[\e[32m maketimer \e[0m] [",os.time(),"] test func simulation()");
var simulation = func() {
    var running=1;
    while(running) {
        running=0;
        foreach(var i;keys(fg_globals.task)) {
            if (!contains(fg_globals.task,i))
                continue;
            if (coroutine.resume(fg_globals.task[i])!=nil) {
                running=1;
            } else {
                remove_task(i);
            }
        }
        foreach(var i;keys(fg_globals.event)) {
            if (!contains(fg_globals.event,i))
                continue;
            if (coroutine.resume(fg_globals.event[i])!=nil) {
                running=1;
            } else {
                remove_event(i);
            }
        }
    }
}

println("[\e[32m maketimer \e[0m] [",os.time(),"] test func maketimer_multi_coroutine_test(size)");
var maketimer_multi_coroutine_test = func(coroutine_size) {
    if (coroutine_size<1)
        return;
    var task_vec=[];
    setsize(task_vec,coroutine_size);
    forindex(var i;task_vec)
        task_vec[i] = func {};
    task_vec[coroutine_size-1] = func {
        println("\e[101m",coroutine_size," tasks invoked.\e[0m");
        forindex(var i;task_vec)
            task_vec[i].stop();
    }
    var event_vec=[];
    setsize(event_vec,coroutine_size);
    forindex(var i;event_vec)
        event_vec[i] = func {};
    event_vec[coroutine_size-1] = func {
        println("\e[101m",coroutine_size," events invoked.\e[0m");
    }
    var set_vec=[];
    setsize(set_vec,coroutine_size);
    forindex(var i;set_vec)
        set_vec[i] = func {};
    set_vec[coroutine_size-1] = func {
        println("\e[101m",coroutine_size," settimer invoked.\e[0m");
    }

    forindex(var i;task_vec) {
        task_vec[i]=maketimer((i+1)/10,task_vec[i]);
        task_vec[i].start();
    }
    forindex(var i;event_vec) {
        event_vec[i]=maketimer((i+1)/10,event_vec[i]);
        event_vec[i].singleShot=1;
        event_vec[i].start();
    }
    #forindex(var i;set_vec)
    #    settimer(set_vec[i],(i+1)/10);
    simulation();
}

println("[\e[32m geodinfo  \e[0m] [",os.time(),"] init geodinfo(lat,lon)");
var geodinfo = func(lat,lon) {
    return [nil,{
        names:["Road","Freeway"]
    }];
}

println("[\e[32m props     \e[0m] [",os.time(),"] init props");
var props={
    globals:nil,
    Node:nil,
    getNode:func(path,index) {
        path=split('/',path);
        var tmp=me.globals;
        var path_size=size(path);
        for(var i=0;i<path_size-1;i+=1)
            tmp=tmp.val[path[i]];
        if (path_size>0) {
            if (contains(tmp.val,path[i]~'['~index~']'))
                return tmp.val[path[i]~'['~index~']'];
            else
                return tmp.val[path[i]];
        }
        return tmp;
    }
};

println("[\e[32m props     \e[0m] [",os.time(),"] init props.Node");

props.Node={
    new:func(values=nil) {
        var res={
            parents:fg_env_props_node_traits,
            val:{},
            type:'GHOST',
            parent:nil
        };
        if (typeof(values)=="hash")
            res.val=values;
        return res;
    },
    addChild:func(name) {
        if (!contains(me.val,name)) {
            me.val[name]=props.Node.new();
            me.val[name].parent=me;
            return 1;
        }
        return 0;
    },
    addChildren:func(name,cnt=0) {
        for(var i=0;i<cnt;i+=1) {
            var label=name~'['~i~']';
            me.val[label]=props.Node.new();
            me.val[label].parent=me;
        }
        return;
    },
    setValue:func(path,val) {
        path=split('/',path);
        var tmp=me;
        foreach(var label;path)
            tmp=tmp.val[label];
        tmp.val=val;
        if (typeof(val)=='str') {
            if (val=='true' or val=='false')
                tmp.type='BOOL';
            else
                tmp.type='STRING';
        }
        elsif (typeof(val)=='num')
            tmp.type='DOUBLE';
        return;
    },
    setIntValue:func(num) {
        me.val=num;
        me.type='INT';
        return;
    },
    setBoolValue:func(state) {
        me.val=state;
        me.type='BOOL';
        return;
    },
    setDoubleValue:func(num) {
        me.val=num;
        me.type='DOUBLE';
        return;
    },
    getValue:func() {return me.val;},
    getName:func() {
        var val=me.parent.val;
        foreach(var k;keys(val))
            if (val[k]==me)
                return k;
        return '';
    },
    getParent:func() {
        return me.parent;
    },
    getPath:func() {
        if (me.parent==nil) return '';
        return me.parent.getPath()~'/'~me.getName();
    },
    equals:func(node) {return me==node;},
    debug:func(s='') {
        if (typeof(me.val)=='hash') {
            var key=keys(me.val);
            if (!size(key)) {
                println("\e[91m{}\e[0m");
                return;
            }
            println('\e[91m{\e[0m');
            foreach(var k;key) {
                print(s~"   ","\e[34m",k,"\e[0m\e[95m:\e[0m");
                me.val[k].debug(s~"   ");
            }
            println(s,'\e[91m}\e[0m');
        }
        else
            println("\e[35m ",me.val,"\e[0m\e[33m(\e[0m\e[96m",me.type,'\e[0m\e[33m)\e[0m');
        return;
    }
};
var fg_env_props_node_traits=[props.Node];

println("[\e[32m props     \e[0m] [",os.time(),"] init props.globals");
props.globals=props.Node.new();
var c=['aircraft','ai','models','position','orientation','controls','sim','consumables','engines','velocities','accelerations','gear','instrumentation','rotors'];
foreach(var i;c)
    props.getNode('/',1).addChild(i);

props.getNode('/ai',1).addChildren('ai',4);
props.getNode('/aircraft',1).setValue('/','IDG MD-11');
props.getNode('/models',1).addChildren('building',4);
for(var i=0;i<4;i+=1)
    props.getNode('/models/building['~i~']',1).setIntValue(i);

println("[\e[32m props     \e[0m] [",os.time(),"] init /consumables");
props.getNode("/consumables",1).addChild("fuel");
props.getNode("/consumables/fuel",1).addChild("total-fuel-lbs");
props.getNode("/consumables/fuel",1).addChild("total-gal_us");
props.getNode("/consumables/fuel/total-fuel-lbs",1).setValue('/',0);
props.getNode("/consumables/fuel/total-gal_us",1).setValue('/',0);
props.getNode("/consumables/fuel",1).addChildren("tank",4);
for(var i=0;i<4;i+=1) {
    props.getNode("/consumables/fuel/tank["~i~"]",1).addChild("level-lb");
    props.getNode("/consumables/fuel/tank["~i~"]",1).addChild("level-lbs");
    props.getNode("/consumables/fuel/tank["~i~"]",1).addChild("level-gal_us");
    props.getNode("/consumables/fuel/tank["~i~"]",1).addChild("capacity-gal_us");
    props.getNode("/consumables/fuel/tank["~i~"]",1).addChild("density-ppg");

    props.getNode("/consumables/fuel/tank["~i~"]/level-lb",1).setValue('/',0);
    props.getNode("/consumables/fuel/tank["~i~"]/level-lbs",1).setValue('/',0);
    props.getNode("/consumables/fuel/tank["~i~"]/level-gal_us",1).setValue('/',0);
    props.getNode("/consumables/fuel/tank["~i~"]/capacity-gal_us",1).setValue('/',0);
    props.getNode("/consumables/fuel/tank["~i~"]/density-ppg",1).setValue('/',0);
}

println("[\e[32m props     \e[0m] [",os.time(),"] init /controls");
foreach(var i;['anti-ice','APU','armament','autoflight','electric','engines','flight','fuel','gear','hydraulic','lighting','pneumatic','pressurization','seat'])
    props.getNode("/controls",1).addChild(i);

println("[\e[32m props     \e[0m] [",os.time(),"] init /controls/anti-ice");
foreach(var i;['wing-heat','pitot-heat','wiper','window-heat'])
    props.getNode("/controls/anti-ice",1).addChild(i);
props.getNode("/controls/anti-ice",1).addChildren("engine",2);
for(var i=0;i<2;i+=1) {
    props.getNode("/controls/anti-ice/engine["~i~"]",1).addChild("carb-heat");
    props.getNode("/controls/anti-ice/engine["~i~"]",1).addChild("inlet-heat");
}

println("[\e[32m props     \e[0m] [",os.time(),"] init /controls/APU");
props.getNode("/controls/APU",1).addChild("off-start-run");
props.getNode("/controls/APU",1).addChild("fire-switch");

println("[\e[32m props     \e[0m] [",os.time(),"] init /controls/armament");
props.getNode("/controls/armament",1).addChild("master-arm");
props.getNode("/controls/armament",1).addChild("station-select");
props.getNode("/controls/armament",1).addChild("release-all");
props.getNode("/controls/armament",1).addChildren("station",4);
for(var i=0;i<4;i+=1) {
    props.getNode("/controls/armament/station["~i~"]",1).addChild("stick-size");
    props.getNode("/controls/armament/station["~i~"]",1).addChild("release-stick");
    props.getNode("/controls/armament/station["~i~"]",1).addChild("release-all");
    props.getNode("/controls/armament/station["~i~"]",1).addChild("jettison-all");
}

println("[\e[32m props     \e[0m] [",os.time(),"] init /controls/autoflight");
foreach(var i;['autothrottle-arm','autothrottle-engage','heading-select','altitude-select','bank-angle-select','vertical-speed-select','speed-select','mach-select','vertical-mode','lateral-mode'])
    props.getNode("/controls/autoflight",1).addChild(i);
props.getNode("/controls/autoflight",1).addChildren("autopilot",2);
for(var i=0;i<2;i+=1)
    props.getNode("/controls/autoflight/autopilot["~i~"]",1).addChild("engage");

println("[\e[32m props     \e[0m] [",os.time(),"] init /controls/electric");
foreach(var i;['battery-switch','external-power','APU-generator'])
    props.getNode("/controls/electric",1).addChild(i);
props.getNode("/controls/electric",1).addChildren("engine",2);
for(var i=0;i<2;i+=1) {
    props.getNode("/controls/electric/engine["~i~"]",1).addChild("generator");
    props.getNode("/controls/electric/engine["~i~"]",1).addChild("bus-tie");
}

println("[\e[32m props     \e[0m] [",os.time(),"] init /controls/engines");
props.getNode("/controls/engines",1).addChild("throttle-idle");
props.getNode("/controls/engines",1).addChildren("engine",2);
for(var i=0;i<2;i+=1)
    foreach(var j;['throttle','starter','fuel-pump','fire-switch','fire-bottle-discharge','cutoff','mixture','propeller-pitch','magnetos','boost','WEP','cowl-flaps-norm','feather','ignition','augmentation','afterburner','reverser','water-injection','condition'])
        props.getNode("/controls/engines/engine["~i~"]",1).addChild(j);

println("[\e[32m props     \e[0m] [",os.time(),"] init /controls/flight");
foreach(var i;['aileron','aileron-trim','elevator','elevator-trim','rudder','rudder-trim','flaps','slats','BLC','spoilers','speedbrake','wing-sweep','wing-fold','drag-chute'])
    props.getNode("/controls/flight",1).addChild(i);

println("[\e[32m props     \e[0m] [",os.time(),"] init /controls/fuel");
props.getNode("/controls/fuel",1).addChild("dump-value");
props.getNode("/controls/fuel",1).addChildren("tank",4);
for(var i=0;i<4;i+=1) {
    foreach(var j;['fuel-selector','to_engine','to_tank'])
        props.getNode("/controls/fuel/tank["~i~"]",1).addChild(j);
    props.getNode("/controls/fuel/tank["~i~"]",1).addChildren("boost-pump",4);
}

println("[\e[32m props     \e[0m] [",os.time(),"] init /controls/gear");
foreach(var i;['brake-left','brake-right','brake-parking','steering','gear-down','antiskid','tailhook','tailwheel-lock'])
    props.getNode("/controls/gear",1).addChild(i);
props.getNode("/controls/gear",1).addChildren("wheel",4);
for(var i=0;i<4;i+=1)
    props.getNode("/controls/gear/wheel["~i~"]",1).addChild("alternate-extension");

println("[\e[32m props     \e[0m] [",os.time(),"] init /controls/hydraulic");
props.getNode("/controls/hydraulic",1).addChildren("system",2);
for(var i=0;i<2;i+=1) {
    props.getNode("/controls/hydraulic/system["~i~"]",1).addChild("engine-pump");
    props.getNode("/controls/hydraulic/system["~i~"]",1).addChild("electric-pump");
}

println("[\e[32m props     \e[0m] [",os.time(),"] init /controls/lighting");
foreach(var i;['landing-lights','turn-off-lights','formation-lights','taxi-light','logo-lights','nav-lights','beacon','strobe','panel-norm','instruments-norm','dome-norm'])
    props.getNode("/controls/lighting",1).addChild(i);

println("[\e[32m props     \e[0m] [",os.time(),"] init /controls/pneumatic");
props.getNode("/controls/pneumatic",1).addChild("APU-bleed");
props.getNode("/controls/pneumatic",1).addChildren("engine",2);
for(var i=0;i<2;i+=1)
    props.getNode("/controls/pneumatic/engine["~i~"]",1).addChild("bleed");

println("[\e[32m props     \e[0m] [",os.time(),"] init /controls/pressurization");
foreach(var i;['mode','dump','outflow-valve'])
    props.getNode("/controls/pressurization",1).addChild(i);
props.getNode("/controls/pressurization",1).addChildren("pack",4);
for(var i=0;i<4;i+=1)
    props.getNode("/controls/pressurization/pack["~i~"]",1).addChild("pack-on");

println("[\e[32m props     \e[0m] [",os.time(),"] init /controls/seat");
foreach(var i;['vertical-adjust','fore-aft-adjust','cmd_selector_valve'])
    props.getNode("/controls/seat",1).addChild(i);
props.getNode("/controls/seat",1).addChildren("eject",3);
for(var i=0;i<3;i+=1) {
    props.getNode("/controls/seat/eject["~i~"]",1).addChild("initiate");
    props.getNode("/controls/seat/eject["~i~"]",1).addChild("status");
}

println("[\e[32m props     \e[0m] [",os.time(),"] init /engines");
props.getNode("/engines",1).addChildren("engine",2);
for(var i=0;i<2;i+=1)
    foreach(var j;
        ['fuel-flow-gph','fuel-flow-pph','thrust_lb','running','starter','cranking',
        'n1','n2','epr','augmentation','water-injection','ignition','nozzle-pos-norm',
        'inlet-pos-norm','reversed','cutoff','mp-osi','egt-degf','oil-temperature-degf',
        'oil-pressure-psi','cht-degf','rpm','pitch','torque'])
        props.getNode("/engines/engine["~i~"]",1).addChild(j);

println("[\e[32m props     \e[0m] [",os.time(),"] init /position");
foreach(var i;['','altitude-agl-ft','altitude-ft','ground-elev-ft','ground-elev-m','latitude-deg','latitude-string','longitude-deg','longitude-string','sea-level-radius-ft'])
    props.getNode("/position",1).addChild(i);

println("[\e[32m props     \e[0m] [",os.time(),"] init /orientation");
foreach(var i;['roll-deg','pitch-deg','heading-deg','roll-rate-degps','pitch-rate-degps','yaw-rate-degps','side-slip-rad','side-slip-deg','alpha-deg'])
    props.getNode("/orientation",1).addChild(i);

println("[\e[32m props     \e[0m] [",os.time(),"] init /velocities");
foreach(var i;['airspeed-kt','mach','speed-north-fps','speed-east-fps','speed-down-fps','uBody-fps','vBody-fps','wBody-fps','vertical-speed-fps','glideslope'])
    props.getNode("/velocities",1).addChild(i);

println("[\e[32m props     \e[0m] [",os.time(),"] init /accelerations");
foreach(var i;['nlf','ned','pilot'])
    props.getNode("/accelerations",1).addChild(i);
foreach(var i;['north-accel-fps_sec','east-accel-fps_sec','down-accel-fps_sec'])
    props.getNode("/accelerations/ned",1).addChild(i);
foreach(var i;['x-accel-fps_sec','y-accel-fps_sec','z-accel-fps_sec'])
    props.getNode("/accelerations/pilot",1).addChild(i);

println("[\e[32m props     \e[0m] [",os.time(),"] init /gear");
props.getNode("/gear",1).addChild("serviceable");
props.getNode("/gear",1).addChildren("gear",4);
for(var i=0;i<4;i+=1)
    foreach(var j;['cast-angle-deg','compression-m','compression-norm','ground-friction-factor','ground-is-solid','has-brake','rollspeed-ms','wow','xoffset-in','yoffset-in','zoffset-in'])
        props.getNode("/gear/gear["~i~"]",1).addChild(j);

println("[\e[32m props     \e[0m] [",os.time(),"] init /instrumentation");
foreach(var i;['adf','airspeed-indicator','altimeter','annunciator','clock','comm','comm[1]','dme','efis','encoder','flightdirector','gps','gps-annunciator','heading-indicator','heading-indicator-fg','magnetic-compass','marker-beacon','nav','nav[1]','radar','slip-skid-ball','tacan','transponder','turn-indicator','vertical-speed-indicator','wxradar'])
    props.getNode("/instrumentation",1).addChild(i);

println("[\e[32m props     \e[0m] [",os.time(),"] init /rotors");
foreach(var i;['gear','{name}'])
    props.getNode("/rotors",1).addChild(i);
foreach(var i;['torque-sound-filtered','total-torque'])
    props.getNode("/rotors/gear",1).addChild(i);
foreach(var i;['balance','bladesvisible','cone-deg','cone2-deg','roll-deg','rpm','stall','stall-filtered','tilt','torque','yaw-deg'])
    props.getNode("/rotors/{name}",1).addChild(i);
props.getNode("/rotors/{name}",1).addChildren("blade",8);
for(var i=0;i<8;i+=1)
    foreach(var j;['flap-deg','incidence-deg','position-deg'])
        props.getNode("/rotors/{name}/blade["~i~"]",1).addChild(j);

props.getNode("/sim",1).addChild("messages");
props.getNode("/sim",1).addChild("fg-home");
props.getNode("/sim/messages",1).addChild("copilot");

props.getNode("/sim/messages/copilot",1).setValue('/',"nothing");
props.getNode("/position/latitude-deg",1).setValue('/',90);
props.getNode("/position/longitude-deg",1).setValue('/',90);
props.getNode("/position/altitude-ft",1).setValue('/',28.244);
props.getNode("/position/altitude-agl-ft",1).setValue('/',22.4704);
props.getNode("/orientation/heading-deg",1).setValue('/',90);
props.getNode("/controls/flight/rudder",1).setValue('/',0.114);

func() {
    srand();
    var tmp=nil;
    var vec=[props.globals];
    while(size(vec)) {
        tmp=[];
        foreach(var i;vec) {
            if (typeof(i.val)=="hash") {
                if (size(i.val)==0) {
                    i.setDoubleValue(rand()*10);
                } else {
                    foreach(var j;keys(i.val))
                        append(tmp,i.val[j]);
                }
            }
        }
        vec=tmp;
    }
}();

println("[\e[32m props     \e[0m] [",os.time(),"] init done");
println("[\e[32m fg_env    \e[0m] [",os.time(),"] init done");
println("-------------------------------------------------------------");

foreach(var a;runtime.argv())
    if (contains(fg_env_cli,a)) {
        fg_env_cli[a].f();
    }
