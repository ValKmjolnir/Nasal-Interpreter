#include "../nasal.h"
#include <iostream>
#ifdef _WIN32
#include <conio.h>
#else
#include <fcntl.h>
#include <termio.h>
#endif

#ifndef _WIN32
static struct termios init_termios;
static struct termios new_termios;
static int peek_character=-1;

int kbhit(){
    unsigned char ch=0;
    int nread=0;
    if(peek_character!=-1)
        return 1;
    int flag=fcntl(0,F_GETFL);
    flag|=O_NONBLOCK;
    fcntl(0,F_SETFL,flag);
    nread=read(0,&ch,1);
    flag&=(~O_NONBLOCK);
    fcntl(0,F_SETFL,flag);
    if(nread==1)
    {
        peek_character=ch;
        return 1;
    }
    return 0;
}

int getch(){
    int ch=0;
    if(peek_character!=-1)
    {
        ch=peek_character;
        peek_character=-1;
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
    tcgetattr(0,&init_termios);
    new_termios=init_termios;
    new_termios.c_lflag&=~(ICANON|ECHO|ECHONL);
    new_termios.c_cc[VMIN]=0;
    new_termios.c_cc[VTIME]=0;
    tcsetattr(0,TCSANOW,&new_termios);
#endif
    return nil;
}
extern "C" nasal_ref nas_close(std::vector<nasal_ref>& args,nasal_gc& gc){
#ifndef _WIN32
    tcsetattr(0,TCSANOW,&init_termios);
    tcflush(0,TCIOFLUSH);
#endif
    return nil;
}