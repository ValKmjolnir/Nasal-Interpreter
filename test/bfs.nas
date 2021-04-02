import("stl/lib.nas");
import("stl/queue.nas");

rand(time(0));

var map=[];
for(var i=0;i<10;i+=1)
{
    append(map,[]);
    for(var j=0;j<10;j+=1)
        append(map[i],(rand()>0.7));
}

var prt=func()
{
    var s="";
    for(var i=0;i<10;i+=1)
    {
        for(var j=0;j<10;j+=1)
            s~=map[i][j];
        s~='\n';
    }
    print(s,'\n');
}

var bfs=func(begin,end)
{
    var move=[[1,0],[0,1],[-1,0],[0,-1]];
    var que=queue();
    que.push(begin);
    map[begin[0]][begin[1]]=3;
    while(!que.empty())
    {
        var vertex=que.front();
        que.pop();
        foreach(var i;move)
        {
            var x=vertex[0]+i[0];
            var y=vertex[1]+i[1];
            if(x==end[0] and y==end[1])
            {
                map[x][y]='*';
                return;
            }
            if(0<=x and x<10 and 0<=y and y<10 and map[x][y]==0)
            {
                que.push([x,y]);
                map[x][y]=3;
            }
        }
        prt();
    }
    print("cannot reach.\n");
    return;
}

prt();
var begin=[input(),input()];
var end=[input(),input()];
bfs(begin,end);
prt();