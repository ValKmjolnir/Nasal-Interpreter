#ifndef __NASAL_VM_H__
#define __NASAL_VM_H__

class nasal_vm
{
protected:
    /* values of nasal_vm */
    uint32_t                 pc;       // program counter
    const double*            num_table;// const numbers, ref from nasal_codegen
    const std::string*       str_table;// const symbols, ref from nasal_codegen
    std::stack<nasal_func*>  fstk;     // stack to store function, used to get upvalues
    std::stack<nasal_ref*>   lstk;     // stack to store local scopes' begin address
    std::vector<uint32_t>    imm;      // immediate number
    nasal_ref*               mem_addr; // used for mem_call
    /* garbage collector */
    nasal_gc                 gc;
    /* values used for debug */
    size_t                   files_size;
    const std::string*       files;    // ref from nasal_import
    const opcode*            bytecode; // ref from nasal_codegen
    /* canary to avoid stackoverflow */
    nasal_ref*               canary;

    void init(
        const std::vector<std::string>&,
        const std::vector<double>&,
        const std::vector<opcode>&,
        const std::vector<std::string>&);
    /* debug functions */
    bool detail_info;
    void valinfo(nasal_ref&);
    void bytecodeinfo(const char*,const uint32_t);
    void traceback();
    void stackinfo(const uint32_t);
    void global_state();
    void local_state();
    void upval_state();
    void detail();
    void opcallsort(const uint64_t*);
    void die(std::string);
    /* vm calculation functions*/
    bool condition(nasal_ref);
    /* vm operands */
    void opr_intg();
    void opr_intl();
    void opr_loadg();
    void opr_loadl();
    void opr_loadu();
    void opr_pnum();
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
    void opr_findex();
    void opr_feach();
    void opr_callg();
    void opr_calll();
    void opr_upval();
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
    void opr_mupval();
    void opr_mcallv();
    void opr_mcallh();
    void opr_ret();
public:
    void run(
        const nasal_codegen&,
        const nasal_import&,
        const bool,
        const bool);
};

