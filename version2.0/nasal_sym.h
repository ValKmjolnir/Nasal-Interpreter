#ifndef __NASAL_SYM_H__
#define __NASAL_SYM_H__

// all symbol_map use a same gc system named nasal_gc
// see detail of nasal_gc in "nasal_gc.h"
class sym_hash_map
{
	private:
		std::map<std::string,int> sym_map;
	public:
		sym_hash_map()
		{
			sym_map.clear();
			return;
		}
		sym_hash_map(const sym_hash_map& tmp)
		{
			sym_map=tmp.sym_map;
			for(std::map<std::string,int>::iterator i=sym_map.begin();i!=sym_map.end();++i)
				nasal_gc.reference_add(i->second);
			return;
		}
		~sym_hash_map()
		{
			for(std::map<std::string,int>::iterator i=sym_map.begin();i!=sym_map.end();++i)
				nasal_gc.reference_delete(i->second);
			sym_map.clear();
			return;
		}
        void set_clear()
        {
            sym_map.clear();
            return;
        }
		void add_new_symbol(std::string __sym_name)
		{
			if(sym_map.find(__sym_name)==sym_map.end())
				sym_map[__sym_name]=nasal_gc.gc_alloc();
			else
				std::cout<<">> [Runtime] "<<__sym_name<<" exists."<<std::endl;
			return;
		}
		int get_symbol_addr(std::string __sym_name)
		{
			if(sym_map.find(__sym_name)!=sym_map.end())
				return sym_map[__sym_name];
			else
				return -1;
		}
};

#endif