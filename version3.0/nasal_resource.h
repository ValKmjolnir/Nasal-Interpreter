#ifndef __NASAL_RESOURCE_H__
#define __NASAL_RESOURCE_H__

/* filenames of lib files */
#ifndef LIB_FILE_NUM
#define LIB_FILE_NUM 11
const std::string lib_filename[LIB_FILE_NUM]=
{
	"lib/base.nas",
	"lib/bits.nas",
	"lib/io.nas",
	"lib/math.nas",
	"lib/readline.nas",
	"lib/regex.nas",
	"lib/sqlite.nas",
	"lib/system.nas",
	"lib/thread.nas",
	"lib/unix.nas",
	"lib/utf8.nas"
};
#endif

class nasal_resource
{
private:
    std::vector<char> res;
public:
    void               input_file(std::string);
    void               load_lib();
    void               delete_file();
    void               print_file();
    std::vector<char>& get_file();
};

void nasal_resource::input_file(std::string filename)
{
    std::ifstream fin(filename,std::ios::binary);
    if(fin.fail())
    {
        std::cout<<">> [resource] cannot open file \'"<<filename<<"\'."<<std::endl;
        fin.close();
        return;
    }
    while(!fin.eof())
    {
        char c=fin.get();
        if(fin.eof()) break;
        res.push_back(c);
    }
    fin.close();
    return;
}

void nasal_resource::load_lib()
{
    for(int i=0;i<LIB_FILE_NUM;++i)
        this->input_file(lib_filename[i]);
    return;
}

void nasal_resource::delete_file()
{
    res.clear();
    return;
}

void nasal_resource::print_file()
{
    int size=res.size(),line=1;
	std::cout<<line<<"\t";
	std::string unicode_str="";
	for(int i=0;i<size;++i)
	{
		if(res[i]>=0 && unicode_str.length())
		{
			std::cout<<unicode_str;
			unicode_str="";
		}
		if(32<=res[i])    std::cout<<res[i];
		else if(res[i]<0) unicode_str+=res[i];
		else              std::cout<<" ";
		if(res[i]=='\n')
		{
			++line;
			std::cout<<std::endl<<line<<"\t";
		}
	}
	std::cout<<(unicode_str.length()?unicode_str:"")<<std::endl;
	return;
}

std::vector<char>& nasal_resource::get_file()
{
    return res;
}

#endif