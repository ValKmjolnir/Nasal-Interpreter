#ifndef __NASAL_DBG_H__
#define __NASAL_DBG_H__

#include "nasal_vm.h"

class nasal_dbg:public nasal_vm
{
private:
    bool next_step;
    size_t fsize;
    uint16_t bk_fidx;
    uint32_t bk_line;
    fstreamline src;

    std::vector<std::string> parse(const std::string&);
    uint16_t fileindex(const std::string&);
    void err();
    void help();
    void stepinfo();
    void interact();
public:
    nasal_dbg():
        next_step(false),fsize(0),
        bk_fidx(0),bk_line(0){}
    void run(
        const nasal_codegen&,
        const nasal_import&,
        const std::vector<std::string>&
    );
};

std::vector<std::string> nasal_dbg::parse(const std::string& cmd)
{
    std::vector<std::string> res;
    size_t last=0,pos=cmd.find(" ",0);
    while(pos!=std::string::npos)
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

uint16_t nasal_dbg::fileindex(const std::string& filename)
{
    for(uint16_t i=0;i<fsize;++i)
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

void nasal_dbg::stepinfo()
{
    uint32_t begin,end;
    uint32_t line=bytecode[pc].line==0?0:bytecode[pc].line-1;
    src.load(files[bytecode[pc].fidx]);
    std::cout<<"\nsource code:\n";
    begin=(line>>3)==0?0:((line>>3)<<3);
    end=(1+(line>>3))<<3;
    for(uint32_t i=begin;i<end && i<src.size();++i)
        std::cout<<(i==line?"-->  ":"     ")<<src[i]<<"\n";
    std::cout<<"next bytecode:\n";
    begin=(pc>>3)==0?0:((pc>>3)<<3);
    end=(1+(pc>>3))<<3;
    for(uint32_t i=begin;i<end && bytecode[i].op!=op_exit;++i)
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
        !next_step // next step
    )return;

    next_step=false;
    std::string cmd;
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
                for(size_t i=0;i<fsize;++i)
                    std::cout<<"["<<i<<"] "<<files[i]<<"\n";
            else if(res[0]=="g" || res[0]=="global")
                global_state();
            else if(res[0]=="l" || res[0]=="local"){
                local_state();
            }
            else if(res[0]=="u" || res[0]=="upval")
                upval_state();
            else if(res[0]=="r" || res[0]=="register")
                register_info();
            else if(res[0]=="a" || res[0]=="all")
                detail();
            else if(res[0]=="n" || res[0]=="next")
            {
                next_step=true;
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
            int tmp=atoi(res[2].c_str());
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
    const std::vector<std::string>& argv)
{
    detail_info=true;
    fsize=linker.get_file().size();
    init(gen.get_strs(),gen.get_nums(),gen.get_code(),linker.get_file(),argv);
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
    for(auto& i:gen.get_code())
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
        nullptr,                &nasal_dbg::opr_intg,
        &nasal_dbg::opr_intl,   &nasal_dbg::opr_loadg,
        &nasal_dbg::opr_loadl,  &nasal_dbg::opr_loadu,
        &nasal_dbg::opr_pnum,   &nasal_dbg::opr_pnil,
        &nasal_dbg::opr_pstr,   &nasal_dbg::opr_newv,
        &nasal_dbg::opr_newh,   &nasal_dbg::opr_newf,
        &nasal_dbg::opr_happ,   &nasal_dbg::opr_para,
        &nasal_dbg::opr_deft,   &nasal_dbg::opr_dyn,
        &nasal_dbg::opr_unot,   &nasal_dbg::opr_usub,
        &nasal_dbg::opr_add,    &nasal_dbg::opr_sub,
        &nasal_dbg::opr_mul,    &nasal_dbg::opr_div,
        &nasal_dbg::opr_lnk,    &nasal_dbg::opr_addc,
        &nasal_dbg::opr_subc,   &nasal_dbg::opr_mulc,
        &nasal_dbg::opr_divc,   &nasal_dbg::opr_lnkc,
        &nasal_dbg::opr_addeq,  &nasal_dbg::opr_subeq,
        &nasal_dbg::opr_muleq,  &nasal_dbg::opr_diveq,
        &nasal_dbg::opr_lnkeq,  &nasal_dbg::opr_addeqc,
        &nasal_dbg::opr_subeqc, &nasal_dbg::opr_muleqc,
        &nasal_dbg::opr_diveqc, &nasal_dbg::opr_lnkeqc,
        &nasal_dbg::opr_meq,    &nasal_dbg::opr_eq,
        &nasal_dbg::opr_neq,    &nasal_dbg::opr_less,
        &nasal_dbg::opr_leq,    &nasal_dbg::opr_grt,
        &nasal_dbg::opr_geq,    &nasal_dbg::opr_lessc,
        &nasal_dbg::opr_leqc,   &nasal_dbg::opr_grtc,
        &nasal_dbg::opr_geqc,   &nasal_dbg::opr_pop,
        &nasal_dbg::opr_jmp,    &nasal_dbg::opr_jt,
        &nasal_dbg::opr_jf,     &nasal_dbg::opr_cnt,
        &nasal_dbg::opr_findex, &nasal_dbg::opr_feach,
        &nasal_dbg::opr_callg,  &nasal_dbg::opr_calll,
        &nasal_dbg::opr_upval,  &nasal_dbg::opr_callv,
        &nasal_dbg::opr_callvi, &nasal_dbg::opr_callh,
        &nasal_dbg::opr_callfv, &nasal_dbg::opr_callfh,
        &nasal_dbg::opr_callb,  &nasal_dbg::opr_slcbeg,
        &nasal_dbg::opr_slcend, &nasal_dbg::opr_slc,
        &nasal_dbg::opr_slc2,   &nasal_dbg::opr_mcallg,
        &nasal_dbg::opr_mcalll, &nasal_dbg::opr_mupval,
        &nasal_dbg::opr_mcallv, &nasal_dbg::opr_mcallh,
        &nasal_dbg::opr_ret
    };
    std::vector<const nafunc> code;
    for(auto& i:gen.get_code())
    {
        code.push_back(oprs[i.op]);
        imm.push_back(i.num);
    }
    while(code[pc]){
        interact();
        (this->*code[pc])();
        if(top>=canary)
            break;
        ++pc;
    }
#endif

vmexit:
    if(top>=canary)
        die("stack overflow");
    gc.clear();
    imm.clear();
    std::cout<<"[debug] debugger exited\n";
    return;
#ifndef _MSC_VER
#define dbg(op) {interact();op();if(top<canary)goto *code[++pc];goto vmexit;}

intg:   dbg(opr_intg  );
intl:   dbg(opr_intl  );
loadg:  dbg(opr_loadg );
loadl:  dbg(opr_loadl );
loadu:  dbg(opr_loadu );
pnum:   dbg(opr_pnum  );
pnil:   dbg(opr_pnil  );
pstr:   dbg(opr_pstr  );
newv:   dbg(opr_newv  );
newh:   dbg(opr_newh  );
newf:   dbg(opr_newf  );
happ:   dbg(opr_happ  );
para:   dbg(opr_para  );
deft:   dbg(opr_deft  );
dyn:    dbg(opr_dyn   );
unot:   dbg(opr_unot  );
usub:   dbg(opr_usub  );
add:    dbg(opr_add   );
sub:    dbg(opr_sub   );
mul:    dbg(opr_mul   );
div:    dbg(opr_div   );
lnk:    dbg(opr_lnk   );
addc:   dbg(opr_addc  );
subc:   dbg(opr_subc  );
mulc:   dbg(opr_mulc  );
divc:   dbg(opr_divc  );
lnkc:   dbg(opr_lnkc  );
addeq:  dbg(opr_addeq );
subeq:  dbg(opr_subeq );
muleq:  dbg(opr_muleq );
diveq:  dbg(opr_diveq );
lnkeq:  dbg(opr_lnkeq );
addeqc: dbg(opr_addeqc);
subeqc: dbg(opr_subeqc);
muleqc: dbg(opr_muleqc);
diveqc: dbg(opr_diveqc);
lnkeqc: dbg(opr_lnkeqc);
meq:    dbg(opr_meq   );
eq:     dbg(opr_eq    );
neq:    dbg(opr_neq   );
less:   dbg(opr_less  );
leq:    dbg(opr_leq   );
grt:    dbg(opr_grt   );
geq:    dbg(opr_geq   );
lessc:  dbg(opr_lessc );
leqc:   dbg(opr_leqc  );
grtc:   dbg(opr_grtc  );
geqc:   dbg(opr_geqc  );
pop:    dbg(opr_pop   );
jmp:    dbg(opr_jmp   );
jt:     dbg(opr_jt    );
jf:     dbg(opr_jf    );
cnt:    dbg(opr_cnt   );
findex: dbg(opr_findex);
feach:  dbg(opr_feach );
callg:  dbg(opr_callg );
calll:  dbg(opr_calll );
upval:  dbg(opr_upval );
callv:  dbg(opr_callv );
callvi: dbg(opr_callvi);
callh:  dbg(opr_callh );
callfv: dbg(opr_callfv);
callfh: dbg(opr_callfh);
callb:  dbg(opr_callb );
slcbeg: dbg(opr_slcbeg);
slcend: dbg(opr_slcend);
slc:    dbg(opr_slc   );
slc2:   dbg(opr_slc2  );
mcallg: dbg(opr_mcallg);
mcalll: dbg(opr_mcalll);
mupval: dbg(opr_mupval);
mcallv: dbg(opr_mcallv);
mcallh: dbg(opr_mcallh);
ret:    dbg(opr_ret   );
#endif
}

#endif