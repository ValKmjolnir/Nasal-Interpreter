#ifndef __NASAL_CODEGEN_H__
#define __NASAL_CODEGEN_H__

// unfinished
// now it can output ast but it is not byte code yet
// please wait...
class nasal_codegen
{
private:
    std::vector<std::string> number_list;
    std::vector<std::string> string_list;
    std::vector<std::string> symbol_list;
    int error;
    void init();
    void output_int(unsigned int,std::ofstream&);
    void output_root(nasal_ast&,std::ofstream&);
    unsigned int input_int(std::ifstream&);
    void input_root(nasal_ast&,std::ifstream&);
public:
    nasal_codegen();
    int get_error();
    void output_exec(std::string,nasal_ast&);
    void load_exec(std::string,nasal_ast&);
};

nasal_codegen::nasal_codegen()
{
    error=0;
    number_list.clear();
    string_list.clear();
    symbol_list.clear();
    return;
}

int nasal_codegen::get_error()
{
    return error;
}

void nasal_codegen::init()
{
    error=0;
    number_list.clear();
    string_list.clear();
    symbol_list.clear();
    return;
}

void nasal_codegen::output_int(unsigned int num,std::ofstream& fout)
{
    unsigned int get_byte=0xff000000;
    int offset=24;
    for(int i=0;i<4;++i)
    {
        fout<<(unsigned char)((get_byte&num)>>offset);
        get_byte>>=8;
        offset-=8;
    }
    return;
}

void nasal_codegen::output_root(nasal_ast& root,std::ofstream& fout)
{
    output_int(ast_begin,fout);
    unsigned int type=root.get_type();
    output_int(type,fout);
    std::vector<nasal_ast>& ref_vec=root.get_children();
    int root_children_size=ref_vec.size();
    output_int(root_children_size,fout);
    if(type==ast_number || type==ast_string || type==ast_identifier || type==ast_dynamic_id || type==ast_call_hash)
    {
        std::string tmp=root.get_str();
        for(int i=0;i<tmp.length();++i)
            fout<<(unsigned char)tmp[i];
        fout<<'\0';
    }
    for(int i=0;i<root_children_size;++i)
        output_root(ref_vec[i],fout);
    output_int(ast_end,fout);
    return;
}

void nasal_codegen::output_exec(std::string filename,nasal_ast& root)
{
    this->init();
    std::ofstream fout(filename,std::ios::binary);
    if(fout.fail())
    {
        ++error;
        std::cout<<">> [code] cannot generate file \""<<filename<<"\"."<<std::endl;
        fout.close();
        return;
    }
    unsigned char header[4]={0x40,0x56,0x4b,0x21};
    for(int i=0;i<4;++i)
        fout<<header[i];
    output_root(root,fout);
    fout.close();
    return;
}

unsigned int nasal_codegen::input_int(std::ifstream& fin)
{
    unsigned int number=0;
    unsigned char c;
    for(int i=0;i<4;++i)
    {
        c=(unsigned char)fin.get();
        number=(number<<8)+c;
    }
    return number;
}

void nasal_codegen::input_root(nasal_ast& root,std::ifstream& fin)
{
    unsigned int begin=input_int(fin);
    if(begin!=ast_begin)
    {
        ++error;
        std::cout<<">> [code] file format error: without ast_begin."<<std::endl;
        return;
    }
    unsigned int type=input_int(fin);
    unsigned int size=input_int(fin);
    root.set_type(type);
    if(type==ast_number || type==ast_string || type==ast_identifier || type==ast_dynamic_id || type==ast_call_hash)
    {
        std::string tmp="";
        while(1)
        {
            unsigned char c=(unsigned char)fin.get();
            if(!c || fin.eof()) break;
            tmp+=c;
        }
        root.set_str(tmp);
    }
    for(int i=0;i<size;++i)
    {
        nasal_ast new_ast;
        root.add_child(new_ast);
        input_root(root.get_children().back(),fin);
        if(error)
            return;
    }
    unsigned int end=input_int(fin);
    if(end!=ast_end)
    {
        ++error;
        std::cout<<">> [code] file format error: without ast_end."<<std::endl;
        return;
    }
    return;
}

void nasal_codegen::load_exec(std::string filename,nasal_ast& root)
{
    root.clear();
    std::ifstream fin(filename,std::ios::binary);
    if(fin.fail())
    {
        ++error;
        std::cout<<">> [code] cannot open file\""<<filename<<"\"."<<std::endl;
        fin.close();
        return;
    }
    unsigned char header[4]={0x40,0x56,0x4b,0x21};
    for(int i=0;i<4;++i)
        if((unsigned char)fin.get()!=header[i])
        {
            ++error;
            std::cout<<">> [code] \""<<filename<<"\" is not an executable file."<<std::endl;
            return;
        }
    input_root(root,fin);
    return;
}

#endif