import.module.libsock;
import.stl.json;

var jsonrpc=func(){
    var sd=nil;
    return {
        establish:func(ip,port) {
            sd=socket.socket(socket.AF_INET,socket.SOCK_STREAM,socket.IPPROTO_IP);
            if(socket.bind(sd,ip,port)<0) {
                println("failed to bind socket "~sd~" at "~ip~":"~port~".");
                return 0;
            }
            socket.listen(sd,1);
            println("[",os.time(),"] start connection at [",ip,":",port,"]");
            return 1;
        },
        connect:func(ip,port) {
            sd=socket.socket(socket.AF_INET,socket.SOCK_STREAM,socket.IPPROTO_IP);
            while(socket.connect(sd,ip,port)<0) {
                println("[",os.time(),"] failed to connect socket "~sd~" to "~ip~":"~port~", retrying.");
                unix.sleep(1);
            }
            println("[",os.time(),"] start connection at [",ip,":",port,"]");
            return {ip:ip,sd:sd}; # get server ip and sd
        },
        shutdown:func() {
            println("[",os.time(),"] shutdown");
            socket.closesocket(sd);
        },
        accept:func() {
            return socket.accept(sd);
        },
        disconnect:func(client) {
            println("[",os.time(),"] [",client.ip,"] disconnected");
            return socket.closesocket(client.sd);
        },
        recv:func(client){
            var data=socket.recv(client.sd,2048);
            if(data.size<=0){
                println("[",os.time(),"] [",client.ip,"] closed connection");
                return nil;
            }
            println("[",os.time(),"] [",client.ip,"] receive ",data.str);
            return data.str;
        },
        send:func(client,content){
            println("[",os.time(),"] [",client.ip,"] sending ",content);
            return socket.send(client.sd,content);
        }
    };
}();

var id=1;
var args=runtime.argv();
if (size(args)!=0) {
    if (args[0]=="--server") {
        jsonrpc.establish("127.0.0.1",8080);
        var client=jsonrpc.accept();
        jsonrpc.send(client, JSON.stringify({jsonrpc:2.0, id:1, result:{info:"from server: first info"}}));
        while(1) {
            var data=jsonrpc.recv(client);
            if (data!=nil) {
                data=JSON.parse(data);
            } else {
                break;
            }
            jsonrpc.send(client, JSON.stringify({jsonrpc:2.0, id:data.id+1, result:{info:"from server"}}));
        }
        jsonrpc.disconnect(client);
    } elsif (args[0]=="--client") {
        var server=jsonrpc.connect("127.0.0.1",8080);
        while(1) {
            unix.sleep(1);
            var respond=JSON.parse(jsonrpc.recv(server));
            var data=JSON.stringify({jsonrpc:2.0, id:respond.id+1, result:{info:"from client"}});
            jsonrpc.send(server, data);
        }
        jsonrpc.disconnect(server);
    }
}