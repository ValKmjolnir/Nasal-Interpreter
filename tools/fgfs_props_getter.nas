# used to get property tree from fgfs by httpd
# use --httpd=5500 to start fgfs
# and use this script to get property tree
# 2023/11/06 ValKmjolnir

use module.libsock;
use std.json;

var socket = libsock.socket;
var sd = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP);

while((var err = socket.connect(sd, "127.0.0.1", 5500))==socket.SOCKET_ERROR) {
    println("[", os.time(), "] connect to 127.0.0.1:5500 failed: ", socket.errno());
    unix.sleep(1);
}
println("[", os.time(), "] connect to 127.0.0.1:5500 succeeded");


var header = "GET /json/ HTTP/1.1\n\r\n";
var res = socket.send(sd, header);
var message = socket.recv(sd, 1024);

var total_source = message.str;
var total_size = message.size;

while(message.size==1024) {
    message = socket.recv(sd, 1024);
    total_source ~= message.str;
    total_size += message.size;
}

socket.closesocket(sd);

var A3E_position = find("{", total_source);
var end_0_position = find("0\r\n\r\n", total_source);
var props = substr(total_source, A3E_position, end_0_position-A3E_position);
props = json.parse(props);
json.check_error();

var dfs = func(tree, indent = "") {
    println(indent, "---------");
    println(indent, "path : \"", tree.path, "\"");
    println(indent, "name : \"", tree.name, "\"");
    println(indent, "index : \"", tree.index, "\"");
    println(indent, "type : \"", tree.type, "\"");
    println(indent, "nChildren : \"", tree.nChildren, "\"");
    println(indent, "---------");
    
    if (contains(tree, "children")) {
        println(indent, "children :");
        foreach(var i; tree.children) {
            dfs(i, indent~"  ");
        }
    }

}

dfs(props);