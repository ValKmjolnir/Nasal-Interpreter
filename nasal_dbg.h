#ifndef __NASAL_DBG_H__
#define __NASAL_DBG_H__

#include "nasal_vm.h"

class nasal_dbg:public nasal_vm
{
private:
    bool  next;
    usize fsize;
    u16   bk_fidx;
    u32   bk_line;
    nasal_err& src;

    std::vector<string> parse(const string&);
    u16 fileindex(const string&);
    void err();
    void help();
    void opcallsort(const u64*);
    void stepinfo();
    void interact();
public:
    nasal_dbg(nasal_err& nerr):
        next(false),fsize(0),
        bk_fidx(0),bk_line(0),
        src(nerr){}
    void run(
        const nasal_codegen&,
        const nasal_import&,
        const std::vector<string>&,
        bool
    );
};

std::vector<string> nasal_dbg::parse(const string& cmd)
{
    std::vector<string> res;
    usize last=0,pos=cmd.find(" ",0);
    while(pos!=string::npos)
    {
        if(pos>last)
            res.push_back(cmd.substr(last,pos-last));
        last=pos+1;
        pos=cmd.find(" ",last);
    }
    if(last<cmd.length())
        res.push_back(cmd.substr(last));
    return res;
}

u16 nasal_dbg::fileindex(const string& filename)
{
    for(u16 i=0;i<fsize;++i)
        if(filename==files[i])
            return i;
    return 65535;
}

void nasal_dbg::err()
{
    std::cerr
    <<"incorrect command\n"
    <<"input \'h\' to get help\n";
}

void nasal_dbg::help()
{
    std::cout
    <<"<option>\n"
    <<"\th,   help      | get help\n"
    <<"\tbt,  backtrace | get function call trace\n"
    <<"\tc,   continue  | run program until break point or exit\n"
    <<"\tf,   file      | see all the compiled files\n"
    <<"\tg,   global    | see global values\n"
    <<"\tl,   local     | see local values\n"
    <<"\tu,   upval     | see upvalue\n"
    <<"\tr,   register  | show vm register detail\n"
    <<"\ta,   all       | show global,local and upvalue\n"
    <<"\tn,   next      | execute next bytecode\n"
    <<"\tq,   exit      | exit debugger\n"
    <<"<option> <filename> <line>\n"
    <<"\tbk,  break     | set break point\n";
}

void nasal_dbg::opcallsort(const u64* arr)
{
    typedef std::pair<u32,u64> op;
    std::vector<op> opcall;
    u64 total=0;
    for(u32 i=0;i<op_ret+1;++i)
    {
        total+=arr[i];
        opcall.push_back({i,arr[i]});
    }
    std::sort(opcall.begin(),opcall.end(),
        [](const op& a,const op& b){return a.second>b.second;}
    );
    std::clog<<"\noperands call info";
    for(auto& i:opcall)
    {
        u64 rate=i.second*100/total;
        if(!rate)
        {
            std::clog<<"\n ...";
            break;
        }
        std::clog<<"\n "<<code_table[i.first].name
                 <<" : "<<i.second<<" ("<<rate<<"%)";
    }
    std::clog<<"\n total  : "<<total<<'\n';
}

void nasal_dbg::stepinfo()
{
    u32 begin,end;
    u32 line=bytecode[pc].line==0?0:bytecode[pc].line-1;
    src.load(files[bytecode[pc].fidx]);
    std::cout<<"\nsource code:\n";
    begin=(line>>3)==0?0:((line>>3)<<3);
    end=(1+(line>>3))<<3;
    for(u32 i=begin;i<end && i<src.size();++i)
        std::cout<<(i==line?"-->  ":"     ")<<src[i]<<"\n";
    std::cout<<"next bytecode:\n";
    begin=(pc>>3)==0?0:((pc>>3)<<3);
    end=(1+(pc>>3))<<3;
    for(u32 i=begin;i<end && bytecode[i].op!=op_exit;++i)
        bytecodeinfo(i==pc?"-->  ":"     ",i);
    stackinfo(10);
}

