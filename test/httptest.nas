import("module/libsock.nas");

var sd=socket.socket(socket.AF_INET,socket.SOCK_STREAM,socket.IPPROTO_IP);
socket.bind(sd,"127.0.0.1",8080);
socket.listen(sd,1);

# var filter={
#     " ":"&nbsp;",
#     "&":"&amp;",
#     "<":"&lt;",
#     ">":"&gt;",
#     "\"":"&quot;",
#     "'":"&qpos;"
# };
# var html_read_file=func(filename){
#     var fd=io.open(filename,"rb");
#     var content="";
#     while(1){
#         var s=mut(" ");
#         if(io.read(fd,s,1)==0)
#             break;
#         if(contains(filter,chr(s[0])))
#             content~=filter[chr(s[0])];
#         else
#             content~=s;
#     }
#     io.close(fd);
#     return content;
# }

var respond={
    ok:func(html){
        return "Http/1.1 200 OK\n\n"~html~"\n";
    },
    not_found:"Http/1.1 404 NOT FOUND\n\n<!DOCTYPE html>
        <head>
            <title> 404 not found </title>
            <meta charset=\"utf-8\">
        </head>
        <body>
            <text>
                404 NOT FOUND!
            </text>
        </body>
    </html>\n",
    teapot:"Http/1.1 418 I'm a teapot\n\n<!DOCTYPE html>
        <head>
            <title> I'm a teapot </title>
            <meta charset=\"utf-8\">
        </head>
        <body>
            <text>
                This server cannot brew coffee because it is a teapot.
            </text>
        </body>
    </html>\n"
};

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
        println("[",os.time(),"] ",client.ip," request connection closed");
        continue;
    }
    var first=split("\n",recieve_data.str)[0];
    var (type,path)=split(" ",first)[0,1];
    println("[",os.time(),"] ",client.ip," request ",type," [",path,"]");
    if(path=="/" or path=="/index")
        socket.send(client.sd,respond.ok(io.fin("./doc/nasal-http-test-web.html")));
    elsif(path=="/favicon.ico")
        socket.send(client.sd,respond.ok(io.fin("./pic/favicon.ico")));
    elsif(path=="/pic/nasal.png" or path=="/pic/benchmark.png" or path=="/pic/mandelbrot.png")
        socket.send(client.sd,respond.ok(io.fin("."~path)));
    else{
        var filename=substr(path,1,size(path)-1);
        if(contains(files,filename)){
            # var page="<head><title> "~filename~" </title><meta charset=\"utf-8\"></head>\n<body><pre>\n";
            # var page_back="</pre>\n</body>\n";
            # socket.send(client.sd,respond.ok(page~io.fin("./test/"~filename)~page_back));
            socket.send(client.sd,respond.ok(io.fin("./test/"~filename)));
        }
        elsif(filename=="teapot")
            socket.send(client.sd,respond.teapot);
        else
            socket.send(client.sd,respond.not_found);
    }
    socket.closesocket(client.sd);
}
socket.closesocket(sd);