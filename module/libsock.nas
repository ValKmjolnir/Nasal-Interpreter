var socket=func(){
    var lib=dylib.dlopen("./module/libnasock"~(os.platform()=="windows"?".dll":".so"));
    var sock=dylib.dlsym(lib,"nas_socket");
    var closesocket=dylib.dlsym(lib,"nas_closesocket");
    var call=dylib.dlcall;
    return {
        AF_UNSPEC:0,AF_UNIX:1,AF_INET:2,AF_IMPLINK:3,
        AF_PUP:4,AF_CHAOS:5,AF_IPX:6,AF_NS:6,
        AF_ISO:7,AF_OSI:7,AF_ECMA:8,AF_DATAKIT:9,
        AF_CCITT:10,AF_SNA:11,AF_DECnet:12,AF_DLI:13,
        AF_LAT:14,AF_HYLINK:15,AF_APPLETALK:16,AF_NETBIOS:17,
        AF_VOICEVIEW:18,AF_FIREFOX:19,AF_UNKNOWN1:20,AF_BAN:21,
        AF_MAX:22,

        SOCK_STREAM:1,SOCK_DGRAM:2,SOCK_RAW:3,SOCK_RDM:4,
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

        socket:func(af,type,proto){
            return call(sock,af,type,proto);
        },
        closesocket:func(sd){
            return call(closesocket,sd);
        }
    };
}();