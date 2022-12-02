import.module.libkey;
var color=[
    "\e[31m","\e[32m","\e[33m","\e[34m","\e[35m","\e[36m",
    "\e[91m","\e[92m","\e[93m","\e[94m","\e[95m","\e[96m",
];
var blocktype=[
    [0,1,2,3  ],
    [4,5,6,7  ],
    [8,9,10,11],
    [12,13    ],
    [14       ],
    [15,16    ],
    [17,18    ]
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

var color_count=0;
var counter=0;
var package=[0,1,2,3,4,5,6];
var exchange=func(){
    for(var i=6;i>=0;i-=1){
        var index=int(i*rand());
        (package[i],package[index])=(package[index],package[i]);
    }
}

var block={
    x:0,
    y:0,
    rotate:0,
    type:nil,
    shape:nil,
    color:nil,
    new:func(x=0,y=0){
        (me.x,me.y)=(x,y);
        me.rotate=0;
        me.type=blocktype[package[counter]];
        counter+=1;
        if(counter==7){
            exchange();
            counter=0;
        }
        
        me.shape=blockshape[me.type[me.rotate]];

        me.color=color_count;
        color_count+=1;
        if(color_count>=size(color))
            color_count=0;
        
        return {parents:[block]};
    }
};

var mapgen=func(mapx,mapy){
    var (score,gameover)=(0,0);
    var (empty,unset,full)=(0,1,2);

    if(mapx<1 or mapy<1)
        die("map_x or map_y must be greater than 1");
    
    # use in print
    var line="";
    for(var i=0;i<mapx;i+=1)
        line~="══";
    var head="\e[32m╔"~line~"╗\e[0m\n";
    var tail="\e[32m╚"~line~"╝\e[0m\n";

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

        # check if has enough place to place a new block
        foreach(var i;blk.shape)
            if(map[blk.y+i[1]][blk.x+i[0]]>=full){
                gameover=1;
                return;
            }
        # update map
        foreach(var i;blk.shape)
            map[blk.y+i[1]][blk.x+i[0]]=unset;
    }
    new_block(); # initialize the first block

    # color print
    var map_print=func(){
        var s="\e[1;1H"~head;
        for(var y=0;y<mapy;y+=1){
            s~="\e[32m║\e[0m";
            for(var x=0;x<mapx;x+=1){
                var c=map[y][x];
                if(c==empty)
                    s~="  ";
                elsif(c==unset)
                    s~=color[blk.color]~"██\e[0m";
                elsif(c>=full)
                    s~=color[c-full]~"██\e[0m";
            }
            s~="\e[32m║\e[0m\n";
        }
        s~=tail;
        print(s,"\e[31ms\e[32mc\e[33mo\e[34mr\e[35me\e[36m: \e[0m",score,'\n');
    }

    var moveleft=func(){
        var (x,y)=(blk.x-1,blk.y);
        foreach(var i;blk.shape){
            if(x+i[0]<0)
                return;
            if(map[y+i[1]][x+i[0]]>=full)
                return;
        }
        # update block state and map
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
            if(map[y+i[1]][x+i[0]]>=full)
                return;
        }
        # update block state and map
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
            if(map[y+i[1]][x+i[0]]>=full)
                return;
        }

        # update block state and map
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
        # check if falls to the edge of other blocks or map
        var sethere=0;
        foreach(var i;blk.shape)
            if(y+i[1]>=mapy or map[y+i[1]][x+i[0]]>=full){
                sethere=1;
                break;
            }
        # set block here and generate a new block
        if(sethere){
            foreach(var i;blk.shape)
                map[blk.y+i[1]][blk.x+i[0]]=blk.color+full;
            checkmap();
            new_block();
            map_print();
            return;
        }
        # update block state and map
        foreach(var i;blk.shape)
            map[blk.y+i[1]][blk.x+i[0]]=empty;
        blk.y=y;
        foreach(var i;blk.shape)
            map[blk.y+i[1]][blk.x+i[0]]=unset;
        map_print();
    }

    var checkmap=func(){
        var lines=1;
        for(var y=mapy-1;y>=0;y-=1){
            # check if this line is full of blocks
            var tmp=0;
            for(var x=0;x<mapx;x+=1){
                if(map[y][x]<full)
                    break;
                tmp+=map[y][x];
            }
            # if is full, clear this line and
            # all the lines above fall one block
            if(x==mapx){
                score+=lines*tmp;
                lines*=2;
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
        checkmap:checkmap,
        gameover:func(){return gameover;}
    };
}

var main=func(argv){
    var should_skip=(size(argv)!=0 and argv[0]=="--skip");
    var init_counter=should_skip?5:30;
    # windows use chcp 65001 to output unicode
    if(os.platform()=="windows")
        system("chcp 65001");
    
    print(
        "\ec\e[1:1H",
        "╔═════════════════════════╗\n",
        "║         TETRIS          ║\n",
        "╠═════════════════════════╣\n",
        "║  w:rotate, a:move left  ║\n",
        "║  s:fall,   d:move right ║\n",
        "║  p:pause,  q:quit       ║\n",
        "╠═════════════════════════╣\n",
        "║  press any key to start ║\n",
        "╚═════════════════════════╝\n"
    );

    rand(time(0));
    exchange();
    var map=mapgen(mapx:12,mapy:18);

    if(!should_skip){
        libkey.getch();
    }
    print("\ec");
    map.print();

    var counter=init_counter;
    while(1){
        # nonblock input one character
        var ch=libkey.nonblock();
        if(ch){
            if(ch=='a'[0])     # move left
                map.moveleft();
            elsif(ch=='d'[0])  # move right
                map.moveright();
            elsif(ch=='w'[0])  # rotate
                map.rotate();
            elsif(ch=='s'[0])  # move down
                map.fall();
            elsif(ch=='q'[0])  # quit the game
                break;
            if(ch=='p'[0]){    # pause the game
                print("\rpress any key to continue...");
                libkey.getch();
                print("\r                            ");
            }
            map.checkmap();
            if(map.gameover())
                break;
        }
        if(!counter){
            # automatically fall one block and check
            map.fall();
            map.checkmap();
            if(map.gameover())
                break;
            counter=init_counter;
        }
        unix.sleep(0.02);
        counter-=1;
    }

    print(
        map.gameover()?
        "\e[31mg\e[32ma\e[33mm\e[34me \e[35mo\e[36mv\e[94me\e[31mr \e[32m~\e[0m\n":
        "\e[31ms\e[32me\e[33me \e[34my\e[35mo\e[36mu \e[94m~\e[0m\n"
    );
    print(
        "\e[31me\e[32mn\e[33mt\e[34me\e[35mr ",
        "\e[36m'\e[94mq\e[95m' ",
        "\e[35mt\e[36mo \e[94mq\e[95mu\e[91mi\e[92mt\e[0m\n"
    );
    if(!should_skip){
        while(libkey.getch()!='q'[0]);
    }
};

main(runtime.argv());