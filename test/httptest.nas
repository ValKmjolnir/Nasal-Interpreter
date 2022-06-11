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
        <img src=\"/nasal.png\" width=\"100\" height=\"40\"><br /></img>
        <text>
            Hello, this is a simple HTML document just for test. This simple http server is written in nasal.<br />
            Nasal is an ECMAscript-like programming language that used in Flightgear designed by Andy Ross.<br /><br />
            The interpreter is totally rewritten by ValKmjolnir using C++(-std=c++11) without reusing the code in Andy Ross's nasal interpreter.<br />
            But we really appreciate that Andy created this amazing programming language and his interpreter project.<br /><br />
            Now this project uses MIT license(2021/5/4). Edit it if you want, use this project to learn or create more interesting things(But don't forget me XD).<br/>
        </text>
        <img src=\"/benchmark.png\" width=\"400\" height=\"300\"></img>
        <img src=\"/mandelbrot.png\" width=\"400\" height=\"300\"><br /></img>
        <text>
            Here is the benchmark of different versions of nasal interpreter(left).<br />
            Look at this beautiful picture(right).<br />
            Nasal can run this test file(test/bfcolored.nas) in about 220 seconds.<br />
            In fact this test file cost over 2200 seconds before ver 8.0 .<br />
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

var files=func(){
    var res={};
    var dd=unix.opendir("./test");
    while((var name=unix.readdir(dd))!=nil)
        res[name]=1;
    return res;
}();

while(1){
    var client=socket.accept(sd);
    var recieve_data=socket.recv(client.sd,1024);
    if(!recieve_data.size){
        println("[",client.ip,"] request connection closed");
        continue;
    }
    var first=split("\n",recieve_data.str)[0];
    var (type,path)=split(" ",first)[0,1];
    println("[",client.ip,"] request ",type," [",path,"]");
    if(path=="/")
        socket.send(client.sd,httpheader~index~httptail);
    elsif(path=="/favicon.ico")
        socket.send(client.sd,httpheader~io.fin("./pic/favicon.ico")~httptail);
    elsif(path=="/nasal.png")
        socket.send(client.sd,httpheader~io.fin("./pic/nasal.png")~httptail);
    elsif(path=="/benchmark.png")
        socket.send(client.sd,httpheader~io.fin("./pic/benchmark.png")~httptail);
    elsif(path=="/mandelbrot.png")
        socket.send(client.sd,httpheader~io.fin("./pic/mandelbrot.png")~httptail);
    else{
        var filename=substr(path,1,size(path)-1);
        if(contains(files,filename))
            socket.send(client.sd,httpheader~io.fin("./test/"~filename)~httptail)
        else
            socket.send(client.sd,httpheader~notfound~httptail);
    }
    socket.closesocket(client.sd);
}
socket.closesocket(sd);