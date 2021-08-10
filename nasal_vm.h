#ifndef __NASAL_VM_H__
#define __NASAL_VM_H__

class nasal_vm
{
private:
    /* reference from nasal_gc */
    nasal_val**&             stack_top;// stack top
    /* values of nasal_vm */
    uint32_t                 pc;       // program counter
    std::stack<uint32_t>     ret;      // ptr stack stores address for function to return
    std::stack<int>          counter;  // iterator stack for forindex/foreach
    std::vector<double>      num_table;// numbers used in process(const calculation)
    std::vector<std::string> str_table;// symbols used in process
    std::vector<uint32_t>    imm;      // immediate number
    nasal_val**              mem_addr; // used for mem_call
    nasal_gc                 gc;       // garbage collector
    /* values used for debug */
    std::vector<opcode>      bytecode; // bytecode
    std::vector<std::string> files;    // files
    /* debug functions */
    void bytecodeinfo(uint32_t);
    void traceback();
    void stackinfo(int);
    void die(std::string);
    void stackoverflow();
    /* vm calculation functions*/
    bool condition(nasal_val*);
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
    void opr_addc();
    void opr_subc();
    void opr_mulc();
    void opr_divc();
    void opr_lnkc();
    void opr_addeq();
    void opr_subeq();
    void opr_muleq();
    void opr_diveq();
    void opr_lnkeq();
    void opr_addeqc();
    void opr_subeqc();
    void opr_muleqc();
    void opr_diveqc();
    void opr_lnkeqc();
    void opr_meq();
    void opr_eq();
    void opr_neq();
    void opr_less();
    void opr_leq();
    void opr_grt();
    void opr_geq();
    void opr_lessc();
    void opr_leqc();
    void opr_grtc();
    void opr_geqc();
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
        std::vector<std::string>&);
    void clear();
    void run(std::vector<opcode>&,bool);
};

