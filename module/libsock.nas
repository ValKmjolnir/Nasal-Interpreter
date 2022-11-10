var socket=func(){
    var lib=dylib.dlopen("libnasock"~(os.platform()=="windows"?".dll":".so"));
    
    var sock=dylib.dlsym(lib,"nas_socket");
    var closesocket=dylib.dlsym(lib,"nas_closesocket");
    var shutdown=dylib.dlsym(lib,"nas_shutdown");
    var bind=dylib.dlsym(lib,"nas_bind");
    var listen=dylib.dlsym(lib,"nas_listen");
    var connect=dylib.dlsym(lib,"nas_connect");
    var accept=dylib.dlsym(lib,"nas_accept");
    var send=dylib.dlsym(lib,"nas_send");
    var sendto=dylib.dlsym(lib,"nas_sendto");
    var recv=dylib.dlsym(lib,"nas_recv");
    var recvfrom=dylib.dlsym(lib,"nas_recvfrom");
    var errno=dylib.dlsym(lib,"nas_errno");

    var (invoke,invoke_i,invoke_ii,invoke_iii,invoke_iiii,invoke_iiiii)=(
        dylib.limitcall(0),
        dylib.limitcall(1),
        dylib.limitcall(2),
        dylib.limitcall(3),
        dylib.limitcall(4),
        dylib.limitcall(5),
    );
    return {
        AF_UNSPEC:0,
        AF_UNIX:1,
        AF_INET:2,
        AF_IMPLINK:3,
        AF_PUP:4,
        AF_CHAOS:5,
        AF_IPX:6,
        AF_NS:6,
        AF_ISO:7,
        AF_OSI:7,
        AF_ECMA:8,
        AF_DATAKIT:9,
        AF_CCITT:10,
        AF_SNA:11,
        AF_DECnet:12,
        AF_DLI:13,
        AF_LAT:14,
        AF_HYLINK:15,
        AF_APPLETALK:16,
        AF_NETBIOS:17,
        AF_VOICEVIEW:18,
        AF_FIREFOX:19,
        AF_UNKNOWN1:20,
        AF_BAN:21,
        AF_MAX:22,

        SOCK_STREAM:1,
        SOCK_DGRAM:2,
        SOCK_RAW:3,
        SOCK_RDM:4,
        SOCK_SEQPACKET:5,

        IPPROTO_IP:0,IPPROTO_ICMP:1,IPPROTO_IGMP:2,IPPROTO_GGP:3,
        IPPROTO_TCP:6,IPPROTO_PUP:12,IPPROTO_UDP:17,IPPROTO_IDP:22,
        IPPROTO_ND:77,IPPROTO_RAW:255,IPPROTO_MAX:256,

        IPPORT_ECHO:7,IPPORT_DISCARD:9,IPPORT_SYSTAT:11,IPPORT_DAYTIME:13,
        IPPORT_NETSTAT:15,IPPORT_FTP:21,IPPORT_TELNET:23,IPPORT_SMTP:25,
        IPPORT_TIMESERVER:37,IPPORT_NAMESERVER:42,IPPORT_WHOIS:43,IPPORT_MTP:57,
        IPPORT_TFTP:69,IPPORT_RJE:77,IPPORT_FINGER:79,IPPORT_TTYLINK:87,
        IPPORT_SUPDUP:95,IPPORT_EXECSERVER:512,IPPORT_LOGINSERVER:513,IPPORT_CMDSERVER:514,
        IPPORT_EFSSERVER:520,IPPORT_BIFFUDP:512,IPPORT_WHOSERVER:513,IPPORT_ROUTESERVER:520,
        IPPORT_RESERVED:1024,

        SHUT_RD  :0x00,
        SHUT_WR  :0x01,
        SHUT_RDWR:0x02,

        MSG_OOB:0x1,
        MSG_PEEK:0x2,
        MSG_DONTROUTE:0x4,

        socket:func(af,type,proto){
            return invoke_iii(sock,af,type,proto);
        },
        closesocket:func(sd){
            return invoke_i(closesocket,sd);
        },
        shutdown: func(sd,how){
            return invoke_ii(shutdown,sd,how);
        },
        bind: func(sd,ip,port){
            return invoke_iii(bind,sd,ip,port);
        },
        listen: func(sd,backlog){
            return invoke_ii(listen,sd,backlog);
        },
        connect: func(sd,hostname,port){
            return invoke_iii(connect,sd,hostname,port);
        },
        accept: func(sd){
            return invoke_i(accept,sd);
        },
        send: func(sd,buff,flags=0){
            return invoke_iii(send,sd,buff,flags);
        },
        sendto: func(sd,hostname,port,buff,flags=0){
            return invoke_iiiii(sendto,sd,hostname,port,buff,flags);
        },
        recv: func(sd,len,flags=0){
            return invoke_iii(recv,sd,len,flags);
        },
        recvfrom: func(sd,len,flags=0){
            return invoke_iii(recvfrom,sd,len,flags);
        },
        errno: func(){
            return invoke(errno);
        }
    };
}();