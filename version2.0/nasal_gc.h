#ifndef __NASAL_GC_H__
#define __NASAL_GC_H__

// all identifiers in nasal points to a memory space in nasal_gc.
// memory space uses std::vector<gc_unit>.
// gc_unit is a struct which has nasal_scalar and refcnt in it.(more details please see the definition of gc_unit bellow)
// when an identifier needs to be assigned,but the type of value is not the same as the identifier's value
// the identifier will get a new memory space in nasal_gc and do deep_copy()
// and the memory space that is not used ,its ref_cnt-=1.

class nasal_function
{
	private:
		std::list<std::map<std::string,int> > local_scope;
		abstract_syntax_tree parameter_list;
		abstract_syntax_tree function_root;
		// parent_hash_addr is used to store the address of the hash which has this nasal_function
		// because nasal_function needs this address to adjust the identifier called 'me' in local_scope
		// 'me' is the identifier which points to the hash which has this nasal_function
	public:
		void set_clear();
		void set_local_scope(std::list<std::map<std::string,int> >&);
		void set_paramemter_list(abstract_syntax_tree&);
		void set_statement_block(abstract_syntax_tree&);
		std::list<std::map<std::string,int> >& get_local_scope();
		abstract_syntax_tree& get_parameter_list();
		abstract_syntax_tree& get_statement_block();
		void deep_copy(nasal_function&);
};

class nasal_number
{
	private:
		double nas_number;
	public:
		void   set_clear();
		void   set_number(double);
		double get_number();
		void   deep_copy(nasal_number&);
};

class nasal_string
{
	private:
		std::string nas_string;
	public:
		void set_clear();
		void set_string(std::string);
		std::string get_string();
		void deep_copy(nasal_string&);
};

class nasal_vector
{
	private:
		std::vector<int> nas_array;
	public:
		void set_clear();
		void vec_push(int);
		int* get_elem_addr(int);
		int  get_elem(int);
		int  vec_pop();
		int  get_size();
		int* get_parent_hash_member_addr(std::string);
		int  get_parent_hash_member(std::string);
		void generate_new_hash();
		void deep_copy(nasal_vector&);
};

class nasal_hash
{
	private:
		std::map<std::string,int> nas_hash;
	public:
		void set_clear();
		int* get_hash_member_addr(std::string);
		int  get_hash_member(std::string);
		void hash_push(std::string,int);
		void hash_pop(std::string);
		void deep_copy(nasal_hash&);
};

class nasal_scalar
{
	private:
		int type;
		nasal_string   var_string;
		nasal_number   var_number;
		nasal_vector   var_vector;
		nasal_hash     var_hash;
		nasal_function var_func;
	public:
		nasal_scalar();
		void            set_type(int);
		int             get_type();
		nasal_number&   get_number();
		nasal_string&   get_string();
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
	bool         is_const;
	nasal_scalar elem;
	int          refcnt;
	gc_unit()
	{
		collected=true;
		refcnt=0;
		return;
	}
};
#ifndef NAS_POOL_SIZE
#define NAS_POOL_SIZE 128
struct memory_block
{
	gc_unit space[NAS_POOL_SIZE];
	memory_block* next;
};

class memory_block_list
{
	private:
		memory_block* head;
		int mem_size;
		int blk_size;
	public:
		memory_block_list()
		{
			mem_size=0;
			blk_size=1;
			head=new memory_block;
			head->next=NULL;
			return;
		}
		~memory_block_list()
		{
			mem_size=0;
			blk_size=0;
			memory_block* ptr=head;
			while(ptr)
			{
				memory_block* tmp_ptr=ptr;
				ptr=ptr->next;
				delete tmp_ptr;
			}
			return;
		}
		void clear()
		{
			memory_block* ptr=head;
			while(ptr)
			{
				memory_block* tmp_ptr=ptr;
				ptr=ptr->next;
				delete tmp_ptr;
			}
			mem_size=0;
			blk_size=1;
			head=new memory_block;
			head->next=NULL;
			return;
		}
		gc_unit& operator[](int address)
		{
			int block_num=address/NAS_POOL_SIZE;
			int block_plc=address%NAS_POOL_SIZE;
			memory_block* ptr=head;
			for(int i=0;i<block_num;++i)
				ptr=ptr->next;
			return ptr->space[block_plc];
		}
		void push_back()
		{
			++mem_size;
			if(mem_size>blk_size*NAS_POOL_SIZE)
			{
				memory_block* ptr=head;
				while(ptr->next)
					ptr=ptr->next;
				ptr->next=new memory_block;
				ptr->next->next=NULL;
				++blk_size;
			}
			return;
		}
		int size()
		{
			return mem_size;
		}
		int capacity()
		{
			return NAS_POOL_SIZE*blk_size;
		}
};
#endif