void nasal_vm::init(
    std::vector<std::string>& strs,
    std::vector<double>&      nums,
    std::vector<std::string>& filenames)
{
    gc.gc_init(nums,strs);
    gc.val_stack[STACK_MAX_DEPTH-1]=nullptr;
    num_table=nums; // get constant numbers
    str_table=strs; // get constant strings & symbols
    files=filenames;// get filenames for debugger
    return;
}
void nasal_vm::clear()
{
    gc.gc_clear();
    while(!ret.empty())
        ret.pop();
    while(!counter.empty())
        counter.pop();
    num_table.clear();
    str_table.clear();
    imm.clear();
    return;
}
void nasal_vm::bytecodeinfo(uint32_t p)
{
    printf("\t0x%.8x: %s 0x%.8x",p,code_table[bytecode[p].op].name,bytecode[p].num);
    if(bytecode[p].op==op_callb)
        printf(":%s",builtin_func[bytecode[p].num].name);
    printf(" (%s line %d)\n",files[bytecode[p].fidx].c_str(),bytecode[p].line);
    return;
}
void nasal_vm::traceback()
{
    printf("trace back:\n");
    uint32_t same_cnt=0,last_point=0;
    for(uint32_t point=0;!ret.empty();last_point=point,ret.pop())
    {
        point=ret.top();
        if(point==last_point)
        {
            ++same_cnt;
            continue;
        }
        if(same_cnt)
        {
            printf("\t0x%.8x: %d same call(s) ...\n",last_point,same_cnt);
            same_cnt=0;
        }
        bytecodeinfo(point);
    }
    if(same_cnt)
        printf("\t0x%.8x: %d same call(s) ...\n",last_point,same_cnt);
    return;
}
void nasal_vm::stackinfo(int limit)
{
    printf("vm stack(limit %d):\n",limit);
    uint32_t same_cnt=0;
    nasal_val* last_ptr=(nasal_val*)0xffff;
    for(int i=0;i<limit && stack_top-i>=gc.val_stack;++i)
    {
        if(stack_top[-i]==last_ptr)
        {
            ++same_cnt;
            continue;
        }
        if(same_cnt)
        {
            printf("\t%p ...  | %d same value(s)\n",last_ptr,same_cnt);
            same_cnt=0;
        }
        last_ptr=stack_top[-i];
        printf("\t%p ",stack_top[-i]);
        if(!stack_top[-i])
            printf("nullptr");
        else
            switch(stack_top[-i]->type)
            {
                case vm_nil:  printf("nil  | gc.nil_addr");break;
                case vm_num:  printf("num  | %lf",stack_top[-i]->ptr.num);break;
                case vm_str:  printf("str  | ");raw_string(*stack_top[-i]->ptr.str);break;
                case vm_func: printf("func | func(%lu para){..}",stack_top[-i]->ptr.func->key_table.size());break;
                case vm_vec:  printf("vec  | [%lu val]",stack_top[-i]->ptr.vec->elems.size());break;
                case vm_hash: printf("hash | {%lu member}",stack_top[-i]->ptr.hash->elems.size());break;
                default:      printf("unknown");break;
            }
        putchar('\n');
    }
    if(same_cnt)
       printf("\t%p ...  | %d same value(s)\n",last_ptr,same_cnt);
    return;
}
void nasal_vm::die(std::string str)
{
    printf("[vm] error at 0x%.8x: %s\n",pc,str.c_str());
    // trace back will use ret_stack
    ret.push(pc);
    traceback();
    stackinfo(10);
    gc.val_stack[STACK_MAX_DEPTH-1]=(nasal_val*)0xffff;
    return;
}
void nasal_vm::stackoverflow()
{
    printf("[vm] stack overflow\n");
    traceback();
    stackinfo(10);
    return;
}
inline bool nasal_vm::condition(nasal_val* val_addr)
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
inline void nasal_vm::opr_intg()
{
    // global values store on stack
    for(uint32_t i=0;i<imm[pc];++i)
        (stack_top++)[0]=gc.nil_addr;
    --stack_top;// point to the top
    return;
}
inline void nasal_vm::opr_intl()
{
    stack_top[0]->ptr.func->closure.resize(imm[pc],gc.nil_addr);
    return;
}
inline void nasal_vm::opr_offset()
{
    stack_top[0]->ptr.func->offset=imm[pc];
    return;
}
inline void nasal_vm::opr_loadg()
{
    gc.val_stack[imm[pc]]=(stack_top--)[0];
    return;
}
inline void nasal_vm::opr_loadl()
{
    gc.local.back()[imm[pc]]=(stack_top--)[0];
    return;
}
inline void nasal_vm::opr_pnum()
{
    (++stack_top)[0]=gc.num_addrs[imm[pc]];
    return;
}
inline void nasal_vm::opr_pone()
{
    (++stack_top)[0]=gc.one_addr;
    return;
}
inline void nasal_vm::opr_pzero()
{
    (++stack_top)[0]=gc.zero_addr;
    return;
}
inline void nasal_vm::opr_pnil()
{
    (++stack_top)[0]=gc.nil_addr;
    return;
}
inline void nasal_vm::opr_pstr()
{
    (++stack_top)[0]=gc.str_addrs[imm[pc]];
    return;
}
inline void nasal_vm::opr_newv()
{
    nasal_val*  vec_addr=gc.gc_alloc(vm_vec);
    nasal_val** begin=stack_top-imm[pc]+1;
    auto& vec=vec_addr->ptr.vec->elems;// stack_top-imm[pc] stores the vector
    vec.resize(imm[pc]);
    for(uint32_t i=0;i<imm[pc];++i)
        vec[i]=begin[i];
    begin[0]=vec_addr;
    stack_top=begin;
    return;
}
inline void nasal_vm::opr_newh()
{
    (++stack_top)[0]=gc.gc_alloc(vm_hash);
    return;
}
inline void nasal_vm::opr_newf()
{
    (++stack_top)[0]=gc.gc_alloc(vm_func);
    stack_top[0]->ptr.func->entry=imm[pc];
    if(gc.local.empty())
        stack_top[0]->ptr.func->closure.push_back(gc.nil_addr);// me
    else
        stack_top[0]->ptr.func->closure=gc.local.back();// local contains 'me'
    return;
}
inline void nasal_vm::opr_happ()
{
    nasal_val* val=stack_top[0];
    (--stack_top)[0]->ptr.hash->elems[str_table[imm[pc]]]=val;
    return;
}
inline void nasal_vm::opr_para()
{
    nasal_func* func=stack_top[0]->ptr.func;
    int size=func->key_table.size();
    func->key_table[str_table[imm[pc]]]=size;
    func->default_para.push_back(nullptr);
    return;
}
inline void nasal_vm::opr_defpara()
{
    nasal_val* def_val=stack_top[0];
    nasal_func* func=(--stack_top)[0]->ptr.func;
    int size=func->key_table.size();
    func->key_table[str_table[imm[pc]]]=size;
    func->default_para.push_back(def_val);
    return;
}
inline void nasal_vm::opr_dynpara()
{
    stack_top[0]->ptr.func->dynpara=imm[pc];
    return;
}
inline void nasal_vm::opr_unot()
{
    nasal_val* val=stack_top[0];
    switch(val->type)
    {
        case vm_nil:stack_top[0]=gc.one_addr;break;
        case vm_num:stack_top[0]=val->ptr.num?gc.zero_addr:gc.one_addr;break;
        case vm_str:
        {
            double num=str2num(val->ptr.str->c_str());
            if(std::isnan(num))
                stack_top[0]=val->ptr.str->empty()?gc.one_addr:gc.zero_addr;
            else
                stack_top[0]=num?gc.zero_addr:gc.one_addr;
        }
        break;
        default:die("unot: incorrect value type");break;
    }
    return;
}
inline void nasal_vm::opr_usub()
{
    nasal_val* new_val=gc.gc_alloc(vm_num);
    new_val->ptr.num=-stack_top[0]->to_number();
    stack_top[0]=new_val;
    return;
}