void nasal_vm::init(
    const std::vector<std::string>& strs,
    const std::vector<double>&      nums,
    const std::vector<opcode>&      code,
    const std::vector<std::string>& filenames)
{
    gc.init(strs);
    num_table=nums.data();
    str_table=strs.data();
    bytecode=code.data();
    files=filenames.data();
    files_size=filenames.size();
    /* set canary and program counter */
    canary=gc.stack+STACK_MAX_DEPTH-1;
    pc=0;
}
void nasal_vm::valinfo(nasal_ref& val)
{
    const nasal_val* p=val.value.gcobj;
    printf("\t");
    switch(val.type)
    {
        case vm_none: printf("| null |\n");break;
        case vm_ret:  printf("| addr | pc:0x%x\n",val.ret());break;
        case vm_cnt:  printf("| cnt  | %ld\n",val.cnt());break;
        case vm_nil:  printf("| nil  |\n");break;
        case vm_num:  printf("| num  | ");std::cout<<val.num()<<'\n';break;
        case vm_str:  printf("| str  | <0x%lx> %s\n",(uint64_t)p,rawstr(*val.str()).c_str());break;
        case vm_func: printf("| func | <0x%lx> entry:0x%x\n",(uint64_t)p,val.func()->entry);break;
        case vm_vec:  printf("| vec  | <0x%lx> [%lu val]\n",(uint64_t)p,val.vec()->elems.size());break;
        case vm_hash: printf("| hash | <0x%lx> {%lu val}\n",(uint64_t)p,val.hash()->elems.size());break;
        case vm_obj:  printf("| obj  | <0x%lx> obj:0x%lx\n",(uint64_t)p,(uint64_t)val.obj()->ptr);break;
        default:      printf("| err  | <0x%lx> unknown object\n",(uint64_t)p);break;
    }
}
void nasal_vm::bytecodeinfo(const char* header,const uint32_t p)
{
    const opcode& c=bytecode[p];
    printf("%s0x%.8x: %s 0x%x",header,p,code_table[c.op].name,c.num);
    switch(c.op)
    {
        case op_addc:  case op_subc:   case op_mulc:  case op_divc:
        case op_addeqc:case op_subeqc: case op_muleqc:case op_diveqc:
        case op_lessc: case op_leqc:   case op_grtc:  case op_geqc:
        case op_pnum:
            std::cout<<" ("<<num_table[c.num]<<")";break;
        case op_callb:
            printf(" <%s@0x%lx>",builtin[c.num].name,(uint64_t)builtin[c.num].func);break;
        case op_happ:  case op_pstr:
        case op_lnkc:  case op_lnkeqc:
        case op_callh: case op_mcallh:
        case op_para:  case op_defpara:case op_dynpara:
            printf(" (\"%s\")",rawstr(str_table[c.num]).c_str());break;
        case op_upval:case op_mupval:  case op_loadu:
            printf(" (0x%x[0x%x])",(c.num>>16)&0xffff,c.num&0xffff);break;
        default:break;
    }
    printf(" (%s:%d)\n",files[c.fidx].c_str(),c.line);
}
void nasal_vm::traceback()
{
    uint32_t global_size=bytecode[0].num; // bytecode[0] is op_intg
    nasal_ref* top=gc.top;
    nasal_ref* bottom=gc.stack+global_size;
    std::stack<uint32_t> ret;
    for(nasal_ref* i=bottom;i<=top;++i)
        if(i->type==vm_ret)
            ret.push(i->ret());
    // push pc to ret stack to store the position program crashed
    ret.push(pc);
    printf("trace back:\n");
    uint32_t same=0,last=0xffffffff;
    for(uint32_t point=0;!ret.empty();last=point,ret.pop())
    {
        if((point=ret.top())==last)
        {
            ++same;
            continue;
        }
        if(same)
            printf("\t0x%.8x: %d same call(s)\n",last,same);
        same=0;
        bytecodeinfo("\t",point);
    }
    if(same)
        printf("\t0x%.8x: %d same call(s)\n",last,same);
}
void nasal_vm::stackinfo(const uint32_t limit=10)
{
    uint32_t global_size=bytecode[0].num; // bytecode[0] is op_intg
    nasal_ref* top=gc.top;
    nasal_ref* bottom=gc.stack+global_size;
    if(top<bottom)
    {
        printf("vm stack(limit %d, total 0)\n",limit);
        return;
    }
    printf("vm stack(limit %d, total %ld):\n",limit,top-bottom+1);
    for(uint32_t i=0;i<limit && top>=bottom;++i,--top)
    {
        printf("0x%.8lx",top-gc.stack);
        valinfo(top[0]);
    }
}
void nasal_vm::global_state()
{
    if(!bytecode[0].num || gc.stack[0].type==vm_none) // bytecode[0].op is op_intg
    {
        printf("no global value exists\n");
        return;
    }
    printf("global(base %p):\n",gc.stack);
    for(uint32_t i=0;i<bytecode[0].num;++i)
    {
        printf("0x%.8x",i);
        valinfo(gc.stack[i]);
    }
}
void nasal_vm::local_state()
{
    if(lstk.empty() || !fstk.top()->lsize)
    {
        printf("no local value exists\n");
        return;
    }
    printf("local(base %p):\n",lstk.top());
    for(uint32_t i=0;i<fstk.top()->lsize;++i)
    {
        printf("0x%.8x",i);
        valinfo(lstk.top()[i]);
    }
}
void nasal_vm::upval_state()
{
    if(fstk.empty() || fstk.top()->upvalue.empty())
    {
        printf("no upvalue exists\n");
        return;
    }
    printf("upvalue:\n");
    auto& upval=fstk.top()->upvalue;
    for(uint32_t i=0;i<upval.size();++i)
    {
        printf("-> upval[%u]:\n",i);
        auto& uv=upval[i].upval();
        for(uint32_t j=0;j<uv.size;++j)
        {
            printf("   0x%.8x",j);
            valinfo(uv[j]);
        }
    }
}
void nasal_vm::detail()
{
    printf("maddr: 0x%lx\n",(uint64_t)mem_addr);
    global_state();
    local_state();
    upval_state();
}
void nasal_vm::opcallsort(const uint64_t* arr)
{
    typedef std::pair<uint32_t,uint64_t> op;
    std::vector<op> opcall;
    for(uint32_t i=0;i<=op_ret;++i)
        opcall.push_back({i,arr[i]});
    std::sort(
        opcall.begin(),
        opcall.end(),
        [](op& a,op& b){return a.second>b.second;}
    );
    uint64_t total=0;
    for(auto& i:opcall)
    {
        if(!i.second)
            break;
        total+=i.second;
        std::cout<<'\n'<<code_table[i.first].name<<": "<<i.second;
    }
    std::cout<<"\ntotal : "<<total<<'\n';
}
void nasal_vm::die(std::string str)
{
    printf("[vm] %s\n",str.c_str());
    traceback();
    stackinfo();
    if(detail_info)
        detail();
    std::exit(1);
}
inline bool nasal_vm::condition(nasal_ref val)
{
    if(val.type==vm_num)
        return val.value.num;
    else if(val.type==vm_str)
    {
        double num=str2num(val.str()->c_str());
        if(std::isnan(num))
            return !val.str()->empty();
        return num;
    }
    return false;
}
inline void nasal_vm::opr_intg()
{
    // global values store on stack
    for(uint32_t i=0;i<imm[pc];++i)
        (gc.top++)[0].type=vm_nil;
    --gc.top;// point to the top
}
inline void nasal_vm::opr_intl()
{
    gc.top[0].func()->local.resize(imm[pc],nil);
    gc.top[0].func()->lsize=imm[pc];
}
inline void nasal_vm::opr_loadg()
{
    gc.stack[imm[pc]]=(gc.top--)[0];
}
inline void nasal_vm::opr_loadl()
{
    lstk.top()[imm[pc]]=(gc.top--)[0];
}
inline void nasal_vm::opr_loadu()
{
    fstk.top()->upvalue[(imm[pc]>>16)&0xffff].upval()[imm[pc]&0xffff]=(gc.top--)[0];
}
inline void nasal_vm::opr_pnum()
{
    (++gc.top)[0]={vm_num,num_table[imm[pc]]};
}
inline void nasal_vm::opr_pnil()
{
    (++gc.top)[0]={vm_nil,(double)0};
}
inline void nasal_vm::opr_pstr()
{
    (++gc.top)[0]=gc.strs[imm[pc]];
}
inline void nasal_vm::opr_newv()
{
    nasal_ref newv=gc.alloc(vm_vec);
    auto& vec=newv.vec()->elems;
    vec.resize(imm[pc]);
    // use top-=imm[pc]-1 here will cause error if imm[pc] is 0
    gc.top=gc.top-imm[pc]+1;
    for(uint32_t i=0;i<imm[pc];++i)
        vec[i]=gc.top[i];
    gc.top[0]=newv;
}
inline void nasal_vm::opr_newh()
{
    (++gc.top)[0]=gc.alloc(vm_hash);
}
inline void nasal_vm::opr_newf()
{
    (++gc.top)[0]=gc.alloc(vm_func);
    nasal_func* func=gc.top[0].func();
    func->entry=imm[pc];
    func->psize=1;

    /* this means you create a new function in local scope */
    if(!lstk.empty())
    {
        func->upvalue=fstk.top()->upvalue;
        nasal_ref upval=(gc.upvalue.back().type==vm_nil)?gc.alloc(vm_upval):gc.upvalue.back();
        upval.upval().size=fstk.top()->lsize;
        upval.upval().stk=lstk.top();
        func->upvalue.push_back(upval);
        gc.upvalue.back()=upval;
    }
}
inline void nasal_vm::opr_happ()
{
    gc.top[-1].hash()->elems[str_table[imm[pc]]]=gc.top[0];
    --gc.top;
}
inline void nasal_vm::opr_para()
{
    nasal_func* func=gc.top[0].func();
    func->keys[str_table[imm[pc]]]=func->psize;// func->size has 1 place reserved for "me"
    func->local[func->psize++]={vm_none};
}
inline void nasal_vm::opr_defpara()
{
    nasal_ref val=gc.top[0];
    nasal_func* func=(--gc.top)[0].func();
    func->keys[str_table[imm[pc]]]=func->psize;// func->size has 1 place reserved for "me"
    func->local[func->psize++]=val;
}
inline void nasal_vm::opr_dynpara()
{
    gc.top[0].func()->dynpara=imm[pc];
}
inline void nasal_vm::opr_unot()
{
    nasal_ref val=gc.top[0];
    switch(val.type)
    {
        case vm_nil:gc.top[0]=one;break;
        case vm_num:gc.top[0]=val.num()?zero:one;break;
        case vm_str:
        {
            double num=str2num(val.str()->c_str());
            if(std::isnan(num))
                gc.top[0]={vm_num,(double)val.str()->empty()};
            else
                gc.top[0]=num?zero:one;
        }
        break;
        default:die("unot: incorrect value type");break;
    }
}
inline void nasal_vm::opr_usub()
{
    gc.top[0]={vm_num,-gc.top[0].to_number()};
}

