# wave collapse function 2022/4/10
# by ValKmjolnir
import("lib.nas");

srand();

var table=[
    # c ,w,a,s,d
    ["═",0,1,0,1],
    ["║",1,0,1,0],

    ["╔",0,0,1,1],
    ["╗",0,1,1,0],
    ["╚",1,0,0,1],
    ["╝",1,1,0,0],

    ["╔",0,0,1,1],
    ["╗",0,1,1,0],
    ["╚",1,0,0,1],
    ["╝",1,1,0,0],

    ["╠",1,0,1,1],
    ["╣",1,1,1,0],
    ["╦",0,1,1,1],
    ["╩",1,1,0,1],
    ["╬",1,1,1,1],

    [" ",0,0,0,0],
    [" ",0,0,0,0],
    [" ",0,0,0,0],
    [" ",0,0,0,0],
    [" ",0,0,0,0],
    [" ",0,0,0,0],
    [" ",0,0,0,0],
    [" ",0,0,0,0],
    [" ",0,0,0,0],
    [" ",0,0,0,0],
    [" ",0,0,0,0],
    [" ",0,0,0,0]
];

var map=func(){
    var vec=nil;
    var (x,y)=(10,10);
    var s=size(table);
    var generator=func(i){
        var tmp=[];
        foreach(var elem;table)
            if(elem[1]==vec[i-1][0][3])
                append(tmp,elem);
        vec[i][0]=tmp[rand()*size(tmp)];

        for(var j=1;j<x;j+=1){
            var tmp=[];
            foreach(var elem;table)
                if(elem[2]==vec[i][j-1][4] and elem[1]==vec[i-1][j][3])
                    append(tmp,elem);
            vec[i][j]=tmp[rand()*size(tmp)];
        }
    }
    return {
        new:func(_x=10,_y=10){
            (x,y)=(_x,_y);
            vec=[];
            setsize(vec,y);
            for(var i=0;i<y;i+=1){
                vec[i]=[];
                setsize(vec[i],x);
            }
            vec[0][0]=table[rand()*s];
            for(var i=1;i<x;i+=1){
                var tmp=[];
                foreach(var elem;table)
                    if(elem[2]==vec[0][i-1][4])
                        append(tmp,elem);
                vec[0][i]=tmp[rand()*size(tmp)];
            }
            for(var i=1;i<y;i+=1){
                generator(i);
            }
        },
        print:func(){
            var str="\e[0;0H";
            foreach(var _y;vec){
                foreach(var _x;_y)
                    str~=_x[0];
                str~="\n";
            }
            print(str);
        },
        next:func(){
            var tmp=vec[0];
            for(var i=0;i<y-1;i+=1)
                vec[i]=vec[i+1];
            vec[-1]=tmp;
            generator(y-1);
        }
    }
}();

print("\ec");
map.new(100,20);
for(var i=0;i<500;i+=1){
    map.print();
    map.next();
    unix.sleep(1/30);
}