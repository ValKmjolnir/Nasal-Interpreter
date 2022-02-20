import("lib.nas");
import("module/libkey.nas");

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
    [[0,0],[1,0],[2,0],[0,1]],
    [[0,0],[0,1],[0,2],[1,2]],
    [[0,0],[0,1],[-1,1],[-2,1]],
    #   []      []  [][][]  []
    # [][][]  [][]    []    [][]
    #           []          []
    [[0,0],[0,1],[-1,1],[1,1]],
    [[0,0],[0,1],[-1,1],[0,2]],
    [[0,0],[-1,0],[1,0],[0,1]],
    [[0,0],[0,1],[0,2],[1,1]],
    # []  [][][][]
    # []
    # []
    # []
    [[0,0],[0,1],[0,2],[0,3]],
    [[0,0],[1,0],[2,0],[3,0]],
    # [][]
    # [][]
    [[0,0],[1,0],[0,1],[1,1]],
    #   []  [][]
    # [][]    [][]
    # []
    [[0,0],[0,1],[-1,1],[-1,2]],
    [[0,0],[1,0],[1,1],[2,1]],
    # []      [][]
    # [][]  [][]
    #   []
    [[0,0],[0,1],[1,1],[1,2]],
    [[0,0],[1,0],[0,1],[-1,1]]
];
var block={
    x:0,
    y:0,
    rotate:0,
    type:nil,
    shape:nil,
    new:func(x=0,y=0){
        (me.x,me.y)=(x,y);
        me.rotate=0;
        me.type=blocktype[rand()*size(blocktype)];
        me.shape=blockshape[me.type[me.rotate]];
        return {parents:[block]};
    }
};

var mapgen=func(mapx,mapy){
    var score=0;
    var (empty,unset,full)=(0,1,2);

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
            append(tmp,empty);
        append(map,tmp);
    }

    var blk=nil;
    var new_block=func(){
        blk=block.new(int(mapx/2),0);
        # check game end unfinished
        foreach(var i;blk.shape)
            map[blk.y+i[1]][blk.x+i[0]]=unset;
    }
    new_block();

    # color print
    var front=[
        "31","32","33","34","35","36",
        "91","92","93","94","95","96",
    ];
    var map_print=func(){
        print("\e[1;1Hscore: ",score,"\n");
        var s=table;
        for(var y=0;y<mapy;y+=1){
            s~="\e[44m \e[0m";
            for(var x=0;x<mapx;x+=1){
                s~=(map[y][x]!=empty)?"\e["~front[rand()*12]~"m██\e[0m":"  ";
            }
            s~="\e[44m \e[0m\n";
        }
        s~=table;
        print(s);
    }

    var moveleft=func(){
        var (x,y)=(blk.x-1,blk.y);
        foreach(var i;blk.shape){
            if(x+i[0]<0)
                return;
            if(map[y+i[1]][x+i[0]]==full)
                return;
        }
        foreach(var i;blk.shape)
            map[blk.y+i[1]][blk.x+i[0]]=empty;
        blk.x=x;
        foreach(var i;blk.shape)
            map[blk.y+i[1]][blk.x+i[0]]=unset;
        map_print();
    }

    var moveright=func(){
        var (x,y)=(blk.x+1,blk.y);
        foreach(var i;blk.shape){
            if(x+i[0]>=mapx)
                return;
            if(map[y+i[1]][x+i[0]]==full)
                return;
        }
        foreach(var i;blk.shape)
            map[blk.y+i[1]][blk.x+i[0]]=empty;
        blk.x=x;
        foreach(var i;blk.shape)
            map[blk.y+i[1]][blk.x+i[0]]=unset;
        map_print();
    }

    var rotate=func(){
        var (r,x,y)=(blk.rotate,blk.x,blk.y);
        r=(r+1>=size(blk.type))?0:r+1;
        var shape=blockshape[blk.type[r]];
        foreach(var i;shape){
            if(x+i[0]>=mapx or x+i[0]<0 or y+i[1]>=mapy or y+i[1]<0)
                return;
            if(map[y+i[1]][x+i[0]]==full)
                return;
        }
        foreach(var i;blk.shape)
            map[blk.y+i[1]][blk.x+i[0]]=empty;
        blk.rotate=r;
        blk.shape=shape;
        foreach(var i;blk.shape)
            map[blk.y+i[1]][blk.x+i[0]]=unset;
        map_print();
    }

    var fall=func(){
        var (x,y)=(blk.x,blk.y+1);
        var sethere=0;
        foreach(var i;blk.shape){
            if(y+i[1]>=mapy or map[y+i[1]][x+i[0]]==full){
                sethere=1;
                break;
            }
        }
        if(sethere){
            foreach(var i;blk.shape)
                map[blk.y+i[1]][blk.x+i[0]]=full;
            checkmap();
            new_block();
            map_print();
            return;
        }
        foreach(var i;blk.shape)
            map[blk.y+i[1]][blk.x+i[0]]=empty;
        blk.y=y;
        foreach(var i;blk.shape)
            map[blk.y+i[1]][blk.x+i[0]]=unset;
        map_print();
    }

    var checkmap=func(){
        for(var y=mapy-1;y>=0;y-=1){
            for(var x=0;x<mapx;x+=1)
                if(map[y][x]!=full)
                    break;
            if(x==mapx){
                score+=mapx;
                for(var t=y;t>=1;t-=1)
                    for(var x=0;x<mapx;x+=1)
                        map[t][x]=map[t-1][x];
                for(var x=0;x<mapx;x+=1)
                    map[0][x]=empty;
                y+=1;
            }
        }
        map_print();
    }
    return {
        print:map_print,
        moveleft:moveleft,
        moveright:moveright,
        rotate:rotate,
        fall:fall,
        checkmap:checkmap
    };
}

var main=func(){
    print("\ec");

    rand(time(0));
    var map=mapgen(mapx:15,mapy:15);

    libkey.init();
    while(1){
        var ch=libkey.nonblock();
        if(ch){
            if(ch=='a'[0])     # move left
                map.moveleft();
            elsif(ch=='d'[0])  # move right
                map.moveright();
            elsif(ch=='s'[0])  # rotate
                map.rotate();
            elsif(ch==' '[0])  # move down
                map.fall();
            elsif(ch=='q'[0])
                break;
            if(ch=='p'[0]){
                print("\rpress any key to continue...");
                libkey.getch();
                print("\r                            ");
            }
            map.checkmap();
            unix.sleep(0.01);
        }
        else{
            map.fall();
            map.checkmap();
            unix.sleep(0.65);
        }
    }
    libkey.close();

    print("\ec\e[31ms\e[32me\e[33me \e[34my\e[35mo\e[36mu \e[94m~\e[0m\n");
};

main();
