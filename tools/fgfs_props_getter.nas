# used to get property tree from fgfs by httpd
# use --httpd=5500 to start fgfs
# and use this script to get property tree
# 2023/11/06 ValKmjolnir

use module.libsock;
use std.json;

var new_getter = func(hostname, port) {
    var socket = libsock.socket;
    var sd = socket.socket(
        socket.AF_INET,
        socket.SOCK_STREAM,
        socket.IPPROTO_TCP
    );

    while((var err = socket.connect(sd, hostname, port))==socket.SOCKET_ERROR) {
        println(
            "[", os.time(), "] connect to ",
            hostname, ":", port, " failed: ",
            socket.errno()
        );
        unix.sleep(1);
    }
    println("[", os.time(), "] connect to ", hostname, ":", port, " succeeded");

    var getprop = func(path) {
        var header = "GET /json"~path~" HTTP/1.1\n\r\n";
        var res = socket.send(sd, header);
        var message = socket.recv(sd, 1024);

        var total_source = message.str;
        while(message.size!=0) {
            message = socket.recv(sd, 1024);
            total_source ~= message.str;
        }

        var begin_position = find("{", total_source);
        var end_position = find("0\r\n\r\n", total_source);
        var props = substr(total_source, begin_position, end_position-begin_position);
        props = json.parse(props);
        json.check_error();
        if (size(props)==0) {
            println("getprop: node \"", path, "\" not found");
        }

        return props;
    }

    var close = func {
        socket.closesocket(sd);
    }

    return {
        getprop: getprop,
        close: close
    };
}

var dump = func(tree, indent = "") {
    if (size(tree)==0) {
        return;
    }
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
            dump(i, indent~"  ");
        }
    }
}

if (size(arg)<2) {
    println("require hostname and port");
    exit(-1);
}
if (size(arg)>2) {
    println("too many arguments, only require hostname and port");
    exit(-1);
}

var getter = new_getter(arg[0], num(arg[1]));

var get_props = func(path = "/") {
    var props = getter.getprop(path);
    dump(props);
}

get_props();
get_props("/e-tron/dialog/config");
get_props("/e-tron/dialog/config/a");

getter.close();