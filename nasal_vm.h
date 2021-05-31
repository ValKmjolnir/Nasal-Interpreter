#ifndef __NASAL_VM_H__
#define __NASAL_VM_H__

class nasal_vm
{
private:
    /* reference from nasal_gc */
    nasal_val**&             stack_top;  // stack top
    /* values of nasal_vm */
    int                      pc;         // program counter
    bool                     loop_mark;  // when mark is false,break the main loop
    std::stack<int>          ret;        // ptr stack stores address for function to return
    std::stack<int>          counter;    // iterator stack for forindex/foreach
    std::vector<std::string> str_table;  // symbols used in process
    std::vector<opcode>      exec_code;  // byte codes store here
    std::stack<nasal_val**>  addr_stack; // stack for mem_call
    nasal_gc                 gc;         //garbage collector
    
    void die(std::string);
    bool condition(nasal_val*);
    void opr_nop();
    void opr_intg();
    void opr_intl();
    void opr_offset();
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
    void opr_callg();
    void opr_calll();
    void opr_callv();
    void opr_callvi();
    void opr_callh();
    void opr_callf();
    void opr_callb();
    void opr_slcbegin();
    void opr_slcend();
    void opr_slc();
    void opr_slc2();
    void opr_mcallg();
    void opr_mcalll();
    void opr_mcallv();
    void opr_mcallh();
    void opr_ret();
public:
    nasal_vm();
    void init(
        std::vector<std::string>&,
        std::vector<double>&,
        std::vector<opcode>&);
    void clear();
    void run();
};

nasal_vm::nasal_vm():stack_top(gc.stack_top)
{
    return;
}

