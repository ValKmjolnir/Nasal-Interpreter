#pragma once

#include "nasal_import.h"
#include "nasal_err.h"
#include "nasal_opcode.h"
#include "nasal_vm.h"
#include <algorithm>

class dbg:public vm {
private:
    bool  next;
    usize fsize;
    u16   bk_fidx;
    u32   bk_line;
    error& src;

    std::vector<string> parse(const string&);
    u16 fileindex(const string&);
    void err();
    void help();
    void callsort(const u64*);
    void stepinfo();
    void interact();
public:
    dbg(error& err):
        next(false),fsize(0),
        bk_fidx(0),bk_line(0),
        src(err) {}
    void run(
        const codegen&,
        const linker&,
        const std::vector<string>&
    );
};

std::vector<string> dbg::parse(const string& cmd) {
    std::vector<string> res;
    usize last=0,pos=cmd.find(" ",0);
    while(pos!=string::npos) {
        if (pos>last) {
            res.push_back(cmd.substr(last,pos-last));
        }
        last=pos+1;
        pos=cmd.find(" ",last);
    }
    if (last<cmd.length()) {
        res.push_back(cmd.substr(last));
    }
    return res;
}

u16 dbg::fileindex(const string& filename) {
    for(u16 i=0;i<fsize;++i) {
        if (filename==files[i]) {
            return i;
        }
    }
    return 65535;
}

void dbg::err() {
    std::cerr
    <<"incorrect command\n"
    <<"input \'h\' to get help\n";
}

