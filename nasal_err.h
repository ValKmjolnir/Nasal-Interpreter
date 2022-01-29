#ifndef __NASAL_ERR_H__
#define __NASAL_ERR_H__

#include <iostream>
#include <fstream>
#include <cstring>

class file_line
{
protected:
    std::string file;
    std::vector<std::string> res;
public:
    void load(const std::string& f)
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
        std::string line;
        while(!fin.eof())
        {
            std::getline(fin,line);
            res.push_back(line);
        }
    }
    void clear()
    {
        std::vector<std::string> tmp;
        res.swap(tmp);
    }
    const std::string& operator[](const uint32_t line){return res[line];}
    const std::string& name(){return file;}
    size_t size(){return res.size();}
};

class nasal_err:public file_line
{
private:
    uint32_t error;
public:
    void err(const char* stage,const std::string& info)
    {
        ++error;
        std::cerr<<"["<<stage<<"] "<<info<<'\n';
    }
    void err(const char* stage,uint32_t line,uint32_t column,const std::string& info)
    {
        ++error;
        if(!line)
        {
            std::cerr<<"["<<stage<<"] "<<file<<": "<<info<<'\n';
            return;
        }
        std::cerr<<"["<<stage<<"] "<<file<<":"<<line<<":"<<column<<" "<<info<<"\n"<<res[line-1]<<'\n';
        for(int i=0;i<(int)column-1;++i)
            std::cerr<<char(" \t"[res[line-1][i]=='\t']);
        std::cerr<<"^\n";
    }
    void err(const char* stage,uint32_t line,const std::string& info)
    {
        ++error;
        if(!line)
        {
            std::cerr<<"["<<stage<<"] "<<file<<": "<<info<<'\n';
            return;
        }
        std::cerr<<"["<<stage<<"] "<<file<<":"<<line<<" "<<info<<"\n"<<res[line-1]<<'\n';
    }
    void chkerr(){if(error)std::exit(1);}
};

#endif