#ifndef __NASAL_ERR_H__
#define __NASAL_ERR_H__

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
        std::ifstream fin(f,std::ios::binary);
        if(fin.fail())
        {
            std::cerr<<bold_cyan<<"[src] "<<reset<<"cannot open file <"<<f<<">\n";
            std::exit(1);
        }
        string line;
        while(!fin.eof())
        {
            std::getline(fin,line);
            res.push_back(line);
        }
    }
    void clear()
    {
        std::vector<string> tmp;
        res.swap(tmp);
    }
    const string& operator[](usize n){return res[n];}
    const string& name(){return file;}
    usize size(){return res.size();}
};

class nasal_err:public fstreamline
{
private:
    u32 error;
public:
    nasal_err():error(0){}
    void err(const char* stage,const string& info)
    {
        ++error;
        std::cerr<<bold_cyan<<"["<<stage<<"] "<<reset<<info<<"\n";
    }
    void err(const char* stage,u32 line,u32 column,const string& info)
    {
        ++error;
        const string& code=res[line-1];
        std::cerr<<bold_cyan<<"["<<stage<<"] "
                 <<bold_orange<<file<<":"<<line<<":"<<column<<" "
                 <<bold_white<<info<<reset<<"\n"<<code<<"\n";
        for(i32 i=0;i<(i32)column-1;++i)
            std::cerr<<char(" \t"[code[i]=='\t']);
        std::cerr<<bold_red<<"^\n"<<reset;
    }
    void err(const char* stage,u32 line,const string& info)
    {
        ++error;
        std::cerr<<bold_cyan<<"["<<stage<<"] "
                 <<bold_orange<<file<<":"<<line<<" "
                 <<bold_white<<info<<reset<<"\n"<<res[line-1]<<'\n';
    }
    void chkerr(){if(error)std::exit(1);}
};

#endif