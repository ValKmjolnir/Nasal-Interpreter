#include "../nasal.h"
#include <unistd.h>
#include <iostream>
#ifdef _WIN32
#include <conio.h>
#else
#include <fcntl.h>
#include <termios.h>
#endif

class noecho_input {
private:
#ifndef _WIN32
    struct termios init_termios;
    struct termios new_termios;
    int peek_char=-1;
#endif
public:
    noecho_input() {
#ifndef _WIN32
        tcflush(0, TCIOFLUSH);
        tcgetattr(0, &init_termios);
        new_termios=init_termios;
        new_termios.c_lflag&=~(ICANON|ECHO|ECHONL|ECHOE);
        // vmin=0 is nonblock input, but in wsl there is a bug that will block input
        // so we use fcntl to write the nonblock input
        new_termios.c_cc[VMIN]=1;
        new_termios.c_cc[VTIME]=0;
        tcsetattr(0, TCSANOW, &new_termios);
#endif
    }
    ~noecho_input() {
#ifndef _WIN32
        tcflush(0, TCIOFLUSH);
        tcsetattr(0, TCSANOW, &init_termios);
#endif
    }
    int noecho_kbhit() {
#ifndef _WIN32
        unsigned char ch=0;
        int nread=0;
        if (peek_char!=-1) {
            return 1;
        }
        int flag=fcntl(0, F_GETFL);
        fcntl(0, F_SETFL,flag|O_NONBLOCK);
        nread=read(0, &ch, 1);
        fcntl(0, F_SETFL, flag);
        if (nread==1) {
            peek_char=ch;
            return 1;
        }
        return 0;
#else
        return kbhit();
#endif
    }
    int noecho_getch() {
#ifndef _WIN32
        int ch=0;
        if (peek_char!=-1) {
            ch=peek_char;
            peek_char=-1;
            return ch;
        }
        ssize_t tmp=read(0, &ch, 1);
        return ch;
#else
        return getch();
#endif
    }
};

noecho_input this_window;

var nas_getch(var* args, usize size, gc* ngc) {
    return var::num((double)this_window.noecho_getch());
}

var nas_kbhit(var* args, usize size, gc* ngc) {
    return var::num((double)this_window.noecho_kbhit());
}

var nas_noblock(var* args, usize size, gc* ngc) {
    if (this_window.noecho_kbhit()) {
        return var::num((double)this_window.noecho_getch());
    }
    return nil;
}

mod_func func_tbl[]={
    {"nas_getch",nas_getch},
    {"nas_kbhit",nas_kbhit},
    {"nas_noblock",nas_noblock},
    {nullptr,nullptr}
};

extern "C" mod_func* get() {
    return func_tbl;
}