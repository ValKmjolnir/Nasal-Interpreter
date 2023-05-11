import.stl.process_bar;

var new_map=func(width,height){
    var tmp=[];
    setsize(tmp,height);
    forindex(var i;tmp){
        tmp[i]=[];
        setsize(tmp[i],width);
    }
    return tmp;
}

var prt=func(map){
    var s='\ec\e[1;1H';
    foreach(var line;map){
        foreach(var elem;line)
            s~=elem~' ';
        s~='\n';
    }
    print(s);
    unix.sleep(1/160);
}

var run=func(width,height){

    var check=func(_width,_height){
        if(_height>=height) _height=0;
        if(_width>=width) _width=0;
        return map[_height][_width]=='O';
    }

    # enable ANSI escape sequence
    if(os.platform()=="windows")
        system("color");
    print("\ec");
    rand(time(0));
    var map=new_map(width,height);
    var tmp=new_map(width,height);

    forindex(var i;map)
        forindex(var j;map[i])
            map[i][j]=rand()<0.45?'O':'.';

    for(var r=0;r<100;r+=1){
        prt(map);
        for(var i=0;i<height;i+=1)
            for(var j=0;j<width;j+=1){
                var cnt=check(j,i+1)+check(j+1,i)+check(j,i-1)+check(j-1,i)+check(j+1,i+1)+check(j-1,i-1)+check(j+1,i-1)+check(j-1,i+1);
                if(cnt==2)    tmp[i][j]=map[i][j];
                elsif(cnt==3) tmp[i][j]='O';
                else          tmp[i][j]='.';
            }
        (map,tmp)=(tmp,map);
    }
    return;
};

var ppm_gen=func(width,height){
    var pixels=width*height;

    var new_map=func(){
        var tmp=[];
        setsize(tmp,pixels);
        return tmp;
    }

    # iteration counter and trigger for ppm/data generator
    var iter_to_print=0;

    var init=func(){
        var res=new_map();
        if(io.exists(".life_data")) {
            var vec=split("\n",io.readfile(".life_data"));
            if (num(vec[0])!=width or num(vec[1])!=height) {
                die("incorrect width or height: "~vec[0]~":"~str(width)~" / "~vec[1]~":"~str(height))
            }
            iter_to_print=num(vec[2]);
            var data=vec[3];
            var n="1"[0];
            for(var i=0;i<pixels;i+=1){
                res[i]=data[i]==n?1:0;
            }
            return res;
        }

        rand(time(0));
        for(var i=0;i<pixels;i+=1)
            res[i]=0;
        
        for(var i=int(height/5*2);i<int(height/5*3);i+=1)
            forindex(var j;res[i])
                res[i*width+j]=rand()<0.5?1:0;
        return res;
    }

    var store=func(){
        var fd=io.open(".life_data","w");
        io.write(fd,str(width)~"\n"~str(height)~"\n"~str(iter_to_print)~"\n");
        for(var i=0;i<pixels;i+=1){
            io.write(fd,map[i]==1?"1":"0");
        }
        io.close(fd);
    }

    var gen=func(filename){
        # P3 use ASCII number
        # P6 use binary character
        var fd=io.open(filename,"wb");
        io.write(fd,"P6\n"~width~" "~height~"\n255\n");
        var white=char(255)~char(255)~char(255);
        var black=char(0)~char(0)~char(0);
        for(var i=0;i<pixels;i+=1){
            io.write(fd,map[i]==1?white:black);
        }
        io.close(fd);
    }

    var ts=maketimestamp();
    var bar=process_bar.high_resolution_bar(50);
    var map=init();
    var tmp=new_map();
    
    var check=func(_width,_height){
        if(_height>=height) _height=0;
        if(_width>=width) _width=0;
        return map[_height*width+_width]==1;
    }

    for(var r=0;r<1001;r+=1){
        ts.stamp();
        for(var i=0;i<height;i+=1){
            for(var j=0;j<width;j+=1){
                var cnt=check(j,i+1)+check(j+1,i)+check(j,i-1)+check(j-1,i)+check(j+1,i+1)+check(j-1,i-1)+check(j+1,i-1)+check(j-1,i+1);
                if(cnt==2)    tmp[i*width+j]=map[i*width+j];
                elsif(cnt==3) tmp[i*width+j]=1;
                else          tmp[i*width+j]=0;
            }
        }
        (map,tmp)=(tmp,map);
        var calc_tm=ts.elapsedMSec();
        var duration=int(calc_tm/1000*(1e3-r));
        if(r-int(r/10)*10==0) {
            if(r){
                store();
            }
            gen("iteration_"~str(iter_to_print)~".ppm");
            iter_to_print+=10;
        }
        print(bar.bar((r+1)/1e3),int((r+1)/1e3*100),"% ",calc_tm," ms, ",int(duration/60)," min ",duration-int(duration/60)*60," s remaining, iteration ",r,"   \r");
    }
    println();
}

if (size(runtime.argv()) and runtime.argv()[0]=="--generate-ppm") {
    ppm_gen(1500*4,1500);
} else {
    run(40,25);
}