#define op_calc(type)\
    nasal_ref val(vm_num,gc.top[-1].to_number() type gc.top[0].to_number());\
    (--gc.top)[0]=val;

inline void nasal_vm::opr_add(){op_calc(+);}
inline void nasal_vm::opr_sub(){op_calc(-);}
inline void nasal_vm::opr_mul(){op_calc(*);}
inline void nasal_vm::opr_div(){op_calc(/);}
inline void nasal_vm::opr_lnk()
{
    nasal_ref val=gc.alloc(vm_str);
    *val.str()=gc.top[-1].to_string()+gc.top[0].to_string();
    (--gc.top)[0]=val;
}

#define op_calc_const(type)\
    nasal_ref val(vm_num,gc.top[0].to_number() type num_table[imm[pc]]);\
    gc.top[0]=val;

inline void nasal_vm::opr_addc(){op_calc_const(+);}
inline void nasal_vm::opr_subc(){op_calc_const(-);}
inline void nasal_vm::opr_mulc(){op_calc_const(*);}
inline void nasal_vm::opr_divc(){op_calc_const(/);}
inline void nasal_vm::opr_lnkc()
{
    nasal_ref val=gc.alloc(vm_str);
    *val.str()=gc.top[0].to_string()+str_table[imm[pc]];
    gc.top[0]=val;
}

