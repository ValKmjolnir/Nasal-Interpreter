#ifndef __NASAL_GC_H__
#define __NASAL_GC_H__

#define MEM_BLK_SIZE 256 // 0x00 ~ 0xff
#define GC_BLK_SIZE  256 // 0x00 ~ 0xff
/*
nasal_number: basic type(double)
nasal_string: basic type(std::string)
nasal_vector: elems[i] -> address in memory -> value address in gc
nasal_hash:   elems[key] -> address in memory -> value address in gc
nasal_function: closure -> value address in gc(type: nasal_closure)
nasal_closure: std::list<std::map<std::string,int>> -> std::map<std::string,int> -> (int) -> address in memory -> value address in gc
*/

class nasal_vector
{
private:
    // this int points to the space in nasal_vm::memory_manager_memory
    std::vector<int> elems;
public:
    nasal_vector();
    ~nasal_vector();
    void add_elem(int);
    int  del_elem(int);
    int  get_value_address(int);
    int  get_mem_address(int);
};

class nasal_hash
{
private:
    // this int points to the space in nasal_vm::memory_manager_memory
    std::map<std::string,int> elems;
public:
    nasal_hash();
    ~nasal_hash();
    void add_elem(std::string,int);
    void del_elem(std::string);
    int  get_value_address(std::string);
    int  get_mem_address(std::string);
};

class nasal_function
{
private:
    // this int points to the space in nasal_vm::garbage_collector_memory
    int closure_addr;
    nasal_ast argument_list;
    nasal_ast function_expr;
public:
    nasal_function();
    ~nasal_function();
    void set_closure_addr(int);
    int  get_closure_addr();
    void set_arguments(nasal_ast&);
    void set_run_block(nasal_ast&);
};

class nasal_closure
{
private:
    // int in std::map<std::string,int> points to the space in nasal_vm::memory_manager_memory
    // and this memory_manager_memory space stores an address to garbage_collector_memory
    // and this address points to an nasal_hash
    std::list<std::map<std::string,int> > elems;
public:
    nasal_closure();
    ~nasal_closure();
    void add_scope();
    void del_scope();
    void add_new_value(std::string,int);
    int  get_value_address(std::string);
    int  get_mem_address(std::string);
};

class nasal_scalar
{
private:
    int type;
    void* scalar_ptr;
public:
    nasal_scalar();
    ~nasal_scalar();
    void clear();
    bool set_type(int);
    void set_number(double);
    void set_string(std::string);
    double get_number();
    std::string get_string();
    nasal_vector& get_vector();
    nasal_hash& get_hash();
    nasal_function& get_func();
    nasal_closure& get_closure();
    // parameter: memory_manager_memory address
    int nasal_scalar_add(int,int);
    int nasal_scalar_sub(int,int);
    int nasal_scalar_mult(int,int);
    int nasal_scalar_div(int,int);
    int nasal_scalar_link(int,int);
    int nasal_scalar_unary_sub(int);
    int nasal_scalar_unary_not(int);
    int nasal_scalar_cmp_equal(int,int);
    int nasal_scalar_cmp_not_equal(int,int);
    int nasal_scalar_cmp_less(int,int);
    int nasal_scalar_cmp_greater(int,int);
    int nasal_scalar_cmp_less_or_equal(int,int);
    int nasal_scalar_cmp_greater_or_equal(int,int);
};

class nasal_virtual_machine
{
    struct gc_unit
    {
        bool collected;
        char ref_cnt;
        nasal_scalar elem;
        gc_unit()
        {
            collected=true;
            ref_cnt=0;
            return;
        }
    };
private:
    nasal_scalar error_returned_value;
    std::queue<int> garbage_collector_free_space;
    std::vector<gc_unit*> garbage_collector_memory;
    std::queue<int> memory_manager_free_space;
    std::vector<int*> memory_manager_memory;
public:
    ~nasal_virtual_machine();
    int gc_alloc();             // garbage collector gives a new space
    nasal_scalar& gc_get(int);  // get scalar that stored in gc
    int add_reference(int);
    int del_reference(int);
    int mem_alloc();            // memory gives a new space
    int mem_free(int);          // give space back to memory
    int mem_change(int,int);    // change value in memory space
    int mem_init(int,int);      // init value in memory space
    int mem_get(int);           // get value in memory space
};

/*
    nasal runtime virtual machine
    nasal_scalar_calculator gives functions that calculate nasal_scalar
    example: nasal_scalar_calculator.nasal_scalar_add(0x00007c00,0x0000aa55)
*/
nasal_virtual_machine nasal_vm;
nasal_scalar nasal_scalar_calculator;

/*functions of nasal_vector*/
nasal_vector::nasal_vector()
{
    elems.clear();
    return;
}
nasal_vector::~nasal_vector()
{
    int size=elems.size();
    for(int i=0;i<size;++i)
        nasal_vm.mem_free(elems[i]);
    elems.clear();
    return;
}
void nasal_vector::add_elem(int memory_address)
{
    elems.push_back(memory_address);
    return;
}
int nasal_vector::del_elem(int index)
{
    if(index>=elems.size())
        return -1;
    int ret=elems[index];
    nasal_vm.mem_free(ret);
    elems.pop_back();
    return ret;
}
int nasal_vector::get_value_address(int index)
{
    int vec_size=elems.size();
    int left_range=-vec_size;
    int right_range=vec_size-1;
    if(index<left_range || index>right_range)
    {
        std::cout<<">> [runtime] nasal_vector::get_value_address: index out of range."<<std::endl;
        return -1;
    }
    return nasal_vm.mem_get(elems[(index+vec_size)%vec_size]);
}
int nasal_vector::get_mem_address(int index)
{
    int vec_size=elems.size();
    int left_range=-vec_size;
    int right_range=vec_size-1;
    if(index<left_range || index>right_range)
    {
        std::cout<<">> [runtime] nasal_vector::get_mem_address: index out of range."<<std::endl;
        return -1;
    }
    return elems[(index+vec_size)%vec_size];
}