#define op_calc(type)\
    nasal_val* new_val=gc.gc_alloc(vm_num);\
    new_val->ptr.num=stack_top[-1]->to_number() type stack_top[0]->to_number();\
    (--stack_top)[0]=new_val;

inline void nasal_vm::opr_add(){op_calc(+);}
inline void nasal_vm::opr_sub(){op_calc(-);}
inline void nasal_vm::opr_mul(){op_calc(*);}
inline void nasal_vm::opr_div(){op_calc(/);}
inline void nasal_vm::opr_lnk()
{
    nasal_val* new_val=gc.gc_alloc(vm_str);
    *new_val->ptr.str=stack_top[-1]->to_string()+stack_top[0]->to_string();
    (--stack_top)[0]=new_val;
    return;
}

#define op_calc_const(type)\
    nasal_val* new_val=gc.gc_alloc(vm_num);\
    new_val->ptr.num=stack_top[0]->to_number() type num_table[imm[pc]];\
    stack_top[0]=new_val;

inline void nasal_vm::opr_addc(){op_calc_const(+);}
inline void nasal_vm::opr_subc(){op_calc_const(-);}
inline void nasal_vm::opr_mulc(){op_calc_const(*);}
inline void nasal_vm::opr_divc(){op_calc_const(/);}
inline void nasal_vm::opr_lnkc()
{
    nasal_val* new_val=gc.gc_alloc(vm_str);
    *new_val->ptr.str=stack_top[0]->to_string()+str_table[imm[pc]];
    stack_top[0]=new_val;
    return;
}

#define op_calc_eq(type)\
    nasal_val* new_val=gc.gc_alloc(vm_num);\
    new_val->ptr.num=mem_addr[0]->to_number() type stack_top[-1]->to_number();\
    (--stack_top)[0]=mem_addr[0]=new_val;

inline void nasal_vm::opr_addeq(){op_calc_eq(+);}
inline void nasal_vm::opr_subeq(){op_calc_eq(-);}
inline void nasal_vm::opr_muleq(){op_calc_eq(*);}
inline void nasal_vm::opr_diveq(){op_calc_eq(/);}
inline void nasal_vm::opr_lnkeq()
{
    nasal_val* new_val=gc.gc_alloc(vm_str);
    *new_val->ptr.str=mem_addr[0]->to_string()+stack_top[-1]->to_string();
    (--stack_top)[0]=mem_addr[0]=new_val;
    return;
}

#define op_calc_eq_const(type)\
    nasal_val* new_val=gc.gc_alloc(vm_num);\
    new_val->ptr.num=mem_addr[0]->to_number() type num_table[imm[pc]];\
    stack_top[0]=mem_addr[0]=new_val;

