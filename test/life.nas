import("lib.nas");

var map=nil;

var check=func(x,y)
{
    if(x>14) x=0;
    if(y>19) y=0;
    return map[x][y];
}

var new_map=func()
{
    var tmp=[];
    setsize(tmp,15);
    forindex(var i;tmp)
    {
        tmp[i]=[];
        setsize(tmp[i],20);
    }
    return tmp;
}

var prt=func()
{
    var s='';
    foreach(var line;map)
    {
        foreach(var elem;line)
            s~=elem~' ';
        s~='\n';
    }
    if(os.platform()=="windows")
        system("cls");
    else
        system("clear");
    print(s);
}

func()
{
    rand(time(0));
    map=new_map();
    forindex(var i;map)
        forindex(var j;map[i])
            map[i][j]=rand()>0.7?'O':' ';
    var calc=[[0,1],[1,0],[0,-1],[-1,0],[1,1],[1,-1],[-1,-1],[-1,1]];
    for(var r=0;r<100;r+=1)
    {
        prt(map);
        var tmp=new_map();
        for(var i=0;i<15;i+=1)
            for(var j=0;j<20;j+=1)
            {
                var cnt=0;
                foreach(var k;calc)
                    cnt+=(check(i+k[0],j+k[1])=='O');
                if(cnt==2)    tmp[i][j]=map[i][j];
                elsif(cnt==3) tmp[i][j]='O';
                else          tmp[i][j]=' ';
            }
        map=tmp;
    }
    return;
}();