void dbg::help() {
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

void dbg::callsort(const u64* arr) {
    typedef std::pair<u32,u64> op;
    std::vector<op> opcall;
    u64 total=0;
    for(u32 i=0;i<op_ret+1;++i) {
        total+=arr[i];
        opcall.push_back({i,arr[i]});
    }
    std::sort(opcall.begin(),opcall.end(),
        [](const op& a,const op& b) {return a.second>b.second;}
    );
    std::clog<<"\noperands call info (<1% ignored)\n";
    for(auto& i:opcall) {
        u64 rate=i.second*100/total;
        if (!rate) {
            break;
        }
        std::clog<<" "<<opname[i.first]<<" : "<<i.second<<" ("<<rate<<"%)\n";
    }
    std::clog<<" total  : "<<total<<'\n';
}

void dbg::stepinfo() {
    u32 line=bytecode[ctx.pc].line==0?0:bytecode[ctx.pc].line-1;
    u32 begin=(line>>3)==0?0:((line>>3)<<3);
    u32 end=(1+(line>>3))<<3;
    src.load(files[bytecode[ctx.pc].fidx]);
    std::cout<<"\nsource code:\n";
    for(u32 i=begin;i<end && i<src.size();++i) {
        std::cout<<(i==line?back_white:reset)<<(i==line?"--> ":"    ")<<src[i]<<reset<<"\n";
    }

    begin=(ctx.pc>>3)==0?0:((ctx.pc>>3)<<3);
    end=(1+(ctx.pc>>3))<<3;
    codestream::set(cnum,cstr,files);
    std::cout<<"next bytecode:\n";
    for(u32 i=begin;i<end && bytecode[i].op!=op_exit;++i) {
        std::cout
        <<(i==ctx.pc?back_white:reset)
        <<(i==ctx.pc?"--> ":"    ")
        <<codestream(bytecode[i],i)
        <<reset<<"\n";
    }
    stackinfo(10);
}

void dbg::interact() {
    // special operand
    if (bytecode[ctx.pc].op==op_exit) {
        return;
    }
    
    if (
        (bytecode[ctx.pc].fidx!=bk_fidx || bytecode[ctx.pc].line!=bk_line) && // break point
        !next // next step
    ) {
        return;
    }

    next=false;
    string cmd;
    stepinfo();
    while(true) {
        std::cout<<">> ";
        std::getline(std::cin,cmd);
        auto res=parse(cmd);
        if (res.size()==0) {
            stepinfo();
        } else if (res.size()==1) {
            if (res[0]=="h" || res[0]=="help") {
                help();
            } else if (res[0]=="bt" || res[0]=="backtrace") {
                traceback();
            } else if (res[0]=="c" || res[0]=="continue") {
                return;
            } else if (res[0]=="f" || res[0]=="file") {
                for(usize i=0;i<fsize;++i) {
                    std::cout<<"["<<i<<"] "<<files[i]<<"\n";
                }
            } else if (res[0]=="g" || res[0]=="global") {
                gstate();
            } else if (res[0]=="l" || res[0]=="local") {
                lstate();
            } else if (res[0]=="u" || res[0]=="upval") {
                ustate();
            } else if (res[0]=="r" || res[0]=="register") {
                reginfo();
            } else if (res[0]=="a" || res[0]=="all") {
                detail();
            } else if (res[0]=="n" || res[0]=="next") {
                next=true;
                return;
            } else if (res[0]=="q" || res[0]=="exit")
                std::exit(0);
            else {
                err();
            }
        } else if (res.size()==3 && (res[0]=="bk" || res[0]=="break")) {
            bk_fidx=fileindex(res[1]);
            if (bk_fidx==65535) {
                std::cout<<"cannot find file named `"<<res[1]<<"`\n";
                bk_fidx=0;
            }
            i32 tmp=atoi(res[2].c_str());
            if (tmp<=0) {
                std::cout<<"incorrect line number `"<<res[2]<<"`\n";
            } else {
                bk_line=tmp;
            }
        } else {
            err();
        }
    }
}

void dbg::run(
    const codegen& gen,
    const linker& linker,
    const std::vector<string>& argv)
{
    verbose=true;
    fsize=linker.filelist().size();
    init(gen.strs(),gen.nums(),gen.codes(),linker.filelist(),argv);
    u64 count[op_ret+1]={0};
    typedef void (dbg::*nafunc)();
    const nafunc oprs[]={
        nullptr,        &dbg::o_intg,
        &dbg::o_intl,   &dbg::o_loadg,
        &dbg::o_loadl,  &dbg::o_loadu,
        &dbg::o_pnum,   &dbg::o_pnil,
        &dbg::o_pstr,   &dbg::o_newv,
        &dbg::o_newh,   &dbg::o_newf,
        &dbg::o_happ,   &dbg::o_para,
        &dbg::o_deft,   &dbg::o_dyn,
        &dbg::o_lnot,   &dbg::o_usub,
        &dbg::o_bnot,   &dbg::o_btor,
        &dbg::o_btxor,  &dbg::o_btand,
        &dbg::o_add,    &dbg::o_sub,
        &dbg::o_mul,    &dbg::o_div,
        &dbg::o_lnk,    &dbg::o_addc,
        &dbg::o_subc,   &dbg::o_mulc,
        &dbg::o_divc,   &dbg::o_lnkc,
        &dbg::o_addeq,  &dbg::o_subeq,
        &dbg::o_muleq,  &dbg::o_diveq,
        &dbg::o_lnkeq,  &dbg::o_bandeq,
        &dbg::o_boreq,  &dbg::o_bxoreq,
        &dbg::o_addeqc, &dbg::o_subeqc,
        &dbg::o_muleqc, &dbg::o_diveqc,
        &dbg::o_lnkeqc, &dbg::o_addecp,
        &dbg::o_subecp, &dbg::o_mulecp,
        &dbg::o_divecp, &dbg::o_lnkecp,
        &dbg::o_meq,    &dbg::o_eq,
        &dbg::o_neq,    &dbg::o_less,
        &dbg::o_leq,    &dbg::o_grt,
        &dbg::o_geq,    &dbg::o_lessc,
        &dbg::o_leqc,   &dbg::o_grtc,
        &dbg::o_geqc,   &dbg::o_pop,
        &dbg::o_jmp,    &dbg::o_jt,
        &dbg::o_jf,     &dbg::o_cnt,
        &dbg::o_findex, &dbg::o_feach,
        &dbg::o_callg,  &dbg::o_calll,
        &dbg::o_upval,  &dbg::o_callv,
        &dbg::o_callvi, &dbg::o_callh,
        &dbg::o_callfv, &dbg::o_callfh,
        &dbg::o_callb,  &dbg::o_slcbeg,
        &dbg::o_slcend, &dbg::o_slc,
        &dbg::o_slc2,   &dbg::o_mcallg,
        &dbg::o_mcalll, &dbg::o_mupval,
        &dbg::o_mcallv, &dbg::o_mcallh,
        &dbg::o_ret
    };
    std::vector<u32> code;
    for(auto& i:gen.codes()) {
        code.push_back(i.op);
        imm.push_back(i.num);
    }
    while(oprs[code[ctx.pc]]) {
        interact();
        ++count[code[ctx.pc]];
        (this->*oprs[code[ctx.pc]])();
        if (ctx.top>=ctx.canary) {
            die("stack overflow");
        }
        ++ctx.pc;
    }

    callsort(count);
    ngc.info();
    ngc.clear();
    imm.clear();
    return;
}