#define op_calc_eq(type)\
    nasal_ref val(vm_num,mem_addr[0].to_number() type gc.top[-1].to_number());\
    (--gc.top)[0]=mem_addr[0]=val;

inline void nasal_vm::opr_addeq(){op_calc_eq(+);}
inline void nasal_vm::opr_subeq(){op_calc_eq(-);}
inline void nasal_vm::opr_muleq(){op_calc_eq(*);}
inline void nasal_vm::opr_diveq(){op_calc_eq(/);}
inline void nasal_vm::opr_lnkeq()
{
    nasal_ref val=gc.alloc(vm_str);
    *val.str()=mem_addr[0].to_string()+gc.top[-1].to_string();
    (--gc.top)[0]=mem_addr[0]=val;
}

#define op_calc_eq_const(type)\
    nasal_ref val(vm_num,mem_addr[0].to_number() type num_table[imm[pc]]);\
    gc.top[0]=mem_addr[0]=val;

inline void nasal_vm::opr_addeqc(){op_calc_eq_const(+);}
inline void nasal_vm::opr_subeqc(){op_calc_eq_const(-);}
inline void nasal_vm::opr_muleqc(){op_calc_eq_const(*);}
inline void nasal_vm::opr_diveqc(){op_calc_eq_const(/);}
inline void nasal_vm::opr_lnkeqc()
{
    nasal_ref val=gc.alloc(vm_str);
    *val.str()=mem_addr[0].to_string()+str_table[imm[pc]];
    gc.top[0]=mem_addr[0]=val;
}

inline void nasal_vm::opr_meq()
{
    mem_addr[0]=(--gc.top)[0];
}
inline void nasal_vm::opr_eq()
{
    nasal_ref val2=gc.top[0];
    nasal_ref val1=(--gc.top)[0];
    if(val1.type==vm_nil && val2.type==vm_nil)
        gc.top[0]=one;
    else if(val1.type==vm_str && val2.type==vm_str)
        gc.top[0]=(*val1.str()==*val2.str())?one:zero;
    else if(val1.type==vm_num || val2.type==vm_num)
        gc.top[0]=(val1.to_number()==val2.to_number())?one:zero;
    else
        gc.top[0]=(val1==val2)?one:zero;
}
inline void nasal_vm::opr_neq()
{
    nasal_ref val2=gc.top[0];
    nasal_ref val1=(--gc.top)[0];
    if(val1.type==vm_nil && val2.type==vm_nil)
        gc.top[0]=zero;
    else if(val1.type==vm_str && val2.type==vm_str)
        gc.top[0]=(*val1.str()!=*val2.str())?one:zero;
    else if(val1.type==vm_num || val2.type==vm_num)
        gc.top[0]=(val1.to_number()!=val2.to_number())?one:zero;
    else
        gc.top[0]=(val1!=val2)?one:zero;
}

#define op_cmp(type)\
    --gc.top;\
    gc.top[0]=(gc.top[0].to_number() type gc.top[1].to_number())?one:zero;

inline void nasal_vm::opr_less(){op_cmp(<);}
inline void nasal_vm::opr_leq(){op_cmp(<=);}
inline void nasal_vm::opr_grt(){op_cmp(>);}
inline void nasal_vm::opr_geq(){op_cmp(>=);}

#define op_cmp_const(type)\
    gc.top[0]=(gc.top[0].to_number() type num_table[imm[pc]])?one:zero;

inline void nasal_vm::opr_lessc(){op_cmp_const(<);}
inline void nasal_vm::opr_leqc(){op_cmp_const(<=);}
inline void nasal_vm::opr_grtc(){op_cmp_const(>);}
inline void nasal_vm::opr_geqc(){op_cmp_const(>=);}

