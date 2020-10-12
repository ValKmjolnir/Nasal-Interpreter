#ifndef __NASAL_CODEGEN_H__
#define __NASAL_CODEGEN_H__

// unfinished

enum bytecode
{
    code_nop=0,

    code_number_list,
    code_string_list,
    code_symbol_list,
};

class nasal_codegen
{
private:
    std::vector<std::string> number_list;
    std::vector<std::string> string_list;
    std::vector<std::string> symbol_list;
    int error;
    void die(std::string);
    void init();
    void output_int(unsigned int,std::ofstream&);
    void generate_root(nasal_ast&,std::ofstream&);
    void generate_number_list(std::ofstream&);
    void generate_string_list(std::ofstream&);
    void generate_symbol_list(std::ofstream&);
public:
    nasal_codegen();
    void main_progress(std::string,nasal_ast&);
};

class nasal_bytecode_runtime
{
private:
public:
};

nasal_codegen::nasal_codegen()
{
    error=0;
    number_list.clear();
    string_list.clear();
    symbol_list.clear();
    return;
}

void nasal_codegen::die(std::string info)
{
    ++error;
    std::cout<<">> [code] "<<info<<std::endl;
    return;
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

void nasal_codegen::generate_root(nasal_ast& root,std::ofstream& fout)
{
    std::vector<nasal_ast>& ref_vec=root.get_children();
    int root_children_size=ref_vec.size();
    for(int i=0;i<root_children_size;++i)
    {
        switch(ref_vec[i].get_type())
        {
            case ast_definition:break;
            case ast_multi_assign:break;
            case ast_conditional:break;
            case ast_while:case ast_for:case ast_forindex:case ast_foreach:
                break;
            case ast_nil:case ast_number:case ast_string:case ast_function:break;
            case ast_identifier:
            case ast_vector:case ast_hash:
            case ast_call:
            case ast_equal:case ast_add_equal:case ast_sub_equal:case ast_mult_equal:case ast_div_equal:case ast_link_equal:
            case ast_unary_sub:case ast_unary_not:
            case ast_add:case ast_sub:case ast_mult:case ast_div:case ast_link:
            case ast_trinocular:break;
            case ast_break:die("break in main.");break;
            case ast_continue:die("continue in main.");break;
            case ast_return:die("return in main.");break;
        }
        if(error)
            break;
    }
    output_int(code_nop,fout);
    return;
}
void nasal_codegen::generate_number_list(std::ofstream& fout)
{
    output_int(code_number_list,fout);
    int size=number_list.size();
    output_int(size,fout);
    for(int i=0;i<size;++i)
        fout<<number_list[i]<<"\0";
    return;
}
void nasal_codegen::generate_string_list(std::ofstream& fout)
{
    output_int(code_string_list,fout);
    int size=string_list.size();
    output_int(size,fout);
    for(int i=0;i<size;++i)
        fout<<string_list[i]<<"\0";
    return;
}
void nasal_codegen::generate_symbol_list(std::ofstream& fout)
{
    output_int(code_symbol_list,fout);
    int size=symbol_list.size();
    output_int(size,fout);
    for(int i=0;i<size;++i)
        fout<<symbol_list[i]<<"\0";
    return;
}

void nasal_codegen::main_progress(std::string filename,nasal_ast& root)
{
    this->init();
    std::ofstream fout(filename,std::ios::binary);
    if(fout.fail())
    {
        std::cout<<">> [code] cannot generate file \""<<filename<<"\"."<<std::endl;
        fout.close();
        return;
    }
    unsigned char header[4]={0x40,0x56,0x4b,0x21};
    for(int i=0;i<4;++i)
        fout<<header[i];
    generate_root(root,fout);
    generate_number_list(fout);
    generate_string_list(fout);
    generate_symbol_list(fout);
    fout.close();
    return;
}

#endif