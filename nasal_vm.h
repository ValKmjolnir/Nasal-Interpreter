#ifndef __NASAL_VM_H__
#define __NASAL_VM_H__
#define STACK_MAX_DEPTH (65536<<4)

class nasal_vm
{
private:
    std::vector<std::string> str_table;    // symbols used in process
    bool                     loop_mark;    // when mark is false,break the main loop
    int                      pc;           // program counter
    int                      me_index;     // this is the index of "me" in str_table
    std::vector<opcode>      exec_code;    // byte codes store here
    std::stack<nasal_val**>  addr_stack;   // stack for mem_call
    std::stack<int>          call_stack;   // ptr stack stores address for function to return
    std::stack<int>          counter;      // iterator stack for forindex/foreach
    
    void die(std::string);
    bool check_condition(nasal_val*);
    void opr_nop();
    void opr_load();
    void opr_loadg();
    void opr_loadl();
    void opr_pnum();
    void opr_pone();
    void opr_pzero();
    void opr_pnil();
    void opr_pstr();
    void opr_newv();
    void opr_newh();
    void opr_newf();
    void opr_vapp();
    void opr_happ();
    void opr_para();
    void opr_defpara();
    void opr_dynpara();
    void opr_unot();
    void opr_usub();
    void opr_add();
    void opr_sub();
    void opr_mul();
    void opr_div();
    void opr_lnk();
    void opr_addeq();
    void opr_subeq();
    void opr_muleq();
    void opr_diveq();
    void opr_lnkeq();
    void opr_meq();
    void opr_eq();
    void opr_neq();
    void opr_less();
    void opr_leq();
    void opr_grt();
    void opr_geq();
    void opr_pop();
    void opr_jmp();
    void opr_jt();
    void opr_jf();
    void opr_counter();
    void opr_cntpop();
    void opr_findex();
    void opr_feach();
    void opr_call();
    void opr_callv();
    void opr_callvi();
    void opr_callh();
    void opr_callf();
    void opr_callb();
    void opr_slcbegin();
    void opr_slcend();
    void opr_slc();
    void opr_slc2();
    void opr_mcall();
    void opr_mcallv();
    void opr_mcallh();
    void opr_ret();
public:
    void init(
        std::vector<std::string>&,
        std::vector<double>&,
        std::vector<opcode>&);
    void clear();
    void run();
};

