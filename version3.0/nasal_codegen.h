#ifndef __NASAL_CODEGEN_H__
#define __NASAL_CODEGEN_H__

// unfinished
// now it can output ast but it is not byte code yet
// please wait...
class nasal_codegen
{
private:
    std::vector<unsigned char> file_header;
    std::vector<unsigned char> output_file;
    std::vector<std::string> string_table;
    int error;
    void init();
    void output_int(unsigned int,std::vector<unsigned char>&);
    void output_short(unsigned short,std::vector<unsigned char>&);
    void output_root(nasal_ast&);
    unsigned int input_int(std::ifstream&);
    unsigned short input_short(std::ifstream&);
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
    string_table.clear();
    return;
}

int nasal_codegen::get_error()
{
    return error;
}

void nasal_codegen::init()
{
    error=0;
    string_table.clear();
    file_header.clear();
    output_file.clear();
    return;
}

void nasal_codegen::output_int(unsigned int num,std::vector<unsigned char>& vec)
{
    unsigned int tmp=0xff000000;
    unsigned int offset=24;
    for(int i=0;i<4;++i)
    {
        vec.push_back((unsigned char)((tmp&num)>>offset));
        offset-=8;
        tmp>>=8;
    }
    return;
}

void nasal_codegen::output_short(unsigned short num,std::vector<unsigned char>& vec)
{
    vec.push_back((unsigned char)((num&0xff00)>>8));
    vec.push_back((unsigned char)(num&0x00ff));
    return;
}

void nasal_codegen::output_root(nasal_ast& root)
{
    unsigned char type=(unsigned char)root.get_type();
    output_file.push_back(type);
    std::vector<nasal_ast>& ref_vec=root.get_children();
    unsigned short root_children_size=ref_vec.size();
    output_short(root_children_size,output_file);
    if(type==ast_number || type==ast_string || type==ast_identifier || type==ast_dynamic_id || type==ast_call_hash)
    {
        std::string tmp=root.get_str();
        if(std::find(string_table.begin(),string_table.end(),tmp)==string_table.end())
        {
            string_table.push_back(tmp);
            output_short(string_table.size()-1,output_file);
        }
        else
        {
            for(int i=0;i<string_table.size();++i)
                if(string_table[i]==tmp)
                {
                    output_short(i,output_file);
                    break;
                }
        }
    }
    for(unsigned short i=0;i<root_children_size;++i)
        output_root(ref_vec[i]);
    return;
}

void nasal_codegen::output_exec(std::string filename,nasal_ast& root)
{
    // initializing
    this->init();

    // put header input file_header
    unsigned char header[4]={0x40,0x56,0x4b,0x21};
    for(int i=0;i<4;++i)
        file_header.push_back(header[i]);
    
    // main progress,put codes into output_file
    output_root(root);

    // put string table into file_header
    output_short(string_table.size(),file_header);
    for(int i=0;i<string_table.size();++i)
    {
        int len=string_table[i].length();
        for(int j=0;j<len;++j)
            file_header.push_back((unsigned char)string_table[i][j]);
        file_header.push_back(0);
    }

    // output to file
    std::ofstream fout(filename,std::ios::binary);
    if(fout.fail())
    {
        ++error;
        std::cout<<">> [code] cannot generate file \""<<filename<<"\"."<<std::endl;
        fout.close();
        return;
    }
    int size=file_header.size();
    for(int i=0;i<size;++i)
        fout<<file_header[i];
    size=output_file.size();
    for(int i=0;i<size;++i)
        fout<<output_file[i];
    fout.close();
    return;
}

unsigned int nasal_codegen::input_int(std::ifstream& fin)
{
    unsigned int number=0;
    for(int i=0;i<4;++i)
    {
        number<<=8;
        number+=(unsigned char)fin.get();
    }
    return number;
}

unsigned short nasal_codegen::input_short(std::ifstream& fin)
{
    unsigned short number=(unsigned char)fin.get();
    number=(number<<8)+(unsigned char)fin.get();
    return number;
}

void nasal_codegen::input_root(nasal_ast& root,std::ifstream& fin)
{
    unsigned char type=(unsigned char)fin.get();
    unsigned short size=input_short(fin);
    root.set_type(type);
    if(type==ast_number || type==ast_string || type==ast_identifier || type==ast_dynamic_id || type==ast_call_hash)
    {
        std::string tmp=string_table[input_short(fin)];
        root.set_str(tmp);
    }
    for(unsigned short i=0;i<size;++i)
    {
        nasal_ast new_ast;
        root.add_child(new_ast);
        input_root(root.get_children().back(),fin);
        if(error)
            return;
    }
    return;
}

void nasal_codegen::load_exec(std::string filename,nasal_ast& root)
{
    // initializing
    string_table.clear();
    root.clear();

    // start input
    std::ifstream fin(filename,std::ios::binary);
    if(fin.fail())
    {
        ++error;
        std::cout<<">> [code] cannot open file\""<<filename<<"\"."<<std::endl;
        fin.close();
        return;
    }

    // check header
    unsigned char header[4]={0x40,0x56,0x4b,0x21};
    for(int i=0;i<4;++i)
        if((unsigned char)fin.get()!=header[i])
        {
            ++error;
            std::cout<<">> [code] \""<<filename<<"\" is not an executable file."<<std::endl;
            return;
        }
    
    // input string table
    unsigned short string_num=input_short(fin);
    for(int i=0;i<string_num;++i)
    {
        std::string tmp="";
        while(1)
        {
            unsigned char c=(unsigned char)fin.get();
            if(!c || fin.eof()) break;
            tmp+=c;
        }
        string_table.push_back(tmp);
    }

    // generate root
    input_root(root,fin);
    return;
}

#endif