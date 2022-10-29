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
std::ostream& red(std::ostream& s)
{
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0c);
#else
    s<<"\033[91;1m";
#endif
    return s;
}
std::ostream& cyan(std::ostream& s)
{
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x03);
#else
    s<<"\033[36;1m";
#endif
    return s;
}
std::ostream& orange(std::ostream& s)
{
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0e);
#else
    s<<"\033[93;1m";
#endif
    return s;
}
std::ostream& white(std::ostream& s)
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
            std::cerr<<red<<"src: "<<reset<<"cannot open <"<<f<<">\n";
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
    void err(const string& stage,const string& info)
    {
        ++cnt;
        std::cerr<<red<<stage<<": "
                 <<white<<info<<reset<<"\n\n";
    }
    void err(const string& stage,u32 line,u32 col,const string& info)
    {
        ++cnt;
        const string& code=res[line-1];
        const string iden=identation(std::to_string(line).length());
        std::cerr<<red<<stage<<": "
                 <<white<<info<<reset<<"\n"
                 <<cyan<<" --> "<<reset
                 <<orange<<file<<":"<<line<<":"<<col<<"\n";
        if(!line)
        {
            std::cerr<<"\n";
            return;
        }
        std::cerr<<cyan<<iden<<" | "<<reset<<"\n"
                 <<cyan<<line<<" | "<<reset<<code<<"\n"
                 <<cyan<<iden<<" | "<<reset;
        for(i32 i=0;i<(i32)col-1;++i)
            std::cerr<<char(" \t"[code[i]=='\t']);
        std::cerr<<red<<"^ "<<info<<reset<<"\n\n";
    }
    void err(const string& stage,u32 line,const string& info)
    {
        ++cnt;
        const string iden=identation(std::to_string(line).length());
        std::cerr<<red<<stage<<": "
                 <<white<<info<<reset<<"\n"
                 <<cyan<<" --> "<<reset
                 <<orange<<file<<":"<<line<<"\n";
        if(!line)
        {
            std::cerr<<"\n";
            return;
        }
        std::cerr<<cyan<<iden<<" | "<<reset<<"\n"
                 <<cyan<<line<<" | "<<reset<<res[line-1]<<"\n"
                 <<cyan<<iden<<" | "<<reset<<"\n\n";
    }
    void chkerr() const {if(cnt)std::exit(1);}
};
