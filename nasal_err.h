#ifndef __NASAL_ERR_H__
#define __NASAL_ERR_H__

#include <iostream>
#include <fstream>
#include <sstream> // MSVC need this to use std::getline
#include <cstring>

#ifdef _WIN32
#include <windows.h> // use SetConsoleTextAttribute
#endif

class fstreamline
{
protected:
    string file;
    std::vector<string> res;
public:
    void load(const string& f)
    {
        if(file==f) // don't need to load a loaded file
            return;
        file=f;
        res.clear();
        std::ifstream fin(f,std::ios::binary);
        if(fin.fail())
        {
            std::cerr<<"[src] cannot open file <"<<f<<">\n";
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
    void printstg(const char* stage)
    {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO scrinfo;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE),&scrinfo);
        SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE),0x03);
        std::cerr<<"[";
        SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE),0x0c);
        std::cerr<<stage;
        SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE),0x03);
        std::cerr<<"] ";
        SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE),scrinfo.wAttributes);
#else
        std::cerr<<"\033[36m[\033[91m"<<stage<<"\033[36m]\033[0m ";
#endif
    }
public:
    nasal_err():error(0){}
    void err(const char* stage,const string& info)
    {
        ++error;
        printstg(stage);
        std::cerr<<info<<"\n";
    }
    void err(const char* stage,u32 line,u32 column,const string& info)
    {
        ++error;
        const string& code=res[line-1];
        printstg(stage);
        std::cerr<<file<<":"<<line<<":"<<column<<" "<<info<<"\n"<<code<<"\n";
        for(i32 i=0;i<(i32)column-1;++i)
            std::cerr<<char(" \t"[code[i]=='\t']);
        std::cerr<<"^\n";
    }
    void err(const char* stage,u32 line,const string& info)
    {
        ++error;
        printstg(stage);
        std::cerr<<file<<":"<<line<<" "<<info<<"\n"<<res[line-1]<<'\n';
    }
    void chkerr(){if(error)std::exit(1);}
};

#endif