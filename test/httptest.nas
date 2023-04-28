import.module.libsock;

var http=func(){
    var sd=nil;
    return {
        establish:func(ip,port){
            sd=socket.socket(socket.AF_INET,socket.SOCK_STREAM,socket.IPPROTO_IP);
            if(socket.bind(sd,ip,port)<0){
                println("failed to bind socket "~sd~" at IP: "~ip~" port: "~port~".");
                return;
            }
            socket.listen(sd,1);
            println("[",os.time(),"] start server at [",ip,":",port,"]");
        },
        shutdown:func(){
            println("[",os.time(),"] shutdown server");
            socket.closesocket(sd);
        },
        accept:func(){
            return socket.accept(sd);
        },
        disconnect:func(client,log=0){
            if(log)
                println("[",os.time(),"] [",client.ip,"] disconnected");
            return socket.closesocket(client.sd);
        },
        recv:func(client){
            var data=socket.recv(client.sd,2048);
            if(!data.size){
                println("[",os.time(),"] [",client.ip,"] closed connection");
                return nil;
            }
            var first=split("\n",data.str)[0];
            var (type,path)=split(" ",first)[0,1];
            println("[",os.time(),"] [",client.ip,"] request ",type," [",path,"]");
            return {type:type,path:path};
        },
        send:func(client,content){
            println("[",os.time(),"] [",client.ip,"] get size ",size(content)," byte(s)");
            return socket.send(client.sd,content);
        }
    };
}();
http.establish("127.0.0.1",8080);