/*functions of nasal_hash*/
nasal_hash::nasal_hash()
{
    elems.clear();
    return;
}
nasal_hash::~nasal_hash()
{
    for(std::map<std::string,int>::iterator iter=elems.begin();iter!=elems.end();++iter)
        nasal_vm.mem_free(iter->second);
    elems.clear();
    return;
}
void nasal_hash::add_elem(std::string key,int memory_address)
{
    if(elems.find(key)==elems.end())
        elems[key]=memory_address;
    else
        std::cout<<">> [vm] nasal_hash::add_elem: "<<key<<" already exists."<<std::endl;
    return;
}
void nasal_hash::del_elem(std::string key)
{
    if(elems.find(key)!=elems.end())
    {
        nasal_vm.mem_free(elems[key]);
        elems.erase(key);
    }
    else
        std::cout<<">> [vm] nasal_hash::del_elem: "<<key<<" does not exist."<<std::endl;
    return;
}
int nasal_hash::get_value_address(std::string key)
{
    if(elems.find(key)!=elems.end())
        return nasal_vm.mem_get(elems[key]);
    else
        std::cout<<">> [vm] nasal_hash:get_value_address: "<<key<<" does not exist."<<std::endl;
    return -1;
}
int nasal_hash::get_mem_address(std::string key)
{
    if(elems.find(key)!=elems.end())
        return elems[key];
    else
        std::cout<<">> [vm] nasal_hash:get_mem_address: "<<key<<" does not exist."<<std::endl;
    return -1;
}

/*functions of nasal_function*/
nasal_function::nasal_function()
{
    closure_addr=-1;
    argument_list.clear();
    function_expr.clear();
    return;
}
nasal_function::~nasal_function()
{
    nasal_vm.del_reference(closure_addr);
    argument_list.clear();
    function_expr.clear();
    return;
}
void nasal_function::set_closure_addr(int value_address)
{
    closure_addr=value_address;
    return;
}
int nasal_function::get_closure_addr()
{
    return this->closure_addr;
}
void nasal_function::set_arguments(nasal_ast& node)
{
    argument_list=node;
    return;
}
void nasal_function::set_run_block(nasal_ast& node)
{
    function_expr=node;
    return;
}

/*functions of nasal_closure*/
nasal_closure::nasal_closure()
{
    elems.clear();
    return;
}
nasal_closure::~nasal_closure()
{
    for(std::list<std::map<std::string,int> >::iterator i=elems.begin();i!=elems.end();++i)
    {
        for(std::map<std::string,int>::iterator j=i->begin();j!=i->end();++j)
            nasal_vm.mem_free(j->second);
        i->clear();
    }
    elems.clear();
    return;
}
void nasal_closure::add_scope()
{
    std::map<std::string,int> new_scope;
    this->elems.push_back(new_scope);
    return;
}
void nasal_closure::del_scope()
{
    if(this->elems.empty())
        return;
    for(std::map<std::string,int>::iterator i=this->elems.back().begin();i!=this->elems.back().end();++i)
        nasal_vm.mem_free(i->second);
    this->elems.pop_back();
    return;
}
void nasal_closure::add_new_value(std::string key,int value_address)
{
    int new_mem_address=nasal_vm.mem_alloc();
    nasal_vm.mem_init(new_mem_address,value_address);
    if(elems.back().find(key)!=elems.back().end())
    {
        // if this value already exists,delete the old value and update a new value
        int old_mem_address=elems.back()[key];
        nasal_vm.mem_free(old_mem_address);
    }
    elems.back()[key]=new_mem_address;
    return;
}
int nasal_closure::get_value_address(std::string key)
{
    int ret_address=-1;
    for(std::list<std::map<std::string,int> >::iterator i=elems.begin();i!=elems.end();++i)
    {
        if(i->find(key)!=i->end())
            ret_address=nasal_vm.mem_get((*i)[key]);
    }
    return ret_address;
}
int nasal_closure::get_mem_address(std::string key)
{
    int ret_address=-1;
    for(std::list<std::map<std::string,int> >::iterator i=elems.begin();i!=elems.end();++i)
    {
        if(i->find(key)!=i->end())
            ret_address=(*i)[key];
    }
    return ret_address;
}

