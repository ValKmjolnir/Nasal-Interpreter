use std.udp;

var argument = arg[0];
if (argument=="server") {
    var server = udp.udp_server("127.0.0.1", 5506);
    while(1) {
        var message = server.recvfrom();
        server.sendto(message.fromip, message.port, "from server");
    }
} elsif (argument=="client") {
    var client = udp.udp_client();
    while(1) {
        client.sendto("127.0.0.1", 5506, "hello");
        var message = client.recvfrom();
        println(message);
        unix.sleep(1);
    }
}
