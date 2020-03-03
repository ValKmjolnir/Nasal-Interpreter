#ifndef __NASAL_GC_H__
#define __NASAL_GC_H__

class nasal_function
{
	private:
		std::list<std::map<std::string,int> > local_scope;
		abstract_syntax_tree function_root;
	public:
		nasal_function();
		~nasal_function();
		void set_clear();
		void set_statement_block(abstract_syntax_tree&);
		nasal_function& operator=(const nasal_function&);
		std::list<std::map<std::string,int> >& get_local_scope();
		abstract_syntax_tree& get_statement_block();
};

class nasal_scalar
{
	private:
		int type;
		std::string               var_string;
		double                    var_number;
		std::vector<int>          var_array;
		std::map<std::string,int> var_hash;
		nasal_function            var_func;
	public:
		nasal_scalar();
		nasal_scalar(const nasal_scalar&);
		nasal_scalar& operator=(const nasal_scalar&);
		void   set_clear();
		void   set_type(int);
		void   set_number(double);
		void   set_string(std::string&);
		void   vector_push(int);
		int    vector_pop();
		void   hash_add_new_member(std::string,int);
		void   hash_delete_member(std::string);
		int    hash_get_member(std::string);
		int    get_type();
		double get_number();
		std::string     get_string();
		nasal_function& get_function();
};

struct gc_unit
{
	// collected: If gc collected this item,it'll be set to true.Otherwise it is false.
	// elem:      Item that this unit stores
	// refcnt:    Reference counter
	bool         collected;
	nasal_scalar elem;
	int          refcnt;
	gc_unit()
	{
		collected=true;
		elem.set_clear();
		refcnt=0;
		return;
	}
	gc_unit(const gc_unit& tmp)
	{
		collected=tmp.collected;
		elem     =tmp.elem;
		refcnt   =tmp.refcnt;
		return;
	}
	void set_clear()
	{
		collected=true;
		elem.set_clear();
		refcnt=0;
		return;
	}
};

class gc_manager
{
	private:
		// free_space list is used to store space that is not in use.
		std::list<int> free_space;
		std::vector<gc_unit> memory;
	public:
		gc_manager()
		{
			memory.clear();
			free_space.clear();
			return;
		}
		~gc_manager()
		{
			memory.clear();
			free_space.clear();
			return;
		}
		void gc_init()
		{
			// this function must be called in class nasal_runtime
			// before running any codes
			std::vector<gc_unit> tmp_vec;
			memory.clear();
			memory.swap(tmp_vec);
			// clear the memory capacity by using tmp_vec.~vector<gc_unit>()
			free_space.clear();
			return;
		}
		void gc_scanner()
		{
			int memory_size=memory.size();
			for(int i=0;i<memory_size;++i)
				if((memory[i].refcnt<=0) && (!memory[i].collected))
				{
					// need more details here
					// especially vector and hash
					memory[i].set_clear();
					free_space.push_back(i);
					std::cout<<">> [Gc] collected ";
					prt_hex(i);
					std::cout<<"."<<std::endl;
				}
			return;
		}
		int gc_alloc()
		{
			if(free_space.empty())
			{
				gc_unit new_unit;
				memory.push_back(new_unit);
				free_space.push_back(memory.size()-1);
			}
			int alloc_plc=free_space.front();
			free_space.pop_front();
			memory[alloc_plc].collected=false;
			memory[alloc_plc].refcnt=1;
			return alloc_plc;
		}
		bool place_check(const int place)
		{
			// check if this place is in memory
			// and this place is uncollected
			return (place<memory.size()) && (!memory[place].collected);
		}
		void reference_add(const int place)
		{
			if(place<memory.size())
				++memory[place].refcnt;
			else
			{
				std::cout<<">> [Gc] fatal error: unexpected memory place ";
				prt_hex(place);
				std::cout<<" ."<<std::endl;
			}
			return;
		}
		void reference_delete(const int place)
		{
			if(place<memory.size())
				--memory[place].refcnt;
			else
			{
				std::cout<<">> [Gc] fatal error: unexpected memory place ";
				prt_hex(place);
				std::cout<<" ."<<std::endl;
			}
			return;
		}
		void info_print()
		{
			std::cout<<">> [Gc] memory size:"<<memory.size()*sizeof(gc_unit)<<" byte."<<std::endl;
			std::cout<<">> [Gc] memory capacity:"<<memory.capacity()*sizeof(gc_unit)<<" byte."<<std::endl;
			std::cout<<">> [Gc] memory usage: "<<std::endl;
			int cnt=0;
			for(int i=0;i<memory.size();++i)
				if(!memory[i].collected)
				{
					prt_hex(i);
					std::cout<<"["<<memory[i].refcnt<<"]";
					// cnt is used to check if it is the right time to output in the next line
					++cnt;
					if(!(cnt%8))
						std::cout<<std::endl;
					else
						std::cout<<" ";
				}
			if(cnt%8)
				std::cout<<std::endl;
			return;
		}
};
gc_manager nasal_gc;
// this object is used in "nasal_runtime.h"
// because there must be only one gc when running a program