/*functions of nasal_scalar*/
nasal_scalar::nasal_scalar()
{
    this->type=vm_nil;
    this->scalar_ptr=(void*)NULL;
    return;
}
nasal_scalar::~nasal_scalar()
{
    switch(this->type)
    {
        case vm_nil:break;
        case vm_number:   delete (double*)(this->scalar_ptr);         break;
        case vm_string:   delete (std::string*)(this->scalar_ptr);    break;
        case vm_vector:   delete (nasal_vector*)(this->scalar_ptr);   break;
        case vm_hash:     delete (nasal_hash*)(this->scalar_ptr);     break;
        case vm_function: delete (nasal_function*)(this->scalar_ptr); break;
        case vm_closure:  delete (nasal_closure*)(this->scalar_ptr);  break;
    }
    return;
}
void nasal_scalar::clear()
{
    this->type=vm_nil;
    switch(this->type)
    {
        case vm_nil:break;
        case vm_number:   delete (double*)(this->scalar_ptr);         break;
        case vm_string:   delete (std::string*)(this->scalar_ptr);    break;
        case vm_vector:   delete (nasal_vector*)(this->scalar_ptr);   break;
        case vm_hash:     delete (nasal_hash*)(this->scalar_ptr);     break;
        case vm_function: delete (nasal_function*)(this->scalar_ptr); break;
        case vm_closure:  delete (nasal_closure*)(this->scalar_ptr);  break;
    }
    return;
}
bool nasal_scalar::set_type(int nasal_scalar_type)
{
    bool ret=true;
    this->type=nasal_scalar_type;
    switch(nasal_scalar_type)
    {
        case vm_nil:      this->scalar_ptr=(void*)NULL;                 break;
        case vm_number:   this->scalar_ptr=(void*)(new double);         break;
        case vm_string:   this->scalar_ptr=(void*)(new std::string);    break;
        case vm_vector:   this->scalar_ptr=(void*)(new nasal_vector);   break;
        case vm_hash:     this->scalar_ptr=(void*)(new nasal_hash);     break;
        case vm_function: this->scalar_ptr=(void*)(new nasal_function); break;
        case vm_closure:  this->scalar_ptr=(void*)(new nasal_closure);  break;
        default:
            std::cout<<">> [vm] error scalar type: "<<nasal_scalar_type<<std::endl;
            this->type=vm_nil;
            this->scalar_ptr=(void*)NULL;
            ret=false;
            break;
    }
    return ret;
}
void nasal_scalar::set_number(double num)
{
    *(double*)(this->scalar_ptr)=num;
    return;
}
void nasal_scalar::set_string(std::string str)
{
    *(std::string*)(this->scalar_ptr)=str;
    return;
}
double nasal_scalar::get_number()
{
    return *(double*)(this->scalar_ptr);
}
std::string nasal_scalar::get_string()
{
    return *(std::string*)(this->scalar_ptr);
}
nasal_vector& nasal_scalar::get_vector()
{
    return *(nasal_vector*)(this->scalar_ptr);
}
nasal_hash& nasal_scalar::get_hash()
{
    return *(nasal_hash*)(this->scalar_ptr);
}
nasal_function& nasal_scalar::get_func()
{
    return *(nasal_function*)(this->scalar_ptr);
}
nasal_closure& nasal_scalar::get_closure()
{
    return *(nasal_closure*)(this->scalar_ptr);
}
int nasal_scalar::nasal_scalar_add(int a_scalar_addr,int b_scalar_addr)
{
    if(a_scalar_addr<0 || b_scalar_addr<0)
    {
        std::cout<<">> [vm] scalar_add: memory returned an invalid address"<<std::endl;
        return -1;
    }
    nasal_scalar& a_ref=nasal_vm.gc_get(a_scalar_addr);
    nasal_scalar& b_ref=nasal_vm.gc_get(b_scalar_addr);
    if((a_ref.type!=vm_number && a_ref.type!=vm_string)||(b_ref.type!=vm_number && b_ref.type!=vm_string))
    {
        std::cout<<">> [vm] scalar_add: error value type.only number and string can take part in add."<<std::endl;
        return -1;
    }
    double a_num;
    double b_num;
    if(a_ref.type==vm_number) a_num=*(double*)a_ref.scalar_ptr;
    else
    {
        if(check_numerable_string(*(std::string*)a_ref.scalar_ptr))
            a_num=trans_string_to_number(*(std::string*)a_ref.scalar_ptr);
        else
        {
            std::cout<<">> [vm] scalar_add: "<<*(std::string*)a_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
            return -1;
        }
    }
    if(b_ref.type==vm_number) b_num=*(double*)b_ref.scalar_ptr;
    else
    {
        if(check_numerable_string(*(std::string*)b_ref.scalar_ptr))
            b_num=trans_string_to_number(*(std::string*)b_ref.scalar_ptr);
        else
        {
            std::cout<<">> [vm] scalar_add: "<<*(std::string*)b_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
            return -1;
        }
    }
    double result=a_num+b_num;
    int new_value_address=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_value_address).set_type(vm_number);
    nasal_vm.gc_get(new_value_address).set_number(result);
    return new_value_address;
}
int nasal_scalar::nasal_scalar_sub(int a_scalar_addr,int b_scalar_addr)
{
    if(a_scalar_addr<0 || b_scalar_addr<0)
    {
        std::cout<<">> [vm] scalar_sub: memory returned an invalid address"<<std::endl;
        return -1;
    }
    nasal_scalar& a_ref=nasal_vm.gc_get(a_scalar_addr);
    nasal_scalar& b_ref=nasal_vm.gc_get(b_scalar_addr);
    if((a_ref.type!=vm_number && a_ref.type!=vm_string)||(b_ref.type!=vm_number && b_ref.type!=vm_string))
    {
        std::cout<<">> [vm] scalar_sub: error value type.only number and string can take part in sub."<<std::endl;
        return -1;
    }
    double a_num;
    double b_num;
    if(a_ref.type==vm_number) a_num=*(double*)a_ref.scalar_ptr;
    else
    {
        if(check_numerable_string(*(std::string*)a_ref.scalar_ptr))
            a_num=trans_string_to_number(*(std::string*)a_ref.scalar_ptr);
        else
        {
            std::cout<<">> [vm] scalar_sub: "<<*(std::string*)a_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
            return -1;
        }
    }
    if(b_ref.type==vm_number) b_num=*(double*)b_ref.scalar_ptr;
    else
    {
        if(check_numerable_string(*(std::string*)b_ref.scalar_ptr))
            b_num=trans_string_to_number(*(std::string*)b_ref.scalar_ptr);
        else
        {
            std::cout<<">> [vm] scalar_sub: "<<*(std::string*)b_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
            return -1;
        }
    }
    double result=a_num-b_num;
    int new_value_address=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_value_address).set_type(vm_number);
    nasal_vm.gc_get(new_value_address).set_number(result);
    return new_value_address;
}
int nasal_scalar::nasal_scalar_mult(int a_scalar_addr,int b_scalar_addr)
{
    if(a_scalar_addr<0 || b_scalar_addr<0)
    {
        std::cout<<">> [vm] scalar_mult: memory returned an invalid address"<<std::endl;
        return -1;
    }
    nasal_scalar& a_ref=nasal_vm.gc_get(a_scalar_addr);
    nasal_scalar& b_ref=nasal_vm.gc_get(b_scalar_addr);
    if((a_ref.type!=vm_number && a_ref.type!=vm_string)||(b_ref.type!=vm_number && b_ref.type!=vm_string))
    {
        std::cout<<">> [vm] scalar_mult: error value type.only number and string can take part in mult."<<std::endl;
        return -1;
    }
    double a_num;
    double b_num;
    if(a_ref.type==vm_number) a_num=*(double*)a_ref.scalar_ptr;
    else
    {
        if(check_numerable_string(*(std::string*)a_ref.scalar_ptr))
            a_num=trans_string_to_number(*(std::string*)a_ref.scalar_ptr);
        else
        {
            std::cout<<">> [vm] scalar_mult: "<<*(std::string*)a_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
            return -1;
        }
    }
    if(b_ref.type==vm_number) b_num=*(double*)b_ref.scalar_ptr;
    else
    {
        if(check_numerable_string(*(std::string*)b_ref.scalar_ptr))
            b_num=trans_string_to_number(*(std::string*)b_ref.scalar_ptr);
        else
        {
            std::cout<<">> [vm] scalar_mult: "<<*(std::string*)b_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
            return -1;
        }
    }
    double result=a_num*b_num;
    int new_value_address=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_value_address).set_type(vm_number);
    nasal_vm.gc_get(new_value_address).set_number(result);
    return new_value_address;
}
int nasal_scalar::nasal_scalar_div(int a_scalar_addr,int b_scalar_addr)
{
    if(a_scalar_addr<0 || b_scalar_addr<0)
    {
        std::cout<<">> [vm] scalar_div: memory returned an invalid address"<<std::endl;
        return -1;
    }
    nasal_scalar& a_ref=nasal_vm.gc_get(a_scalar_addr);
    nasal_scalar& b_ref=nasal_vm.gc_get(b_scalar_addr);
    if((a_ref.type!=vm_number && a_ref.type!=vm_string)||(b_ref.type!=vm_number && b_ref.type!=vm_string))
    {
        std::cout<<">> [vm] scalar_div: error value type.only number and string can take part in div."<<std::endl;
        return -1;
    }
    double a_num;
    double b_num;
    if(a_ref.type==vm_number) a_num=*(double*)a_ref.scalar_ptr;
    else
    {
        if(check_numerable_string(*(std::string*)a_ref.scalar_ptr))
            a_num=trans_string_to_number(*(std::string*)a_ref.scalar_ptr);
        else
        {
            std::cout<<">> [vm] scalar_div: "<<*(std::string*)a_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
            return -1;
        }
    }
    if(b_ref.type==vm_number) b_num=*(double*)b_ref.scalar_ptr;
    else
    {
        if(check_numerable_string(*(std::string*)b_ref.scalar_ptr))
            b_num=trans_string_to_number(*(std::string*)b_ref.scalar_ptr);
        else
        {
            std::cout<<">> [vm] scalar_div: "<<*(std::string*)b_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
            return -1;
        }
    }
    double result=a_num/b_num;
    int new_value_address=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_value_address).set_type(vm_number);
    nasal_vm.gc_get(new_value_address).set_number(result);
    return new_value_address;
}
int nasal_scalar::nasal_scalar_link(int a_scalar_addr,int b_scalar_addr)
{
    if(a_scalar_addr<0 || b_scalar_addr<0)
    {
        std::cout<<">> [vm] scalar_link: memory returned an invalid address"<<std::endl;
        return -1;
    }
    nasal_scalar& a_ref=nasal_vm.gc_get(a_scalar_addr);
    nasal_scalar& b_ref=nasal_vm.gc_get(b_scalar_addr);
    if((a_ref.type!=vm_number && a_ref.type!=vm_string)||(b_ref.type!=vm_number && b_ref.type!=vm_string))
    {
        std::cout<<">> [vm] scalar_link: error value type.only number and string can take part in link."<<std::endl;
        return -1;
    }
    std::string a_str;
    std::string b_str;
    a_str=(a_ref.type==vm_number)? trans_number_to_string(*(double*)a_ref.scalar_ptr):*(std::string*)a_ref.scalar_ptr;
    b_str=(b_ref.type==vm_number)? trans_number_to_string(*(double*)b_ref.scalar_ptr):*(std::string*)b_ref.scalar_ptr;
    std::string result=a_str+b_str;
    int new_value_address=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_value_address).set_type(vm_string);
    nasal_vm.gc_get(new_value_address).set_string(result);
    return new_value_address;
}
int nasal_scalar::nasal_scalar_unary_sub(int a_scalar_addr)
{
    if(a_scalar_addr<0)
    {
        std::cout<<">> [vm] scalar_unary_sub: memory returned an invalid address"<<std::endl;
        return -1;
    }
    nasal_scalar& a_ref=nasal_vm.gc_get(a_scalar_addr);
    if(a_ref.type!=vm_number && a_ref.type!=vm_string)
    {
        std::cout<<">> [vm] scalar_unary_sub: error value type.only number and string can take part in unary sub."<<std::endl;
        return -1;
    }
    double a_num;
    if(a_ref.type==vm_number) a_num=*(double*)a_ref.scalar_ptr;
    else
    {
        if(check_numerable_string(*(std::string*)a_ref.scalar_ptr))
            a_num=trans_string_to_number(*(std::string*)a_ref.scalar_ptr);
        else
        {
            std::cout<<">> [vm] scalar_unary_sub: "<<*(std::string*)a_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
            return -1;
        }
    }
    int new_value_address=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_value_address).set_type(vm_number);
    nasal_vm.gc_get(new_value_address).set_number(-a_num);
    return new_value_address;
}
int nasal_scalar::nasal_scalar_unary_not(int a_scalar_addr)
{
    if(a_scalar_addr<0)
    {
        std::cout<<">> [vm] scalar_unary_not: memory returned an invalid address"<<std::endl;
        return -1;
    }
    nasal_scalar& a_ref=nasal_vm.gc_get(a_scalar_addr);
    int new_value_address=-1;
    if(a_ref.type==vm_nil)
    {
        new_value_address=nasal_vm.gc_alloc();
        nasal_vm.gc_get(new_value_address).set_type(vm_number);
        nasal_vm.gc_get(new_value_address).set_number(1);
    }
    else if(a_ref.type==vm_number)
    {
        double number=(double)(*(double*)(a_ref.scalar_ptr)==0);
        new_value_address=nasal_vm.gc_alloc();
        nasal_vm.gc_get(new_value_address).set_type(vm_number);
        nasal_vm.gc_get(new_value_address).set_number(number);
    }
    else if(a_ref.type==vm_string)
    {
        if(check_numerable_string(*(std::string*)a_ref.scalar_ptr))
        {
            double number=(double)(trans_string_to_number(*(std::string*)a_ref.scalar_ptr)==0);
            new_value_address=nasal_vm.gc_alloc();
            nasal_vm.gc_get(new_value_address).set_type(vm_number);
            nasal_vm.gc_get(new_value_address).set_number(number);
        }
        else
        {
            new_value_address=nasal_vm.gc_alloc();
            nasal_vm.gc_get(new_value_address).set_type(vm_number);
            nasal_vm.gc_get(new_value_address).set_number(!(*(std::string*)a_ref.scalar_ptr).length());
        }
    }
    else
        std::cout<<">> [vm] scalar_unary_not: error value type.number,string and nil can take part in unary not."<<std::endl;
    return new_value_address;
}
int nasal_scalar::nasal_scalar_cmp_equal(int a_scalar_addr,int b_scalar_addr)
{
    if(a_scalar_addr<0 || b_scalar_addr<0)
    {
        std::cout<<">> [vm] scalar_cmp_equal: memory returned an invalid address"<<std::endl;
        return -1;
    }
    if(a_scalar_addr==b_scalar_addr)
    {
        int new_value_address=nasal_vm.gc_alloc();
        nasal_vm.gc_get(new_value_address).set_type(vm_number);
        nasal_vm.gc_get(new_value_address).set_number(1);
        return new_value_address;
    }
    nasal_scalar& a_ref=nasal_vm.gc_get(a_scalar_addr);
    nasal_scalar& b_ref=nasal_vm.gc_get(b_scalar_addr);
    if(a_ref.type==vm_nil && b_ref.type==vm_nil)
    {
        int new_value_address=nasal_vm.gc_alloc();
        nasal_vm.gc_get(new_value_address).set_type(vm_number);
        nasal_vm.gc_get(new_value_address).set_number(1);
        return new_value_address;
    }
    else if((a_ref.type==vm_number || a_ref.type==vm_string) && (b_ref.type==vm_number || b_ref.type==vm_string))
    {
        double a_num;
        double b_num;
        if(a_ref.type==vm_number) a_num=*(double*)a_ref.scalar_ptr;
        else
        {
            if(check_numerable_string(*(std::string*)a_ref.scalar_ptr))
                a_num=trans_string_to_number(*(std::string*)a_ref.scalar_ptr);
            else
            {
                std::cout<<">> [vm] scalar_cmp_equal: "<<*(std::string*)a_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
                return -1;
            }
        }
        if(b_ref.type==vm_number) b_num=*(double*)b_ref.scalar_ptr;
        else
        {
            if(check_numerable_string(*(std::string*)b_ref.scalar_ptr))
                b_num=trans_string_to_number(*(std::string*)b_ref.scalar_ptr);
            else
            {
                std::cout<<">> [vm] scalar_cmp_equal: "<<*(std::string*)b_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
                return -1;
            }
        }
        int new_value_address=nasal_vm.gc_alloc();
        nasal_vm.gc_get(new_value_address).set_type(vm_number);
        nasal_vm.gc_get(new_value_address).set_number((double)(a_num==b_num));
        return new_value_address;
    }
    else
    {
        int new_value_address=nasal_vm.gc_alloc();
        nasal_vm.gc_get(new_value_address).set_type(vm_number);
        nasal_vm.gc_get(new_value_address).set_number(0);
        return new_value_address;
    }
    return -1;
}
int nasal_scalar::nasal_scalar_cmp_not_equal(int a_scalar_addr,int b_scalar_addr)
{
    if(a_scalar_addr<0 || b_scalar_addr<0)
    {
        std::cout<<">> [vm] scalar_cmp_not_equal: memory returned an invalid address"<<std::endl;
        return -1;
    }
    if(a_scalar_addr==b_scalar_addr)
    {
        int new_value_address=nasal_vm.gc_alloc();
        nasal_vm.gc_get(new_value_address).set_type(vm_number);
        nasal_vm.gc_get(new_value_address).set_number(0);
        return new_value_address;
    }
    nasal_scalar& a_ref=nasal_vm.gc_get(a_scalar_addr);
    nasal_scalar& b_ref=nasal_vm.gc_get(b_scalar_addr);
    if(a_ref.type==vm_nil && b_ref.type==vm_nil)
    {
        int new_value_address=nasal_vm.gc_alloc();
        nasal_vm.gc_get(new_value_address).set_type(vm_number);
        nasal_vm.gc_get(new_value_address).set_number(0);
        return new_value_address;
    }
    else if((a_ref.type==vm_number || a_ref.type==vm_string) && (b_ref.type==vm_number || b_ref.type==vm_string))
    {
        double a_num;
        double b_num;
        if(a_ref.type==vm_number) a_num=*(double*)a_ref.scalar_ptr;
        else
        {
            if(check_numerable_string(*(std::string*)a_ref.scalar_ptr))
                a_num=trans_string_to_number(*(std::string*)a_ref.scalar_ptr);
            else
            {
                std::cout<<">> [vm] scalar_cmp_equal: "<<*(std::string*)a_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
                return -1;
            }
        }
        if(b_ref.type==vm_number) b_num=*(double*)b_ref.scalar_ptr;
        else
        {
            if(check_numerable_string(*(std::string*)b_ref.scalar_ptr))
                b_num=trans_string_to_number(*(std::string*)b_ref.scalar_ptr);
            else
            {
                std::cout<<">> [vm] scalar_cmp_equal: "<<*(std::string*)b_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
                return -1;
            }
        }
        int new_value_address=nasal_vm.gc_alloc();
        nasal_vm.gc_get(new_value_address).set_type(vm_number);
        nasal_vm.gc_get(new_value_address).set_number((double)(a_num!=b_num));
        return new_value_address;
    }
    else
    {
        int new_value_address=nasal_vm.gc_alloc();
        nasal_vm.gc_get(new_value_address).set_type(vm_number);
        nasal_vm.gc_get(new_value_address).set_number(1);
        return new_value_address;
    }
    return -1;
}
int nasal_scalar::nasal_scalar_cmp_less(int a_scalar_addr,int b_scalar_addr)
{
    if(a_scalar_addr<0 || b_scalar_addr<0)
    {
        std::cout<<">> [vm] scalar_cmp_less: memory returned an invalid address"<<std::endl;
        return -1;
    }
    nasal_scalar& a_ref=nasal_vm.gc_get(a_scalar_addr);
    nasal_scalar& b_ref=nasal_vm.gc_get(b_scalar_addr);
    if((a_ref.type!=vm_number && a_ref.type!=vm_string)||(b_ref.type!=vm_number && b_ref.type!=vm_string))
    {
        std::cout<<">> [vm] scalar_cmp_less: error value type.only number and string can take part in cmp_less."<<std::endl;
        return -1;
    }
    double a_num;
    double b_num;
    if(a_ref.type==vm_number) a_num=*(double*)a_ref.scalar_ptr;
    else
    {
        if(check_numerable_string(*(std::string*)a_ref.scalar_ptr))
            a_num=trans_string_to_number(*(std::string*)a_ref.scalar_ptr);
        else
        {
            std::cout<<">> [vm] scalar_cmp_less: "<<*(std::string*)a_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
            return -1;
        }
    }
    if(b_ref.type==vm_number) b_num=*(double*)b_ref.scalar_ptr;
    else
    {
        if(check_numerable_string(*(std::string*)b_ref.scalar_ptr))
            b_num=trans_string_to_number(*(std::string*)b_ref.scalar_ptr);
        else
        {
            std::cout<<">> [vm] scalar_cmp_less: "<<*(std::string*)b_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
            return -1;
        }
    }
    double result=(double)(a_num<b_num);
    int new_value_address=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_value_address).set_type(vm_number);
    nasal_vm.gc_get(new_value_address).set_number(result);
    return new_value_address;
}
int nasal_scalar::nasal_scalar_cmp_greater(int a_scalar_addr,int b_scalar_addr)
{
    if(a_scalar_addr<0 || b_scalar_addr<0)
    {
        std::cout<<">> [vm] scalar_cmp_greater: memory returned an invalid address"<<std::endl;
        return -1;
    }
    nasal_scalar& a_ref=nasal_vm.gc_get(a_scalar_addr);
    nasal_scalar& b_ref=nasal_vm.gc_get(b_scalar_addr);
    if((a_ref.type!=vm_number && a_ref.type!=vm_string)||(b_ref.type!=vm_number && b_ref.type!=vm_string))
    {
        std::cout<<">> [vm] scalar_cmp_greater: error value type.only number and string can take part in cmp_greater."<<std::endl;
        return -1;
    }
    double a_num;
    double b_num;
    if(a_ref.type==vm_number) a_num=*(double*)a_ref.scalar_ptr;
    else
    {
        if(check_numerable_string(*(std::string*)a_ref.scalar_ptr))
            a_num=trans_string_to_number(*(std::string*)a_ref.scalar_ptr);
        else
        {
            std::cout<<">> [vm] scalar_cmp_greater: "<<*(std::string*)a_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
            return -1;
        }
    }
    if(b_ref.type==vm_number) b_num=*(double*)b_ref.scalar_ptr;
    else
    {
        if(check_numerable_string(*(std::string*)b_ref.scalar_ptr))
            b_num=trans_string_to_number(*(std::string*)b_ref.scalar_ptr);
        else
        {
            std::cout<<">> [vm] scalar_cmp_greater: "<<*(std::string*)b_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
            return -1;
        }
    }
    double result=(double)(a_num>b_num);
    int new_value_address=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_value_address).set_type(vm_number);
    nasal_vm.gc_get(new_value_address).set_number(result);
    return new_value_address;
}
int nasal_scalar::nasal_scalar_cmp_less_or_equal(int a_scalar_addr,int b_scalar_addr)
{
    if(a_scalar_addr<0 || b_scalar_addr<0)
    {
        std::cout<<">> [vm] scalar_cmp_lequal: memory returned an invalid address"<<std::endl;
        return -1;
    }
    nasal_scalar& a_ref=nasal_vm.gc_get(a_scalar_addr);
    nasal_scalar& b_ref=nasal_vm.gc_get(b_scalar_addr);
    if((a_ref.type!=vm_number && a_ref.type!=vm_string)||(b_ref.type!=vm_number && b_ref.type!=vm_string))
    {
        std::cout<<">> [vm] scalar_cmp_lequal: error value type.only number and string can take part in cmp_less_or_equal."<<std::endl;
        return -1;
    }
    double a_num;
    double b_num;
    if(a_ref.type==vm_number) a_num=*(double*)a_ref.scalar_ptr;
    else
    {
        if(check_numerable_string(*(std::string*)a_ref.scalar_ptr))
            a_num=trans_string_to_number(*(std::string*)a_ref.scalar_ptr);
        else
        {
            std::cout<<">> [vm] scalar_cmp_lequal: "<<*(std::string*)a_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
            return -1;
        }
    }
    if(b_ref.type==vm_number) b_num=*(double*)b_ref.scalar_ptr;
    else
    {
        if(check_numerable_string(*(std::string*)b_ref.scalar_ptr))
            b_num=trans_string_to_number(*(std::string*)b_ref.scalar_ptr);
        else
        {
            std::cout<<">> [vm] scalar_cmp_lequal: "<<*(std::string*)b_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
            return -1;
        }
    }
    double result=(double)(a_num<=b_num);
    int new_value_address=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_value_address).set_type(vm_number);
    nasal_vm.gc_get(new_value_address).set_number(result);
    return new_value_address;
}
int nasal_scalar::nasal_scalar_cmp_greater_or_equal(int a_scalar_addr,int b_scalar_addr)
{
    if(a_scalar_addr<0 || b_scalar_addr<0)
    {
        std::cout<<">> [vm] scalar_cmp_gequal: memory returned an invalid address"<<std::endl;
        return -1;
    }
    nasal_scalar& a_ref=nasal_vm.gc_get(a_scalar_addr);
    nasal_scalar& b_ref=nasal_vm.gc_get(b_scalar_addr);
    if((a_ref.type!=vm_number && a_ref.type!=vm_string)||(b_ref.type!=vm_number && b_ref.type!=vm_string))
    {
        std::cout<<">> [vm] scalar_cmp_gequal: error value type.only number and string can take part in cmp_greater_or_equal."<<std::endl;
        return -1;
    }
    double a_num;
    double b_num;
    if(a_ref.type==vm_number) a_num=*(double*)a_ref.scalar_ptr;
    else
    {
        if(check_numerable_string(*(std::string*)a_ref.scalar_ptr))
            a_num=trans_string_to_number(*(std::string*)a_ref.scalar_ptr);
        else
        {
            std::cout<<">> [vm] scalar_cmp_gequal: "<<*(std::string*)a_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
            return -1;
        }
    }
    if(b_ref.type==vm_number) b_num=*(double*)b_ref.scalar_ptr;
    else
    {
        if(check_numerable_string(*(std::string*)b_ref.scalar_ptr))
            b_num=trans_string_to_number(*(std::string*)b_ref.scalar_ptr);
        else
        {
            std::cout<<">> [vm] scalar_cmp_gequal: "<<*(std::string*)b_ref.scalar_ptr<<" is not a numerable string."<<std::endl;
            return -1;
        }
    }
    double result=(double)(a_num>=b_num);
    int new_value_address=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_value_address).set_type(vm_number);
    nasal_vm.gc_get(new_value_address).set_number(result);
    return new_value_address;
}