void nasal_dbg::interact()
{
    // special operand
    if(bytecode[pc].op==op_intg)
    {
        std::cout
        <<"[debug] nasal debug mode\n"
        <<"input \'h\' to get help\n";
    }
    else if(bytecode[pc].op==op_exit)
        return;
    
    if(
        (bytecode[pc].fidx!=bk_fidx || bytecode[pc].line!=bk_line) && // break point
        !next // next step
    )return;

    next=false;
    string cmd;
    stepinfo();
    while(1)
    {
        std::cout<<">> ";
        std::getline(std::cin,cmd);
        auto res=parse(cmd);
        if(res.size()==1)
        {
            if(res[0]=="h" || res[0]=="help")
                help();
            else if(res[0]=="bt" || res[0]=="backtrace")
                traceback();
            else if(res[0]=="c" || res[0]=="continue")
                return;
            else if(res[0]=="f" || res[0]=="file")
                for(usize i=0;i<fsize;++i)
                    std::cout<<"["<<i<<"] "<<files[i]<<"\n";
            else if(res[0]=="g" || res[0]=="global")
                gstate();
            else if(res[0]=="l" || res[0]=="local")
                lstate();
            else if(res[0]=="u" || res[0]=="upval")
                ustate();
            else if(res[0]=="r" || res[0]=="register")
                reginfo();
            else if(res[0]=="a" || res[0]=="all")
                detail();
            else if(res[0]=="n" || res[0]=="next")
            {
                next=true;
                return;
            }
            else if(res[0]=="q" || res[0]=="exit")
                std::exit(0);
            else
                err();
        }
        else if(res.size()==3 && (res[0]=="bk" || res[0]=="break"))
        {
            bk_fidx=fileindex(res[1]);
            if(bk_fidx==65535)
            {
                std::cout<<"cannot find file named `"<<res[1]<<"`\n";
                bk_fidx=0;
            }
            i32 tmp=atoi(res[2].c_str());
            if(tmp<=0)
                std::cout<<"incorrect line number `"<<res[2]<<"`\n";
            else
                bk_line=tmp;
        }
        else
            err();
    }
}