inline void nasal_vm::opr_addeqc(){op_calc_eq_const(+);}
inline void nasal_vm::opr_subeqc(){op_calc_eq_const(-);}
inline void nasal_vm::opr_muleqc(){op_calc_eq_const(*);}
inline void nasal_vm::opr_diveqc(){op_calc_eq_const(/);}
inline void nasal_vm::opr_lnkeqc()
{
    nasal_val* new_val=gc.gc_alloc(vm_str);
    *new_val->ptr.str=mem_addr[0]->to_string()+str_table[imm[pc]];
    stack_top[0]=mem_addr[0]=new_val;
    return;
}

inline void nasal_vm::opr_meq()
{
    mem_addr[0]=(--stack_top)[0];
    return;
}
inline void nasal_vm::opr_eq()
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
inline void nasal_vm::opr_neq()
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

#define op_cmp(type)\
    --stack_top;\
    stack_top[0]=(stack_top[0]->to_number() type stack_top[1]->to_number())?gc.one_addr:gc.zero_addr;

inline void nasal_vm::opr_less(){op_cmp(<);}
inline void nasal_vm::opr_leq(){op_cmp(<=);}
inline void nasal_vm::opr_grt(){op_cmp(>);}
inline void nasal_vm::opr_geq(){op_cmp(>=);}

#define op_cmp_const(type) stack_top[0]=(stack_top[0]->to_number() type num_table[imm[pc]])?gc.one_addr:gc.zero_addr;

inline void nasal_vm::opr_lessc(){op_cmp_const(<);}
inline void nasal_vm::opr_leqc(){op_cmp_const(<=);}
inline void nasal_vm::opr_grtc(){op_cmp_const(>);}
inline void nasal_vm::opr_geqc(){op_cmp_const(>=);}

