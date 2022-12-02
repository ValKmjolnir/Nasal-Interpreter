var map=nil;

var check=func(x,y){
    if(x>29) x=0;
    if(y>49) y=0;
    return map[x][y];
}

var new_map=func(){
    var tmp=[];
    setsize(tmp,30);
    forindex(var i;tmp){
        tmp[i]=[];
        setsize(tmp[i],50);
    }
    return tmp;
}

var prt=func(){
    var s='\e[0;0H';
    foreach(var line;map){
        foreach(var elem;line)
            s~=elem~' ';
        s~='\n';
    }
    print(s);
    unix.sleep(1/160);
}

func(){
    # enable ANSI escape sequence
    if(os.platform()=="windows")
        system("color");
    print("\ec");
    rand(time(0));
    map=new_map();

    forindex(var i;map)
        forindex(var j;map[i])
            map[i][j]=rand()<0.45?'O':' ';

    var calc=[[0,1],[1,0],[0,-1],[-1,0],[1,1],[1,-1],[-1,-1],[-1,1]];
    for(var r=0;r<400;r+=1){
        prt(map);
        var tmp=new_map();
        for(var i=0;i<30;i+=1)
            for(var j=0;j<50;j+=1){
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