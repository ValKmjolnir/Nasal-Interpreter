var geodinfo=func(lat,lon){
    return [nil,{
        names:["Road","Freeway"]
    }];
}

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

props.Node=
{
    new:func(values=nil)
    {
        var result={
            parents:[props.Node],
            val:{},
            type:'GHOST',
            parent:nil
        };
        if(typeof(values)=="hash")
            result.val=values;
        return result;
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
                println("{}");
                return;
            }
            println('{');
            foreach(var k;key)
            {
                print(s~'  ',k,':');
                me.val[k].debug(s~'  ');
            }
            println(s,'}');
        }
        else
            println(me.val,' (',me.type,')');
        return;
    }
};

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
props.globals.debug();

println("-----------------------------------");
