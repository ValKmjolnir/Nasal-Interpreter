#ifndef __NASAL_RESOURCE_H__
#define __NASAL_RESOURCE_H__

class nasal_resource
{
private:
    std::vector<char> res;
public:
    bool input_file(std::string);
    void clear();
    void print_file();
    std::vector<char>& get_file();
};

bool nasal_resource::input_file(std::string filename)
{
    res.clear();
    std::ifstream fin(filename,std::ios::binary);
    if(fin.fail())
    {
        std::cout<<">> [resource] cannot open file \""<<filename<<"\".\n";
        fin.close();
        return false;
    }
    while(!fin.eof())
    {
        char c=fin.get();
        if(fin.eof())
            break;
        res.push_back(c);
    }
    fin.close();
    return true;
}

void nasal_resource::clear()
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
		if(32<=res[i])
            std::cout<<res[i];
		else if(res[i]<0)
            unicode_str+=res[i];
		else
            std::cout<<" ";
		if(res[i]=='\n')
		{
			++line;
			std::cout<<std::endl<<line<<"\t";
		}
	}
	std::cout<<(unicode_str.length()?unicode_str:"")<<'\n';
	return;
}

std::vector<char>& nasal_resource::get_file()
{
    return res;
}

#endif