nasal_function::nasal_function()
{
	local_scope.clear();
	function_root.set_clear();
	return;
}
nasal_function::~nasal_function()
{
	local_scope.clear();
	function_root.set_clear();
	return;
}
void nasal_function::set_clear()
{
	local_scope.clear();
	function_root.set_clear();
	return;
}
void nasal_function::set_statement_block(abstract_syntax_tree& func_block)
{
	function_root=func_block;
	return;
}
nasal_function& nasal_function::operator=(const nasal_function& tmp)
{
	local_scope=tmp.local_scope;
	function_root=tmp.function_root;
	return *this;
}
std::list<std::map<std::string,int> >& nasal_function::get_local_scope()
{
	return local_scope;
}
abstract_syntax_tree& nasal_function::get_statement_block()
{
	return function_root;
}

nasal_scalar::nasal_scalar()
{
	type=scalar_nil;
	var_string="";
	var_number=0;
	var_array.clear();
	var_hash.clear();
	return;
}
nasal_scalar::nasal_scalar(const nasal_scalar& tmp)
{
	type=tmp.type;
	var_string=tmp.var_string;
	var_number=tmp.var_number;
	var_array =tmp.var_array;
	var_hash  =tmp.var_hash;
	var_func  =tmp.var_func;
	return;
}
nasal_scalar& nasal_scalar::operator=(const nasal_scalar& tmp)
{
	type=tmp.type;
	var_string=tmp.var_string;
	var_number=tmp.var_number;
	var_array =tmp.var_array;
	var_hash  =tmp.var_hash;
	var_func  =tmp.var_func;
	return *this;
}
void nasal_scalar::set_clear()
{
	type=scalar_nil;
	var_string.clear();
	var_number=0;
	var_array.clear();
	var_hash.clear();
	var_func.set_clear();
	return;
}
void nasal_scalar::set_type(int tmp_type)
{
	// scalar_function is the last enum in enum::scalar_type
	type=tmp_type>scalar_function? scalar_nil:tmp_type;
	return;
}
void nasal_scalar::set_number(double tmp_number)
{
	var_number=tmp_number;
	return;
}
void nasal_scalar::set_string(std::string& tmp_str)
{
	var_string=tmp_str;
	return;
}
void nasal_scalar::vector_push(int addr)
{
	var_array.push_back(addr);
	return;
}
int nasal_scalar::vector_pop()
{
	int ret=var_array.back();
	var_array.pop_back();
	return ret;
}
void nasal_scalar::hash_add_new_member(std::string member_name,int addr)
{
	// if hash has a new function member
	// this function will get a value named 'me' in its own scope
	// and the 'me' points to the hash that has the function
	if(var_hash.find(member_name)!=var_hash.end())
		std::cout<<">> [Runtime] "<<member_name<<" exists."<<std::endl;
	else
		var_hash[member_name]=addr;
	return;
}
void nasal_scalar::hash_delete_member(std::string member_name)
{
	if(var_hash.find(member_name)!=var_hash.end())
	{
		nasal_gc.reference_delete(var_hash[member_name]);
		var_hash.erase(member_name);
	}
	return;
}
int nasal_scalar::hash_get_member(std::string member_name)
{
	if(var_hash.find(member_name)!=var_hash.end())
		return var_hash[member_name];
	return -1;
}
int nasal_scalar::get_type()
{
	return type;
}
double nasal_scalar::get_number()
{
	return var_number;
}
std::string nasal_scalar::get_string()
{
	return var_string;
}
nasal_function& nasal_scalar::get_function()
{
	return var_func;
}

#endif