inline void nasal_vm::opr_pop()
{
    --stack_top;
    return;
}
inline void nasal_vm::opr_jmp()
{
    pc=imm[pc]-1;
    return;
}
inline void nasal_vm::opr_jt()
{
    if(condition(stack_top[0]))
        pc=imm[pc]-1;
    return;
}
inline void nasal_vm::opr_jf()
{
    if(!condition(stack_top[0]))
        pc=imm[pc]-1;
    --stack_top;
    return;
}
inline void nasal_vm::opr_counter()
{
    counter.push(-1);
    if(stack_top[0]->type!=vm_vec)
        die("cnt: must use vector in forindex/foreach");
    return;
}
inline void nasal_vm::opr_cntpop()
{
    counter.pop();
    return;
}
inline void nasal_vm::opr_findex()
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
inline void nasal_vm::opr_feach()
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
inline void nasal_vm::opr_callg()
{
    (++stack_top)[0]=gc.val_stack[imm[pc]];
    return;
}
inline void nasal_vm::opr_calll()
{
    (++stack_top)[0]=gc.local.back()[imm[pc]];
    return;
}
inline void nasal_vm::opr_callv()
{
    nasal_val* val=stack_top[0];
    nasal_val* vec_addr=(--stack_top)[0];
    if(vec_addr->type==vm_vec)
    {
        stack_top[0]=vec_addr->ptr.vec->get_val(val->to_number());
        if(!stack_top[0])
            die("callv: index out of range:"+std::to_string(val->to_number()));
    }
    else if(vec_addr->type==vm_hash)
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
    else if(vec_addr->type==vm_str)
    {
        std::string& str=*vec_addr->ptr.str;
        int num=val->to_number();
        int str_size=str.length();
        if(num<-str_size || num>=str_size)
        {
            die("callv: index out of range:"+std::to_string(val->to_number()));
            return;
        }
        stack_top[0]=gc.gc_alloc(vm_num);
        stack_top[0]->ptr.num=(str[num>=0? num:num+str_size]);
    }
    else
        die("callv: must call a vector/hash/string");
    return;
}
inline void nasal_vm::opr_callvi()
{
    nasal_val* val=stack_top[0];
    if(val->type!=vm_vec)
    {
        die("callvi: must use a vector");
        return;
    }
    // cannot use operator[],because this may cause overflow
    (++stack_top)[0]=val->ptr.vec->get_val(imm[pc]);
    if(!stack_top[0])
        die("callvi: index out of range:"+std::to_string(imm[pc]));
    return;
}
inline void nasal_vm::opr_callh()
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
inline void nasal_vm::opr_callfv()
{
    // get parameter list and function value
    uint32_t args_size=imm[pc];
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

    uint32_t offset=ref_func.offset;
    uint32_t para_size=ref_func.key_table.size();
    // load arguments
    if(args_size<para_size && !ref_default[args_size])
    {
        // if the first default value is not nullptr,then values after it are not nullptr
        die("callfv: lack argument(s)");
        return;
    }
    // if args_size>para_size,for 0 to args_size will cause corruption
    uint32_t min_size=std::min(para_size,args_size);
    for(uint32_t i=0;i<min_size;++i)
        ref_closure[i+offset]=vec[i];
    for(uint32_t i=args_size;i<para_size;++i)
        ref_closure[i+offset]=ref_default[i];
    // load dynamic argument if args_size>=para_size
    if(ref_func.dynpara>=0)
    {
        nasal_val* vec_addr=gc.gc_alloc(vm_vec);
        for(uint32_t i=para_size;i<args_size;++i)
            vec_addr->ptr.vec->elems.push_back(vec[i]);
        ref_closure[para_size+offset]=vec_addr;
    }
    
    stack_top-=args_size;// pop arguments
    ret.push(pc);
    pc=ref_func.entry-1;
    return;
}
inline void nasal_vm::opr_callfh()
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
    uint32_t offset=ref_func.offset;
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
inline void nasal_vm::opr_callb()
{
    (++stack_top)[0]=(*builtin_func[imm[pc]].func)(gc.local.back(),gc);
    if(!stack_top[0])
        die("native function error.");
    return;
}
inline void nasal_vm::opr_slcbegin()
{
    // | slice_vector | <-- stack_top[0]
    // ----------------
    // | resource_vec | <-- stack_top[-1]
    // ----------------
    (++stack_top)[0]=gc.gc_alloc(vm_vec);
    if(stack_top[-1]->type!=vm_vec)
        die("slcbegin: must slice a vector");
    return;
}
inline void nasal_vm::opr_slcend()
{
    stack_top[-1]=stack_top[0];
    --stack_top;
    return;
}
inline void nasal_vm::opr_slc()
{
    nasal_val* val=(stack_top--)[0];
    nasal_val* res=stack_top[-1]->ptr.vec->get_val(val->to_number());
    if(!res)
        die("slc: index out of range:"+std::to_string(val->to_number()));
    stack_top[0]->ptr.vec->elems.push_back(res);
    return;
}
inline void nasal_vm::opr_slc2()
{
    nasal_val* val2=(stack_top--)[0];
    nasal_val* val1=(stack_top--)[0];
    std::vector<nasal_val*>& ref=stack_top[-1]->ptr.vec->elems;
    std::vector<nasal_val*>& aim=stack_top[0]->ptr.vec->elems;

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
        die("slc2: begin index must be less than end index");
    else if(num1<-ref_size || num1>=ref_size)
        die("slc2: begin index out of range: "+std::to_string(num1));
    else if(num2<-ref_size || num2>=ref_size)
        die("slc2: end index out of range: "+std::to_string(num2));
    else
        for(int i=num1;i<=num2;++i)
            aim.push_back(i>=0?ref[i]:ref[i+ref_size]);
    return;
}
inline void nasal_vm::opr_mcallg()
{
    mem_addr=gc.val_stack+imm[pc];
    (++stack_top)[0]=mem_addr[0];
    return;
}
inline void nasal_vm::opr_mcalll()
{
    mem_addr=&gc.local.back()[imm[pc]];
    (++stack_top)[0]=mem_addr[0];
    return;
}
inline void nasal_vm::opr_mcallv()
{
    nasal_val* val=stack_top[0];
    nasal_val* vec_addr=(--stack_top)[0];
    int type=vec_addr->type;
    if(type==vm_vec)
    {
        mem_addr=vec_addr->ptr.vec->get_mem(val->to_number());
        if(!mem_addr)
            die("mcallv: index out of range:"+std::to_string(val->to_number()));
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
inline void nasal_vm::opr_mcallh()
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
inline void nasal_vm::opr_ret()
{
    gc.local.pop_back();// delete local scope
    pc=ret.top();ret.pop();// fetch pc
    (--stack_top)[0]->ptr.func->closure[0]=gc.nil_addr;// set 'me' to nil
    stack_top[0]=stack_top[1];// rewrite nasal_func with returned value
    return;
}
void nasal_vm::run(std::vector<opcode>& exec,bool op_cnt)
{
    uint64_t count[op_ret+1]={0};
    void* opr_table[]=
    {
        &&nop,     &&intg,     &&intl,   &&offset,
        &&loadg,   &&loadl,    &&pnum,   &&pone,
        &&pzero,   &&pnil,     &&pstr,   &&newv,
        &&newh,    &&newf,     &&happ,   &&para,
        &&defpara, &&dynpara,  &&unot,   &&usub,
        &&add,     &&sub,      &&mul,    &&div,
        &&lnk,     &&addc,     &&subc,   &&mulc,
        &&divc,    &&lnkc,     &&addeq,  &&subeq,
        &&muleq,   &&diveq,    &&lnkeq,  &&addeqc,
        &&subeqc,  &&muleqc,   &&diveqc, &&lnkeqc,
        &&meq,     &&eq,       &&neq,    &&less,
        &&leq,     &&grt,      &&geq,    &&lessc,
        &&leqc,    &&grtc,     &&geqc,   &&pop,
        &&jmp,     &&jt,       &&jf,     &&counter,
        &&cntpop,  &&findex,   &&feach,  &&callg,
        &&calll,   &&callv,    &&callvi, &&callh,
        &&callfv,  &&callfh,   &&callb,  &&slcbegin,
        &&slcend,  &&slc,      &&slc2,   &&mcallg,
        &&mcalll,  &&mcallv,   &&mcallh, &&ret
    };
    
    bytecode=exec;
    std::vector<void*> code;
    for(auto& i:exec)
    {
        code.push_back(opr_table[i.op]);
        imm.push_back(i.num);
    }

    // set canary and program counter
    auto& canary=gc.val_stack[STACK_MAX_DEPTH-1];
    pc=0;
    // run
    goto *code[pc];

nop:
    if(canary && canary!=(nasal_val*)0xffff)
        stackoverflow();
    if(op_cnt)
    {
        std::cout<<std::endl;
        for(int i=0;i<15;++i)
        {
            uint64_t maxnum=0,index=0;
            for(int j=0;j<op_ret+1;++j)
                if(count[j]>maxnum)
                {
                    index=j;
                    maxnum=count[j];
                }
            std::cout<<code_table[index].name<<": "<<maxnum<<"\n";
            count[index]=0;
        }
    }
    return;
#define exec_operand(op,num) {op();++count[num];if(!canary)goto *code[++pc];goto nop;}
#define exec_opnodie(op,num) {op();++count[num];goto *code[++pc];}

intg:    exec_opnodie(opr_intg    ,op_intg    );
intl:    exec_opnodie(opr_intl    ,op_intl    );
offset:  exec_opnodie(opr_offset  ,op_offset  );
loadg:   exec_opnodie(opr_loadg   ,op_loadg   );
loadl:   exec_opnodie(opr_loadl   ,op_loadl   );
pnum:    exec_operand(opr_pnum    ,op_pnum    );
pone:    exec_operand(opr_pone    ,op_pone    );
pzero:   exec_operand(opr_pzero   ,op_pzero   );
pnil:    exec_operand(opr_pnil    ,op_pnil    );
pstr:    exec_operand(opr_pstr    ,op_pstr    );
newv:    exec_operand(opr_newv    ,op_newv    );
newh:    exec_operand(opr_newh    ,op_newh    );
newf:    exec_operand(opr_newf    ,op_newf    );
happ:    exec_opnodie(opr_happ    ,op_happ    );
para:    exec_opnodie(opr_para    ,op_para    );
defpara: exec_opnodie(opr_defpara ,op_defpara );
dynpara: exec_opnodie(opr_dynpara ,op_dynpara );
unot:    exec_operand(opr_unot    ,op_unot    );
usub:    exec_opnodie(opr_usub    ,op_usub    );
add:     exec_opnodie(opr_add     ,op_add     );
sub:     exec_opnodie(opr_sub     ,op_sub     );
mul:     exec_opnodie(opr_mul     ,op_mul     );
div:     exec_opnodie(opr_div     ,op_div     );
lnk:     exec_opnodie(opr_lnk     ,op_lnk     );
addc:    exec_opnodie(opr_addc    ,op_addc    );
subc:    exec_opnodie(opr_subc    ,op_subc    );
mulc:    exec_opnodie(opr_mulc    ,op_mulc    );
divc:    exec_opnodie(opr_divc    ,op_divc    );
lnkc:    exec_opnodie(opr_lnkc    ,op_lnkc    );
addeq:   exec_opnodie(opr_addeq   ,op_addeq   );
subeq:   exec_opnodie(opr_subeq   ,op_subeq   );
muleq:   exec_opnodie(opr_muleq   ,op_muleq   );
diveq:   exec_opnodie(opr_diveq   ,op_diveq   );
lnkeq:   exec_opnodie(opr_lnkeq   ,op_lnkeq   );
addeqc:  exec_opnodie(opr_addeqc  ,op_addeqc  );
subeqc:  exec_opnodie(opr_subeqc  ,op_subeqc  );
muleqc:  exec_opnodie(opr_muleqc  ,op_muleqc  );
diveqc:  exec_opnodie(opr_diveqc  ,op_diveqc  );
lnkeqc:  exec_opnodie(opr_lnkeqc  ,op_lnkeqc  );
meq:     exec_opnodie(opr_meq     ,op_meq     );
eq:      exec_opnodie(opr_eq      ,op_eq      );
neq:     exec_opnodie(opr_neq     ,op_neq     );
less:    exec_opnodie(opr_less    ,op_less    );
leq:     exec_opnodie(opr_leq     ,op_leq     );
grt:     exec_opnodie(opr_grt     ,op_grt     );
geq:     exec_opnodie(opr_geq     ,op_geq     );
lessc:   exec_opnodie(opr_lessc   ,op_lessc   );
leqc:    exec_opnodie(opr_leqc    ,op_leqc    );
grtc:    exec_opnodie(opr_grtc    ,op_grtc    );
geqc:    exec_opnodie(opr_geqc    ,op_geqc    );
pop:     exec_opnodie(opr_pop     ,op_pop     );
jmp:     exec_opnodie(opr_jmp     ,op_jmp     );
jt:      exec_opnodie(opr_jt      ,op_jt      );
jf:      exec_opnodie(opr_jf      ,op_jf      );
counter: exec_operand(opr_counter ,op_cnt     );
cntpop:  exec_opnodie(opr_cntpop  ,op_cntpop  );
findex:  exec_opnodie(opr_findex  ,op_findex  );
feach:   exec_opnodie(opr_feach   ,op_feach   );
callg:   exec_opnodie(opr_callg   ,op_callg   );
calll:   exec_opnodie(opr_calll   ,op_calll   );
callv:   exec_operand(opr_callv   ,op_callv   );
callvi:  exec_operand(opr_callvi  ,op_callvi  );
callh:   exec_operand(opr_callh   ,op_callh   );
callfv:  exec_operand(opr_callfv  ,op_callfv  );
callfh:  exec_operand(opr_callfh  ,op_callfh  );
callb:   exec_operand(opr_callb   ,op_callb   );
slcbegin:exec_operand(opr_slcbegin,op_slcbegin);
slcend:  exec_opnodie(opr_slcend  ,op_slcend  );
slc:     exec_operand(opr_slc     ,op_slc     );
slc2:    exec_operand(opr_slc2    ,op_slc2    );
mcallg:  exec_opnodie(opr_mcallg  ,op_mcallg  );
mcalll:  exec_opnodie(opr_mcalll  ,op_mcalll  );
mcallv:  exec_operand(opr_mcallv  ,op_mcallv  );
mcallh:  exec_operand(opr_mcallh  ,op_mcallh  );
ret:     exec_opnodie(opr_ret     ,op_ret     );
}
#endif