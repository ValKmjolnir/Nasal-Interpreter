#include "../nasal.h"
#include <iostream>
#ifdef _WIN32
#include <conio.h>
#else
#include <fcntl.h>
#include <termios.h>
#endif

#ifndef _WIN32
static struct termios init_termios;
static struct termios new_termios;
static int peek_char=-1;

int kbhit(){
    unsigned char ch=0;
    int nread=0;
    if(peek_char!=-1)
        return 1;
    int flag=fcntl(0,F_GETFL);
    fcntl(0,F_SETFL,flag|O_NONBLOCK);
    nread=read(0,&ch,1);
    fcntl(0,F_SETFL,flag);
    if(nread==1){
        peek_char=ch;
        return 1;
    }
    return 0;
}

int getch(){
    int ch=0;
    if(peek_char!=-1){
        ch=peek_char;
        peek_char=-1;
        return ch;
    }
    read(0,&ch,1);
    return ch;
}
#endif

extern "C" nasal_ref nas_getch(std::vector<nasal_ref>& args,nasal_gc& gc){
    return {vm_num,(double)getch()};
}
extern "C" nasal_ref nas_kbhit(std::vector<nasal_ref>& args,nasal_gc& gc){
    return {vm_num,(double)kbhit()};
}
extern "C" nasal_ref nas_noblock(std::vector<nasal_ref>& args,nasal_gc& gc){
    if(kbhit())
        return {vm_num,(double)getch()};
    return nil;
}
extern "C" nasal_ref nas_init(std::vector<nasal_ref>& args,nasal_gc& gc){
#ifndef _WIN32
    tcflush(0,TCIOFLUSH);
    tcgetattr(0,&init_termios);
    new_termios=init_termios;
    new_termios.c_lflag&=~(ICANON|ECHO|ECHONL|ECHOE);
    // vmin=0 is nonblock input, but in wsl there is a bug that will block input
    // so we use fcntl to write the nonblock input
    new_termios.c_cc[VMIN]=1;
    new_termios.c_cc[VTIME]=0;
    tcsetattr(0,TCSANOW,&new_termios);
#endif
    return nil;
}
extern "C" nasal_ref nas_close(std::vector<nasal_ref>& args,nasal_gc& gc){
#ifndef _WIN32
    tcflush(0,TCIOFLUSH);
    tcsetattr(0,TCSANOW,&init_termios);
#endif
    return nil;
}