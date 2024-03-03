use module.libnasock;
use std.os;
use std.unix;

var udp_server = func(hostname, port, retry_delay = 5) {
    var socket = libnasock.socket;
    var server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM);
    while(socket.bind(server, hostname, port) < 0) {
        println("[", os.time(), "] failed to bind socket "~server~" at ", hostname, ":", port, ".");
        unix.sleep(retry_delay);
        println("[", os.time(), "] retrying...");
    }

    return {
        sendto: func(ip, port, message) {
            var res = socket.sendto(server, ip, port, message);
            println("[", os.time(), "] send message to ", ip, ":", port, " ", res, " byte(s)");
            return res;
        },
        recvfrom: func(length = 1024) {
            var message = socket.recvfrom(server, length);
            println("[", os.time(), "] get message \"", message.str, "\" from ", message.fromip, ":", message.port);
            return message;
        }
    }
}

var udp_client = func(hostname = "", port = -1, retry_delay = 5) {
    var socket = libnasock.socket;
    var client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM);
    if (port > 0 and size(hostname) != 0) {
        while(socket.bind(client, hostname, port)<0) {
            println("[",os.time(),"] failed to bind socket "~client~" at ", hostname, ":", port, ".");
            unix.sleep(retry_delay);
            println("[",os.time(),"] retrying...");
        }
    }

    return {
        sendto: func(ip, port, message) {
            var res = socket.sendto(client, ip, port, message);
            println("[", os.time(), "] send message to ", ip, ":", port, " ", res, " byte(s)");
            return res;
        },
        recvfrom: func(length = 1024) {
            var message = socket.recvfrom(client, length);
            println("[", os.time(), "] get message \"", message.str, "\" from ", message.fromip, ":", message.port);
            return message;
        }
    }
}