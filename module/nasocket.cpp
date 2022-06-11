#include "../nasal.h"
#include <unistd.h>

#ifdef _WIN32
#include <winsock.h>
#pragma comment(lib,"ws2_32")

class WSAmanager{
private:
    WSAData data;
public:
    WSAmanager(){
        WSAStartup(0x1010,&data);
    }
    ~WSAmanager(){
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

extern "C" nasal_ref nas_socket(std::vector<nasal_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num || args[1].type!=vm_num || args[2].type!=vm_num)
        return builtin_err("socket","\"af\", \"type\", \"protocol\" should be number");
    int sd=socket(args[0].num(),args[1].num(),args[2].num());
    return {vm_num,(double)sd};
}

extern "C" nasal_ref nas_closesocket(std::vector<nasal_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return builtin_err("closesocket","\"\" should be number");
#ifdef _WIN32
    return {vm_num,(double)closesocket(args[0].num())};
#else
    return {vm_num,(double)close(args[0].num())};
#endif
}

extern "C" nasal_ref nas_shutdown(std::vector<nasal_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return builtin_err("shutdown","\"sd\" must be a number");
    if(args[1].type!=vm_num)
        return builtin_err("shutdown","\"how\" must be a number");
    return {vm_num,(double)shutdown(args[0].num(),args[1].num())};
}

extern "C" nasal_ref nas_bind(std::vector<nasal_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return builtin_err("bind","\"sd\" muse be a number");
    if(args[1].type!=vm_str)
        return builtin_err("bind","\"ip\" should be a string including an ip with correct format");
    if(args[2].type!=vm_num)
        return builtin_err("bind","\"port\" must be a number");
    sockaddr_in server;
    memset(&server,0,sizeof(sockaddr_in));
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=inet_addr(args[1].str().c_str());
    server.sin_port=htons(args[2].num());
    return {vm_num,(double)bind(args[0].num(),(sockaddr*)&server,sizeof(server))};
}

extern "C" nasal_ref nas_listen(std::vector<nasal_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return builtin_err("listen","\"sd\" must be a number");
    if(args[1].type!=vm_num)
        return builtin_err("listen","\"backlog\" must be a number");
    return{vm_num,(double)listen(args[0].num(),args[1].num())};
}

extern "C" nasal_ref nas_connect(std::vector<nasal_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return builtin_err("connect","\"sd\" must be a number");
    if(args[1].type!=vm_str)
        return builtin_err("connect","\"hostname\" must be a string");
    if(args[2].type!=vm_num)
        return builtin_err("connect","\"port\" must be a number");
    sockaddr_in addr;
    memset(&addr,0,sizeof(sockaddr_in));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(args[2].num());
    hostent* entry=gethostbyname(args[1].str().c_str());
    memcpy(&addr.sin_addr,entry->h_addr,entry->h_length);
    return {vm_num,(double)connect(args[0].num(),(sockaddr*)&addr,sizeof(sockaddr_in))};
}

extern "C" nasal_ref nas_accept(std::vector<nasal_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return builtin_err("accept","\"sd\" must be a number");
    sockaddr_in client;
    int socklen=sizeof(sockaddr_in);
#ifdef _WIN32
    int client_sd=accept(args[0].num(),(sockaddr*)&client,&socklen);
#else
    int client_sd=accept(args[0].num(),(sockaddr*)&client,(socklen_t*)&socklen);
#endif
    if(gc.top+1>=gc.canary)
        return builtin_err("accept","expand temporary space error:stackoverflow");
    (++gc.top)[0]=gc.alloc(vm_hash);
    auto& hash=gc.top[0].hash().elems;
    hash["sd"]={vm_num,(double)client_sd};
    hash["ip"]=gc.alloc(vm_str);
    hash["ip"].str()=inet_ntoa(client.sin_addr);
    --gc.top;
    return gc.top[1];
}

extern "C" nasal_ref nas_send(std::vector<nasal_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return builtin_err("send","\"sd\" must be a number");
    if(args[1].type!=vm_str)
        return builtin_err("send","\"buff\" must be a string");
    if(args[2].type!=vm_num)
        return builtin_err("send","\"flags\" muse be a number");
    return {vm_num,(double)send(args[0].num(),args[1].str().c_str(),args[1].str().length(),args[2].num())};
}

extern "C" nasal_ref nas_sendto(std::vector<nasal_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return builtin_err("sendto","\"sd\" must be a number");
    if(args[1].type!=vm_str)
        return builtin_err("sendto","\"hostname\" must be a string");
    if(args[2].type!=vm_num)
        return builtin_err("sendto","\"port\" must be a number");
    if(args[3].type!=vm_str)
        return builtin_err("sendto","\"buff\" must be a string");
    if(args[4].type!=vm_num)
        return builtin_err("sendto","\"flags\" must be a number");
    sockaddr_in addr;
    memset(&addr,0,sizeof(sockaddr_in));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(args[2].num());
    hostent* entry=gethostbyname(args[1].str().c_str());
    memcpy(&addr.sin_addr,entry->h_addr,entry->h_length);
    return {vm_num,(double)sendto(args[0].num(),args[3].str().c_str(),args[3].str().length(),args[4].num(),(sockaddr*)&addr,sizeof(sockaddr_in))};
}

extern "C" nasal_ref nas_recv(std::vector<nasal_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return builtin_err("recv","\"sd\" must be a number");
    if(args[1].type!=vm_num)
        return builtin_err("recv","\"len\" must be a number");
    if(args[1].num()<=0 || args[1].num()>16*1024*1024)
        return builtin_err("recv","\"len\" out of range");
    if(args[2].type!=vm_num)
        return builtin_err("recv","\"flags\" muse be a number");
    if(gc.top+1>=gc.canary)
        return builtin_err("recv","expand temporary space error:stackoverflow");
    (++gc.top)[0]=gc.alloc(vm_hash);
    auto& hash=gc.top[0].hash().elems;
    hash["str"]=gc.alloc(vm_str);
    char* buf=new char[(int)args[1].num()];
    hash["size"]={vm_num,(double)recv(args[0].num(),buf,args[1].num(),args[2].num())};
    hash["str"].str()=buf;
    delete[] buf;
    --gc.top;
    return gc.top[1];
}

extern "C" nasal_ref nas_recvfrom(std::vector<nasal_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return builtin_err("recvfrom","\"sd\" must be a number");
    if(args[1].type!=vm_num)
        return builtin_err("recvfrom","\"len\" must be a number");
    if(args[1].num()<=0 || args[1].num()>16*1024*1024)
        return builtin_err("recvfrom","\"len\" out of range");
    if(args[2].type!=vm_num)
        return builtin_err("recvfrom","\"flags\" muse be a number");
    if(gc.top+1>=gc.canary)
        return builtin_err("recvfrom","expand temporary space error:stackoverflow");
    sockaddr_in addr;
    int socklen=sizeof(sockaddr_in);
    (++gc.top)[0]=gc.alloc(vm_hash);
    auto& hash=gc.top[0].hash().elems;
    hash["str"]=gc.alloc(vm_str);
    char* buf=new char[(int)args[1].num()+1];
#ifdef _WIN32
    hash["size"]={vm_num,(double)recvfrom(args[0].num(),buf,args[1].num(),args[2].num(),(sockaddr*)&addr,&socklen)};
#else
    hash["size"]={vm_num,(double)recvfrom(args[0].num(),buf,args[1].num(),args[2].num(),(sockaddr*)&addr,(socklen_t*)&socklen)};
#endif
    buf[(int)hash["size"].num()]=0;
    hash["str"].str()=buf;
    delete[] buf;
    hash["fromip"]=gc.alloc(vm_str);
    hash["fromip"].str()=inet_ntoa(addr.sin_addr);
    --gc.top;
    return gc.top[1];
}

extern "C" nasal_ref nas_errno(std::vector<nasal_ref>& args,nasal_gc& gc){
    nasal_ref res=gc.alloc(vm_str);
    res.str()=strerror(errno);
    return res;
}