import.module.libkey;
var list=func(){
    var (begin,end,len)=(nil,nil,0);
    return{
        push_back:func(elem){
            var tmp={
                elem:elem,
                prev:nil,
                next:nil
            };
            if(end!=nil){
                end.next=tmp;
                tmp.prev=end;
                end=tmp;
            }else{
                begin=end=tmp;
            }
            len+=1;
        },
        push_front:func(elem){
            var tmp={
                elem:elem,
                prev:nil,
                next:nil
            };
            if(begin!=nil){
                begin.prev=tmp;
                tmp.next=begin;
                begin=tmp;
            }else{
                begin=end=tmp;
            }
            len+=1;
        },
        pop_back:func(){
            if(end!=nil)
                end=end.prev;
            if(end==nil)
                begin=nil;
            else
                end.next=nil;
            if(len)
                len-=1;
        },
        pop_front:func(){
            if(begin!=nil)
                begin=begin.next;
            if(begin==nil)
                end=nil;
            else
                begin.prev=nil;
            if(len)
                len-=1;
        },
        front:func(){
            if(begin!=nil)
                return begin.elem;
        },
        back:func(){
            if(end!=nil)
                return end.elem;
        },
        length:func(){
            return len;
        }
    };
}

var game=func(x,y){
    rand(time(0));
    var texture=["  ","██","\e[91m██\e[0m"];
    var edge0="╔";
    var edge1="╚";
    for(var i=0;i<x;i+=1){
        edge0~="══";
        edge1~="══";
    }
    edge0~="╗\n";
    edge1~="╝\n";

    var vec=[];
    setsize(vec,x);
    for(var i=0;i<x;i+=1){
        vec[i]=[];
        setsize(vec[i],y);
        for(var j=0;j<y;j+=1)
            vec[i][j]=0;
    }

    var snake=list();
    snake.push_back([int(x/2),int(y/3)]);
    snake.push_back([int(x/2),int(y/3)+1]);
    vec[int(x/2)][int(y/3)]=1;
    vec[int(x/2)][int(y/3)+1]=1;

    var move='w';
    var gameover=0;
    var setapple=func(){
        var (cord_x,cord_y)=(int(rand()*x),int(rand()*y));
        while(vec[cord_x][cord_y]!=0)
            (cord_x,cord_y)=(int(rand()*x),int(rand()*y));
        vec[cord_x][cord_y]=2;
    }
    setapple();

    return {
        print:func(){
            var s="";
            var (fx,fy)=snake.front();
            for(var i=0;i<y;i+=1){
                s~="║";
                for(var j=0;j<x;j+=1){
                    if(fx==j and fy==i)
                        s~="\e[93m"~texture[vec[j][i]]~"\e[0m";
                    else
                        s~=texture[vec[j][i]];
                }
                s~='║\n';
            }
            print('\e[1;1H'~edge0~s~edge1);
        },
        next:func(){
            var (fx,fy)=snake.front();
            var eat=0;
            if(move=="w" and fy-1>=0){
                snake.push_front([fx,fy-1]);
                if(vec[fx][fy-1]==1)
                    gameover=1;
                elsif(vec[fx][fy-1]==2)
                    eat=1;
                vec[fx][fy-1]=1;
            }elsif(move=='a' and fx-1>=0){
                snake.push_front([fx-1,fy]);
                if(vec[fx-1][fy]==1)
                    gameover=1;
                elsif(vec[fx-1][fy]==2)
                    eat=1;
                vec[fx-1][fy]=1;
            }elsif(move=='s' and fy+1<y){
                snake.push_front([fx,fy+1]);
                if(vec[fx][fy+1]==1)
                    gameover=1;
                elsif(vec[fx][fy+1]==2)
                    eat=1;
                vec[fx][fy+1]=1;
            }elsif(move=='d' and fx+1<x){
                snake.push_front([fx+1,fy]);
                if(vec[fx+1][fy]==1)
                    gameover=1;
                elsif(vec[fx+1][fy]==2)
                    eat=1;
                vec[fx+1][fy]=1;
            }else{
                gameover=1;
            }
            if(!gameover and !eat){
                var (bx,by)=snake.back();
                vec[bx][by]=0;
                snake.pop_back();
            }

            if(eat and snake.length()!=x*y)
                setapple();
            elsif(snake.length()==x*y)
                gameover=2;
        },
        move:func(c){
            move=c;
        },
        gameover:func(){
            return gameover;
        }
    }
}

var co=coroutine.create(func(){
    while(1){
        var moved=-1;
        for(var i=0;i<30;i+=1){
            var ch=libkey.nonblock();
            if(moved==-1 and ch!=nil){
                moved=ch;
            }
            coroutine.yield(nil);
            unix.sleep(0.01);
        }
        coroutine.yield(moved);
    }
});

var main=func(argv){
    var should_skip=(size(argv)!=0 and argv[0]=="--skip");
    # enable unicode
    if(os.platform()=="windows")
        system("chcp 65001");
    print("\ec");

    var g=game(15,10);
    g.print();
    print("\rpress any key to start...");
    if(!should_skip){
        libkey.getch();
    }
    print("\r                         \r");
    var counter=20;
    while(1){
        while((var ch=coroutine.resume(co)[0])==nil);
        if(ch!=nil and ch!=-1){
            if(ch=='q'[0])
                break;
            elsif(ch=='p'[0]){
                print("\rpress any key to continue...");
                libkey.getch();
                print("\r                            \r");
            }
            g.move(chr(ch));
        }
        
        g.next();
        if(g.gameover())
            break;
        g.print();
    }

    println(g.gameover()<=1?"game over.":"you win!");
    println("press 'q' to quit.");
    if(should_skip){
        return;
    }
    while(libkey.getch()!='q'[0]);
}

main(runtime.argv());