void nasal_dbg::run(
    const nasal_codegen& gen,
    const nasal_import& linker,
    const std::vector<string>& argv,
    bool  opcnt)
{
    detail_info=true;
    fsize=linker.filelist().size();
    init(gen.strs(),gen.nums(),gen.codes(),linker.filelist(),argv);
    u64 count[op_ret+1]={0};
#ifndef _MSC_VER
    const void* oprs[]=
    {
        &&vmexit, &&intg,   &&intl,   &&loadg,
        &&loadl,  &&loadu,  &&pnum,   &&pnil,
        &&pstr,   &&newv,   &&newh,   &&newf,
        &&happ,   &&para,   &&deft,   &&dyn,
        &&unot,   &&usub,   &&add,    &&sub,
        &&mul,    &&div,    &&lnk,    &&addc,
        &&subc,   &&mulc,   &&divc,   &&lnkc,
        &&addeq,  &&subeq,  &&muleq,  &&diveq,
        &&lnkeq,  &&addeqc, &&subeqc, &&muleqc,
        &&diveqc, &&lnkeqc, &&meq,    &&eq,
        &&neq,    &&less,   &&leq,    &&grt,
        &&geq,    &&lessc,  &&leqc,   &&grtc,
        &&geqc,   &&pop,    &&jmp,    &&jt,
        &&jf,     &&cnt,    &&findex, &&feach,
        &&callg,  &&calll,  &&upval,  &&callv,
        &&callvi, &&callh,  &&callfv, &&callfh,
        &&callb,  &&slcbeg, &&slcend, &&slc,
        &&slc2,   &&mcallg, &&mcalll, &&mupval,
        &&mcallv, &&mcallh, &&ret
    };
    std::vector<const void*> code;
    for(auto& i:gen.codes())
    {
        code.push_back(oprs[i.op]);
        imm.push_back(i.num);
    }
    // goto the first operand
    goto *code[pc];
#else
    typedef void (nasal_dbg::*nafunc)();
    const nafunc oprs[]=
    {
        nullptr,              &nasal_dbg::o_intg,
        &nasal_dbg::o_intl,   &nasal_dbg::o_loadg,
        &nasal_dbg::o_loadl,  &nasal_dbg::o_loadu,
        &nasal_dbg::o_pnum,   &nasal_dbg::o_pnil,
        &nasal_dbg::o_pstr,   &nasal_dbg::o_newv,
        &nasal_dbg::o_newh,   &nasal_dbg::o_newf,
        &nasal_dbg::o_happ,   &nasal_dbg::o_para,
        &nasal_dbg::o_deft,   &nasal_dbg::o_dyn,
        &nasal_dbg::o_unot,   &nasal_dbg::o_usub,
        &nasal_dbg::o_add,    &nasal_dbg::o_sub,
        &nasal_dbg::o_mul,    &nasal_dbg::o_div,
        &nasal_dbg::o_lnk,    &nasal_dbg::o_addc,
        &nasal_dbg::o_subc,   &nasal_dbg::o_mulc,
        &nasal_dbg::o_divc,   &nasal_dbg::o_lnkc,
        &nasal_dbg::o_addeq,  &nasal_dbg::o_subeq,
        &nasal_dbg::o_muleq,  &nasal_dbg::o_diveq,
        &nasal_dbg::o_lnkeq,  &nasal_dbg::o_addeqc,
        &nasal_dbg::o_subeqc, &nasal_dbg::o_muleqc,
        &nasal_dbg::o_diveqc, &nasal_dbg::o_lnkeqc,
        &nasal_dbg::o_meq,    &nasal_dbg::o_eq,
        &nasal_dbg::o_neq,    &nasal_dbg::o_less,
        &nasal_dbg::o_leq,    &nasal_dbg::o_grt,
        &nasal_dbg::o_geq,    &nasal_dbg::o_lessc,
        &nasal_dbg::o_leqc,   &nasal_dbg::o_grtc,
        &nasal_dbg::o_geqc,   &nasal_dbg::o_pop,
        &nasal_dbg::o_jmp,    &nasal_dbg::o_jt,
        &nasal_dbg::o_jf,     &nasal_dbg::o_cnt,
        &nasal_dbg::o_findex, &nasal_dbg::o_feach,
        &nasal_dbg::o_callg,  &nasal_dbg::o_calll,
        &nasal_dbg::o_upval,  &nasal_dbg::o_callv,
        &nasal_dbg::o_callvi, &nasal_dbg::o_callh,
        &nasal_dbg::o_callfv, &nasal_dbg::o_callfh,
        &nasal_dbg::o_callb,  &nasal_dbg::o_slcbeg,
        &nasal_dbg::o_slcend, &nasal_dbg::o_slc,
        &nasal_dbg::o_slc2,   &nasal_dbg::o_mcallg,
        &nasal_dbg::o_mcalll, &nasal_dbg::o_mupval,
        &nasal_dbg::o_mcallv, &nasal_dbg::o_mcallh,
        &nasal_dbg::o_ret
    };
    std::vector<u32> code;
    for(auto& i:gen.codes())
    {
        code.push_back(i.op);
        imm.push_back(i.num);
    }
    while(oprs[code[pc]]){
        interact();
        ++count[code[pc]];
        (this->*oprs[code[pc]])();
        if(top>=canary)
            break;
        ++pc;
    }
#endif

vmexit:
    if(top>=canary)
        die("stack overflow");
    if(opcnt)
        opcallsort(count);
    gc.clear();
    imm.clear();
    std::cout<<"[debug] debugger exited\n";
    return;
#ifndef _MSC_VER
#define dbg(op,num) {\
    interact();\
    op();\
    ++count[num];\
    if(top<canary)\
        goto *code[++pc];\
    goto vmexit;}

