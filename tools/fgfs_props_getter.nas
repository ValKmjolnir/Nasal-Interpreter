# used to get property tree from fgfs by httpd
# use --httpd=5500 to start fgfs
# and use this script to get property tree
# 2023/11/06 ValKmjolnir

use module.libsock;
use std.json;

var connection = {};
connection.new = func(hostname, port) {
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

    var raw = func(s) {
        var v = split("", s);
        var res = "";
        foreach(var i; v) {
            if (i=="\r") {
                res ~= "\\r";
                continue;
            }
            if (i=="\n") {
                res ~= "\\n\n";
                continue;
            }
            res ~= i;
        }
        return res;
    }

    var getprop = func(path) {
        var header = "GET /json"~path~" HTTP/1.1\n\r\n";
        var res = socket.send(sd, header);
        var message = socket.recv(sd, 1024);
        var head_vector = split("\r\n", message.str);
        if (size(head_vector)<11) {
            println("getprop: node \"", path, "\" not found, invalid header");
            logprint(LOG_DEBUG, raw(message.str));
            return {path: path};
        }
        var message_total_size = num("0x"~head_vector[10]);

        var total_source = message.str;
        var total_size = message.size;
        while(total_size<=message_total_size) {
            message = socket.recv(sd, 1024);
            total_source ~= message.str;
            total_size += message.size;
        }
        if (find("0\r\n\r\n", total_source)<0) {
            message = socket.recv(sd, 1024);
            total_source ~= message.str;
            total_size += message.size;
        }

        var begin_position = find("{", total_source);
        if (begin_position<0) {
            println("getprop: node \"", path, "\" not found, invalid begin token");
            return {path: path};
        }
        var end_position = find("0\r\n\r\n", total_source);
        var length = end_position-begin_position;
        if (length<0) {
            println("getprop: node \"", path, "\" not found, invalid end token");
            return {path: path};
        }
        var data = substr(total_source, begin_position, length);
        var props = json.parse(data);
        if (json.get_error()) {
            println("getprop: encounter error when parsing \"", path, "\"");
            logprint(LOG_DEBUG, data);
            return {path: path};
        }
        if (size(props)==0) {
            println("getprop: node \"", path, "\" not found, empty tree node");
        }

        return props;
    }

    var setprop = func(path, data) {
        var header = "POST /json"~path~" HTTP/1.1\n\n";
        header ~= "{\"value\":\""~data~"\"}\n\r\n";

        var res = socket.send(sd, header);
        var message = socket.recv(sd, 1024);
    }

    var close = func {
        socket.closesocket(sd);
    }

    return {
        getprop: getprop,
        setprop: setprop,
        close: close
    };
}

var dump = func(tree, indent = "") {
    if (size(tree)==0) {
        return;
    }

    println(indent, "-------------------");
    var tree_keys = keys(tree);
    sort(tree_keys, func(a,b) {return cmp(a, b)<0;});
    foreach(var key; tree_keys) {
        if (key == "children") {
            continue;
        }
        println(indent, key, " : \"", tree[key], "\"");
    }
    println(indent, "-------------------");

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

var connect = connection.new(arg[0], num(arg[1]));
var count = 0;
var recursive_get_prop = func(path = "/") {
    count += 1;
    if (math.mod(count, 100)==0) {
        println("get ", count," nodes, now: \"", path, "\"");
    }
    var props = connect.getprop(path);
    var tree = {};
    tree.path = props.path;
    if (!contains(props, "children")) {
        return tree;
    }

    tree.children = [];
    foreach(var child; props.children) {
        var node = {};
        node = recursive_get_prop(child.path);
        append(tree.children, node);
    }
    return tree;
}

# takes about 5 min to get whole tree
var props = recursive_get_prop("/");
dump(props);