void nasal_vm::init(
    std::vector<std::string>& strs,
    std::vector<double>& nums,
    std::vector<opcode>& exec)
{
    gc_init();
    str_table=strs; // get constant strings & symbols
    exec_code=exec; // get bytecodes

    val_stack=new nasal_val*[STACK_MAX_DEPTH]; // init runtime stack
    stack_top=val_stack; // set stack_top to val_stack

    zero_addr=new nasal_val(vm_num); // init constant 0
    zero_addr->ptr.num=0;
    memory.push_back(zero_addr);

    one_addr=new nasal_val(vm_num); // init constant 1
    one_addr->ptr.num=1;
    memory.push_back(one_addr);

    nil_addr=new nasal_val(vm_nil); // init nil
    memory.push_back(nil_addr);

    *val_stack=nil_addr; // the first space will not store any values,but gc checks

    num_addrs.clear(); // init constant numbers
    for(int i=0;i<nums.size();++i)
    {
        nasal_val* tmp=new nasal_val(vm_num);
        tmp->ptr.num=nums[i];
        num_addrs.push_back(tmp);
    }

    builtin_use_str_table.clear(); // create map that builtin functions use
    for(int i=0;i<str_table.size();++i)
        builtin_use_str_table[str_table[i]]=i;
    me_index=builtin_use_str_table.count("me")?builtin_use_str_table["me"]:-1; // get symbol index of 'me'

    loop_mark=true; // set loop mark to true

    global=new nasal_val(vm_scop); // init global symbol table
    memory.push_back(global);
    return;
}
void nasal_vm::clear()
{
    gc_clear();

    global=nullptr; // set global to null

    delete []val_stack; // free runtime stack
    while(!addr_stack.empty())
        addr_stack.pop();
    local.clear();
    slice_stack.clear();
    while(!call_stack.empty())
        call_stack.pop();
    while(!counter.empty())
        counter.pop();
    str_table.clear();
    exec_code.clear();
    return;
}
void nasal_vm::die(std::string str)
{
    printf(">> [vm] 0x%.8x: %s\n",pc,str.data());
    loop_mark=false;
    return;
}
bool nasal_vm::check_condition(nasal_val* value_addr)
{
    int type=value_addr->type;
    if(type==vm_num)
        return value_addr->ptr.num;
    else if(type==vm_str)
    {
        std::string& str=*value_addr->ptr.str;
        double number=trans_string_to_number(str);
        if(std::isnan(number))
            return str.length();
        return number;
    }
    return false;
}
void nasal_vm::opr_nop()
{
    loop_mark=false;
    return;
}
void nasal_vm::opr_load()
{
    nasal_val* val=*stack_top--;
    (local.empty()?global:local.back())->ptr.scop->elems[exec_code[pc].num]=val;
    return;
}
void nasal_vm::opr_loadg()
{
    global->ptr.scop->elems[exec_code[pc].num]=*stack_top--;
    return;
}
void nasal_vm::opr_loadl()
{
    local.back()->ptr.scop->elems[exec_code[pc].num]=*stack_top--;
    return;
}
void nasal_vm::opr_pnum()
{
    nasal_val* val=num_addrs[exec_code[pc].num];
    *(++stack_top)=val;
    return;
}
void nasal_vm::opr_pone()
{
    *(++stack_top)=one_addr;
    return;
}
void nasal_vm::opr_pzero()
{
    *(++stack_top)=zero_addr;
    return;
}
void nasal_vm::opr_pnil()
{
    *(++stack_top)=nil_addr;
    return;
}
void nasal_vm::opr_pstr()
{
    nasal_val* val=gc_alloc(vm_str);
    *val->ptr.str=str_table[exec_code[pc].num];
    *(++stack_top)=val;
    return;
}
void nasal_vm::opr_newv()
{
    nasal_val* vec=gc_alloc(vm_vec);
    *(++stack_top)=vec;
    return;
}
void nasal_vm::opr_newh()
{
    nasal_val* hash=gc_alloc(vm_hash);
    *(++stack_top)=hash;
    return;
}
void nasal_vm::opr_newf()
{
    nasal_val* val=gc_alloc(vm_func);
    val->ptr.func->entry=exec_code[pc].num;
    if(!local.empty())
        val->ptr.func->closure=local.back()->ptr.scop->elems;
    *(++stack_top)=val;
    return;
}
void nasal_vm::opr_vapp()
{
    nasal_val* val=*stack_top--;
    (*stack_top)->ptr.vec->elems.push_back(val);
    return;
}
void nasal_vm::opr_happ()
{
    nasal_val* val=*stack_top--;
    (*stack_top)->ptr.hash->elems[str_table[exec_code[pc].num]]=val;
    return;
}
void nasal_vm::opr_para()
{
    (*stack_top)->ptr.func->para.push_back(exec_code[pc].num);
    (*stack_top)->ptr.func->default_para.push_back(nullptr);
    return;
}
void nasal_vm::opr_defpara()
{
    nasal_val* def_val=*stack_top--;
    (*stack_top)->ptr.func->para.push_back(exec_code[pc].num);
    (*stack_top)->ptr.func->default_para.push_back(def_val);
    return;
}
void nasal_vm::opr_dynpara()
{
    (*stack_top)->ptr.func->dynpara=exec_code[pc].num;
    return;
}
void nasal_vm::opr_unot()
{
    nasal_val* val=*stack_top;
    nasal_val* new_val=nullptr;
    int type=val->type;
    if(type==vm_nil)
        new_val=one_addr;
    else if(type==vm_num)
        new_val=val->ptr.num?zero_addr:one_addr;
    else if(type==vm_str)
    {
        double number=trans_string_to_number(*val->ptr.str);
        if(std::isnan(number))
            new_val=val->ptr.str->length()?zero_addr:one_addr;
        else
            new_val=number?zero_addr:one_addr;
    }
    else
        die("unot: incorrect value type");
    *stack_top=new_val;
    return;
}
void nasal_vm::opr_usub()
{
    nasal_val* val=*stack_top;
    nasal_val* new_val=gc_alloc(vm_num);
    new_val->ptr.num=-val->to_number();
    *stack_top=new_val;
    return;
}
void nasal_vm::opr_add()
{
    nasal_val* new_val=gc_alloc(vm_num);
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    new_val->ptr.num=val1->to_number()+val2->to_number();
    *stack_top=new_val;
    return;
}
void nasal_vm::opr_sub()
{
    nasal_val* new_val=gc_alloc(vm_num);
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    new_val->ptr.num=val1->to_number()-val2->to_number();
    *stack_top=new_val;
    return;
}
void nasal_vm::opr_mul()
{
    nasal_val* new_val=gc_alloc(vm_num);
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    new_val->ptr.num=val1->to_number()*val2->to_number();
    *stack_top=new_val;
    return;
}
void nasal_vm::opr_div()
{
    nasal_val* new_val=gc_alloc(vm_num);
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    new_val->ptr.num=val1->to_number()/val2->to_number();
    *stack_top=new_val;
    return;
}
void nasal_vm::opr_lnk()
{
    nasal_val* new_val=gc_alloc(vm_str);
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    *new_val->ptr.str=val1->to_string()+val2->to_string();
    *stack_top=new_val;
    return;
}
void nasal_vm::opr_addeq()
{
    nasal_val** mem_addr=addr_stack.top();
    addr_stack.pop();
    nasal_val* val2=*stack_top;
    nasal_val* val1=*mem_addr;
    nasal_val* new_val=gc_alloc(vm_num);
    new_val->ptr.num=val1->to_number()+val2->to_number();
    *stack_top=new_val;
    *mem_addr=new_val;
    return;
}
void nasal_vm::opr_subeq()
{
    nasal_val** mem_addr=addr_stack.top();
    addr_stack.pop();
    nasal_val* val2=*stack_top;
    nasal_val* val1=*mem_addr;
    nasal_val* new_val=gc_alloc(vm_num);
    new_val->ptr.num=val1->to_number()-val2->to_number();
    *stack_top=new_val;
    *mem_addr=new_val;
    return;
}
void nasal_vm::opr_muleq()
{
    nasal_val** mem_addr=addr_stack.top();
    addr_stack.pop();
    nasal_val* val2=*stack_top;
    nasal_val* val1=*mem_addr;
    nasal_val* new_val=gc_alloc(vm_num);
    new_val->ptr.num=val1->to_number()*val2->to_number();
    *stack_top=new_val;
    *mem_addr=new_val;
    return;
}
void nasal_vm::opr_diveq()
{
    nasal_val** mem_addr=addr_stack.top();
    addr_stack.pop();
    nasal_val* val2=*stack_top;
    nasal_val* val1=*mem_addr;
    nasal_val* new_val=gc_alloc(vm_num);
    new_val->ptr.num=val1->to_number()/val2->to_number();
    *stack_top=new_val;
    *mem_addr=new_val;
    return;
}
void nasal_vm::opr_lnkeq()
{
    nasal_val** mem_addr=addr_stack.top();
    addr_stack.pop();
    nasal_val* val2=*stack_top;
    nasal_val* val1=*mem_addr;
    nasal_val* new_val=gc_alloc(vm_str);
    *new_val->ptr.str=val1->to_string()+val2->to_string();
    *stack_top=new_val;
    *mem_addr=new_val;
    return;
}
void nasal_vm::opr_meq()
{
    nasal_val** mem_addr=addr_stack.top();
    addr_stack.pop();
    nasal_val* val=*stack_top;
    *mem_addr=val;
    return;
}
void nasal_vm::opr_eq()
{
    nasal_val* new_val=gc_alloc(vm_num);
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    int a_type=val1->type;
    int b_type=val2->type;
    if(a_type==vm_nil && b_type==vm_nil)
        *stack_top=one_addr;
    else if(a_type==vm_str && b_type==vm_str)
    {
        new_val->ptr.num=(*val1->ptr.str==*val2->ptr.str);
        *stack_top=new_val;
    }
    else if(a_type==vm_num || b_type==vm_num)
    {
        new_val->ptr.num=(val1->to_number()==val2->to_number());
        *stack_top=new_val;
    }
    else
    {
        new_val->ptr.num=(val1==val2);
        *stack_top=new_val;
    }
    return;
}
void nasal_vm::opr_neq()
{
    nasal_val* new_val=gc_alloc(vm_num);
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    int a_type=val1->type;
    int b_type=val2->type;
    if(a_type==vm_nil && b_type==vm_nil)
        *stack_top=zero_addr;
    else if(a_type==vm_str && b_type==vm_str)
    {
        new_val->ptr.num=(*val1->ptr.str!=*val2->ptr.str);
        *stack_top=new_val;
    }
    else if(a_type==vm_num || b_type==vm_num)
    {
        new_val->ptr.num=(val1->to_number()!=val2->to_number());
        *stack_top=new_val;
    }
    else
    {
        new_val->ptr.num=(val1!=val2);
        *stack_top=new_val;
    }
    return;
}
void nasal_vm::opr_less()
{
    nasal_val* new_val=gc_alloc(vm_num);
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    int a_type=val1->type;
    int b_type=val2->type;
    if(a_type==vm_str && b_type==vm_str)
        new_val->ptr.num=(*val1->ptr.str<*val2->ptr.str);
    else
        new_val->ptr.num=(val1->to_number()<val2->to_number());
    *stack_top=new_val;
    return;
}
void nasal_vm::opr_leq()
{
    nasal_val* new_val=gc_alloc(vm_num);
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    int a_type=val1->type;
    int b_type=val2->type;
    if(a_type==vm_str && b_type==vm_str)
        new_val->ptr.num=(*val1->ptr.str<=*val2->ptr.str);
    else
        new_val->ptr.num=(val1->to_number()<=val2->to_number());
    *stack_top=new_val;
    return;
}
void nasal_vm::opr_grt()
{
    nasal_val* new_val=gc_alloc(vm_num);
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    int a_type=val1->type;
    int b_type=val2->type;
    if(a_type==vm_str && b_type==vm_str) 
        new_val->ptr.num=(*val1->ptr.str>*val2->ptr.str);
    else
        new_val->ptr.num=(val1->to_number()>val2->to_number());
    *stack_top=new_val;
    return;
}
void nasal_vm::opr_geq()
{
    nasal_val* new_val=gc_alloc(vm_num);
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    int a_type=val1->type;
    int b_type=val2->type;
    if(a_type==vm_str && b_type==vm_str)
        new_val->ptr.num=(*val1->ptr.str>=*val2->ptr.str);
    else
        new_val->ptr.num=(val1->to_number()>=val2->to_number());
    *stack_top=new_val;
    return;
}
void nasal_vm::opr_pop()
{
    --stack_top;
    return;
}
void nasal_vm::opr_jmp()
{
    pc=exec_code[pc].num-1;
    return;
}
void nasal_vm::opr_jt()
{
    if(check_condition(*stack_top))
        pc=exec_code[pc].num-1;
    return;
}
void nasal_vm::opr_jf()
{
    if(!check_condition(*stack_top))
        pc=exec_code[pc].num-1;
    --stack_top;
    return;
}
void nasal_vm::opr_counter()
{
    if((*stack_top)->type!=vm_vec)
    {
        die("cnt: must use vector in forindex/foreach");
        return;
    }
    counter.push(-1);
    return;
}
void nasal_vm::opr_cntpop()
{
    counter.pop();
    return;
}
void nasal_vm::opr_findex()
{
    if(++counter.top()>=(*stack_top)->ptr.vec->elems.size())
    {
        pc=exec_code[pc].num-1;
        return;
    }
    nasal_val* res=gc_alloc(vm_num);
    res->ptr.num=counter.top();
    *(++stack_top)=res;
    return;
}
void nasal_vm::opr_feach()
{
    std::vector<nasal_val*>& ref=(*stack_top)->ptr.vec->elems;
    ++counter.top();
    if(counter.top()>=ref.size())
    {
        pc=exec_code[pc].num-1;
        return;
    }
    nasal_val* res=ref[counter.top()];
    *(++stack_top)=res;
    return;
}
void nasal_vm::opr_call()
{
    nasal_val* val=nullptr;
    int name_index=exec_code[pc].num;
    if(!local.empty())
        val=local.back()->ptr.scop->get_val(name_index);
    if(val)
    {
        *(++stack_top)=val;
        return;
    }
    val=global->ptr.scop->get_val(name_index);
    if(val)
    {
        *(++stack_top)=val;
        return;
    }
    die("call: cannot find symbol named \""+str_table[name_index]+"\"");
    return;
}
void nasal_vm::opr_callv()
{
    nasal_val* val=*stack_top--;
    nasal_val* vec_addr=*stack_top;
    int type=vec_addr->type;
    if(type==vm_vec)
    {
        int num=val->to_number();
        nasal_val* res=vec_addr->ptr.vec->get_val(num);
        if(res)
            *stack_top=res;
        else
        {
            die("callv: index out of range");
            return;
        }
    }
    else if(type==vm_hash)
    {
        if(val->type!=vm_str)
        {
            die("callv: must use string as the key");
            return;
        }
        nasal_val* res=vec_addr->ptr.hash->get_val(*val->ptr.str);
        if(!res)
        {
            die("callv: cannot find member \""+*val->ptr.str+"\" of this hash");
            return;
        }
        if(res->type==vm_func)
            res->ptr.func->closure[me_index]=val;
        *stack_top=res;
    }
    else if(type==vm_str)
    {
        std::string& str=*vec_addr->ptr.str;
        int num=val->to_number();
        int str_size=str.length();
        if(num<-str_size || num>=str_size)
        {
            die("callv: index out of range");
            return;
        }
        nasal_val* res=gc_alloc(vm_num);
        res->ptr.num=(str[num>=0? num:num+str_size]);
        *stack_top=res;
    }
    else
        die("callv: must call a vector/hash/string");
    return;
}
void nasal_vm::opr_callvi()
{
    nasal_val* val=*stack_top;
    if(val->type!=vm_vec)
    {
        die("callvi: multi-definition/multi-assignment must use a vector");
        return;
    }
    // cannot use operator[],because this may cause overflow
    nasal_val* res=val->ptr.vec->get_val(exec_code[pc].num);
    if(!res)
    {
        die("callvi: index out of range");
        return;
    }
    *(++stack_top)=res;
    return;
}
void nasal_vm::opr_callh()
{
    nasal_val* val=*stack_top;
    if(val->type!=vm_hash)
    {
        die("callh: must call a hash");
        return;
    }
    nasal_val* res=val->ptr.hash->get_val(str_table[exec_code[pc].num]);
    if(!res)
    {
        die("callh: hash member \""+str_table[exec_code[pc].num]+"\" does not exist");
        return;
    }
    if(res->type==vm_func)
        res->ptr.func->closure[me_index]=val;
    *stack_top=res;
    return;
}
void nasal_vm::opr_callf()
{
    nasal_val* closure=gc_alloc(vm_scop);
    nasal_val* para_addr=*stack_top;
    nasal_val* func_addr=*(stack_top-1);
    if(func_addr->type!=vm_func)
    {
        die("callf: called a value that is not a function");
        return;
    }
    nasal_func& ref_func=*func_addr->ptr.func;
    nasal_scop& ref_closure=*closure->ptr.scop;
    ref_closure.elems=ref_func.closure;
    local.push_back(closure);
    if(para_addr->type==vm_vec)
    {
        std::vector<nasal_val*>& ref_vec=para_addr->ptr.vec->elems;
        std::vector<int>& ref_para=ref_func.para;
        std::vector<nasal_val*>& ref_default=ref_func.default_para;
        int i=0;
        for(;i<ref_para.size();++i)
        {
            if(i>=ref_vec.size())
            {
                if(!ref_default[i])
                {
                    die("callf: lack argument(s)");
                    return;
                }
                ref_closure.elems[ref_para[i]]=ref_default[i];
            }
            else
            {
                nasal_val* tmp=ref_vec[i];
                ref_closure.elems[ref_para[i]]=tmp;
            }
        }
        if(ref_func.dynpara>=0)
        {
            nasal_val* vec_addr=gc_alloc(vm_vec);
            for(;i<ref_vec.size();++i)
            {
                nasal_val* tmp=ref_vec[i];
                vec_addr->ptr.vec->elems.push_back(tmp);
            }
            ref_closure.elems[ref_func.dynpara]=vec_addr;
        }
    }
    else
    {
        nasal_hash& ref_hash=*para_addr->ptr.hash;
        std::vector<int>& ref_para=ref_func.para;
        std::vector<nasal_val*>& ref_default=ref_func.default_para;
        if(ref_func.dynpara>=0)
        {
            die("callf: special call cannot use dynamic parameter");
            return;
        }
        for(int i=0;i<ref_para.size();++i)
        {
            nasal_val* tmp=ref_hash.get_special_para(str_table[ref_para[i]]);
            if(!tmp)
                tmp=ref_default[i];
            if(!tmp)
            {
                die("callf: lack argument(s)");
                return;
            }
            ref_closure.elems[ref_para[i]]=tmp;
        }
    }
    --stack_top;
    call_stack.push(pc);
    pc=ref_func.entry-1;
    return;
}
void nasal_vm::opr_callb()
{
    nasal_val* res=(*builtin_func[exec_code[pc].num].func)(local.back());
    *(++stack_top)=res;
    loop_mark=res;
    return;
}
void nasal_vm::opr_slcbegin()
{
    slice_stack.push_back(gc_alloc(vm_vec));
    if((*stack_top)->type!=vm_vec)
        die("slcbegin: must slice a vector");
    return;
}
void nasal_vm::opr_slcend()
{
    *stack_top=slice_stack.back();
    slice_stack.pop_back();
    return;
}
void nasal_vm::opr_slc()
{
    nasal_val* val=*stack_top--;
    double num;
    switch(val->type)
    {
        case vm_num:num=val->ptr.num;break;
        case vm_str:num=trans_string_to_number(*val->ptr.str);break;
        default:die("slc: error value type");break;
    }
    nasal_val* res=(*stack_top)->ptr.vec->get_val((int)num);
    if(res)
        slice_stack.back()->ptr.vec->elems.push_back(res);
    else
        die("slc: index out of range");
    return;
}
void nasal_vm::opr_slc2()
{
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top--;
    std::vector<nasal_val*>& ref=(*stack_top)->ptr.vec->elems;
    std::vector<nasal_val*>& aim=slice_stack.back()->ptr.vec->elems;

    int type1=val1->type,type2=val2->type;
    int num1=val1->to_number();
    int num2=val2->to_number();
    int ref_size=ref.size();
    if(type1==vm_nil && type2==vm_nil)
    {
        num1=0;
        num2=ref_size-1;
    }
    else if(type1==vm_nil && type2!=vm_nil)
        num1=num2<0? -ref_size:0;
    else if(type1!=vm_nil && type2==vm_nil)
        num2=num1<0? -1:ref_size-1;

    if(num1>=num2)
    {
        die("slc2: begin index must be less than end index");
        return;
    }
    if(num1<-ref_size || num1>=ref_size || num2<-ref_size || num2>=ref_size)
    {
        die("slc2: begin or end index out of range");
        return;
    }
    for(int i=num1;i<=num2;++i)
    {
        nasal_val* tmp=ref[i];
        aim.push_back(tmp);
    }
    return;
}
void nasal_vm::opr_mcall()
{
    nasal_val** mem_addr=nullptr;
    int name_index=exec_code[pc].num;
    if(!local.empty())
        mem_addr=local.back()->ptr.scop->get_mem(name_index);
    if(mem_addr)
    {
        addr_stack.push(mem_addr);
        return;
    }
    mem_addr=global->ptr.scop->get_mem(name_index);
    if(mem_addr)
    {
        addr_stack.push(mem_addr);
        return;
    }
    die("mcall: cannot find symbol named \""+str_table[name_index]+"\"");
    return;
}
void nasal_vm::opr_mcallv()
{
    nasal_val*  val=*stack_top--;
    nasal_val** vec_addr=addr_stack.top();
    addr_stack.pop();
    int type=(*vec_addr)->type;
    if(type==vm_vec)
    {
        int num;
        switch(val->type)
        {
            case vm_num:num=(int)val->ptr.num;break;
            case vm_str:num=(int)trans_string_to_number(*val->ptr.str);break;
            default:die("mcallv: error value type");break;
        }
        nasal_val** res=(*vec_addr)->ptr.vec->get_mem(num);
        if(!res)
        {
            die("mcallv: index out of range");
            return;
        }
        addr_stack.push(res);
    }
    else if(type==vm_hash)
    {
        if(val->type!=vm_str)
        {
            die("mcallv: must use string as the key");
            return;
        }
        nasal_hash& ref=*(*vec_addr)->ptr.hash;
        std::string str=*val->ptr.str;
        nasal_val** res=ref.get_mem(str);
        if(!res)
        {
            ref.elems[str]=nil_addr;
            res=ref.get_mem(str);
        }
        addr_stack.push(res);
    }
    else
    {
        die("mcallv: cannot get memory space in a string");
        return;
    }
    return;
}
void nasal_vm::opr_mcallh()
{
    nasal_val** mem_addr=nullptr;
    nasal_val** hash_addr=addr_stack.top();
    addr_stack.pop();
    if((*hash_addr)->type!=vm_hash)
    {
        die("mcallh: must call a hash");
        return;
    }
    nasal_hash& ref=*(*hash_addr)->ptr.hash;
    std::string& str=str_table[exec_code[pc].num];
    mem_addr=ref.get_mem(str);
    if(!mem_addr) // create a new key
    {
        ref.elems[str]=nil_addr;
        mem_addr=ref.get_mem(str);
    }
    addr_stack.push(mem_addr);
    return;
}
void nasal_vm::opr_ret()
{
    local.pop_back();
    pc=call_stack.top();
    call_stack.pop();
    nasal_val* ret_val=*stack_top--;
    *stack_top=ret_val;
    return;
}
void nasal_vm::run()
{
    static void (nasal_vm::*opr_table[])()=
    {
        &nasal_vm::opr_nop,
        &nasal_vm::opr_load,
        &nasal_vm::opr_loadg,
        &nasal_vm::opr_loadl,
        &nasal_vm::opr_pnum,
        &nasal_vm::opr_pone,
        &nasal_vm::opr_pzero,
        &nasal_vm::opr_pnil,
        &nasal_vm::opr_pstr,
        &nasal_vm::opr_newv,
        &nasal_vm::opr_newh,
        &nasal_vm::opr_newf,
        &nasal_vm::opr_vapp,
        &nasal_vm::opr_happ,
        &nasal_vm::opr_para,
        &nasal_vm::opr_defpara,
        &nasal_vm::opr_dynpara,
        &nasal_vm::opr_unot,
        &nasal_vm::opr_usub,
        &nasal_vm::opr_add,
        &nasal_vm::opr_sub,
        &nasal_vm::opr_mul,
        &nasal_vm::opr_div,
        &nasal_vm::opr_lnk,
        &nasal_vm::opr_addeq,
        &nasal_vm::opr_subeq,
        &nasal_vm::opr_muleq,
        &nasal_vm::opr_diveq,
        &nasal_vm::opr_lnkeq,
        &nasal_vm::opr_meq,
        &nasal_vm::opr_eq,
        &nasal_vm::opr_neq,
        &nasal_vm::opr_less,
        &nasal_vm::opr_leq,
        &nasal_vm::opr_grt,
        &nasal_vm::opr_geq,
        &nasal_vm::opr_pop,
        &nasal_vm::opr_jmp,
        &nasal_vm::opr_jt,
        &nasal_vm::opr_jf,
        &nasal_vm::opr_counter,
        &nasal_vm::opr_cntpop,
        &nasal_vm::opr_findex,
        &nasal_vm::opr_feach,
        &nasal_vm::opr_call,
        &nasal_vm::opr_callv,
        &nasal_vm::opr_callvi,
        &nasal_vm::opr_callh,
        &nasal_vm::opr_callf,
        &nasal_vm::opr_callb,
        &nasal_vm::opr_slcbegin,
        &nasal_vm::opr_slcend,
        &nasal_vm::opr_slc,
        &nasal_vm::opr_slc2,
        &nasal_vm::opr_mcall,
        &nasal_vm::opr_mcallv,
        &nasal_vm::opr_mcallh,
        &nasal_vm::opr_ret
    };
    clock_t begin_time=clock();
    // main loop
    for(pc=0;loop_mark;++pc)
        (this->*opr_table[exec_code[pc].op])();
    
    float total_time=((double)(clock()-begin_time))/CLOCKS_PER_SEC;
    std::cout<<">> [vm] process exited after "<<total_time<<"s.\n";
    return;
}
#endif