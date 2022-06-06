#include "../nasal.h"

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
#include <sys/socket.h>
#endif

extern "C" nasal_ref nas_socket(std::vector<nasal_ref>& args,nasal_gc& gc){
    if(args.size()!=3)
        return builtin_err("socket","invalid arguments");
    if(args[0].type!=vm_num || args[1].type!=vm_num || args[2].type!=vm_num)
        return builtin_err("socket","\"af\", \"type\", \"protocol\" should be number");
    int sd=socket(args[0].num(),args[1].num(),args[2].num());
    return {vm_num,(double)sd};
}

extern "C" nasal_ref nas_closesocket(std::vector<nasal_ref>& args,nasal_gc& gc){
    if(args.size()!=1)
        return builtin_err("closesocket","invalid arguments");
    if(args[0].type!=vm_num)
        return builtin_err("closesocket","\"\" should be number");
#ifdef _WIN32
    return {vm_num,(double)closesocket(args[0].num())};
#else
    return {vm_num,(double)close(args[0].num())};
#endif
}