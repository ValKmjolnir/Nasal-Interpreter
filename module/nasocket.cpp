#include "../nasal.h"
#include "../nasal_gc.h"
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

extern "C" nas_ref nas_socket(std::vector<nas_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num || args[1].type!=vm_num || args[2].type!=vm_num)
        return nas_err("socket","\"af\", \"type\", \"protocol\" should be number");
    int sd=socket(args[0].num(),args[1].num(),args[2].num());
    return {vm_num,(double)sd};
}

extern "C" nas_ref nas_closesocket(std::vector<nas_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return nas_err("closesocket","\"\" should be number");
#ifdef _WIN32
    return {vm_num,(double)closesocket(args[0].num())};
#else
    return {vm_num,(double)close(args[0].num())};
#endif
}

extern "C" nas_ref nas_shutdown(std::vector<nas_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return nas_err("shutdown","\"sd\" must be a number");
    if(args[1].type!=vm_num)
        return nas_err("shutdown","\"how\" must be a number");
    return {vm_num,(double)shutdown(args[0].num(),args[1].num())};
}

extern "C" nas_ref nas_bind(std::vector<nas_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return nas_err("bind","\"sd\" muse be a number");
    if(args[1].type!=vm_str)
        return nas_err("bind","\"ip\" should be a string including an ip with correct format");
    if(args[2].type!=vm_num)
        return nas_err("bind","\"port\" must be a number");
    sockaddr_in server;
    memset(&server,0,sizeof(sockaddr_in));
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=inet_addr(args[1].str().c_str());
    server.sin_port=htons(args[2].num());
    return {vm_num,(double)bind(args[0].num(),(sockaddr*)&server,sizeof(server))};
}

extern "C" nas_ref nas_listen(std::vector<nas_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return nas_err("listen","\"sd\" must be a number");
    if(args[1].type!=vm_num)
        return nas_err("listen","\"backlog\" must be a number");
    return{vm_num,(double)listen(args[0].num(),args[1].num())};
}

extern "C" nas_ref nas_connect(std::vector<nas_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return nas_err("connect","\"sd\" must be a number");
    if(args[1].type!=vm_str)
        return nas_err("connect","\"hostname\" must be a string");
    if(args[2].type!=vm_num)
        return nas_err("connect","\"port\" must be a number");
    sockaddr_in addr;
    memset(&addr,0,sizeof(sockaddr_in));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(args[2].num());
    hostent* entry=gethostbyname(args[1].str().c_str());
    memcpy(&addr.sin_addr,entry->h_addr,entry->h_length);
    return {vm_num,(double)connect(args[0].num(),(sockaddr*)&addr,sizeof(sockaddr_in))};
}

extern "C" nas_ref nas_accept(std::vector<nas_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return nas_err("accept","\"sd\" must be a number");
    sockaddr_in client;
    int socklen=sizeof(sockaddr_in);
#ifdef _WIN32
    int client_sd=accept(args[0].num(),(sockaddr*)&client,&socklen);
#else
    int client_sd=accept(args[0].num(),(sockaddr*)&client,(socklen_t*)&socklen);
#endif
    nas_ref res=gc.temp=gc.alloc(vm_hash);
    auto& hash=res.hash().elems;
    hash["sd"]={vm_num,(double)client_sd};
    hash["ip"]=gc.newstr(inet_ntoa(client.sin_addr));
    gc.temp=nil;
    return res;
}

extern "C" nas_ref nas_send(std::vector<nas_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return nas_err("send","\"sd\" must be a number");
    if(args[1].type!=vm_str)
        return nas_err("send","\"buff\" must be a string");
    if(args[2].type!=vm_num)
        return nas_err("send","\"flags\" muse be a number");
    return {vm_num,(double)send(args[0].num(),args[1].str().c_str(),args[1].str().length(),args[2].num())};
}

extern "C" nas_ref nas_sendto(std::vector<nas_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return nas_err("sendto","\"sd\" must be a number");
    if(args[1].type!=vm_str)
        return nas_err("sendto","\"hostname\" must be a string");
    if(args[2].type!=vm_num)
        return nas_err("sendto","\"port\" must be a number");
    if(args[3].type!=vm_str)
        return nas_err("sendto","\"buff\" must be a string");
    if(args[4].type!=vm_num)
        return nas_err("sendto","\"flags\" must be a number");
    sockaddr_in addr;
    memset(&addr,0,sizeof(sockaddr_in));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(args[2].num());
    hostent* entry=gethostbyname(args[1].str().c_str());
    memcpy(&addr.sin_addr,entry->h_addr,entry->h_length);
    return {vm_num,(double)sendto(args[0].num(),args[3].str().c_str(),args[3].str().length(),args[4].num(),(sockaddr*)&addr,sizeof(sockaddr_in))};
}

extern "C" nas_ref nas_recv(std::vector<nas_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return nas_err("recv","\"sd\" must be a number");
    if(args[1].type!=vm_num)
        return nas_err("recv","\"len\" must be a number");
    if(args[1].num()<=0 || args[1].num()>16*1024*1024)
        return nas_err("recv","\"len\" out of range");
    if(args[2].type!=vm_num)
        return nas_err("recv","\"flags\" muse be a number");
    nas_ref res=gc.temp=gc.alloc(vm_hash);
    auto& hash=res.hash().elems;
    char* buf=new char[(int)args[1].num()];
    hash["size"]={vm_num,(double)recv(args[0].num(),buf,args[1].num(),args[2].num())};
    hash["str"]=gc.newstr(buf);
    delete[] buf;
    gc.temp=nil;
    return res;
}

extern "C" nas_ref nas_recvfrom(std::vector<nas_ref>& args,nasal_gc& gc){
    if(args[0].type!=vm_num)
        return nas_err("recvfrom","\"sd\" must be a number");
    if(args[1].type!=vm_num)
        return nas_err("recvfrom","\"len\" must be a number");
    if(args[1].num()<=0 || args[1].num()>16*1024*1024)
        return nas_err("recvfrom","\"len\" out of range");
    if(args[2].type!=vm_num)
        return nas_err("recvfrom","\"flags\" muse be a number");
    sockaddr_in addr;
    int socklen=sizeof(sockaddr_in);
    nas_ref res=gc.temp=gc.alloc(vm_hash);
    auto& hash=res.hash().elems;
    char* buf=new char[(int)args[1].num()+1];
#ifdef _WIN32
    hash["size"]={vm_num,(double)recvfrom(args[0].num(),buf,args[1].num(),args[2].num(),(sockaddr*)&addr,&socklen)};
#else
    hash["size"]={vm_num,(double)recvfrom(args[0].num(),buf,args[1].num(),args[2].num(),(sockaddr*)&addr,(socklen_t*)&socklen)};
#endif
    buf[(int)hash["size"].num()]=0;
    hash["str"]=gc.newstr(buf);
    delete[] buf;
    hash["fromip"]=gc.newstr(inet_ntoa(addr.sin_addr));
    gc.temp=nil;
    return res;
}

extern "C" nas_ref nas_errno(std::vector<nas_ref>& args,nasal_gc& gc){
    return gc.newstr(strerror(errno));
}