var highlight_style="
<style>
    body{
        background: #303030;
    }
    pre{
        background: #303030;
        font-family: 'Courier New', Courier, monospace;
        font-size: small;
        color: #d4d4d4;
    }
    code{
        color: white;
        font-family: 'Courier New', Courier, monospace;
        font-size: small;
        text-align: left;
    }
    code.key{color: #f895e7;}
    code.id{color: #8abef0;}
    code.opr{color: #f895e7;}
    code.brace{color: #eafd70;}
    code.str{color: #a5ffd0;}
    code.num{color: #ff9a41;}
    code.note{color: #808080;}
</style>";

var html_read_file=func(filename){
    var timer=maketimestamp();
    timer.stamp();
    var keyword=["var","func","for","while","foreach","forindex","break","continue","return","if","else","elsif","nil"];
    var file_text=io.readfile(filename);
    var (s,index,len)=("",-1,size(file_text));
    var content="";

    var next=func(){
        if(index+1>=len)
            return index+1;
        index+=1;
        s=char(file_text[index]);
        return index;
    }
    var prev=func(){
        index-=1;
        s=char(file_text[index]);
    }

    while(1){
        if(next()>=len)
            break;
        if(s==">")
            content~="<code class=\"opr\">&gt;</code>";
        elsif(s=="<")
            content~="<code class=\"opr\">&lt;</code>";
        elsif(s=="[" or s=="]" or s=="(" or s==")" or s=="{" or s=="}")
            content~="<code class=\"brace\">"~s~"</code>";
        elsif(s=="=" or s=="," or s==";" or s==":" or s=="|" or s=="&" or s=="!" or s=="?" or s=="+" or s=="-" or s=="*" or s=="/" or s=="~" or s==".")
            content~="<code class=\"opr\">"~s~"</code>";
        elsif(s=="_" or ("a"[0]<=s[0] and s[0]<="z"[0]) or ("A"[0]<=s[0] and s[0]<="Z"[0]) or s[0]<0 or s[0]>=128){
            var tmp=""~s; # generate a new string
            while(1){
                if(next()>=len)
                    break;
                if(s=="_" or ("a"[0]<=s[0] and s[0]<="z"[0]) or ("A"[0]<=s[0] and s[0]<="Z"[0]) or ("0"[0]<=s[0] and s[0]<="9"[0]) or s[0]<0 or s[0]>=128)
                    tmp~=s;
                else{
                    prev();
                    break;
                }
            }
            var is_key=0;
            foreach(var i;keyword)
                if(tmp==i){
                    is_key=1;
                    content~="<code class=\"key\">"~tmp~"</code>";
                    break;
                }
            if(!is_key)
                content~="<code class=\"id\">"~tmp~"</code>";
        }elsif("0"[0]<=s[0] and s[0]<="9"[0]){
            content~="<code class=\"num\">"~s;
            if(next()>=len){
                content~="</code>";
                break;
            }
            if(s=="o"){
                content~="o";
                while(1){
                    if(next()>=len)
                        break;
                    if("0"[0]<=s[0] and s[0]<="7"[0])
                        content~=s;
                    else
                        break;
                }
                content~="</code>";
                prev();
            }elsif(s=="x"){
                content~="x";
                while(1){
                    if(next()>=len)
                        break;
                    if(("0"[0]<=s[0] and s[0]<="9"[0]) or ("a"[0]<=s[0] and s[0]<='f') or ("A"[0]<=s[0] or s[0]<="F"))
                        content~=s;
                    else
                        break;
                }
                content~="</code>";
                prev();
            }elsif(("0"[0]<=s[0] and s[0]<="9"[0]) or s=="." or s=="e"){
                while("0"[0]<=s[0] and s[0]<="9"[0]){
                    content~=s;
                    if(next()>=len)
                        break;
                }
                if(s=="."){
                    content~=s;
                    if(next()>=len)
                        break;
                }
                while("0"[0]<=s[0] and s[0]<="9"[0]){
                    content~=s;
                    if(next()>=len)
                        break;
                }
                if(s=="e"){
                    content~=s;
                    if(next()>=len)
                        break;
                    if(s=="-" or s=="+"){
                        content~=s;
                        if(next()>=len)
                            break;
                    }
                }
                while("0"[0]<=s[0] and s[0]<="9"[0]){
                    content~=s;
                    if(next()>=len)
                        break;
                }
                prev();
                content~="</code>";
            }else{
                prev();
                content~="</code>";
            }
        }elsif(s=="\"" or s=="'" or s=="`"){
            var quot=s~""; # generate a new string
            content~="<code class=\"str\">"~s;
            while(1){
                if(next()>=len)
                    break;
                if(s==quot){
                    content~=s~"</code>";
                    break;
                }elsif(s=="\\"){
                    content~=s;
                    if(next()>=len)
                        break;
                    content~=s;
                }elsif(s==">"){
                    content~="&gt;";
                }elsif(s=="<"){
                    content~="&lt;";
                }else{
                    content~=s;
                }
            }
        }elsif(s=="#"){
            content~="<code class=\"note\">"~s;
            while(1){
                if(next()>=len)
                    break;
                if(s=="\n" or s=="\r"){
                    content~=s;
                    break;
                }elsif(s==">"){
                    content~="&gt;";
                }elsif(s=="<"){
                    content~="&lt;";
                }else{
                    content~=s;
                }
            }
            content~="</code>";
        }
        else
            content~=s;
    }
    println("[",os.time(),"] analyzed [",filename,"] for ",timer.elapsedMSec(),"ms");
    return content;
}

var respond={
    ok:func(html){
        println("[",os.time(),"] respond 200 OK");
        return "Http/1.1 200 OK\n\n"~html~"\n";
    },
    not_found:func(){
        println("[",os.time(),"] respond 404 NOT FOUND");
        return "Http/1.1 404 NOT FOUND\n\n<!DOCTYPE html>
            <head>
                <title> 404 not found </title>
                <meta charset=\"utf-8\">
            </head>
            <body>
                <text>
                    404 NOT FOUND!
                </text>
            </body>
        </html>\n";
    },
    teapot:func(){
        println("[",os.time(),"] respond 418 I'm a teapot");
        return "Http/1.1 418 I'm a teapot\n\n<!DOCTYPE html>
            <head>
                <title> I'm a teapot </title>
                <meta charset=\"utf-8\">
            </head>
            <body>
                <text>
                    This server cannot brew coffee because it is a teapot.
                </text>
            </body>
        </html>\n";
    }
};

var files=func(){
    var res={};
    var dd=unix.opendir("./test");
    while((var name=unix.readdir(dd))!=nil)
        res[name]=1;
    return res;
}();

while(1){
    var client=http.accept();
    var data=http.recv(client);
    if(data==nil){
        http.disconnect(client);
        continue;
    }
    if(data.type=="GET"){
        var path=data.path;
        var args=split("?",path);
        var tmp={};
        if(size(args)==2){
            path=args[0];
            args=split("=",args[1]);
            for(var i=0;i<size(args);i+=2)
                tmp[args[i]]=args[i+1];
        }
        if(path=="/" or path=="/index"){
            if(contains(tmp,"filename") and contains(files,tmp.filename)){
                var filename=tmp.filename;
                var page="<!DOCTYPE html><head><title> "~filename~" </title><meta charset=\"utf-8\">"~highlight_style~"</head>\n<body><pre>\n";
                var page_back="</pre>\n</body>\n</html>\n";
                http.send(client,respond.ok(page~html_read_file("./test/"~filename)~page_back));
            }else{
                http.send(client,respond.ok(io.readfile("./doc/nasal-http-test-web.html")));
            }
        }
        elsif(path=="/shutdown"){
            http.send(client,respond.ok("http server shutdown."));
            break;
        }
        elsif(path=="/favicon.ico")
            http.send(client,respond.ok(io.readfile("./doc/pic/favicon.ico")));
        elsif(path=="/license")
            http.send(client,respond.ok(io.readfile("./LICENSE")));
        elsif(path=="/doc/pic/nasal.png" or path=="/doc/pic/benchmark.png" or path=="/doc/pic/mandelbrot.png")
            http.send(client,respond.ok(io.readfile("."~path)));
        else{
            var filename=substr(path,1,size(path)-1);
            if(contains(files,filename)){
                var page="<!DOCTYPE html><head><title> "~filename~" </title><meta charset=\"utf-8\">"~highlight_style~"</head>\n<body><pre>\n";
                var page_back="</pre>\n</body>\n</html>\n";
                http.send(client,respond.ok(page~html_read_file("./test/"~filename)~page_back));
            }
            elsif(filename=="teapot")
                http.send(client,respond.teapot());
            else
                http.send(client,respond.not_found());
        }
    }elsif(data.type=="POST"){
        http.send(client,respond.not_found);
    }
    http.disconnect(client);
}
http.shutdown();