intg:   dbg(o_intg  ,op_intg  );
intl:   dbg(o_intl  ,op_intl  );
loadg:  dbg(o_loadg ,op_loadg );
loadl:  dbg(o_loadl ,op_loadl );
loadu:  dbg(o_loadu ,op_loadu );
pnum:   dbg(o_pnum  ,op_pnum  );
pnil:   dbg(o_pnil  ,op_pnil  );
pstr:   dbg(o_pstr  ,op_pstr  );
newv:   dbg(o_newv  ,op_newv  );
newh:   dbg(o_newh  ,op_newh  );
newf:   dbg(o_newf  ,op_newf  );
happ:   dbg(o_happ  ,op_happ  );
para:   dbg(o_para  ,op_para  );
deft:   dbg(o_deft  ,op_deft  );
dyn:    dbg(o_dyn   ,op_dyn   );
unot:   dbg(o_unot  ,op_unot  );
usub:   dbg(o_usub  ,op_usub  );
add:    dbg(o_add   ,op_add   );
sub:    dbg(o_sub   ,op_sub   );
mul:    dbg(o_mul   ,op_mul   );
div:    dbg(o_div   ,op_div   );
lnk:    dbg(o_lnk   ,op_lnk   );
addc:   dbg(o_addc  ,op_addc  );
subc:   dbg(o_subc  ,op_subc  );
mulc:   dbg(o_mulc  ,op_mulc  );
divc:   dbg(o_divc  ,op_divc  );
lnkc:   dbg(o_lnkc  ,op_lnkc  );
addeq:  dbg(o_addeq ,op_addeq );
subeq:  dbg(o_subeq ,op_subeq );
muleq:  dbg(o_muleq ,op_muleq );
diveq:  dbg(o_diveq ,op_diveq );
lnkeq:  dbg(o_lnkeq ,op_lnkeq );
addeqc: dbg(o_addeqc,op_addeqc);
subeqc: dbg(o_subeqc,op_subeqc);
muleqc: dbg(o_muleqc,op_muleqc);
diveqc: dbg(o_diveqc,op_diveqc);
lnkeqc: dbg(o_lnkeqc,op_lnkeqc);
meq:    dbg(o_meq   ,op_meq   );
eq:     dbg(o_eq    ,op_eq    );
neq:    dbg(o_neq   ,op_neq   );
less:   dbg(o_less  ,op_less  );
leq:    dbg(o_leq   ,op_leq   );
grt:    dbg(o_grt   ,op_grt   );
geq:    dbg(o_geq   ,op_geq   );
lessc:  dbg(o_lessc ,op_lessc );
leqc:   dbg(o_leqc  ,op_leqc  );
grtc:   dbg(o_grtc  ,op_grtc  );
geqc:   dbg(o_geqc  ,op_geqc  );
pop:    dbg(o_pop   ,op_pop   );
jmp:    dbg(o_jmp   ,op_jmp   );
jt:     dbg(o_jt    ,op_jt    );
jf:     dbg(o_jf    ,op_jf    );
cnt:    dbg(o_cnt   ,op_cnt   );
findex: dbg(o_findex,op_findex);
feach:  dbg(o_feach ,op_feach );
callg:  dbg(o_callg ,op_callg );
calll:  dbg(o_calll ,op_calll );
upval:  dbg(o_upval ,op_upval );
callv:  dbg(o_callv ,op_callv );
callvi: dbg(o_callvi,op_callvi);
callh:  dbg(o_callh ,op_callh );
callfv: dbg(o_callfv,op_callfv);
callfh: dbg(o_callfh,op_callfh);
callb:  dbg(o_callb ,op_callb );
slcbeg: dbg(o_slcbeg,op_slcbeg);
slcend: dbg(o_slcend,op_slcend);
slc:    dbg(o_slc   ,op_slc   );
slc2:   dbg(o_slc2  ,op_slc2  );
mcallg: dbg(o_mcallg,op_mcallg);
mcalll: dbg(o_mcalll,op_mcalll);
mupval: dbg(o_mupval,op_mupval);
mcallv: dbg(o_mcallv,op_mcallv);
mcallh: dbg(o_mcallh,op_mcallh);
ret:    dbg(o_ret   ,op_ret   );
#endif
}

#endif