void nasal_vm::init(
    std::vector<std::string>& strs,
    std::vector<double>&      nums,
    std::vector<opcode>&      exec)
{
    gc.gc_init(nums,strs);
    str_table=strs; // get constant strings & symbols
    exec_code=exec; // get bytecodes
    loop_mark=true; // set loop mark to true
    return;
}
void nasal_vm::clear()
{
    gc.gc_clear();
    while(!addr_stack.empty())
        addr_stack.pop();
    while(!ret.empty())
        ret.pop();
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
bool nasal_vm::condition(nasal_val* val_addr)
{
    int type=val_addr->type;
    if(type==vm_num)
        return val_addr->ptr.num;
    else if(type==vm_str)
    {
        std::string& str=*val_addr->ptr.str;
        double number=str2num(str);
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
void nasal_vm::opr_intg()
{
    gc.global.resize(exec_code[pc].num,gc.nil_addr);
    return;
}
void nasal_vm::opr_intl()
{
    (*stack_top)->ptr.func->closure.resize(exec_code[pc].num,gc.nil_addr);
    (*stack_top)->ptr.func->closure[0]=gc.nil_addr;// me
    return;
}
void nasal_vm::opr_offset()
{
    (*stack_top)->ptr.func->offset=exec_code[pc].num;
}
void nasal_vm::opr_loadg()
{
    gc.global[exec_code[pc].num]=*stack_top--;
    return;
}
void nasal_vm::opr_loadl()
{
    gc.local.back()[exec_code[pc].num]=*stack_top--;
    return;
}
void nasal_vm::opr_pnum()
{
    *(++stack_top)=gc.num_addrs[exec_code[pc].num];
    return;
}
void nasal_vm::opr_pone()
{
    *(++stack_top)=gc.one_addr;
    return;
}
void nasal_vm::opr_pzero()
{
    *(++stack_top)=gc.zero_addr;
    return;
}
void nasal_vm::opr_pnil()
{
    *(++stack_top)=gc.nil_addr;
    return;
}
void nasal_vm::opr_pstr()
{
    *(++stack_top)=gc.str_addrs[exec_code[pc].num];
    return;
}
void nasal_vm::opr_newv()
{
    nasal_val* vec=gc.gc_alloc(vm_vec);
    *(++stack_top)=vec;
    return;
}
void nasal_vm::opr_newh()
{
    nasal_val* hash=gc.gc_alloc(vm_hash);
    *(++stack_top)=hash;
    return;
}
void nasal_vm::opr_newf()
{
    nasal_val* val=gc.gc_alloc(vm_func);
    val->ptr.func->entry=exec_code[pc].num;
    if(!gc.local.empty())
        val->ptr.func->closure=gc.local.back();// local contains 'me'
    else
        val->ptr.func->closure.push_back(gc.nil_addr);// me
    *(++stack_top)=val;
    return;
}
void nasal_vm::opr_vapp()
{
    nasal_val* vec=*(stack_top-exec_code[pc].num);
    for(nasal_val** i=stack_top-exec_code[pc].num+1;i<=stack_top;++i)
        vec->ptr.vec->elems.push_back(*i);
    stack_top-=exec_code[pc].num;
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
    int size=(*stack_top)->ptr.func->key_table.size();
    (*stack_top)->ptr.func->key_table[str_table[exec_code[pc].num]]=size;
    (*stack_top)->ptr.func->default_para.push_back(nullptr);
    return;
}
void nasal_vm::opr_defpara()
{
    nasal_val* def_val=*stack_top--;
    int size=(*stack_top)->ptr.func->key_table.size();
    (*stack_top)->ptr.func->key_table[str_table[exec_code[pc].num]]=size;
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
        new_val=gc.one_addr;
    else if(type==vm_num)
        new_val=val->ptr.num?gc.zero_addr:gc.one_addr;
    else if(type==vm_str)
    {
        double number=str2num(*val->ptr.str);
        if(std::isnan(number))
            new_val=val->ptr.str->length()?gc.zero_addr:gc.one_addr;
        else
            new_val=number?gc.zero_addr:gc.one_addr;
    }
    else
        die("unot: incorrect value type");
    *stack_top=new_val;
    return;
}
void nasal_vm::opr_usub()
{
    nasal_val* val=*stack_top;
    nasal_val* new_val=gc.gc_alloc(vm_num);
    new_val->ptr.num=-val->to_number();
    *stack_top=new_val;
    return;
}
void nasal_vm::opr_add()
{
    nasal_val* new_val=gc.gc_alloc(vm_num);
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    new_val->ptr.num=val1->to_number()+val2->to_number();
    *stack_top=new_val;
    return;
}
void nasal_vm::opr_sub()
{
    nasal_val* new_val=gc.gc_alloc(vm_num);
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    new_val->ptr.num=val1->to_number()-val2->to_number();
    *stack_top=new_val;
    return;
}
void nasal_vm::opr_mul()
{
    nasal_val* new_val=gc.gc_alloc(vm_num);
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    new_val->ptr.num=val1->to_number()*val2->to_number();
    *stack_top=new_val;
    return;
}
void nasal_vm::opr_div()
{
    nasal_val* new_val=gc.gc_alloc(vm_num);
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    new_val->ptr.num=val1->to_number()/val2->to_number();
    *stack_top=new_val;
    return;
}
void nasal_vm::opr_lnk()
{
    nasal_val* new_val=gc.gc_alloc(vm_str);
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
    nasal_val* new_val=gc.gc_alloc(vm_num);
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
    nasal_val* new_val=gc.gc_alloc(vm_num);
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
    nasal_val* new_val=gc.gc_alloc(vm_num);
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
    nasal_val* new_val=gc.gc_alloc(vm_num);
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
    nasal_val* new_val=gc.gc_alloc(vm_str);
    *new_val->ptr.str=val1->to_string()+val2->to_string();
    *stack_top=new_val;
    *mem_addr=new_val;
    return;
}
void nasal_vm::opr_meq()
{
    *addr_stack.top()=*stack_top;
    addr_stack.pop();
    return;
}
void nasal_vm::opr_eq()
{
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    int a_type=val1->type;
    int b_type=val2->type;
    if(a_type==vm_nil && b_type==vm_nil)
        *stack_top=gc.one_addr;
    else if(a_type==vm_str && b_type==vm_str)
        *stack_top=(*val1->ptr.str==*val2->ptr.str)?gc.one_addr:gc.zero_addr;
    else if(a_type==vm_num || b_type==vm_num)
        *stack_top=(val1->to_number()==val2->to_number())?gc.one_addr:gc.zero_addr;
    else
        *stack_top=(val1==val2)?gc.one_addr:gc.zero_addr;
    return;
}
void nasal_vm::opr_neq()
{
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    int a_type=val1->type;
    int b_type=val2->type;
    if(a_type==vm_nil && b_type==vm_nil)
        *stack_top=gc.zero_addr;
    else if(a_type==vm_str && b_type==vm_str)
        *stack_top=(*val1->ptr.str!=*val2->ptr.str)?gc.one_addr:gc.zero_addr;
    else if(a_type==vm_num || b_type==vm_num)
        *stack_top=(val1->to_number()!=val2->to_number())?gc.one_addr:gc.zero_addr;
    else
        *stack_top=(val1!=val2)?gc.one_addr:gc.zero_addr;
    return;
}
void nasal_vm::opr_less()
{
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    int a_type=val1->type;
    int b_type=val2->type;
    if(a_type==vm_str && b_type==vm_str)
        *stack_top=(*val1->ptr.str<*val2->ptr.str)?gc.one_addr:gc.zero_addr;
    else
        *stack_top=(val1->to_number()<val2->to_number())?gc.one_addr:gc.zero_addr;
    return;
}
void nasal_vm::opr_leq()
{
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    int a_type=val1->type;
    int b_type=val2->type;
    if(a_type==vm_str && b_type==vm_str)
        *stack_top=(*val1->ptr.str<=*val2->ptr.str)?gc.one_addr:gc.zero_addr;
    else
        *stack_top=(val1->to_number()<=val2->to_number())?gc.one_addr:gc.zero_addr;
    return;
}
void nasal_vm::opr_grt()
{
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    int a_type=val1->type;
    int b_type=val2->type;
    if(a_type==vm_str && b_type==vm_str) 
        *stack_top=(*val1->ptr.str>*val2->ptr.str)?gc.one_addr:gc.zero_addr;
    else
        *stack_top=(val1->to_number()>val2->to_number())?gc.one_addr:gc.zero_addr;
    return;
}
void nasal_vm::opr_geq()
{
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top;
    int a_type=val1->type;
    int b_type=val2->type;
    if(a_type==vm_str && b_type==vm_str)
        *stack_top=(*val1->ptr.str>=*val2->ptr.str)?gc.one_addr:gc.zero_addr;
    else
        *stack_top=(val1->to_number()>=val2->to_number())?gc.one_addr:gc.zero_addr;
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
    if(condition(*stack_top))
        pc=exec_code[pc].num-1;
    return;
}
void nasal_vm::opr_jf()
{
    if(!condition(*stack_top))
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
    nasal_val* res=gc.gc_alloc(vm_num);
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
void nasal_vm::opr_callg()
{
    *(++stack_top)=gc.global[exec_code[pc].num];
    return;
}
void nasal_vm::opr_calll()
{
    *(++stack_top)=gc.local.back()[exec_code[pc].num];
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
            res->ptr.func->closure[0]=val;// me
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
        nasal_val* res=gc.gc_alloc(vm_num);
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
        res->ptr.func->closure[0]=val;// me
    *stack_top=res;
    return;
}
void nasal_vm::opr_callf()
{
    // get parameter list and function value
    nasal_val* para_addr=*stack_top;
    nasal_val* func_addr=*(stack_top-1);
    if(func_addr->type!=vm_func)
    {
        die("callf: called a value that is not a function");
        return;
    }
    // push new local scope
    gc.local.push_back(func_addr->ptr.func->closure);
    // load parameters
    nasal_func&              ref_func=*func_addr->ptr.func;
    std::vector<nasal_val*>& ref_default=ref_func.default_para;
    std::vector<nasal_val*>& ref_closure=gc.local.back();
    std::unordered_map<std::string,int>& ref_keys=ref_func.key_table;
    
    if(para_addr->type==vm_vec)
    {
        std::vector<nasal_val*>& args=para_addr->ptr.vec->elems;
        int args_size=args.size();
        int para_size=ref_keys.size();
        for(int i=0;i<para_size;++i)
        {
            if(i>=args_size)
            {
                if(!ref_default[i])
                {
                    die("callf: lack argument(s)");
                    return;
                }
                ref_closure[i+ref_func.offset]=ref_default[i];
            }
            else
                ref_closure[i+ref_func.offset]=args[i];
        }
        if(ref_func.dynpara>=0)
        {
            nasal_val* vec_addr=gc.gc_alloc(vm_vec);
            for(int i=para_size;i<args_size;++i)
                vec_addr->ptr.vec->elems.push_back(args[i]);
            ref_closure[para_size+ref_func.offset]=vec_addr;
        }
    }
    else
    {
        std::unordered_map<std::string,nasal_val*>& ref_hash=para_addr->ptr.hash->elems;
        if(ref_func.dynpara>=0)
        {
            die("callf: special call cannot use dynamic argument");
            return;
        }
        for(auto& i:ref_keys)
        {
            if(ref_hash.count(i.first))
                ref_closure[i.second+ref_func.offset]=ref_hash[i.first];
            else if(ref_default[i.second])
                ref_closure[i.second+ref_func.offset]=ref_default[i.second];
            else
            {
                die("callf: lack argument(s): \""+i.first+"\"");
                return;
            }
        }
    }
    --stack_top;
    ret.push(pc);
    pc=ref_func.entry-1;
    return;
}
void nasal_vm::opr_callb()
{
    nasal_val* res=(*builtin_func[exec_code[pc].num].func)(gc.local.back(),gc);
    *(++stack_top)=res;
    loop_mark=res;
    return;
}
void nasal_vm::opr_slcbegin()
{
    gc.slice_stack.push_back(gc.gc_alloc(vm_vec));
    if((*stack_top)->type!=vm_vec)
        die("slcbegin: must slice a vector");
    return;
}
void nasal_vm::opr_slcend()
{
    *stack_top=gc.slice_stack.back();
    gc.slice_stack.pop_back();
    return;
}
void nasal_vm::opr_slc()
{
    nasal_val* val=*stack_top--;
    double num;
    switch(val->type)
    {
        case vm_num:num=val->ptr.num;break;
        case vm_str:num=str2num(*val->ptr.str);break;
        default:die("slc: error value type");break;
    }
    nasal_val* res=(*stack_top)->ptr.vec->get_val((int)num);
    if(res)
        gc.slice_stack.back()->ptr.vec->elems.push_back(res);
    else
        die("slc: index out of range");
    return;
}
void nasal_vm::opr_slc2()
{
    nasal_val* val2=*stack_top--;
    nasal_val* val1=*stack_top--;
    std::vector<nasal_val*>& ref=(*stack_top)->ptr.vec->elems;
    std::vector<nasal_val*>& aim=gc.slice_stack.back()->ptr.vec->elems;

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
        aim.push_back(ref[i]);
    return;
}
void nasal_vm::opr_mcallg()
{
    addr_stack.push(&gc.global[exec_code[pc].num]);
    return;
}
void nasal_vm::opr_mcalll()
{
    addr_stack.push(&gc.local.back()[exec_code[pc].num]);
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
            case vm_str:num=(int)str2num(*val->ptr.str);break;
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
        std::string& str=*val->ptr.str;
        nasal_val** res=ref.get_mem(str);
        if(!res)
        {
            ref.elems[str]=gc.nil_addr;
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
        ref.elems[str]=gc.nil_addr;
        mem_addr=ref.get_mem(str);
    }
    addr_stack.push(mem_addr);
    return;
}
void nasal_vm::opr_ret()
{
    gc.local.pop_back();
    pc=ret.top();
    ret.pop();
    nasal_val* ret_val=*stack_top--;
    *stack_top=ret_val;
    return;
}
void nasal_vm::run()
{
    static void (nasal_vm::*opr_table[])()=
    {
        &nasal_vm::opr_nop,
        &nasal_vm::opr_intg,
        &nasal_vm::opr_intl,
        &nasal_vm::opr_offset,
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
        &nasal_vm::opr_callg,
        &nasal_vm::opr_calll,
        &nasal_vm::opr_callv,
        &nasal_vm::opr_callvi,
        &nasal_vm::opr_callh,
        &nasal_vm::opr_callf,
        &nasal_vm::opr_callb,
        &nasal_vm::opr_slcbegin,
        &nasal_vm::opr_slcend,
        &nasal_vm::opr_slc,
        &nasal_vm::opr_slc2,
        &nasal_vm::opr_mcallg,
        &nasal_vm::opr_mcalll,
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