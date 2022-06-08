import("module/libsock.nas");

var sd=socket.socket(socket.AF_INET,socket.SOCK_STREAM,socket.IPPROTO_IP);
socket.bind(sd,"127.0.0.1",8080);
socket.listen(sd,1);

var httpheader="Http/1.1 200 OK\n\n";
var httptail="\n";
var index="<html>
    <head>
        <title> nasal-http-test-web </title>
    </head>
    <body>
        <text>
            Hello, this is a simple HTML document just for test.<br />
            This simple http server is written in nasal.<br />
            Nasal is an ECMAscript-like programming language that used in Flightgear.<br />
            This language is designed by Andy Ross.<br />

            The interpreter is totally rewritten by ValKmjolnir using C++(-std=c++11) without reusing the code in Andy Ross's nasal interpreter.<br />
            But we really appreciate that Andy created this amazing programming language and his interpreter project.<br />

            Now this project uses MIT license(2021/5/4). Edit it if you want, use this project to learn or create more interesting things(But don't forget me XD).
        </text>
    </body>
</html>";
var notfound="<html>
    <head>
        <title> nasal-http-test-web </title>
    </head>
    <body>
        <text>
            404 NOT FOUND!
        </text>
    </body>
</html>";

while(1){
    var client=socket.accept(sd);
    var recieve_data=socket.recv(client.sd,1024);
    if(!recieve_data.size){
        println("[",client.ip,"] request connection closed");
        break;
    }
    var first=split("\n",recieve_data.str)[0];
    var (type,path)=split(" ",first)[0,1];
    println("[",client.ip,"] request ",type," [",path,"]");
    if(path=="/")
        socket.send(client.sd,httpheader~index~httptail);
    elsif(path=="/favicon.ico")
        socket.send(client.sd,httpheader~io.fin("pic/favicon.ico")~httptail);
    else
        socket.send(client.sd,httpheader~notfound~httptail);
    socket.closesocket(client.sd);
}
socket.closesocket(sd);