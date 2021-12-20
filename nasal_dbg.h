#ifndef __NASAL_DBG_H__
#define __NASAL_DBG_H__

#include "nasal_vm.h"

class nasal_dbg:public nasal_vm
{
private:
    std::vector<std::string> parse(std::string&);
    uint16_t get_fileindex(std::string);
    void err();
    void help();
    void interact();
public:
    void run(
        const nasal_codegen&,
        const nasal_import&
    );
};

std::vector<std::string> nasal_dbg::parse(std::string& cmd)
{
    std::vector<std::string> res;
    std::string tmp="";
    for(uint32_t i=0;i<cmd.length();++i)
    {
        if(cmd[i]==' ' && tmp.length())
        {
            res.push_back(tmp);
            tmp="";
            continue;
        }
        tmp+=cmd[i];
    }
    if(tmp.length())
        res.push_back(tmp);
    return res;
}

uint16_t nasal_dbg::get_fileindex(std::string filename)
{
    for(uint16_t i=0;i<files_size;++i)
        if(filename==files[i])
            return i;
    return 65535;
}

void nasal_dbg::err()
{
    std::cout
    <<"incorrect command\n"
    <<"input \'h\' to get help\n";
}

void nasal_dbg::help()
{
    std::cout
    <<"<option>\n"
    <<"\th,    help      | get help\n"
    <<"\tbt,   backtrace | get function call trace\n"
    <<"\tr,    run       | run program until break point or exit\n"
    <<"\tg,    global    | see global values\n"
    <<"\tl,    local     | see local values\n"
    <<"\tu,    upval     | see upvalue\n"
    <<"\tquit, exit      | exit debugger\n";
}

void nasal_dbg::interact()
{
    static uint16_t last_fidx=0;
    static uint32_t last_line=0;
    
    if(bytecode[pc].op==op_intg)
    {
        std::cout
        <<"nasal debug mode\n"
        <<"input \'h\' to get help\n";
    }
    else if(bytecode[pc].op==op_exit)
    {
        std::cout<<"debugger exited successfully\n";
        return;
    }
    else if(bytecode[pc].op==op_nop)
        return;
    if(bytecode[pc].fidx!=last_fidx || bytecode[pc].line!=last_line)
        return;

    last_fidx=0;
    last_line=0;
    std::string cmd;
    bytecodeinfo("->\t",pc);
    for(uint32_t i=1;i<5 && bytecode[pc+i].op!=op_exit;++i)
        bytecodeinfo("  \t",pc+i);
    while(1)
    {
        printf(">> ");
        std::getline(std::cin,cmd);
        auto res=parse(cmd);
        switch(res.size())
        {
            case 1:
                if(res[0]=="h" || res[0]=="help")
                    help();
                else if(res[0]=="bt" || res[0]=="backtrace")
                    traceback();
                else if(res[0]=="r" || res[0]=="run")
                    return;
                else if(res[0]=="g" || res[0]=="global")
                    global_state();
                else if(res[0]=="l" || res[0]=="local")
                    local_state();
                else if(res[0]=="u" || res[0]=="upval")
                    upval_state();
                else if(res[0]=="quit" || res[0]=="exit")
                    std::exit(0);
                else
                    err();
                break;
            case 3:
                std::cout<<"unfinished\n";
                break;
            default:err();break;
        }
    }
}

