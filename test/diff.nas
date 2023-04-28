var myers=func(src,dst,show_table=0){
    (src,dst)=(split("\n",src),split("\n",dst));
    append(src,"");
    append(dst,"");
    var (src_len,dst_len)=(size(src),size(dst));

    var mat=[];
    setsize(mat,dst_len*src_len);
    forindex(var i;mat){
        mat[i]=0;
    }
    var visited=[];
    setsize(visited,dst_len*src_len);
    forindex(var i;visited){
        visited[i]=0;
    }

    forindex(var y;dst)
        forindex(var x;src)
            mat[y*src_len+x]=(src[x]==dst[y]);

    if(show_table){
        var curve=[
            ["+---","|   "],
            ["+---","| \\ "]
        ];
        var s="";
        forindex(var y;dst){
            forindex(var t;curve[0]){
                forindex(var x;src){
                    s~=curve[mat[y*src_len+x]][t];
                }
                s~=["+","|"][t]~"\n";
            }
        }
        forindex(var i;src)
            s~="+---";
        print(s~"+\n");
    }

    var (total,path,vec)=([],[],[[0,0,-1]]);
    visited[0]=1;
    while(size(vec)){
        append(total,vec);
        var tmp=[];
        forindex(var i;vec){
            var elem=vec[i];
            var (x,y)=(elem[1],elem[0]);

            # find solution
            if(x==src_len-1 and y==dst_len-1){
                append(path,vec[i]);
                for(var (prev,iter)=(elem[2],size(total)-1);iter>0;iter-=1){
                    var t=total[iter-1][prev];
                    append(path,t);
                    prev=t[2];
                }

                if(show_table){
                    for(var t=size(path)-1;t>=0;t-=1)
                        print("("~path[t][1]~","~path[t][0]~")",t==0?"":"->");
                    println();
                }

                # reverse path
                for(var t=0;t<size(path)/2;t+=1)
                    (path[t],path[-1-t])=(path[-1-t],path[t]);
                # print diff
                for(var t=1;t<size(path);t+=1){
                    var (prev_x,prev_y)=(path[t-1][1],path[t-1][0]);
                    var (x,y)=(path[t][1],path[t][0]);
                    var (sub_x,sub_y)=(x-prev_x,y-prev_y);
                    if(sub_x==1 and sub_y==1){
                        if(show_table)
                            println("    ",src[prev_x]);
                    }elsif(sub_x==1 and sub_y==0){
                        println("\e[31m -  ",src[prev_x],"\e[0m");
                    }elsif(sub_x==0 and sub_y==1){
                        println("\e[32m +  ",dst[prev_y],"\e[0m");
                    }
                }
                return;
            }

            # do bfs
            if(mat[y*src_len+x]==1){
                if(x+1<src_len and y+1<dst_len and visited[(y+1)*src_len+x+1]==0){
                    append(tmp,[y+1,x+1,i]);
                    visited[(y+1)*src_len+x+1]=1;
                }
            }
            else{
                if(x+1<src_len and visited[y*src_len+x+1]==0){
                    append(tmp,[y,x+1,i]);
                    visited[y*src_len+x+1]=1;
                }
                if(y+1<dst_len and visited[(y+1)*src_len+x]==0){
                    append(tmp,[y+1,x,i]);
                    visited[(y+1)*src_len+x]=1;
                }
            }
        }
        vec=tmp;
    }
}

func(diff){
    diff(
        "var a=0;\nvar b=1;\nprint(\"hello \",a);\nvar c=2;\nc=[];\nvar d=3;\nvar l=list();\nvar q=queue();\n",
        "var a=0;\nvar b=1;\nb=[];\nprintln(\"hello \",a);\nvar c=2;\nvar d=3;\nprintln(\"hello world!\");\nvar l=list();\nvar q=queue();\n",
        1
    );
    print("\n");
    diff(
        "A\nB\nC\nA\nB\nB\nA\n",
        "C\nB\nA\nB\nA\nC\n",
        1
    );
    print("\n");
    diff(
        io.readfile("test/bf.nas"),
        io.readfile("test/bfconvertor.nas")
    );
}(myers);