/*functions of nasal_virtual_machine*/
nasal_virtual_machine::~nasal_virtual_machine()
{
    int gc_mem_size=garbage_collector_memory.size();
    int mm_mem_size=memory_manager_memory.size();
    for(int i=0;i<gc_mem_size;++i)
        delete []garbage_collector_memory[i];
    for(int i=0;i<mm_mem_size;++i)
        delete []memory_manager_memory[i];
    garbage_collector_memory.clear();
    memory_manager_memory.clear();
    return;
}
int nasal_virtual_machine::gc_alloc()
{
    if(garbage_collector_free_space.empty())
    {
        gc_unit* new_block=new gc_unit[256];
        garbage_collector_memory.push_back(new_block);
        int mem_size=garbage_collector_memory.size();
        for(int i=((mem_size-1)<<8);i<(mem_size<<8);++i)
            garbage_collector_free_space.push(i);
    }
    int ret=garbage_collector_free_space.front();
    garbage_collector_memory[ret>>8][ret&0xff].collected=false;
    garbage_collector_memory[ret>>8][ret&0xff].ref_cnt=1;
    garbage_collector_free_space.pop();
    return ret;
}
nasal_scalar& nasal_virtual_machine::gc_get(int value_address)
{
    int blk_num=(value_address>>8);
    int blk_plc=(value_address&0xff);
    if(0<=value_address && value_address<(garbage_collector_memory.size()<<8) && !garbage_collector_memory[blk_num][blk_plc].collected)
        return garbage_collector_memory[blk_num][blk_plc].elem;
    else
    {
        std::cout<<">> [vm] gc_get:unexpected memory \'"<<value_address<<"\'."<<std::endl;
        return error_returned_value;
    }
}
int nasal_virtual_machine::add_reference(int value_address)
{
    int blk_num=(value_address>>8);
    int blk_plc=(value_address&0xff);
    if(0<=value_address && value_address<(garbage_collector_memory.size()<<8) && !garbage_collector_memory[blk_num][blk_plc].collected)
        ++garbage_collector_memory[blk_num][blk_plc].ref_cnt;
    else
    {
        std::cout<<">> [vm] gc_add_ref:unexpected memory \'"<<value_address<<"\'."<<std::endl;
        return 0;
    }
    return 1;
}
int nasal_virtual_machine::del_reference(int value_address)
{
    int blk_num=(value_address>>8);
    int blk_plc=(value_address&0xff);
    if(0<=value_address && value_address<(garbage_collector_memory.size()<<8) && !garbage_collector_memory[blk_num][blk_plc].collected)
        --garbage_collector_memory[blk_num][blk_plc].ref_cnt;
    else
    {
        std::cout<<">> [vm] gc_del_ref:unexpected memory \'"<<value_address<<"\'."<<std::endl;
        return 0;
    }
    if(!garbage_collector_memory[blk_num][blk_plc].ref_cnt)
    {
        garbage_collector_memory[blk_num][blk_plc].collected=true;
        garbage_collector_memory[blk_num][blk_plc].elem.clear();
        garbage_collector_free_space.push(value_address);
    }
    return 1;
}
int nasal_virtual_machine::mem_alloc()
{
    if(memory_manager_free_space.empty())
    {
        int* new_block=new int[256];
        memory_manager_memory.push_back(new_block);
        int mem_size=memory_manager_memory.size();
        for(int i=((mem_size-1)<<8);i<(mem_size<<8);++i)
            memory_manager_free_space.push(i);
    }
    int ret=memory_manager_free_space.front();
    memory_manager_free_space.pop();
    return ret;
}
int nasal_virtual_machine::mem_free(int memory_address)
{
    // mem_free has helped scalar to delete the reference
    // so don't need to delete reference again
    if(0<=memory_address && memory_address<(memory_manager_memory.size()<<8))
    {
        this->del_reference(memory_manager_memory[memory_address>>8][memory_address&0xff]);
        memory_manager_free_space.push(memory_address);
    }
    else
    {
        std::cout<<">> [vm] mem_free:unexpected memory \'"<<memory_address<<"\'."<<std::endl;
        return 0;
    }
    return 1;
}
int nasal_virtual_machine::mem_change(int memory_address,int value_address)
{
    // this progress is used to change a memory space's value address
    // be careful! this process doesn't check if this mem_space is in use.
    if(0<=memory_address && memory_address<(memory_manager_memory.size()<<8))
    {
        this->del_reference(memory_manager_memory[memory_address>>8][memory_address&0xff]);
        memory_manager_memory[memory_address>>8][memory_address&0xff]=value_address;
    }
    else
    {
        std::cout<<">> [vm] mem_store:unexpected memory \'"<<memory_address<<"\'."<<std::endl;
        return 0;
    }
    return 1;
}
int nasal_virtual_machine::mem_init(int memory_address,int value_address)
{
    // this progress is used to init a memory space
    // be careful! this process doesn't check if this mem_space is in use.
    if(0<=memory_address && memory_address<(memory_manager_memory.size()<<8))
        memory_manager_memory[memory_address>>8][memory_address&0xff]=value_address;
    else
    {
        std::cout<<">> [vm] mem_store:unexpected memory \'"<<memory_address<<"\'."<<std::endl;
        return 0;
    }
    return 1;
}
int nasal_virtual_machine::mem_get(int memory_address)
{
    int ret=-1;
    // be careful! this process doesn't check if this mem_space is in use.
    if(0<=memory_address && memory_address<(memory_manager_memory.size()<<8))
        ret=memory_manager_memory[memory_address>>8][memory_address&0xff];
    else
        std::cout<<">> [vm] mem_get:unexpected memory \'"<<memory_address<<"\'."<<std::endl;
    return ret;
}
#endif