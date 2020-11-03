import("lib.nas");

var property_tree=
{
    accelerations:
    {
        'n-z-cg-fps_sec':0,
        ned:
        {
            'down-accel-fps_sec':0,
            'east-accel-fps_sec':0,
            'north-accel-fps_sec':0,
        },
        nlf:0,
        pilot:
        {
            'x-accel-fps_sec':0,
            'y-accel-fps_sec':0,
            'z-accel-fps_sec':0,
        },
        'pilot-g':1,
        'pilot-gdamped':1
    },
    ai:
    {
        models:
        {
            carrier:
            {
                callsign:'',
                controls:{},
                environment:{},
                id:2,
                name:'Nimitz',
                navaids:{},
                orientation:{},
                position:{},
                radar:{},
                sign:'CVN-68',
                sim:{},
                subID:0,
                submodels:
                {
                    path:'',
                    serviceable:1
                },
                'surface-positions':{},
                type:'AI',
                valid:1,
                velocities:{},
                waypoint:{}
            },
            'carrier[1]':
            {
                callsign:'',
                controls:{},
                environment:{},
                id:3,
                name:'Eisenhower',
                navaids:{},
                orientation:{},
                position:{},
                radar:{},
                sign:'CVN-69',
                sim:{},
                subID:0,
                submodels:
                {
                    path:'',
                    serviceable:0
                },
                'surface-positions':{},
                type:'AI',
                valid:1,
                velocities:{},
                waypoint:{}
            },
            count:2,
            'model-added':'/ai[0]/models[0]/carrier[1]',
            'model-removed':nil,
            'num-players':0
        },
        submodels:
        {
            contrails:0
        },
    },
    aircraft:
    {
        icao:
        {
            equipment:'SDFGY',
            surveillance:'S',
            type:'ZZZZ',
            'wake-turbulence-category':'L'
        },
        performance:
        {
            approach:
            {
                'airspeed-knots':150,
            },
            climb:'\n\t\t\t\n\t\t\t',
            cruise:
            {
                'airspeed-knots':1000,
                'altitude-ft':4500,
            },
            descent:'\n\t\t\t\n\t\t\t',
            maximum:'\n\t\t\t\n\t\t\t',
            minimum:'\n\t\t\t\n\t\t\t',
        },
        settings:
        {
            fuel_persistent:0,
            ground_services_persistent:0,
            radio_persistent:0,
            tooltips:1,
            weight_persistent:0
        }
    },
    autopilot:
    {
        internal:{},
        locks:{},
        'route-manager':{},
        settings:{},
        'target-tracking':{},
    },
    canvas:
    {
        'by-index':
        {
            texture:
            {
                background:'rgba(0,0,0,0)',
                group:{},
                name:'Tooltip',
                placement:{},
                size:600,
                'size[1]':200,
                status:0,
                'status-msg':'OK',
                view:300,
                'view[1]':100
            },
            'texture[1]':
            {
                background:'rgba(0,0,0,0)',
                group:{},
                mipmapping:1,
                name:'SymbolCache1024x1024',
                placement:{},
                size:1024,
                'size[1]':1024,
                status:0,
                'status-msg':'OK',
                view:1024,
                'view[1]':1024
            },
            'texture[2]':
            {
                background:'rgba(0,0,0,0)',
                group:{},
                mipmapping:1,
                name:'SymbolCache1024x1024',
                placement:{},
                size:1024,
                'size[1]':1024,
                status:0,
                'status-msg':'OK',
                view:1024,
                'view[1]':1024
            },
        }
    },
    command:{},
    consumables:{},
    controls:{},
    cursor:'Aircraft/ufo/Models/cursor.ac',
    devices:{},
    earthview:{},
    engines:{},
    environment:{},
    ephemeris:{},
    fdm:{},
    gear:{},
    hazards:{},
    input:{},
    instrumentation:{},
    'local-weather':{},
    logging:{},
    models:{},
    nasal:{},
    orientation:{},
    position:{},
    rendering:{},
    scenery:{},
    sim:{},
    source:'Models',
    'surface-positions':{},
    systems:{},
    velocities:{},
};

var setprop=func(prop,value)
{
    if(type(prop)!="string")
        die("setprop: prop is not a string");
    var path=split('/',prop);
    var tmp=property_tree;
    var path_size=size(path);
    for(var i=0;i<path_size-1;i+=1)
        tmp=tmp[path[i]];
    tmp[path[path_size-1]]=value;
    return;
}
var getprop=func(prop)
{
    if(type(prop)!="string")
        die("getprop: prop is not a string");
    var path=split('/',prop);
    var tmp=property_tree;
    foreach(var i;path)
        tmp=tmp[i];
    return tmp;
}
setprop("aircraft/icao/type",'IDG MD-11');

var print_prop=func(depth,prop)
{
    var s='';
    for(var i=0;i<depth;i+=1)
        s~='|  ';
    if(type(prop)!="hash")
        return;
    var m=keys(prop);
    foreach(var elem;m)
    {
        print(s,elem,':',prop[elem]);
        print_prop(depth+1,prop[elem]);
    }
    return;
}

print_prop(0,property_tree);