#ifndef __NASAL_VM_H__
#define __NASAL_VM_H__

class nasal_vm
{
private:
    /* reference from nasal_gc */
    nasal_val**&             stack_top;// stack top
    /* values of nasal_vm */
    uint32_t                 pc;       // program counter
    bool                     loop_mark;// when mark is false,break the main loop
    std::stack<int>          ret;      // ptr stack stores address for function to return
    std::stack<int>          counter;  // iterator stack for forindex/foreach
    std::vector<std::string> str_table;// symbols used in process
    std::vector<void (nasal_vm::*)()>
                             exec_code;//function pointer
    std::vector<uint32_t>    imm;      // immediate number
    nasal_val**              mem_addr; // used for mem_call
    nasal_gc                 gc;       // garbage collector
    
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
    void opr_callfv();
    void opr_callfh();
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
    nasal_vm():stack_top(gc.stack_top){};
    void init(
        std::vector<std::string>&,
        std::vector<double>&,
        std::vector<opcode>&);
    void clear();
    void run();
};

void nasal_vm::init(
    std::vector<std::string>& strs,
    std::vector<double>&      nums,
    std::vector<opcode>&      exec)
{
    gc.gc_init(nums,strs);
    gc.val_stack[STACK_MAX_DEPTH-1]=nullptr;
    str_table=strs; // get constant strings & symbols
    void (nasal_vm::*opr_table[])()=
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
        &nasal_vm::opr_callfv,
        &nasal_vm::opr_callfh,
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
    for(auto& i:exec)
    {
        exec_code.push_back(opr_table[i.op]);
        imm.push_back(i.num);
    }
    loop_mark=true; // set loop mark to true
    return;
}
void nasal_vm::clear()
{
    gc.gc_clear();
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
    printf(">> [vm] 0x%.8x: %s\n",pc,str.c_str());
    loop_mark=false;
    return;
}
bool nasal_vm::condition(nasal_val* val_addr)
{
    if(val_addr->type==vm_num)
        return val_addr->ptr.num;
    else if(val_addr->type==vm_str)
    {
        std::string& str=*val_addr->ptr.str;
        double num=str2num(str.c_str());
        if(std::isnan(num))
            return str.empty();
        return num;
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
    gc.global.resize(imm[pc],gc.nil_addr);
    return;
}
void nasal_vm::opr_intl()
{
    stack_top[0]->ptr.func->closure.resize(imm[pc],gc.nil_addr);
    return;
}
void nasal_vm::opr_offset()
{
    stack_top[0]->ptr.func->offset=imm[pc];
    return;
}
void nasal_vm::opr_loadg()
{
    gc.global[imm[pc]]=(stack_top--)[0];
    return;
}
void nasal_vm::opr_loadl()
{
    gc.local.back()[imm[pc]]=(stack_top--)[0];
    return;
}
void nasal_vm::opr_pnum()
{
    (++stack_top)[0]=gc.num_addrs[imm[pc]];
    return;
}
void nasal_vm::opr_pone()
{
    (++stack_top)[0]=gc.one_addr;
    return;
}
void nasal_vm::opr_pzero()
{
    (++stack_top)[0]=gc.zero_addr;
    return;
}
void nasal_vm::opr_pnil()
{
    (++stack_top)[0]=gc.nil_addr;
    return;
}
void nasal_vm::opr_pstr()
{
    (++stack_top)[0]=gc.str_addrs[imm[pc]];
    return;
}
void nasal_vm::opr_newv()
{
    nasal_val*  vec_addr=gc.gc_alloc(vm_vec);
    nasal_val** begin=stack_top-imm[pc]+1;
    auto& vec=vec_addr->ptr.vec->elems;// stack_top-imm[pc] stores the vector
    vec.resize(imm[pc]);
    for(int i=0;i<imm[pc];++i)
        vec[i]=begin[i];
    begin[0]=vec_addr;
    stack_top=begin;
    return;
}
void nasal_vm::opr_newh()
{
    (++stack_top)[0]=gc.gc_alloc(vm_hash);
    return;
}
void nasal_vm::opr_newf()
{
    nasal_val* val=gc.gc_alloc(vm_func);
    val->ptr.func->entry=imm[pc];
    if(gc.local.empty())
        val->ptr.func->closure.push_back(gc.nil_addr);// me
    else
        val->ptr.func->closure=gc.local.back();// local contains 'me'
    (++stack_top)[0]=val;
    return;
}
void nasal_vm::opr_happ()
{
    nasal_val* val=stack_top[0];
    (--stack_top)[0]->ptr.hash->elems[str_table[imm[pc]]]=val;
    return;
}
void nasal_vm::opr_para()
{
    nasal_func* func=stack_top[0]->ptr.func;
    int size=func->key_table.size();
    func->key_table[str_table[imm[pc]]]=size;
    func->default_para.push_back(nullptr);
    return;
}
void nasal_vm::opr_defpara()
{
    nasal_val* def_val=stack_top[0];
    nasal_func* func=(--stack_top)[0]->ptr.func;
    int size=func->key_table.size();
    func->key_table[str_table[imm[pc]]]=size;
    func->default_para.push_back(def_val);
    return;
}
void nasal_vm::opr_dynpara()
{
    stack_top[0]->ptr.func->dynpara=imm[pc];
    return;
}
void nasal_vm::opr_unot()
{
    nasal_val* val=stack_top[0];
    int type=val->type;
    if(type==vm_nil)
        stack_top[0]=gc.one_addr;
    else if(type==vm_num)
        stack_top[0]=val->ptr.num?gc.zero_addr:gc.one_addr;
    else if(type==vm_str)
    {
        double num=str2num(val->ptr.str->c_str());
        if(std::isnan(num))
            stack_top[0]=val->ptr.str->empty()?gc.one_addr:gc.zero_addr;
        else
            stack_top[0]=num?gc.zero_addr:gc.one_addr;
    }
    else
        die("unot: incorrect value type");
    return;
}
void nasal_vm::opr_usub()
{
    nasal_val* new_val=gc.gc_alloc(vm_num);
    new_val->ptr.num=-stack_top[0]->to_number();
    stack_top[0]=new_val;
    return;
}
void nasal_vm::opr_add()
{
    nasal_val* new_val=gc.gc_alloc(vm_num);
    new_val->ptr.num=stack_top[-1]->to_number()+stack_top[0]->to_number();
    (--stack_top)[0]=new_val;
    return;
}
void nasal_vm::opr_sub()
{
    nasal_val* new_val=gc.gc_alloc(vm_num);
    new_val->ptr.num=stack_top[-1]->to_number()-stack_top[0]->to_number();
    (--stack_top)[0]=new_val;
    return;
}
void nasal_vm::opr_mul()
{
    nasal_val* new_val=gc.gc_alloc(vm_num);
    new_val->ptr.num=stack_top[-1]->to_number()*stack_top[0]->to_number();
    (--stack_top)[0]=new_val;
    return;
}
void nasal_vm::opr_div()
{
    nasal_val* new_val=gc.gc_alloc(vm_num);
    new_val->ptr.num=stack_top[-1]->to_number()/stack_top[0]->to_number();
    (--stack_top)[0]=new_val;
    return;
}
void nasal_vm::opr_lnk()
{
    nasal_val* new_val=gc.gc_alloc(vm_str);
    *new_val->ptr.str=stack_top[-1]->to_string()+stack_top[0]->to_string();
    (--stack_top)[0]=new_val;
    return;
}
void nasal_vm::opr_addeq()
{
    nasal_val* new_val=gc.gc_alloc(vm_num);
    new_val->ptr.num=mem_addr[0]->to_number()+stack_top[-1]->to_number();
    (--stack_top)[0]=mem_addr[0]=new_val;
    return;
}
void nasal_vm::opr_subeq()
{
    nasal_val* new_val=gc.gc_alloc(vm_num);
    new_val->ptr.num=mem_addr[0]->to_number()-stack_top[-1]->to_number();
    (--stack_top)[0]=mem_addr[0]=new_val;
    return;
}
void nasal_vm::opr_muleq()
{
    nasal_val* new_val=gc.gc_alloc(vm_num);
    new_val->ptr.num=mem_addr[0]->to_number()*stack_top[-1]->to_number();
    (--stack_top)[0]=mem_addr[0]=new_val;
    return;
}
void nasal_vm::opr_diveq()
{
    nasal_val* new_val=gc.gc_alloc(vm_num);
    new_val->ptr.num=mem_addr[0]->to_number()/stack_top[-1]->to_number();
    (--stack_top)[0]=mem_addr[0]=new_val;
    return;
}
void nasal_vm::opr_lnkeq()
{
    nasal_val* new_val=gc.gc_alloc(vm_str);
    *new_val->ptr.str=mem_addr[0]->to_string()+stack_top[-1]->to_string();
    (--stack_top)[0]=mem_addr[0]=new_val;
    return;
}
void nasal_vm::opr_meq()
{
    mem_addr[0]=(--stack_top)[0];
    return;
}
void nasal_vm::opr_eq()
{
    nasal_val* val2=stack_top[0];
    nasal_val* val1=(--stack_top)[0];
    int a_type=val1->type;
    int b_type=val2->type;
    if(a_type==vm_nil && b_type==vm_nil)
        stack_top[0]=gc.one_addr;
    else if(a_type==vm_str && b_type==vm_str)
        stack_top[0]=(*val1->ptr.str==*val2->ptr.str)?gc.one_addr:gc.zero_addr;
    else if(a_type==vm_num || b_type==vm_num)
        stack_top[0]=(val1->to_number()==val2->to_number())?gc.one_addr:gc.zero_addr;
    else
        stack_top[0]=(val1==val2)?gc.one_addr:gc.zero_addr;
    return;
}
void nasal_vm::opr_neq()
{
    nasal_val* val2=stack_top[0];
    nasal_val* val1=(--stack_top)[0];
    int a_type=val1->type;
    int b_type=val2->type;
    if(a_type==vm_nil && b_type==vm_nil)
        stack_top[0]=gc.zero_addr;
    else if(a_type==vm_str && b_type==vm_str)
        stack_top[0]=(*val1->ptr.str!=*val2->ptr.str)?gc.one_addr:gc.zero_addr;
    else if(a_type==vm_num || b_type==vm_num)
        stack_top[0]=(val1->to_number()!=val2->to_number())?gc.one_addr:gc.zero_addr;
    else
        stack_top[0]=(val1!=val2)?gc.one_addr:gc.zero_addr;
    return;
}
void nasal_vm::opr_less()
{
    --stack_top;
    stack_top[0]=(stack_top[0]->to_number()<stack_top[1]->to_number())?gc.one_addr:gc.zero_addr;
    return;
}
void nasal_vm::opr_leq()
{
    --stack_top;
    stack_top[0]=(stack_top[0]->to_number()<=stack_top[1]->to_number())?gc.one_addr:gc.zero_addr;
    return;
}
void nasal_vm::opr_grt()
{
    --stack_top;
    stack_top[0]=(stack_top[0]->to_number()>stack_top[1]->to_number())?gc.one_addr:gc.zero_addr;
    return;
}
void nasal_vm::opr_geq()
{
    --stack_top;
    stack_top[0]=(stack_top[0]->to_number()>=stack_top[1]->to_number())?gc.one_addr:gc.zero_addr;
    return;
}
void nasal_vm::opr_pop()
{
    --stack_top;
    return;
}
void nasal_vm::opr_jmp()
{
    pc=imm[pc]-1;
    return;
}
void nasal_vm::opr_jt()
{
    if(condition(stack_top[0]))
        pc=imm[pc]-1;
    return;
}
void nasal_vm::opr_jf()
{
    if(!condition(stack_top[0]))
        pc=imm[pc]-1;
    --stack_top;
    return;
}
void nasal_vm::opr_counter()
{
    counter.push(-1);
    if(stack_top[0]->type!=vm_vec)
        die("cnt: must use vector in forindex/foreach");
    return;
}
void nasal_vm::opr_cntpop()
{
    counter.pop();
    return;
}
void nasal_vm::opr_findex()
{
    if(++counter.top()>=stack_top[0]->ptr.vec->elems.size())
    {
        pc=imm[pc]-1;
        return;
    }
    (++stack_top)[0]=gc.gc_alloc(vm_num);
    stack_top[0]->ptr.num=counter.top();
    return;
}
void nasal_vm::opr_feach()
{
    std::vector<nasal_val*>& ref=stack_top[0]->ptr.vec->elems;
    if(++counter.top()>=ref.size())
    {
        pc=imm[pc]-1;
        return;
    }
    (++stack_top)[0]=ref[counter.top()];
    return;
}
void nasal_vm::opr_callg()
{
    (++stack_top)[0]=gc.global[imm[pc]];
    return;
}
void nasal_vm::opr_calll()
{
    (++stack_top)[0]=gc.local.back()[imm[pc]];
    return;
}
void nasal_vm::opr_callv()
{
    nasal_val* val=stack_top[0];
    nasal_val* vec_addr=(--stack_top)[0];
    int type=vec_addr->type;
    if(type==vm_vec)
    {
        stack_top[0]=vec_addr->ptr.vec->get_val(val->to_number());
        if(!stack_top[0])
            die("callv: index out of range:"+num2str(val->to_number()));
    }
    else if(type==vm_hash)
    {
        if(val->type!=vm_str)
        {
            die("callv: must use string as the key");
            return;
        }
        stack_top[0]=vec_addr->ptr.hash->get_val(*val->ptr.str);
        if(!stack_top[0])
        {
            die("callv: cannot find member \""+*val->ptr.str+"\" of this hash");
            return;
        }
        if(stack_top[0]->type==vm_func)
            stack_top[0]->ptr.func->closure[0]=val;// me
    }
    else if(type==vm_str)
    {
        std::string& str=*vec_addr->ptr.str;
        int num=val->to_number();
        int str_size=str.length();
        if(num<-str_size || num>=str_size)
        {
            die("callv: index out of range:"+num2str(val->to_number()));
            return;
        }
        stack_top[0]=gc.gc_alloc(vm_num);
        stack_top[0]->ptr.num=(str[num>=0? num:num+str_size]);
    }
    else
        die("callv: must call a vector/hash/string");
    return;
}
void nasal_vm::opr_callvi()
{
    nasal_val* val=stack_top[0];
    if(val->type!=vm_vec)
    {
        die("callvi: multi-definition/multi-assignment must use a vector");
        return;
    }
    // cannot use operator[],because this may cause overflow
    (++stack_top)[0]=val->ptr.vec->get_val(imm[pc]);
    if(!stack_top[0])
        die("callvi: index out of range:"+num2str(imm[pc]));
    return;
}
void nasal_vm::opr_callh()
{
    nasal_val* val=stack_top[0];
    if(val->type!=vm_hash)
    {
        die("callh: must call a hash");
        return;
    }
    stack_top[0]=val->ptr.hash->get_val(str_table[imm[pc]]);
    if(!stack_top[0])
    {
        die("callh: member \""+str_table[imm[pc]]+"\" does not exist");
        return;
    }
    if(stack_top[0]->type==vm_func)
        stack_top[0]->ptr.func->closure[0]=val;// me
    return;
}
void nasal_vm::opr_callfv()
{
    // get parameter list and function value
    int args_size=imm[pc];
    nasal_val** vec=stack_top-args_size+1;
    nasal_val* func_addr=vec[-1];
    if(func_addr->type!=vm_func)
    {
        die("callfv: must call a function");
        return;
    }
    // push new local scope
    auto& ref_func=*func_addr->ptr.func;
    gc.local.push_back(ref_func.closure);
    // load parameters
    auto& ref_default=ref_func.default_para;
    auto& ref_closure=gc.local.back();

    int offset=ref_func.offset;
    int para_size=ref_func.key_table.size();
    // load arguments
    if(args_size<para_size && !ref_default[args_size])
    {
        // if the first default value is not nullptr,then values after it are not nullptr
        die("callfv: lack argument(s)");
        return;
    }
    // if args_size>para_size,for 0 to args_size will cause corruption
    for(int i=0;i<para_size;++i)
        ref_closure[i+offset]=(i<args_size)?vec[i]:ref_default[i];
    // load dynamic argument if args_size>=para_size
    if(ref_func.dynpara>=0)
    {
        nasal_val* vec_addr=gc.gc_alloc(vm_vec);
        for(int i=para_size;i<args_size;++i)
            vec_addr->ptr.vec->elems.push_back(vec[i]);
        ref_closure[para_size+offset]=vec_addr;
    }
    
    stack_top-=args_size;// pop arguments
    ret.push(pc);
    pc=ref_func.entry-1;
    return;
}
void nasal_vm::opr_callfh()
{
    // get parameter list and function value
    auto& ref_hash=stack_top[0]->ptr.hash->elems;
    nasal_val* func_addr=stack_top[-1];
    if(func_addr->type!=vm_func)
    {
        die("callfh: must call a function");
        return;
    }
    // push new local scope
    auto& ref_func=*func_addr->ptr.func;
    gc.local.push_back(ref_func.closure);
    // load parameters
    auto& ref_default=ref_func.default_para;
    auto& ref_closure=gc.local.back();

    if(ref_func.dynpara>=0)
    {
        die("callfh: special call cannot use dynamic argument");
        return;
    }
    int offset=ref_func.offset;
    for(auto& i:ref_func.key_table)
    {
        if(ref_hash.count(i.first))
            ref_closure[i.second+offset]=ref_hash[i.first];
        else if(ref_default[i.second])
            ref_closure[i.second+offset]=ref_default[i.second];
        else
        {
            die("callfh: lack argument(s): \""+i.first+"\"");
            return;
        }
    }

    --stack_top;// pop hash
    ret.push(pc);
    pc=ref_func.entry-1;
    return;
}
void nasal_vm::opr_callb()
{
    loop_mark=(++stack_top)[0]=(*builtin_func[imm[pc]].func)(gc.local.back(),gc);
    return;
}
void nasal_vm::opr_slcbegin()
{
    gc.slice_stack.push_back(gc.gc_alloc(vm_vec));
    if(stack_top[0]->type!=vm_vec)
        die("slcbegin: must slice a vector");
    return;
}
void nasal_vm::opr_slcend()
{
    stack_top[0]=gc.slice_stack.back();
    gc.slice_stack.pop_back();
    return;
}
void nasal_vm::opr_slc()
{
    nasal_val* val=(stack_top--)[0];
    nasal_val* res=stack_top[0]->ptr.vec->get_val(val->to_number());
    if(!res)
        die("slc: index out of range:"+num2str(val->to_number()));
    gc.slice_stack.back()->ptr.vec->elems.push_back(res);
    return;
}
void nasal_vm::opr_slc2()
{
    nasal_val* val2=(stack_top--)[0];
    nasal_val* val1=(stack_top--)[0];
    std::vector<nasal_val*>& ref=stack_top[0]->ptr.vec->elems;
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
    else if(num1<-ref_size || num1>=ref_size)
    {
        die("slc2: begin index out of range: "+num2str(num1));
        return;
    }
    else if(num2<-ref_size || num2>=ref_size)
    {
        die("slc2: end index out of range: "+num2str(num2));
        return;
    }
    for(int i=num1;i<=num2;++i)
        aim.push_back(i>=0?ref[i]:ref[i+ref_size]);
    return;
}
void nasal_vm::opr_mcallg()
{
    mem_addr=&gc.global[imm[pc]];
    (++stack_top)[0]=mem_addr[0];
    return;
}
void nasal_vm::opr_mcalll()
{
    mem_addr=&gc.local.back()[imm[pc]];
    (++stack_top)[0]=mem_addr[0];
    return;
}
void nasal_vm::opr_mcallv()
{
    nasal_val* val=stack_top[0];
    nasal_val* vec_addr=(--stack_top)[0];
    int type=vec_addr->type;
    if(type==vm_vec)
    {
        mem_addr=vec_addr->ptr.vec->get_mem(val->to_number());
        if(!mem_addr)
            die("mcallv: index out of range:"+num2str(val->to_number()));
    }
    else if(type==vm_hash)
    {
        if(val->type!=vm_str)
        {
            die("mcallv: must use string as the key");
            return;
        }
        nasal_hash& ref=*vec_addr->ptr.hash;
        std::string& str=*val->ptr.str;
        mem_addr=ref.get_mem(str);
        if(!mem_addr)
        {
            ref.elems[str]=gc.nil_addr;
            mem_addr=ref.get_mem(str);
        }
    }
    else
        die("mcallv: cannot get memory space in other types");
    return;
}
void nasal_vm::opr_mcallh()
{
    nasal_val* hash_addr=stack_top[0];
    if(hash_addr->type!=vm_hash)
    {
        die("mcallh: must call a hash");
        return;
    }
    nasal_hash& ref=*hash_addr->ptr.hash;
    std::string& str=str_table[imm[pc]];
    mem_addr=ref.get_mem(str);
    if(!mem_addr) // create a new key
    {
        ref.elems[str]=gc.nil_addr;
        mem_addr=ref.get_mem(str);
    }
    return;
}
void nasal_vm::opr_ret()
{
    gc.local.pop_back();// delete local scope
    pc=ret.top();ret.pop();// fetch pc
    (--stack_top)[0]->ptr.func->closure[0]=gc.nil_addr;// set 'me' to nil
    stack_top[0]=stack_top[1];// rewrite nasal_func with returned value
    return;
}
void nasal_vm::run()
{
    clock_t begin_time=clock();
    for(pc=0;loop_mark&&!gc.val_stack[STACK_MAX_DEPTH-1];++pc)
        (this->*exec_code[pc])();
    if(gc.val_stack[STACK_MAX_DEPTH-1])
        die("stack overflow");
    std::cout<<">> [vm] process exited after "<<((double)(clock()-begin_time))/CLOCKS_PER_SEC<<"s.\n";
    return;
}
#endif