void nasal_dbg::run(
    const nasal_codegen& gen,
    const nasal_import& linker)
{
    detail_info=true;
    init(gen.get_strs(),gen.get_nums(),linker.get_file());
    const void* opr_table[]=
    {
        &&nop,     &&intg,     &&intl,   &&loadg,
        &&loadl,   &&loadu,    &&pnum,   &&pone,
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
        &&findex,  &&feach,    &&callg,  &&calll,
        &&upval,   &&callv,    &&callvi, &&callh,
        &&callfv,  &&callfh,   &&callb,  &&slcbegin,
        &&slcend,  &&slc,      &&slc2,   &&mcallg,
        &&mcalll,  &&mupval,   &&mcallv, &&mcallh,
        &&ret,     &&vmexit
    };
    bytecode=gen.get_code().data();
    std::vector<const void*> code;
    for(auto& i:gen.get_code())
    {
        code.push_back(opr_table[i.op]);
        imm.push_back(i.num);
    }

    // set canary and program counter
    auto canary=gc.stack+STACK_MAX_DEPTH-1;
    pc=0;
    // goto the first operand
    goto *code[pc];

vmexit:
    if(gc.top>=canary)
        die("stack overflow");
    gc.clear();
    imm.clear();
    return;
#define dbg(op) {interact();op();if(gc.top<canary)goto *code[++pc];goto vmexit;}

nop:     dbg(opr_nop     );
intg:    dbg(opr_intg    );
intl:    dbg(opr_intl    );
loadg:   dbg(opr_loadg   );
loadl:   dbg(opr_loadl   );
loadu:   dbg(opr_loadu   );
pnum:    dbg(opr_pnum    );
pone:    dbg(opr_pone    );
pzero:   dbg(opr_pzero   );
pnil:    dbg(opr_pnil    );
pstr:    dbg(opr_pstr    );
newv:    dbg(opr_newv    );
newh:    dbg(opr_newh    );
newf:    dbg(opr_newf    );
happ:    dbg(opr_happ    );
para:    dbg(opr_para    );
defpara: dbg(opr_defpara );
dynpara: dbg(opr_dynpara );
unot:    dbg(opr_unot    );
usub:    dbg(opr_usub    );
add:     dbg(opr_add     );
sub:     dbg(opr_sub     );
mul:     dbg(opr_mul     );
div:     dbg(opr_div     );
lnk:     dbg(opr_lnk     );
addc:    dbg(opr_addc    );
subc:    dbg(opr_subc    );
mulc:    dbg(opr_mulc    );
divc:    dbg(opr_divc    );
lnkc:    dbg(opr_lnkc    );
addeq:   dbg(opr_addeq   );
subeq:   dbg(opr_subeq   );
muleq:   dbg(opr_muleq   );
diveq:   dbg(opr_diveq   );
lnkeq:   dbg(opr_lnkeq   );
addeqc:  dbg(opr_addeqc  );
subeqc:  dbg(opr_subeqc  );
muleqc:  dbg(opr_muleqc  );
diveqc:  dbg(opr_diveqc  );
lnkeqc:  dbg(opr_lnkeqc  );
meq:     dbg(opr_meq     );
eq:      dbg(opr_eq      );
neq:     dbg(opr_neq     );
less:    dbg(opr_less    );
leq:     dbg(opr_leq     );
grt:     dbg(opr_grt     );
geq:     dbg(opr_geq     );
lessc:   dbg(opr_lessc   );
leqc:    dbg(opr_leqc    );
grtc:    dbg(opr_grtc    );
geqc:    dbg(opr_geqc    );
pop:     dbg(opr_pop     );
jmp:     dbg(opr_jmp     );
jt:      dbg(opr_jt      );
jf:      dbg(opr_jf      );
counter: dbg(opr_counter );
findex:  dbg(opr_findex  );
feach:   dbg(opr_feach   );
callg:   dbg(opr_callg   );
calll:   dbg(opr_calll   );
upval:   dbg(opr_upval   );
callv:   dbg(opr_callv   );
callvi:  dbg(opr_callvi  );
callh:   dbg(opr_callh   );
callfv:  dbg(opr_callfv  );
callfh:  dbg(opr_callfh  );
callb:   dbg(opr_callb   );
slcbegin:dbg(opr_slcbegin);
slcend:  dbg(opr_slcend  );
slc:     dbg(opr_slc     );
slc2:    dbg(opr_slc2    );
mcallg:  dbg(opr_mcallg  );
mcalll:  dbg(opr_mcalll  );
mupval:  dbg(opr_mupval  );
mcallv:  dbg(opr_mcallv  );
mcallh:  dbg(opr_mcallh  );
ret:     dbg(opr_ret     );
}

#endif