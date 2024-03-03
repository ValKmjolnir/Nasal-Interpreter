# used to get property tree from fgfs by httpd
# use --httpd=5500 to start fgfs
# and use this script to get property tree
# 2023/11/06 ValKmjolnir

use module.libnasock;
use std.json;
use std.os;
use std.unix;

var _raw_str = func(s) {
    var v = split("", s);
    var res = "";
    foreach(var i; v) {
        if (i=="\r") {
            res ~= "\\r";
            continue;
        }
        if (i=="\n") {
            res ~= "\\n";
            continue;
        }
        res ~= i;
    }
    return res;
}

var _get_time = func() {
    return "["~os.time()~"]";
}

var _connect = func(hostname, port) {
    var socket = libnasock.socket;
    var sd = socket.socket(
        socket.AF_INET,
        socket.SOCK_STREAM,
        socket.IPPROTO_TCP
    );
    var ip_info = hostname~":"~port;
    while((var err = socket.connect(sd, hostname, port))==socket.SOCKET_ERROR) {
        println(_get_time(), " failed to connect ", ip_info, ": ", socket.errno());
        unix.sleep(1);
    }
    println(_get_time(), " connect to ", ip_info, " succeeded");
    return sd;
}

var new = func(hostname, port) {
    var socket = libnasock.socket;
    var sd = _connect(hostname, port);

    var getprop = func(path) {
        if (size(path)==0 or path[0]!='/'[0]) {
            println("node \"", path, "\" not found, invalid path");
            return {path: path};
        }
        # GET header
        var header = "GET /json"~path~" HTTP/1.1\r\n\r\n";
        var res = socket.send(sd, header);

        # get message head 1024
        var message = socket.recv(sd, 1024);
        if (find("404 Not Found", message.str)>=0) {
            println("node \"", path, "\" not found, get 404 response");
            return {path: path};
        }
        # get total message
        var total_source = message.str;
        # 0\r\n\r\n is the tail of chunked http info
        while(find("0\r\n\r\n", total_source)<0) {
            message = socket.recv(sd, 1024);
            total_source ~= message.str;
        }

        # get json in this message
        var begin_position = find("{", total_source);
        var end_position = find("0\r\n\r\n", total_source);
        var length = end_position-begin_position;
        if (begin_position<0) {
            println("node \"", path, "\" not found, invalid begin token");
            return {path: path};
        }
        if (length<0) {
            println("node \"", path, "\" not found, invalid end token");
            return {path: path};
        }
        var data = substr(total_source, begin_position, length);

        # parse this json and return
        var props = json.parse(data);
        if (size(json.get_error())>0) {
            println("encounter error when parsing \"", path, "\":\n", json.get_error());
            logprint(LOG_DEBUG, _raw_str(data));
            return {path: path};
        }

        # empty prop node is not allowed...
        if (size(props)==0) {
            println("node \"", path, "\" not found, empty tree node");
        }

        return props;
    }

    var setprop = func(path, data) {
        if (size(path)==0 or path[0]!='/'[0]) {
            println("node \"", path, "\" not found, invalid path");
            return {path: path};
        }
        # POST header
        var header = "POST /json"~path~" HTTP/1.1\r\n\r\n";
        # generate value
        header ~= "{\"value\":\""~data~"\"}\r\n\r\n";

        var res = socket.send(sd, header);
        var message = socket.recv(sd, 1024);
    }

    return {
        getprop: getprop,
        setprop: setprop
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
        foreach(var i; tree.children) {
            dump(i, indent~"    ");
        }
    }
}

