import.module.libsock;
import.stl.json;

var gettime=func(){
    return split(" ",os.time())[1];
}

var jsonRPC=func(){
    var sd=nil;
    return {
        establish:func(ip,port) {
            sd=socket.socket(socket.AF_INET,socket.SOCK_STREAM,socket.IPPROTO_IP);
            if(socket.bind(sd,ip,port)<0) {
                println("[",gettime(),"] failed to bind socket ",sd," at ",ip,":",port);
                return 0;
            }
            socket.listen(sd,1);
            println("[",gettime(),"] start connection at [",ip,":",port,"]");
            return 1;
        },
        connect:func(ip,port) {
            sd=socket.socket(socket.AF_INET,socket.SOCK_STREAM,socket.IPPROTO_IP);
            while(socket.connect(sd,ip,port)<0) {
                println("[",gettime(),"] failed to connect socket ",sd," to ",ip,":",port);
                unix.sleep(5);
            }
            println("[",gettime(),"] start connection at [",ip,":",port,"]");
            return {ip:ip,sd:sd}; # get server ip and sd
        },
        shutdown:func() {
            println("[",gettime(),"] shutdown");
            socket.closesocket(sd);
        },
        accept:func() {
            println("[",gettime(),"] accpet sd ",sd);
            return socket.accept(sd);
        },
        disconnect:func(client) {
            println("[",gettime(),"] disconnected");
            return socket.closesocket(client.sd);
        },
        recv:func(client){
            var data=socket.recv(client.sd,2048);
            if(data.size<=0){
                println("[",gettime(),"] closed connection, recv failed");
                return nil;
            }
            println("[",gettime(),"] receive ",data.str);
            return data.str;
        },
        send:func(client,content){
            println("[",gettime(),"] sending ",content);
            return socket.send(client.sd,content);
        }
    };
}();

var server=func(ip,port) {
    var methods={
        callHello:func(vec) {
            var res="hello ";
            foreach(var i;vec) {
                res~=i~" ";
            }
            return res;
        },
        onRandom:func(vec) {
            return "rand: "~str(rand());
        }
    };

    jsonRPC.establish(ip,port);
    while(1) {
        var client=jsonRPC.accept();
        while(1) {
            var data=jsonRPC.recv(client);
            if (data!=nil) {
                data=JSON.parse(data);
            } else {
                break;
            }
            if (contains(methods,data.method)) {
                jsonRPC.send(client, JSON.stringify({
                    jsonrpc:2.0,
                    id:data.id,
                    error:"null",
                    result:methods[data.method](data.params)
                }));
            } else {
                jsonRPC.send(client, JSON.stringify({
                    jsonrpc:2.0,
                    id:data.id,
                    error:"no such method \\\""~data.method~"\\\"",
                    result:"null"
                }));
            }
            
        }
    }
    jsonRPC.disconnect(client);
}

var client=func(ip,port) {
    srand();
    var call_id=1;
    var methods=["callHello","notExist","onRandom"];
    var params=[["a","b"],["1","2"]];

    var server=jsonRPC.connect(ip,port);
    while(1) {
        unix.sleep(5);
        var data=JSON.stringify({jsonrpc:2.0, id:call_id, method:methods[rand()*size(methods)],params:params[rand()*size(params)]});
        jsonRPC.send(server, data);
        var respond=jsonRPC.recv(server);
        if (respond==nil) {
            break;
        }
        println("[",gettime(),"] result: ",JSON.parse(respond).result);
        call_id+=1;
    }
    jsonRPC.disconnect(server);
}

func() {
    var args=runtime.argv();
    if (size(args)!=0) {
        if (args[0]=="--server") {
            server("127.0.0.1",5007);
        } elsif (args[0]=="--client") {
            client("127.0.0.1",5007);
        }
    }
}();