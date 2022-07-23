#ifndef __NASAL_ERR_H__
#define __NASAL_ERR_H__

#include <iostream>
#include <fstream>
#include <cstring>

class fstreamline
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
    const std::string& operator[](const u32 line){return res[line];}
    const std::string& name(){return file;}
    usize size(){return res.size();}
};

class nasal_err:public fstreamline
{
private:
    u32 error;
public:
    nasal_err():error(0){}
    void err(const char* stage,const std::string& info)
    {
        ++error;
        std::cerr<<"["<<stage<<"] "<<info<<"\n";
    }
    void err(const char* stage,u32 line,u32 column,const std::string& info)
    {
        ++error;
        const std::string& code=res[line-1];
        std::cerr<<"["<<stage<<"] "<<file<<":"<<line<<":"<<column<<" "<<info<<"\n"<<code<<"\n";
        for(int i=0;i<(int)column-1;++i)
            std::cerr<<char(" \t"[code[i]=='\t']);
        std::cerr<<"^\n";
    }
    void err(const char* stage,u32 line,const std::string& info)
    {
        ++error;
        std::cerr<<"["<<stage<<"] "<<file<<":"<<line<<" "<<info<<"\n"<<res[line-1]<<'\n';
    }
    void chkerr(){if(error)std::exit(1);}
};

#endif