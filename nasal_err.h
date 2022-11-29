#pragma once

#include <iostream>
#include <fstream>
#include <sstream> // MSVC need this to use std::getline
#include <cstring>
#include <vector>

#include "nasal.h"

#ifdef _WIN32
#include <windows.h> // use SetConsoleTextAttribute
struct for_reset {
    CONSOLE_SCREEN_BUFFER_INFO scr;
    for_reset() {
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&scr);
    }
} reset_ter_color;
#endif

std::ostream& back_white(std::ostream& s) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0xf0);
#else
    s<<"\033[7m";
#endif
    return s;
}

std::ostream& red(std::ostream& s) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0c);
#else
    s<<"\033[91;1m";
#endif
    return s;
}

std::ostream& cyan(std::ostream& s) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x03);
#else
    s<<"\033[36;1m";
#endif
    return s;
}

std::ostream& orange(std::ostream& s) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0e);
#else
    s<<"\033[93;1m";
#endif
    return s;
}

std::ostream& white(std::ostream& s) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0f);
#else
    s<<"\033[0m\033[1m";
#endif
    return s;
}

std::ostream& reset(std::ostream& s) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),reset_ter_color.scr.wAttributes);
#else
    s<<"\033[0m";
#endif
    return s;
}

class flstream {
protected:
    string file;
    std::vector<string> res;
public:
    flstream():file("") {}
    void load(const string& f) {
        if (file==f) { // don't need to load a loaded file
            return;
        } else {
            file=f;
        }

        res.clear();
        std::ifstream in(f,std::ios::binary);
        if (in.fail()) {
            std::cerr<<red<<"src: "<<reset<<"cannot open <"<<f<<">\n";
            std::exit(1);
        }
        
        while(!in.eof()) {
            string line;
            std::getline(in,line);
            res.push_back(line);
        }
    }
    const string& operator[](usize n) const {return res[n];}
    const string& name() const {return file;}
    usize size() const {return res.size();}
};

class error:public flstream {
private:
    u32 cnt;
    string identation(usize len) {
        string tmp="";
        tmp.resize(len,' ');
        return tmp;
    }
public:
    error():cnt(0) {}
    void fatal(const string& stage,const string& info) {
        std::cerr<<red<<stage<<": "<<white<<info<<reset<<"\n";
        if (file.length()) {
            std::cerr<<cyan<<" --> "<<red<<file<<"\n\n";
        } else {
            std::cerr<<"\n";
        }
        std::exit(1);
    }
    void err(const string& stage,const string& info) {
        ++cnt;
        std::cerr<<red<<stage<<": "<<white<<info<<reset<<"\n";
        if (file.length()) {
            std::cerr<<cyan<<" --> "<<red<<file<<"\n\n";
        } else {
            std::cerr<<"\n";
        }
    }
    void err(const string& stage,u32 line,u32 col,u32 len,const string& info) {
        ++cnt;
        col=col?col:1;
        len=len?len:1;

        std::cerr
        <<red<<stage<<": "<<white<<info<<reset<<"\n"
        <<cyan<<"  --> "<<red<<file<<":"<<line<<":"<<col<<reset<<"\n";

        const string& code=line?res[line-1]:"# empty line";
        const string iden=identation(std::to_string(line).length());

        std::cerr
        <<cyan<<line<<" | "<<reset<<code<<"\n"
        <<cyan<<iden<<" | "<<reset;
        for(i32 i=0;i<(i32)col-(i32)len;++i)
            std::cerr<<char(" \t"[code[i]=='\t']);
        for(u32 i=0;i<len;++i)
            std::cerr<<red<<"^";
        std::cerr<<red<<" "<<info<<reset<<"\n\n";
    }
    void chkerr() const {
        if (cnt) {
            std::exit(1);
        }
    }
};
