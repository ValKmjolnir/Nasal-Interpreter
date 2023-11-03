#include "../src/nasal.h"
#include "../src/nasal_type.h"
#include "../src/nasal_gc.h"

#ifndef _MSC_VER
#include <unistd.h>
#endif

#ifdef _WIN32
#include <winsock.h>
#pragma comment(lib,"ws2_32")

class WSAmanager {
private:
    WSAData data;
public:
    WSAmanager() {
        WSAStartup(0x1010, &data);
    }
    ~WSAmanager() {
        WSACleanup();
    }
};

static WSAmanager win;
#else
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

namespace nasal {

var nas_socket(var* args, usize size, gc* ngc) {
    if (args[0].type!=vm_num || args[1].type!=vm_num || args[2].type!=vm_num)
        return nas_err("socket", "\"af\", \"type\", \"protocol\" should be number");
    int sd = socket(args[0].num(), args[1].num(), args[2].num());
    return var::num(static_cast<double>(sd));
}

var nas_closesocket(var* args, usize size, gc* ngc) {
    if (args[0].type!=vm_num)
        return nas_err("closesocket", "\"sd\" should be number");
#ifdef _WIN32
    return var::num(static_cast<double>(closesocket(args[0].num())));
#else
    return var::num(static_cast<double>(close(args[0].num())));
#endif
}

var nas_shutdown(var* args, usize size, gc* ngc) {
    if (args[0].type!=vm_num)
        return nas_err("shutdown", "\"sd\" must be a number");
    if (args[1].type!=vm_num)
        return nas_err("shutdown", "\"how\" must be a number");
    return var::num(static_cast<double>(shutdown(args[0].num(), args[1].num())));
}

var nas_bind(var* args, usize size, gc* ngc) {
    if (args[0].type!=vm_num)
        return nas_err("bind", "\"sd\" muse be a number");
    if (args[1].type!=vm_str)
        return nas_err("bind", "\"ip\" should be a string including an ip with correct format");
    if (args[2].type!=vm_num)
        return nas_err("bind", "\"port\" must be a number");
    sockaddr_in server;
    memset(&server, 0, sizeof(sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(args[1].str().c_str());
    server.sin_port = htons(args[2].num());
    return var::num(static_cast<double>(bind(
        args[0].num(),
        (sockaddr*)&server,
        sizeof(server)
    )));
}

var nas_listen(var* args, usize size, gc* ngc) {
    if (args[0].type!=vm_num)
        return nas_err("listen", "\"sd\" must be a number");
    if (args[1].type!=vm_num)
        return nas_err("listen", "\"backlog\" must be a number");
    return var::num(static_cast<double>(listen(args[0].num(), args[1].num())));
}

var nas_connect(var* args, usize size, gc* ngc) {
    if (args[0].type!=vm_num)
        return nas_err("connect", "\"sd\" must be a number");
    if (args[1].type!=vm_str)
        return nas_err("connect", "\"hostname\" must be a string");
    if (args[2].type!=vm_num)
        return nas_err("connect", "\"port\" must be a number");
    sockaddr_in addr;
    memset(&addr, 0, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(args[2].num());
    hostent* entry = gethostbyname(args[1].str().c_str());
    memcpy(&addr.sin_addr, entry->h_addr, entry->h_length);
    return var::num(static_cast<double>(connect(
        args[0].num(),
        reinterpret_cast<sockaddr*>(&addr),
        sizeof(sockaddr_in)
    )));
}

var nas_accept(var* args, usize size, gc* ngc) {
    if (args[0].type!=vm_num)
        return nas_err("accept", "\"sd\" must be a number");
    sockaddr_in client;
    int socklen = sizeof(sockaddr_in);
#ifdef _WIN32
    int client_sd = accept(args[0].num(), (sockaddr*)&client, &socklen);
#else
    int client_sd = accept(args[0].num(), (sockaddr*)&client, (socklen_t*)&socklen);
#endif
    var res = ngc->temp = ngc->alloc(vm_hash);
    auto& hash = res.hash().elems;
    hash["sd"] = var::num(static_cast<double>(client_sd));
    hash["ip"] = ngc->newstr(inet_ntoa(client.sin_addr));
    ngc->temp = nil;
    return res;
}

var nas_send(var* args, usize size, gc* ngc) {
    if (args[0].type!=vm_num)
        return nas_err("send", "\"sd\" must be a number");
    if (args[1].type!=vm_str)
        return nas_err("send", "\"buff\" must be a string");
    if (args[2].type!=vm_num)
        return nas_err("send", "\"flags\" muse be a number");
    return var::num(static_cast<double>(send(
        args[0].num(),
        args[1].str().c_str(),
        args[1].str().length(),
        args[2].num()
    )));
}

var nas_sendto(var* args, usize size, gc* ngc) {
    if (args[0].type!=vm_num)
        return nas_err("sendto", "\"sd\" must be a number");
    if (args[1].type!=vm_str)
        return nas_err("sendto", "\"hostname\" must be a string");
    if (args[2].type!=vm_num)
        return nas_err("sendto", "\"port\" must be a number");
    if (args[3].type!=vm_str)
        return nas_err("sendto", "\"buff\" must be a string");
    if (args[4].type!=vm_num)
        return nas_err("sendto", "\"flags\" must be a number");
    sockaddr_in addr;
    memset(&addr, 0, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(args[2].num());
    hostent* entry = gethostbyname(args[1].str().c_str());
    memcpy(&addr.sin_addr, entry->h_addr, entry->h_length);
    return var::num(static_cast<double>(sendto(
        args[0].num(),
        args[3].str().c_str(),
        args[3].str().length(),
        args[4].num(),
        (sockaddr*)&addr,
        sizeof(sockaddr_in)
    )));
}

var nas_recv(var* args, usize size, gc* ngc) {
    if (args[0].type!=vm_num)
        return nas_err("recv", "\"sd\" must be a number");
    if (args[1].type!=vm_num)
        return nas_err("recv", "\"len\" must be a number");
    if (args[1].num()<=0 || args[1].num()>16*1024*1024)
        return nas_err("recv", "\"len\" out of range");
    if (args[2].type!=vm_num)
        return nas_err("recv", "\"flags\" muse be a number");
    var res = ngc->temp = ngc->alloc(vm_hash);
    auto& hash = res.hash().elems;
    char* buf = new char[static_cast<int>(args[1].num())];
    auto recvsize = recv(args[0].num(), buf,args[1].num(), args[2].num());
    hash["size"] = var::num(static_cast<double>(recvsize));
    buf[recvsize>=0? recvsize:0] = 0;
    hash["str"] = ngc->newstr(buf);
    delete[] buf;
    ngc->temp = nil;
    return res;
}

var nas_recvfrom(var* args, usize size, gc* ngc) {
    if (args[0].type!=vm_num)
        return nas_err("recvfrom", "\"sd\" must be a number");
    if (args[1].type!=vm_num)
        return nas_err("recvfrom", "\"len\" must be a number");
    if (args[1].num()<=0 || args[1].num()>16*1024*1024)
        return nas_err("recvfrom", "\"len\" out of range");
    if (args[2].type!=vm_num)
        return nas_err("recvfrom", "\"flags\" muse be a number");
    sockaddr_in addr;
    int socklen = sizeof(sockaddr_in);
    var res = ngc->temp = ngc->alloc(vm_hash);
    auto& hash = res.hash().elems;
    char* buf = new char[static_cast<int>(args[1].num()+1)];
#ifdef _WIN32
    auto recvsize = recvfrom(
        args[0].num(),
        buf,
        args[1].num(),
        args[2].num(),
        (sockaddr*)&addr,
        &socklen
    );
#else
    auto recvsize = recvfrom(
        args[0].num(),
        buf,
        args[1].num(),
        args[2].num(),
        (sockaddr*)&addr,
        (socklen_t*)&socklen
    );
#endif
    hash["size"] = var::num(static_cast<double>(recvsize));
    buf[recvsize>=0? recvsize:0] = 0;
    hash["str"] = ngc->newstr(buf);
    delete[] buf;
    hash["fromip"] = ngc->newstr(inet_ntoa(addr.sin_addr));
    ngc->temp = nil;
    return res;
}

var nas_errno(var* args, usize size, gc* ngc) {
    return ngc->newstr(strerror(errno));
}

module_func_info func_tbl[] = {
    {"nas_socket", nas_socket},
    {"nas_closesocket", nas_closesocket},
    {"nas_shutdown", nas_shutdown},
    {"nas_bind", nas_bind},
    {"nas_listen", nas_listen},
    {"nas_connect", nas_connect},
    {"nas_accept", nas_accept},
    {"nas_send", nas_send},
    {"nas_sendto", nas_sendto},
    {"nas_recv", nas_recv},
    {"nas_recvfrom", nas_recvfrom},
    {"nas_errno", nas_errno},
    {nullptr, nullptr}
};

extern "C" module_func_info* get() {
    return func_tbl;
}

}
