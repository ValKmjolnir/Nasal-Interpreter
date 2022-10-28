#pragma once

#include <iostream>
#include <fstream>
#include <sstream> // MSVC need this to use std::getline
#include <cstring>

#ifdef _WIN32
#include <windows.h> // use SetConsoleTextAttribute
struct for_reset
{
    CONSOLE_SCREEN_BUFFER_INFO scr;
    for_reset(){
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&scr);
    }
}reset_ter_color;
#endif

std::ostream& back_white(std::ostream& s)
{
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0xf0);
#else
    s<<"\033[7m";
#endif
    return s;
}
std::ostream& bold_red(std::ostream& s)
{
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0c);
#else
    s<<"\033[91;1m";
#endif
    return s;
}
std::ostream& bold_cyan(std::ostream& s)
{
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x03);
#else
    s<<"\033[36;1m";
#endif
    return s;
}
std::ostream& bold_orange(std::ostream& s)
{
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0e);
#else
    s<<"\033[93;1m";
#endif
    return s;
}
std::ostream& bold_white(std::ostream& s)
{
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0f);
#else
    s<<"\033[0m\033[1m";
#endif
    return s;
}
std::ostream& reset(std::ostream& s)
{
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),reset_ter_color.scr.wAttributes);
#else
    s<<"\033[0m";
#endif
    return s;
}

class fstreamline
{
protected:
    string file;
    std::vector<string> res;
public:
    void load(const string& f)
    {
        if(file==f) return; // don't need to load a loaded file
        file=f;
        res.clear();
        std::ifstream in(f,std::ios::binary);
        if(in.fail())
        {
            std::cerr<<bold_red<<"src: "<<reset<<"cannot open <"<<f<<">\n";
            std::exit(1);
        }
        string line;
        while(!in.eof())
        {
            std::getline(in,line);
            res.push_back(line);
        }
    }
    const string& operator[](usize n){return res[n];}
    const string& name(){return file;}
    usize size(){return res.size();}
};

class error:public fstreamline
{
private:
    u32 cnt;
    string identation(usize len)
    {
        string tmp="";
        tmp.resize(len,' ');
        return tmp;
    }
public:
    error():cnt(0){}
    void err(const char* stage,const string& info)
    {
        ++cnt;
        std::cerr<<bold_red<<stage<<": "
                 <<bold_white<<info<<reset<<"\n\n";
    }
    void err(const char* stage,u32 line,u32 column,const string& info)
    {
        ++cnt;
        const string& code=res[line-1];
        const string ident=identation(std::to_string(line).length());
        std::cerr<<bold_red<<stage<<": "
                 <<bold_white<<info<<reset<<"\n"
                 <<bold_cyan<<" --> "<<reset
                 <<bold_orange<<file<<":"<<line<<":"<<column<<"\n";
        if(!line)
        {
            std::cerr<<"\n";
            return;
        }
        std::cerr<<bold_cyan<<ident<<" | "<<reset<<"\n"
                 <<bold_cyan<<line<<" | "<<reset<<code<<"\n"
                 <<bold_cyan<<ident<<" | "<<reset;
        for(i32 i=0;i<(i32)column-1;++i)
            std::cerr<<char(" \t"[code[i]=='\t']);
        std::cerr<<bold_red<<"^ "<<info<<reset<<"\n\n";
    }
    void err(const char* stage,u32 line,const string& info)
    {
        ++cnt;
        const string ident=identation(std::to_string(line).length());
        std::cerr<<bold_red<<stage<<": "
                 <<bold_white<<info<<reset<<"\n"
                 <<bold_cyan<<" --> "<<reset
                 <<bold_orange<<file<<":"<<line<<"\n";
        if(!line)
        {
            std::cerr<<"\n";
            return;
        }
        std::cerr<<bold_cyan<<ident<<" | "<<reset<<"\n"
                 <<bold_cyan<<line<<" | "<<reset<<res[line-1]<<"\n"
                 <<bold_cyan<<ident<<" | "<<reset<<"\n\n";
    }
    void chkerr(){if(cnt)std::exit(1);}
};
