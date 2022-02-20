import("lib.nas");
import("module/libkey.nas");

var mapgen=func(mapx,mapy){
    if(mapx<1 or mapy<1)
        die("map_x or map_y must be greater than 1");
    # use in print
    var table="\e[44m ";
    for(var i=0;i<mapx;i+=1)
        table~="  ";
    table~=" \e[0m\n";
    # generate new map
    var map=[];
    for(var y=0;y<mapy;y+=1){
        var tmp=[];
        for(var x=0;x<mapx;x+=1)
            append(tmp,rand()>0.9);
        append(map,tmp);
    }

    # color print
    var front=[
    "31","32","33","34","35","36",
    "91","92","93","94","95","96",
    ];
    var map_print=func(){
        var s="\e[1;1H"~table;
        for(var y=0;y<mapy;y+=1){
            s~="\e[44m \e[0m";
            for(var x=0;x<mapx;x+=1)
                s~=map[y][x]?"\e["~front[rand()*12]~"m██\e[0m":"  ";
            s~="\e[44m \e[0m\n";
        }
        s~=table;
        print(s);
    }

    var moveleft=func(block){
        for(var y=0;y<mapy;y+=1){
            var tmp=map[y][0];
            for(var x=1;x<mapx;x+=1)
                map[y][x-1]=map[y][x];
            map[y][mapx-1]=tmp;
        }
    }

    var moveright=func(block){
        for(var y=0;y<mapy;y+=1){
            var tmp=map[y][mapx-1];
            for(var x=mapx-1;x>=1;x-=1)
                map[y][x]=map[y][x-1];
            map[y][0]=tmp;
        }
    }

    var rotate=func(block){

    }

    var setblock=func(block){

    }

    var checkmap=func(block){
        map_print(map);
    }
    return {
        print:map_print,
        moveleft:moveleft,
        moveright:moveright,
        rotate:rotate,
        setblock:setblock,
        checkmap:checkmap
    };
}

var blocktype=[
    [0,1,2,3],
    [4,5,6,7],
    [8,9,10,11],
    [12,13],
    [14],
    [15,16],
    [17,18]
];
var blockshape=[
    # [][]            []  [][][]
    # []    []        []      []
    # []    [][][]  [][]
    [[0,0],[1,0],[0,1],[0,2]],
    [[0,0],[0,1],[1,1],[2,1]],
    [[0,0],[0,1],[0,2],[-1,2]],
    [[0,0],[1,0],[2,0],[2,1]],
    # [][]  [][][]  []
    #   []  []      []        []
    #   []          [][]  [][][]
    [[0,0],[1,0],[1,1],[1,2]],
    [[0,0],[],[],[]],
    [[0,0],[],[],[]],
    [[0,0],[],[],[]],
    #   []      []  [][][]  []
    # [][][]  [][]    []    [][]
    #           []          []
    [[0,0],[],[],[]],
    [[0,0],[],[],[]],
    [[0,0],[],[],[]],
    [[0,0],[],[],[]],
    # []  [][][][]
    # []
    # []
    # []
    [[0,0],[],[],[]],
    [[0,0],[],[],[]],
    # [][]
    # [][]
    [[0,0],[],[],[]],
    #   []  [][]
    # [][]    [][]
    # []
    [[0,0],[],[],[]],
    [[0,0],[],[],[]],
    # []      [][]
    # [][]  [][]
    #   []
    [[0,0],[],[],[]],
    [[0,0],[],[],[]]
];
var block={
    x:0,
    y:0,
    type:nil,
    shape:nil,
    new:func(x=0,y=0){
        (me.x,me.y)=(x,y);
        me.type=blocktype[rand()*size(blocktype)];
        me.shape=blockshape[me.type][0];
        return {parents:[block]};
    }
};

var main=func(){
    print("\ec");

    rand(time(0));
    var map=mapgen(mapx:15,mapy:15);
    var blk=block.new();
    
    libkey.init();
    while(1){
        var ch=libkey.nonblock();
        if(ch){
            if(ch=='a'[0])     # move left
                map.moveleft(blk);
            elsif(ch=='d'[0])  # move right
                map.moveright(blk);
            elsif(ch=='s'[0])  # rotate
                map.rotate(blk);
            elsif(ch==' '[0])  # set block
                map.setblock(blk);
            elsif(ch=='q'[0])
                break;
            map.checkmap(blk);
            unix.sleep(0.03);
        }
        else{
            blk.y+=1;
            map.checkmap(blk);
            unix.sleep(0.3);
        }
    }
    libkey.close();

    print("\ec\e[91ms\e[92me\e[93me \e[94my\e[95mo\e[96mu \e[34m~\e[0m\n");
};

main();
