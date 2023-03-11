import.stl.queue;

rand(time(0));
var pixel=[' ','#','.','*'];
var map=[];
for(var i=0;i<10;i+=1){
    append(map,[]);
    for(var j=0;j<20;j+=1)
        append(map[i],(rand()>0.7));
}

var prt=func(){
    var s="\e[0;0H+--------------------+\n";
    for(var i=0;i<10;i+=1){
        s~="|";
        for(var j=0;j<20;j+=1)
            s~=pixel[map[i][j]];
        s~='|\n';
    }
    s~='+--------------------+\n';
    print(s);
    unix.sleep(1/800);
}

var bfs=func(begin,end){
    var move=[[1,0],[0,1],[-1,0],[0,-1]];
    var que=queue();
    que.push(begin);
    map[begin[0]][begin[1]]=2;
    map[end[0]][end[1]]=0;
    if(map[1][0]==1 and map[0][1]==1)
        map[1][0]=0;
    while(!que.empty()){
        var vertex=que.front();
        que.pop();
        foreach(var i;move){
            var x=vertex[0]+i[0];
            var y=vertex[1]+i[1];
            if(x==end[0] and y==end[1]){
                map[x][y]=3;
                prt();
                print("reached.\n");
                return;
            }
            if(0<=x and x<10 and 0<=y and y<20 and map[x][y]==0){
                que.push([x,y]);
                map[x][y]=2;
            }
        }
        prt();
    }
    print("cannot reach.\n");
    return;
}

# enable ANSI escape sequence
if(os.platform()=="windows")
    system("color");
print("\ec");
bfs([0,0],[9,19]);