#include "nasal_dbg.h"

std::vector<std::string> dbg::parse(const std::string& cmd) {
    std::vector<std::string> res;
    usize last = 0, pos = cmd.find(" ", 0);
    while(pos!=std::string::npos) {
        if (pos>last) {
            res.push_back(cmd.substr(last, pos-last));
        }
        last = pos+1;
        pos = cmd.find(" ", last);
    }
    if (last<cmd.length()) {
        res.push_back(cmd.substr(last));
    }
    return res;
}

u16 dbg::file_index(const std::string& filename) const {
    for(u16 i = 0; i<fsize; ++i) {
        if (filename==files[i]) {
            return i;
        }
    }
    return 65535;
}

void dbg::err() {
    std::cerr
    << "incorrect command\n"
    << "input \'h\' to get help\n";
}

void dbg::help() {
    std::clog
    << "<option>\n"
    << "  h,   help      | get help\n"
    << "  bt,  backtrace | get function call trace\n"
    << "  c,   continue  | run program until break point or exit\n"
    << "  f,   file      | see all the compiled files\n"
    << "  g,   global    | see global values\n"
    << "  l,   local     | see local values\n"
    << "  u,   upval     | see upvalue\n"
    << "  r,   register  | show vm register detail\n"
    << "  a,   all       | show global,local and upvalue\n"
    << "  n,   next      | execute next bytecode\n"
    << "  q,   exit      | exit debugger\n"
    << "<option> <filename> <line>\n"
    << "  bk,  break     | set break point\n";
}

void dbg::list_file() const {
    for(usize i = 0; i<fsize; ++i) {
        std::clog << "[" << i << "] " << files[i] << "\n";
    }
}

void dbg::call_sort(const u64* arr) const {
    typedef std::pair<u32,u64> op;
    std::vector<op> opcall;
    u64 total = 0;
    for(u32 i = 0; i<op_ret+1; ++i) {
        total += arr[i];
        opcall.push_back({i, arr[i]});
    }
    std::sort(opcall.begin(), opcall.end(),
        [](const op& a, const op& b) {return a.second>b.second;}
    );
    std::clog << "\noperands call info (<1% ignored)\n";
    for(auto& i : opcall) {
        u64 rate = i.second*100/total;
        if (!rate) {
            break;
        }
        std::clog << " " << opname[i.first] << " : ";
        std::clog << i.second << " (" << rate << "%)\n";
    }
    std::clog << " total  : " << total << '\n';
}

void dbg::step_info() {
    u32 line = bytecode[ctx.pc].line==0? 0:bytecode[ctx.pc].line-1;
    u32 begin = (line>>3)==0? 0:((line>>3)<<3);
    u32 end = (1+(line>>3))<<3;
    src.load(files[bytecode[ctx.pc].fidx]);
    std::clog << "\nsource code:\n";
    for(u32 i = begin; i<end && i<src.size(); ++i) {
        std::clog << (i==line? back_white:reset);
        std::clog << (i==line? "--> ":"    ") << src[i] << reset << "\n";
    }

    begin = (ctx.pc>>3)==0? 0:((ctx.pc>>3)<<3);
    end = (1+(ctx.pc>>3))<<3;
    codestream::set(cnum, cstr, files);
    std::clog << "next bytecode:\n";
    for(u32 i = begin; i<end && bytecode[i].op!=op_exit; ++i) {
        std::clog
        << (i==ctx.pc? back_white:reset)
        << (i==ctx.pc? "--> ":"    ")
        << codestream(bytecode[i], i)
        << reset << "\n";
    }
    stackinfo(10);
}

void dbg::interact() {
    // special operand, end execution
    if (bytecode[ctx.pc].op==op_exit) {
        return;
    }
    
    if ((bytecode[ctx.pc].fidx!=bk_fidx ||
        bytecode[ctx.pc].line!=bk_line) && // break point
        !next) {// next step
        return;
    }

    next = false;
    std::string cmd;
    step_info();
    while(true) {
        std::clog << ">> ";
        std::getline(std::cin, cmd);
        auto res=parse(cmd);
        if (res.size()==0) {
            step_info();
        } else if (res.size()==1) {
            switch(get_cmd_type(res[0])) {
                case dbg_cmd::cmd_help: help(); break;
                case dbg_cmd::cmd_backtrace: traceback(); break;
                case dbg_cmd::cmd_continue: return;
                case dbg_cmd::cmd_list_file: list_file(); break;
                case dbg_cmd::cmd_global: gstate(); break;
                case dbg_cmd::cmd_local: lstate(); break;
                case dbg_cmd::cmd_upval: ustate(); break;
                case dbg_cmd::cmd_register: reginfo(); break;
                case dbg_cmd::cmd_show_all: detail(); break;
                case dbg_cmd::cmd_next: next = true; return;
                case dbg_cmd::cmd_exit: std::exit(0);
                default: err(); break;
            }
        } else if (res.size()==3 &&
            get_cmd_type(res[0])==dbg_cmd::cmd_break_point) {
            bk_fidx = file_index(res[1]);
            if (bk_fidx==65535) {
                std::clog << "cannot find file named `" << res[1] << "`\n";
                continue;
            }
            i32 tmp = atoi(res[2].c_str());
            if (tmp<=0) {
                std::clog << "incorrect line number `" << res[2] << "`\n";
            } else {
                bk_line = tmp;
            }
        } else {
            err();
        }
    }
}

void dbg::run(
    const codegen& gen,
    const linker& linker,
    const std::vector<std::string>& argv) {
    verbose = true;
    fsize = linker.filelist().size();
    init(gen.strs(),
         gen.nums(),
         gen.codes(),
         gen.globals(),
         linker.filelist(),
         argv);
    u64 count[op_ret+1] = {0};
    typedef void (dbg::*nafunc)();
    const nafunc oprs[] = {
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
    for(auto& i : gen.codes()) {
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

    call_sort(count);
    ngc.info();
    ngc.clear();
    imm.clear();
    return;
}