inline void nasal_vm::opr_pop()
{
    --gc.top;
}
inline void nasal_vm::opr_jmp()
{
    pc=imm[pc]-1;
}
inline void nasal_vm::opr_jt()
{
    if(condition(gc.top[0]))
        pc=imm[pc]-1;
}
inline void nasal_vm::opr_jf()
{
    if(!condition(gc.top[0]))
        pc=imm[pc]-1;
    --gc.top;
}
inline void nasal_vm::opr_counter()
{
    (++gc.top)[0]={vm_cnt,(int64_t)-1};
    if(gc.top[-1].type!=vm_vec)
        die("cnt: must use vector in forindex/foreach");
}
inline void nasal_vm::opr_findex()
{
    if(++gc.top[0].cnt()>=gc.top[-1].vec()->elems.size())
    {
        pc=imm[pc]-1;
        return;
    }
    gc.top[1]={vm_num,(double)gc.top[0].cnt()};
    ++gc.top;
}
inline void nasal_vm::opr_feach()
{
    std::vector<nasal_ref>& ref=gc.top[-1].vec()->elems;
    if(++gc.top[0].cnt()>=ref.size())
    {
        pc=imm[pc]-1;
        return;
    }
    gc.top[1]=ref[gc.top[0].cnt()];
    ++gc.top;
}
inline void nasal_vm::opr_callg()
{
    (++gc.top)[0]=gc.stack[imm[pc]];
}
inline void nasal_vm::opr_calll()
{
    (++gc.top)[0]=lstk.top()[imm[pc]];
}
inline void nasal_vm::opr_upval()
{
    (++gc.top)[0]=fstk.top()->upvalue[(imm[pc]>>16)&0xffff].upval()[imm[pc]&0xffff];
}
inline void nasal_vm::opr_callv()
{
    nasal_ref val=gc.top[0];
    nasal_ref vec=(--gc.top)[0];
    if(vec.type==vm_vec)
    {
        gc.top[0]=vec.vec()->get_val(val.to_number());
        if(gc.top[0].type==vm_none)
            die("callv: index out of range:"+std::to_string(val.to_number()));
    }
    else if(vec.type==vm_hash)
    {
        if(val.type!=vm_str)
            die("callv: must use string as the key");
        gc.top[0]=vec.hash()->get_val(*val.str());
        if(gc.top[0].type==vm_none)
            die("callv: cannot find member \""+*val.str()+"\" of this hash");
        if(gc.top[0].type==vm_func)
            gc.top[0].func()->local[0]=val;// 'me'
    }
    else if(vec.type==vm_str)
    {
        std::string& str=*vec.str();
        int num=val.to_number();
        int str_size=str.length();
        if(num<-str_size || num>=str_size)
            die("callv: index out of range:"+std::to_string(val.to_number()));
        gc.top[0]={vm_num,double((uint8_t)str[num>=0? num:num+str_size])};
    }
    else
        die("callv: must call a vector/hash/string");
}
inline void nasal_vm::opr_callvi()
{
    nasal_ref val=gc.top[0];
    if(val.type!=vm_vec)
        die("callvi: must use a vector");

    // cannot use operator[],because this may cause overflow
    (++gc.top)[0]=val.vec()->get_val(imm[pc]);
    if(gc.top[0].type==vm_none)
        die("callvi: index out of range:"+std::to_string(imm[pc]));
}
inline void nasal_vm::opr_callh()
{
    nasal_ref val=gc.top[0];
    if(val.type!=vm_hash)
        die("callh: must call a hash");

    gc.top[0]=val.hash()->get_val(str_table[imm[pc]]);
    if(gc.top[0].type==vm_none)
        die("callh: member \""+str_table[imm[pc]]+"\" does not exist");

    if(gc.top[0].type==vm_func)
        gc.top[0].func()->local[0]=val;// 'me'
}
inline void nasal_vm::opr_callfv()
{
    uint32_t   argc=imm[pc];      // arguments counter
    nasal_ref* args=gc.top-argc+1;// arguments begin address
    if(args[-1].type!=vm_func)
        die("callfv: must call a function");
    
    nasal_func* func=args[-1].func();
    if(gc.top-argc+func->lsize+1>=canary) // gc.top-argc+lsize(local) +1(pc)
        die("stack overflow");
    fstk.push(func);          // push called function into stack to provide upvalue
    lstk.push(gc.top-argc+1); // store begin of local scope

    uint32_t psize=func->psize-1; // parameter size is func->psize-1, 1 is reserved for "me"
    if(argc<psize && func->local[argc+1].type==vm_none)
        die("callfv: lack argument(s)");

    nasal_ref* local=args;
    nasal_ref  dynamic=nil;
    gc.top=local+func->lsize;
    if(func->dynpara>=0)// load dynamic arguments
    {
        dynamic=gc.alloc(vm_vec);
        for(uint32_t i=psize;i<argc;++i)
            dynamic.vec()->elems.push_back(args[i]);
    }
    uint32_t min_size=std::min(psize,argc);
    for(uint32_t i=min_size;i>=1;--i)// load arguments
        local[i]=local[i-1];
    local[0]=func->local[0];// load "me"
    for(uint32_t i=min_size+1;i<func->lsize;++i)// load local scope & default arguments
        local[i]=func->local[i];
    if(func->dynpara>=0)
        local[psize+1]=dynamic;

    gc.top[0]={vm_ret,pc};
    pc=func->entry-1;
    gc.upvalue.push_back(nil);
}
inline void nasal_vm::opr_callfh()
{
    auto& hash=gc.top[0].hash()->elems;
    if(gc.top[-1].type!=vm_func)
        die("callfh: must call a function");

    nasal_func* func=gc.top[-1].func();
    if(gc.top+func->lsize>=canary) // gc.top -1(hash) +lsize(local) +1(pc)
        die("stack overflow");
    if(func->dynpara>=0)
        die("callfh: special call cannot use dynamic argument");
    fstk.push(func);   // push called function into stack to provide upvalue
    lstk.push(gc.top); // store begin of local scope

    nasal_ref* local=gc.top;
    gc.top+=func->lsize;
    for(uint32_t i=0;i<func->lsize;++i)
        local[i]=func->local[i];
    
    for(auto& i:func->keys)
    {
        if(hash.count(i.first))
            local[i.second]=hash[i.first];
        else if(local[i.second].type==vm_none)
            die("callfh: lack argument(s): \""+i.first+"\"");
    }

    gc.top[0]={vm_ret,(uint32_t)pc}; // rewrite top with vm_ret
    pc=func->entry-1;
    gc.upvalue.push_back(nil);
}
inline void nasal_vm::opr_callb()
{
    (++gc.top)[0]=(*builtin[imm[pc]].func)(lstk.top(),gc);
    if(gc.top[0].type==vm_none)
        die("native function error.");
}
inline void nasal_vm::opr_slcbegin()
{
    // | slice_vector | <-- gc.top[0]
    // +--------------+
    // | resource_vec | <-- gc.top[-1]
    // +--------------+
    (++gc.top)[0]=gc.alloc(vm_vec);
    if(gc.top[-1].type!=vm_vec)
        die("slcbegin: must slice a vector");
}
inline void nasal_vm::opr_slcend()
{
    gc.top[-1]=gc.top[0];
    --gc.top;
}
inline void nasal_vm::opr_slc()
{
    nasal_ref val=(gc.top--)[0];
    nasal_ref res=gc.top[-1].vec()->get_val(val.to_number());
    if(res.type==vm_none)
        die("slc: index out of range:"+std::to_string(val.to_number()));
    gc.top[0].vec()->elems.push_back(res);
}
inline void nasal_vm::opr_slc2()
{
    nasal_ref val2=(gc.top--)[0];
    nasal_ref val1=(gc.top--)[0];
    std::vector<nasal_ref>& ref=gc.top[-1].vec()->elems;
    std::vector<nasal_ref>& aim=gc.top[0].vec()->elems;

    uint8_t type1=val1.type,type2=val2.type;
    int num1=val1.to_number();
    int num2=val2.to_number();
    int size=ref.size();
    if(type1==vm_nil && type2==vm_nil)
    {
        num1=0;
        num2=size-1;
    }
    else if(type1==vm_nil && type2!=vm_nil)
        num1=num2<0? -size:0;
    else if(type1!=vm_nil && type2==vm_nil)
        num2=num1<0? -1:size-1;

    if(num1>=num2)
        die("slc2: begin index must be less than end index");
    else if(num1<-size || num1>=size)
        die("slc2: begin index out of range: "+std::to_string(num1));
    else if(num2<-size || num2>=size)
        die("slc2: end index out of range: "+std::to_string(num2));
    else
        for(int i=num1;i<=num2;++i)
            aim.push_back(i>=0?ref[i]:ref[i+size]);
}
inline void nasal_vm::opr_mcallg()
{
    mem_addr=gc.stack+imm[pc];
    (++gc.top)[0]=mem_addr[0];
}
inline void nasal_vm::opr_mcalll()
{
    mem_addr=lstk.top()+imm[pc];
    (++gc.top)[0]=mem_addr[0];
}
inline void nasal_vm::opr_mupval()
{
    mem_addr=&(fstk.top()->upvalue[(imm[pc]>>16)&0xffff].upval()[imm[pc]&0xffff]);
    (++gc.top)[0]=mem_addr[0];
}
inline void nasal_vm::opr_mcallv()
{
    nasal_ref val=gc.top[0];
    nasal_ref vec=(--gc.top)[0];
    if(vec.type==vm_vec)
    {
        mem_addr=vec.vec()->get_mem(val.to_number());
        if(!mem_addr)
            die("mcallv: index out of range:"+std::to_string(val.to_number()));
    }
    else if(vec.type==vm_hash)
    {
        if(val.type!=vm_str)
            die("mcallv: must use string as the key");
        nasal_hash& ref=*vec.hash();
        std::string& str=*val.str();
        mem_addr=ref.get_mem(str);
        if(!mem_addr)
        {
            ref.elems[str]={vm_nil};
            mem_addr=ref.get_mem(str);
        }
    }
    else
        die("mcallv: cannot get memory space in other types");
}
inline void nasal_vm::opr_mcallh()
{
    nasal_ref hash=gc.top[0];
    if(hash.type!=vm_hash)
        die("mcallh: must call a hash");
    nasal_hash& ref=*hash.hash();
    const std::string& str=str_table[imm[pc]];
    mem_addr=ref.get_mem(str);
    if(!mem_addr) // create a new key
    {
        ref.elems[str]={vm_nil};
        mem_addr=ref.get_mem(str);
    }
}
inline void nasal_vm::opr_ret()
{
    // | return value    | <- gc.top[0]
    // +-----------------+
    // | return address  | <- gc.top[-1]
    // +-----------------+
    // | local scope     |
    // +-----------------+ <- local pointer stored in lstk
    // | called function | <- func is stored on stack because gc may mark it
    // +-----------------+
    nasal_ref ret=gc.top[0];
    pc=gc.top[-1].ret();

    gc.top=lstk.top()-1;

    gc.top[0].func()->local[0]={vm_nil,nullptr}; // get func and set 'me' to nil
    gc.top[0]=ret; // rewrite func with returned value

    if(gc.upvalue.back().type==vm_upval) // synchronize upvalue
    {
        auto& upval=gc.upvalue.back().upval();
        auto local=lstk.top();
        auto size=fstk.top()->lsize;
        upval.onstk=false;
        for(uint32_t i=0;i<size;++i)
            upval.elems.push_back(local[i]);
    }
    lstk.pop();
    fstk.pop();
    gc.upvalue.pop_back();
}
void nasal_vm::run(
    const nasal_codegen& gen,
    const nasal_import& linker,
    const bool opcnt,
    const bool detail)
{
    detail_info=detail;
    init(gen.get_strs(),gen.get_nums(),gen.get_code(),linker.get_file());
    uint64_t count[op_ret+1]={0};
    const void* opr_table[]=
    {
        &&vmexit,  &&intg,     &&intl,   &&loadg,
        &&loadl,   &&loadu,    &&pnum,   &&pnil,
        &&pstr,    &&newv,     &&newh,   &&newf,
        &&happ,    &&para,     &&defpara,&&dynpara,
        &&unot,    &&usub,     &&add,    &&sub,
        &&mul,     &&div,      &&lnk,    &&addc,
        &&subc,    &&mulc,     &&divc,   &&lnkc,
        &&addeq,   &&subeq,    &&muleq,  &&diveq,
        &&lnkeq,   &&addeqc,   &&subeqc, &&muleqc,
        &&diveqc,  &&lnkeqc,   &&meq,    &&eq,
        &&neq,     &&less,     &&leq,    &&grt,
        &&geq,     &&lessc,    &&leqc,   &&grtc,
        &&geqc,    &&pop,      &&jmp,    &&jt,
        &&jf,      &&counter,  &&findex, &&feach,
        &&callg,   &&calll,    &&upval,  &&callv,
        &&callvi,  &&callh,    &&callfv, &&callfh,
        &&callb,   &&slcbegin, &&slcend, &&slc,
        &&slc2,    &&mcallg,   &&mcalll, &&mupval,
        &&mcallv,  &&mcallh,   &&ret
    };
    std::vector<const void*> code;
    for(auto& i:gen.get_code())
    {
        code.push_back(opr_table[i.op]);
        imm.push_back(i.num);
    }
    // goto the first operand
    goto *code[pc];

vmexit:
    if(gc.top>=canary)
        die("stack overflow");
    if(opcnt)
        opcallsort(count);
    if(detail_info)
        gc.info();
    gc.clear();
    imm.clear();
    return;
// may cause stackoverflow
#define exec_operand(op,num) {op();++count[num];if(gc.top<canary)goto *code[++pc];goto vmexit;}
// do not cause stackoverflow
#define exec_opnodie(op,num) {op();++count[num];goto *code[++pc];}

intg:    exec_opnodie(opr_intg    ,op_intg    ); // +imm[pc] (detected at codegen)
intl:    exec_opnodie(opr_intl    ,op_intl    ); // -0
loadg:   exec_opnodie(opr_loadg   ,op_loadg   ); // -1
loadl:   exec_opnodie(opr_loadl   ,op_loadl   ); // -1
loadu:   exec_opnodie(opr_loadu   ,op_loadu   ); // -1
pnum:    exec_operand(opr_pnum    ,op_pnum    ); // +1
pnil:    exec_operand(opr_pnil    ,op_pnil    ); // +1
pstr:    exec_operand(opr_pstr    ,op_pstr    ); // +1
newv:    exec_operand(opr_newv    ,op_newv    ); // +1-imm[pc]
newh:    exec_operand(opr_newh    ,op_newh    ); // +1
newf:    exec_operand(opr_newf    ,op_newf    ); // +1
happ:    exec_opnodie(opr_happ    ,op_happ    ); // -1
para:    exec_opnodie(opr_para    ,op_para    ); // -0
defpara: exec_opnodie(opr_defpara ,op_defpara ); // -1
dynpara: exec_opnodie(opr_dynpara ,op_dynpara ); // -0
unot:    exec_opnodie(opr_unot    ,op_unot    ); // -0
usub:    exec_opnodie(opr_usub    ,op_usub    ); // -0
add:     exec_opnodie(opr_add     ,op_add     ); // -1
sub:     exec_opnodie(opr_sub     ,op_sub     ); // -1
mul:     exec_opnodie(opr_mul     ,op_mul     ); // -1
div:     exec_opnodie(opr_div     ,op_div     ); // -1
lnk:     exec_opnodie(opr_lnk     ,op_lnk     ); // -1
addc:    exec_opnodie(opr_addc    ,op_addc    ); // -0
subc:    exec_opnodie(opr_subc    ,op_subc    ); // -0
mulc:    exec_opnodie(opr_mulc    ,op_mulc    ); // -0
divc:    exec_opnodie(opr_divc    ,op_divc    ); // -0
lnkc:    exec_opnodie(opr_lnkc    ,op_lnkc    ); // -0
addeq:   exec_opnodie(opr_addeq   ,op_addeq   ); // -1
subeq:   exec_opnodie(opr_subeq   ,op_subeq   ); // -1
muleq:   exec_opnodie(opr_muleq   ,op_muleq   ); // -1
diveq:   exec_opnodie(opr_diveq   ,op_diveq   ); // -1
lnkeq:   exec_opnodie(opr_lnkeq   ,op_lnkeq   ); // -1
addeqc:  exec_opnodie(opr_addeqc  ,op_addeqc  ); // -0
subeqc:  exec_opnodie(opr_subeqc  ,op_subeqc  ); // -0
muleqc:  exec_opnodie(opr_muleqc  ,op_muleqc  ); // -0
diveqc:  exec_opnodie(opr_diveqc  ,op_diveqc  ); // -0
lnkeqc:  exec_opnodie(opr_lnkeqc  ,op_lnkeqc  ); // -0
meq:     exec_opnodie(opr_meq     ,op_meq     ); // -1
eq:      exec_opnodie(opr_eq      ,op_eq      ); // -1
neq:     exec_opnodie(opr_neq     ,op_neq     ); // -1
less:    exec_opnodie(opr_less    ,op_less    ); // -1
leq:     exec_opnodie(opr_leq     ,op_leq     ); // -1
grt:     exec_opnodie(opr_grt     ,op_grt     ); // -1
geq:     exec_opnodie(opr_geq     ,op_geq     ); // -1
lessc:   exec_opnodie(opr_lessc   ,op_lessc   ); // -0
leqc:    exec_opnodie(opr_leqc    ,op_leqc    ); // -0
grtc:    exec_opnodie(opr_grtc    ,op_grtc    ); // -0
geqc:    exec_opnodie(opr_geqc    ,op_geqc    ); // -0
pop:     exec_opnodie(opr_pop     ,op_pop     ); // -1
jmp:     exec_opnodie(opr_jmp     ,op_jmp     ); // -0
jt:      exec_opnodie(opr_jt      ,op_jt      ); // -0
jf:      exec_opnodie(opr_jf      ,op_jf      ); // -1
counter: exec_opnodie(opr_counter ,op_cnt     ); // -0
findex:  exec_operand(opr_findex  ,op_findex  ); // +1
feach:   exec_operand(opr_feach   ,op_feach   ); // +1
callg:   exec_operand(opr_callg   ,op_callg   ); // +1
calll:   exec_operand(opr_calll   ,op_calll   ); // +1
upval:   exec_operand(opr_upval   ,op_upval   ); // +1
callv:   exec_opnodie(opr_callv   ,op_callv   ); // -0
callvi:  exec_opnodie(opr_callvi  ,op_callvi  ); // -0
callh:   exec_opnodie(opr_callh   ,op_callh   ); // -0
callfv:  exec_opnodie(opr_callfv  ,op_callfv  ); // check in the function
callfh:  exec_opnodie(opr_callfh  ,op_callfh  ); // check in the function
callb:   exec_opnodie(opr_callb   ,op_callb   ); // -0
slcbegin:exec_operand(opr_slcbegin,op_slcbegin); // +1
slcend:  exec_opnodie(opr_slcend  ,op_slcend  ); // -1
slc:     exec_opnodie(opr_slc     ,op_slc     ); // -1
slc2:    exec_opnodie(opr_slc2    ,op_slc2    ); // -2
mcallg:  exec_operand(opr_mcallg  ,op_mcallg  ); // +1
mcalll:  exec_operand(opr_mcalll  ,op_mcalll  ); // +1
mupval:  exec_operand(opr_mupval  ,op_mupval  ); // +1
mcallv:  exec_opnodie(opr_mcallv  ,op_mcallv  ); // -0
mcallh:  exec_opnodie(opr_mcallh  ,op_mcallh  ); // -0
ret:     exec_opnodie(opr_ret     ,op_ret     ); // -1
}
#endif