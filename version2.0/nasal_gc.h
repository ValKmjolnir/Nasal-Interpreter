#ifndef __NASAL_GC_H__
#define __NASAL_GC_H__

class nasal_function
{
	private:
		std::list<std::map<std::string,int> > local_scope;
		abstract_syntax_tree function_root;
		int  parent_hash_addr;
	public:
		nasal_function();
		void set_clear();
		void set_statement_block(abstract_syntax_tree&);
		void set_parent_hash_addr(int);
		std::list<std::map<std::string,int> >& get_local_scope();
		abstract_syntax_tree& get_statement_block();
		void deep_copy(nasal_function&);
};

class nasal_vector
{
	private:
		std::vector<int> nas_array;
	public:
		void set_clear();
		void vec_push(int);
		int  vec_pop();
		int  get_size();
		void deep_copy(nasal_vector&);
};

class nasal_hash
{
	private:
		std::map<std::string,int> nas_hash;
		int self_addr;
	public:
		void set_self_addr(int);
		void set_clear();
		int  get_self_addr();
		void hash_push(std::string,int);
		void hash_pop(std::string);
		void deep_copy(nasal_hash&);
};

class nasal_scalar
{
	private:
		int type;
		std::string    var_string;
		double         var_number;
		nasal_vector   var_vector;
		nasal_hash     var_hash;
		nasal_function var_func;
	public:
		nasal_scalar();
		nasal_scalar(const nasal_scalar&);
		nasal_scalar& operator=(const nasal_scalar&);
		void set_clear();
		void set_type(int);
		void set_number(double);
		void set_string(std::string&);
		int             get_type();
		double          get_number();
		std::string     get_string();
		nasal_vector&   get_vector();
		nasal_hash&     get_hash();
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
		refcnt   =0;
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
		nasal_scalar& get_scalar(int addr)
		{
			return memory[addr].elem;
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
		void reference_delete(const int addr)
		{
			if(addr<memory.size())
			{
				--memory[addr].refcnt;
				if(!memory[addr].refcnt)
				{
					memory[addr].collected=true;
					if(memory[addr].elem.get_type()==scalar_vector)
						memory[addr].elem.get_vector().set_clear();
					else if(memory[addr].elem.get_type()==scalar_hash)
						memory[addr].elem.get_hash().set_clear();
					else if(memory[addr].elem.get_type()==scalar_function)
						memory[addr].elem.get_function().set_clear();
					memory[addr].elem.set_clear();
				}
			}
			else
			{
				std::cout<<">> [Gc] fatal error: unexpected memory address: ";
				prt_hex(addr);
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
	parent_hash_addr=-1;
	return;
}
void nasal_function::set_clear()
{
	for(std::list<std::map<std::string,int> >::iterator iter=local_scope.begin();iter!=local_scope.end();++iter)
		for(std::map<std::string,int>::iterator i=iter->begin();i!=iter->end();++i)
			nasal_gc.reference_delete(i->second);
	local_scope.clear();
	function_root.set_clear();
	parent_hash_addr=-1;
	return;
}
void nasal_function::set_statement_block(abstract_syntax_tree& func_block)
{
	function_root=func_block;
	return;
}
void nasal_function::set_parent_hash_addr(int addr)
{
	parent_hash_addr=addr;
	return;
}
std::list<std::map<std::string,int> >& nasal_function::get_local_scope()
{
	return local_scope;
}
abstract_syntax_tree& nasal_function::get_statement_block()
{
	return function_root;
}
void nasal_function::deep_copy(nasal_function& tmp)
{
	for(std::list<std::map<std::string,int> >::iterator iter=local_scope.begin();iter!=local_scope.end();++iter)
		for(std::map<std::string,int>::iterator i=iter->begin();i!=iter->end();++i)
			nasal_gc.reference_delete(i->second);
	local_scope=tmp.local_scope;
	for(std::list<std::map<std::string,int> >::iterator iter=local_scope.begin();iter!=local_scope.end();++iter)
		for(std::map<std::string,int>::iterator i=iter->begin();i!=iter->end();++i)
			if(i->first!="me")
				nasal_gc.reference_add(i->second);
	if(!local_scope.empty())
	{
		std::list<std::map<std::string,int> >::iterator begin_iter=local_scope.begin();
		if(begin_iter->find("me")!=begin_iter->end())
		{
			if(parent_hash_addr>=0)
			{
				(*begin_iter)["me"]=parent_hash_addr;
				nasal_gc.reference_add(parent_hash_addr);
			}
			else
				begin_iter->erase("me");
		}
	}
	function_root=tmp.function_root;
	return;
}

void nasal_vector::set_clear()
{
	for(int i=0;i<nas_array.size();++i)
		nasal_gc.reference_delete(nas_array[i]);
	std::vector<int> vec_for_swap;
	nas_array.swap(vec_for_swap);
	return;
}
void nasal_vector::vec_push(int addr)
{
	nasal_gc.reference_add(addr);
	nas_array.push_back(addr);
	return;
}
int nasal_vector::vec_pop()
{
	int ret=nas_array.back();
	nas_array.pop_back();
	return ret;
}
int nasal_vector::get_size()
{
	return nas_array.size();
}
void nasal_vector::deep_copy(nasal_vector& tmp)
{
	for(int i=0;i<tmp.nas_array.size();++i)
	{
		int tmp_type=nasal_gc.get_scalar(nas_array[i]).get_type();
		// unfinished
	}
	return;
}

void nasal_hash::set_self_addr(int addr)
{
	// when creating a new hash,gc must give an address to this hash
	self_addr=addr;
	return;
}
void nasal_hash::set_clear()
{
	self_addr=-1;
	for(std::map<std::string,int>::iterator i=nas_hash.begin();i!=nas_hash.end();++i)
		nasal_gc.reference_delete(i->second);
	return;
}
int nasal_hash::get_self_addr()
{
	return self_addr;
}
void nasal_hash::hash_push(std::string member_name,int addr)
{
	if(nas_hash.find(member_name)==nas_hash.end())
	{
		nas_hash[member_name]=addr;
		if(nasal_gc.place_check(addr) && nasal_gc.get_scalar(addr).get_type()==scalar_function)
			nasal_gc.get_scalar(addr).get_function().set_parent_hash_addr(this->get_self_addr());
	}
	return;
}
void nasal_hash::hash_pop(std::string member_name)
{
	if(nas_hash.find(member_name)!=nas_hash.end())
	{
		nasal_gc.reference_delete(nas_hash[member_name]);
		nas_hash.erase(member_name);
	}
	return;
}
void nasal_hash::deep_copy(nasal_hash& tmp)
{
	for(std::map<std::string,int>::iterator i=tmp.nas_hash.begin();i!=tmp.nas_hash.end();++i)
	{
		int tmp_type=nasal_gc.get_scalar(i->second).get_type();
		// unfinished
	}
	return;
}

nasal_scalar::nasal_scalar()
{
	type=scalar_nil;
	var_string="";
	var_number=0;
	return;
}
nasal_scalar::nasal_scalar(const nasal_scalar& tmp)
{
	type=tmp.type;
	var_string=tmp.var_string;
	var_number=tmp.var_number;
	var_vector=tmp.var_vector;
	var_hash  =tmp.var_hash;
	var_func  =tmp.var_func;
	return;
}
nasal_scalar& nasal_scalar::operator=(const nasal_scalar& tmp)
{
	type=tmp.type;
	var_string=tmp.var_string;
	var_number=tmp.var_number;
	var_vector=tmp.var_vector;
	var_hash  =tmp.var_hash;
	var_func  =tmp.var_func;
	return *this;
}
void nasal_scalar::set_clear()
{
	type=scalar_nil;
	var_string.clear();
	var_number=0;
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
nasal_vector& nasal_scalar::get_vector()
{
	return var_vector;
}
nasal_hash& nasal_scalar::get_hash()
{
	return var_hash;
}
nasal_function& nasal_scalar::get_function()
{
	return var_func;
}

#endif
