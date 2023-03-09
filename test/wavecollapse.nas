# wave collapse function 2022/4/10
# by ValKmjolnir
srand();
var table=[
    # c ,w,a,s,d
    ["═",0,1,0,1],
    ["═",0,1,0,1],
    ["═",0,1,0,1],
    ["║",1,0,1,0],

    ["╔",0,0,1,1],
    ["╗",0,1,1,0],
    ["╚",1,0,0,1],
    ["╝",1,1,0,0],

    # ["╠",1,0,1,1],
    # ["╣",1,1,1,0],
    # ["╦",0,1,1,1],
    # ["╩",1,1,0,1],
    # ["╬",1,1,1,1],

    [" ",0,0,0,0],
    [" ",0,0,0,0],
    [" ",0,0,0,0],
    [" ",0,0,0,0],
    [" ",0,0,0,0],
    [" ",0,0,0,0]
];

var map=func(){
    var (vec,x,s)=(nil,nil,size(table));
    var generator=func(){
        var tmp=[];
        foreach(var elem;table)
            if(elem[1]==vec[0][0][3] and elem[2]==0)
                append(tmp,elem);
        vec[1][0]=tmp[rand()*size(tmp)];

        for(var j=1;j<x;j+=1){
            if(vec[0][j][3]==0 and vec[1][j-1][4]==0 and rand()>0.5){
                vec[1][j]=table[-1];
                continue;
            }
            
            tmp=[];
            foreach(var elem;table)
                if(elem[2]==vec[1][j-1][4] and elem[1]==vec[0][j][3]){
                    if((j==x-1 and elem[4]==0) or j<x-1)
                        append(tmp,elem);
                }
            vec[1][j]=tmp[rand()*size(tmp)];
        }
    }
    return {
        new:func(_x=10){
            x=_x;
            vec=[[],[]];
            for(var i=0;i<2;i+=1){
                setsize(vec[i],x);
                for(var j=0;j<x;j+=1)
                    vec[i][j]=table[-1];
            }

            var tmp=[];
            foreach(var elem;table)
                if(elem[1]==0 and elem[2]==0)
                    append(tmp,elem);
            vec[0][0]=tmp[rand()*size(tmp)];

            for(var i=1;i<x;i+=1){
                tmp=[];
                foreach(var elem;table)
                    if(elem[2]==vec[0][i-1][4] and elem[1]==0){
                        if((i==x-1 and elem[4]==0) or i<x-1)
                            append(tmp,elem);
                    }
                vec[0][i]=tmp[rand()*size(tmp)];
            }
            me.print(0);
            generator();
        },
        print:func(index){
            var str="";
            foreach(var _x;vec[index])
                str~=_x[0];
            str~="\n";
            print(str);
        },
        next:func(){
            (vec[0],vec[1])=(vec[1],vec[0]);
            generator();
        }
    }
}();

# enable unicode
if(os.platform()=="windows")
    system("chcp 65001");
map.new(90);

for(var iter=0;iter<40;iter+=1){
    map.print(1);
    map.next();
}