class gc_manager
{
	private:
		// free_space list is used to store space that is not in use.
		std::list<int> free_space;
		/*
		cannot use std::vector to simulate memory
		because if vector memory is not enough,vector will use another larger memory as it's main memory
		then all the things will be moved to a new space,
		at this time if you reference a member in it,this will cause segmentation error.
		*/
		memory_block_list memory;
		bool error_occurred;
	public:
		void gc_init()
		{
			// this function must be called in class nasal_runtime before running any codes
			memory.clear();
			free_space.clear();
			error_occurred=false;
			return;
		}
		int gc_alloc()
		{
			// add a new space for a new value
			// if list free_space is not empty,it will get the address at the front and give it to the new value
			// if list free_space is empty,it will add new space in memory vector and give it to the new value
			// by this way it can manage memory efficiently.
			if(free_space.empty())
			{
				memory.push_back();
				free_space.push_back(memory.size()-1);
			}
			int alloc_plc=free_space.front();
			free_space.pop_front();
			memory[alloc_plc].collected=false;
			memory[alloc_plc].refcnt=1;
			return alloc_plc;
		}
		int get_reference(int addr)
		{
			// get the reference counts of the scalar
			return memory[addr].refcnt;
		}
		nasal_scalar& get_scalar(int addr)
		{
			// get the reference of the scalar
			return memory[addr].elem;
		}
		bool place_check(const int addr)
		{
			// check if this place is in memory
			// and this place is uncollected
			// this function is often used when an identifier is calling a space in memory
			return (0<=addr) && (addr<memory.size()) && (!memory[addr].collected);
		}
		bool reference_add(const int addr)
		{
			if((0<=addr) && (addr<memory.size()) && (!memory[addr].collected))
				++memory[addr].refcnt;
			else
			{
				std::cout<<">> [Gc] fatal error: reference unexpected memory place ";
				prt_hex(addr);
				std::cout<<" ."<<std::endl;
				return false;
			}
			return true;
		}
		bool reference_delete(const int addr)
		{
			if((0<=addr) && (addr<memory.size()) && (!memory[addr].collected))
			{
				--memory[addr].refcnt;
				if(!memory[addr].refcnt)
				{
					// if refcnt is 0,then starting the destructor
					// std::cout<<">> [Gc] collected ";prt_hex(addr);std::cout<<std::endl;
					memory[addr].collected=true;
					switch(memory[addr].elem.get_type())
					{
						case scalar_number:  memory[addr].elem.get_number().set_clear();  break;
						case scalar_string:  memory[addr].elem.get_string().set_clear();  break;
						case scalar_vector:  memory[addr].elem.get_vector().set_clear();  break;
						case scalar_hash:    memory[addr].elem.get_hash().set_clear();    break;
						case scalar_function:memory[addr].elem.get_function().set_clear();break;
						default:break;
					}
					memory[addr].elem.set_type(scalar_nil);
					free_space.push_back(addr);
				}
			}
			else
			{
				std::cout<<">> [Gc] fatal error: delete unexpected memory address: ";
				prt_hex(addr);
				std::cout<<" ."<<std::endl;
				return false;
			}
			return true;
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
		bool check_error()
		{
			return error_occurred;
		}
};
gc_manager nasal_gc;
// this object is used in "nasal_runtime.h"
// because there must be only one gc when running a program(one process)


void nasal_function::set_clear()
{
	for(std::list<std::map<std::string,int> >::iterator iter=local_scope.begin();iter!=local_scope.end();++iter)
		for(std::map<std::string,int>::iterator i=iter->begin();i!=iter->end();++i)
			nasal_gc.reference_delete(i->second);
	local_scope.clear();
	function_root.set_clear();
	return;
}
void nasal_function::set_local_scope(std::list<std::map<std::string,int> >& tmp_scope)
{
	local_scope=tmp_scope;
	for(std::list<std::map<std::string,int> >::iterator iter=local_scope.begin();iter!=local_scope.end();++iter)
		for(std::map<std::string,int>::iterator i=iter->begin();i!=iter->end();++i)
				nasal_gc.reference_add(i->second);
	return;
}
void nasal_function::set_paramemter_list(abstract_syntax_tree& para_list)
{
	parameter_list=para_list;
	return;
}
void nasal_function::set_statement_block(abstract_syntax_tree& func_block)
{
	function_root=func_block;
	return;
}
std::list<std::map<std::string,int> >& nasal_function::get_local_scope()
{
	return local_scope;
}
abstract_syntax_tree& nasal_function::get_parameter_list()
{
	return parameter_list;
}
abstract_syntax_tree& nasal_function::get_statement_block()
{
	return function_root;
}
void nasal_function::deep_copy(nasal_function& tmp)
{
	// before deep copy nasal_functions needs to delete all values in its scope
	for(std::list<std::map<std::string,int> >::iterator iter=local_scope.begin();iter!=local_scope.end();++iter)
		for(std::map<std::string,int>::iterator i=iter->begin();i!=iter->end();++i)
			nasal_gc.reference_delete(i->second);
	// when copying a local scope,one thing that must be noticed is that
	// each identifier in local_scope shares the same address with tmp.local_scope
	// copy all the values in tmp's scope
	local_scope=tmp.local_scope;
	for(std::list<std::map<std::string,int> >::iterator iter=local_scope.begin();iter!=local_scope.end();++iter)
		for(std::map<std::string,int>::iterator i=iter->begin();i!=iter->end();++i)
				nasal_gc.reference_add(i->second);
	// copy abstract_syntax_tree
	parameter_list=tmp.parameter_list;
	function_root=tmp.function_root;
	return;
}

void nasal_number::set_clear()
{
	nas_number=0;
	return;
}
void nasal_number::set_number(double num)
{
	nas_number=num;
	return;
}
double nasal_number::get_number()
{
	return nas_number;
}
void nasal_number::deep_copy(nasal_number& tmp)
{
	nas_number=tmp.nas_number;
	return;
}

void nasal_string::set_clear()
{
	nas_string="";
	return;
}
void nasal_string::set_string(std::string str)
{
	nas_string=str;
	return;
}
std::string nasal_string::get_string()
{
	return nas_string;
}
void nasal_string::deep_copy(nasal_string& tmp)
{
	nas_string=tmp.nas_string;
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
	nas_array.push_back(addr);
	return;
}
int* nasal_vector::get_elem_addr(int addr)
{
	// 0 ~ size-1 -size ~ -1
	int bound=nas_array.size();
	if(-bound<=addr && addr<0)
		return &nas_array[bound+addr];
	else if(0<=addr && addr<bound)
		return &nas_array[addr];
	return NULL;
}
int nasal_vector::get_elem(int addr)
{
	// 0 ~ size-1 -size ~ -1
	int bound=nas_array.size();
	if(-bound<=addr && addr<0)
		return nas_array[bound+addr];
	else if(0<=addr && addr<bound)
		return nas_array[addr];
	return -1;
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
int* nasal_vector::get_parent_hash_member_addr(std::string member_name)
{
	int* ret_addr=NULL;
	for(int i=0;i<nas_array.size();++i)
		if(nasal_gc.get_scalar(nas_array[i]).get_type()==scalar_hash)
		{
			ret_addr=nasal_gc.get_scalar(nas_array[i]).get_hash().get_hash_member_addr(member_name);
			if(ret_addr)
				break;
		}
	return ret_addr;
}
int nasal_vector::get_parent_hash_member(std::string member_name)
{
	int ret_addr=-1;
	for(int i=0;i<nas_array.size();++i)
		if(nasal_gc.get_scalar(nas_array[i]).get_type()==scalar_hash)
		{
			ret_addr=nasal_gc.get_scalar(nas_array[i]).get_hash().get_hash_member(member_name);
			if(ret_addr>=0)
				break;
		}
	return ret_addr;
}
void nasal_vector::generate_new_hash()
{
	for(int i=0;i<nas_array.size();++i)
		if(nas_array[i]>=0 && nasal_gc.get_scalar(nas_array[i]).get_type()==scalar_hash)
		{
			int tmp_addr=nas_array[i];
			nas_array[i]=nasal_gc.gc_alloc();
			nasal_gc.get_scalar(nas_array[i]).set_type(scalar_hash);
			nasal_gc.get_scalar(nas_array[i]).get_hash().deep_copy(nasal_gc.get_scalar(tmp_addr).get_hash());
			nasal_gc.reference_delete(tmp_addr);;
		}
	return;
}
void nasal_vector::deep_copy(nasal_vector& tmp)
{
	// before deep copy,nasal_vector needs to delete all values in it.
	for(int i=0;i<nas_array.size();++i)
		nasal_gc.reference_delete(nas_array[i]);
	// copy process
	for(int i=0;i<tmp.nas_array.size();++i)
	{
		int tmp_type=nasal_gc.get_scalar(tmp.nas_array[i]).get_type();
		int new_addr=nasal_gc.gc_alloc();
		nasal_gc.get_scalar(new_addr).set_type(tmp_type);
		if(tmp_type==scalar_nil)
			;
		else if(tmp_type==scalar_number)
			nasal_gc.get_scalar(new_addr).get_number().deep_copy(nasal_gc.get_scalar(tmp.nas_array[i]).get_number());
		else if(tmp_type==scalar_string)
			nasal_gc.get_scalar(new_addr).get_string().deep_copy(nasal_gc.get_scalar(tmp.nas_array[i]).get_string());
		else if(tmp_type==scalar_vector)
			nasal_gc.get_scalar(new_addr).get_vector().deep_copy(nasal_gc.get_scalar(tmp.nas_array[i]).get_vector());
		else if(tmp_type==scalar_hash)
			nasal_gc.get_scalar(new_addr).get_hash().deep_copy(nasal_gc.get_scalar(tmp.nas_array[i]).get_hash());
		else if(tmp_type==scalar_function)
			nasal_gc.get_scalar(new_addr).get_function().deep_copy(nasal_gc.get_scalar(tmp.nas_array[i]).get_function());
		nas_array.push_back(new_addr);
	}
	return;
}

void nasal_hash::set_clear()
{
	for(std::map<std::string,int>::iterator i=nas_hash.begin();i!=nas_hash.end();++i)
		nasal_gc.reference_delete(i->second);
	return;
}
int* nasal_hash::get_hash_member_addr(std::string member_name)
{
	if(nas_hash.find(member_name)!=nas_hash.end())
		return &nas_hash[member_name];
	else if(nas_hash.find("parents")!=nas_hash.end())
		return nasal_gc.get_scalar(nas_hash["parents"]).get_vector().get_parent_hash_member_addr(member_name);
	return NULL;
}
int nasal_hash::get_hash_member(std::string member_name)
{
	if(nas_hash.find(member_name)!=nas_hash.end())
		return nas_hash[member_name];
	else if(nas_hash.find("parents")!=nas_hash.end())
			return nasal_gc.get_scalar(nas_hash["parents"]).get_vector().get_parent_hash_member(member_name);
	return -1;
}
void nasal_hash::hash_push(std::string member_name,int addr)
{
	nas_hash[member_name]=addr;
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
	// defore deep copy,nasal_hash needs to delete all members in it
	for(std::map<std::string,int>::iterator i=nas_hash.begin();i!=nas_hash.end();++i)
		nasal_gc.reference_delete(i->second);
	// copy process
	for(std::map<std::string,int>::iterator i=tmp.nas_hash.begin();i!=tmp.nas_hash.end();++i)
	{
		int tmp_type=nasal_gc.get_scalar(i->second).get_type();
		int new_addr=nasal_gc.gc_alloc();
		nasal_gc.get_scalar(new_addr).set_type(tmp_type);
		switch(tmp_type)
		{
			case scalar_nil     :break;
			case scalar_number  :nasal_gc.get_scalar(new_addr).get_number().deep_copy(nasal_gc.get_scalar(i->second).get_number());break;
			case scalar_string  :nasal_gc.get_scalar(new_addr).get_string().deep_copy(nasal_gc.get_scalar(i->second).get_string());break;
			case scalar_vector  :nasal_gc.get_scalar(new_addr).get_vector().deep_copy(nasal_gc.get_scalar(i->second).get_vector());break;
			case scalar_hash    :nasal_gc.get_scalar(new_addr).get_hash().deep_copy(nasal_gc.get_scalar(i->second).get_hash());break;
			case scalar_function:nasal_gc.get_scalar(new_addr).get_function().deep_copy(nasal_gc.get_scalar(i->second).get_function());break;
		}
		nas_hash[i->first]=new_addr;
	}
	return;
}

nasal_scalar::nasal_scalar()
{
	type=scalar_nil;
	return;
}
void nasal_scalar::set_type(int tmp_type)
{
	// scalar_function is the last enum in enum::scalar_type
	type=tmp_type>scalar_function? scalar_nil:tmp_type;
	return;
}
int nasal_scalar::get_type()
{
	// get scalar type
	return type;
}
nasal_number& nasal_scalar::get_number()
{
	// get nasal_number
	return var_number;
}
nasal_string& nasal_scalar::get_string()
{
	// get nasal_string
	return var_string;
}
nasal_vector& nasal_scalar::get_vector()
{
	// get nasal_vector
	return var_vector;
}
nasal_hash& nasal_scalar::get_hash()
{
	// get nasal_hash
	return var_hash;
}
nasal_function& nasal_scalar::get_function()
{
	// get nasal_function
	return var_func;
}

#endif

/*
code: var i=1;
int addr=nasal_gc.gc_alloc();
nasal_gc.get_scalar(addr).set_type(scalar_number);
nasal_gc.get_scalar(addr).set_number(1);

code: var i='hello';
int addr=nasal_gc.gc_alloc();
nasal_gc.get_scalar(addr).set_type(scalar_string);
nasal_gc.get_scalar(addr).set_string("hello");

code: var i=[];
int addr=vector_generation();
nasal_gc.get_scalar(addr).set_type(scalar_vector);

code: var i={};
int addr=hash_generation();
nasal_gc.get_scalar(addr).set_type(scalar_hash);
nasal_gc.get_scalar(addr).get_hash().set_self_addr(addr);

code: var i=func{return 0;}
// copy local_scope if needed
// copy abstract_syntax_tree
*/