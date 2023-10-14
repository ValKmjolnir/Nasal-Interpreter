#include "nasal_vm.h"

namespace nasal {

void vm::init(
    const std::vector<std::string>& strs,
    const std::vector<f64>& nums,
    const std::vector<nasal_builtin_table>& natives,
    const std::vector<opcode>& code,
    const std::unordered_map<std::string, i32>& global_symbol,
    const std::vector<std::string>& filenames,
    const std::vector<std::string>& argv
) {
    cnum = nums.data();
    cstr = strs.data();
    bytecode = code.data();
    files = filenames.data();
    global_size = global_symbol.size();

    /* set native functions */
    native = natives;

    /* set context and global */
    if (!is_repl_mode || first_exec_flag) {
        context_and_global_init();
        first_exec_flag = false;
    }

    /* init gc */
    ngc.set(&ctx, global, global_size);
    ngc.init(strs, argv);

    /* init vm globals */
    auto map_instance = ngc.alloc(vm_map);
    global[global_symbol.at("globals")] = map_instance;
    for(const auto& i : global_symbol) {
        map_instance.map().mapper[i.first] = global+i.second;
    }

    /* init vm arg */
    auto arg_instance = ngc.alloc(vm_vec);
    global[global_symbol.at("arg")] = arg_instance;
    arg_instance.vec().elems = ngc.env_argv;
}

void vm::context_and_global_init() {
    /* set canary and program counter */
    ctx.pc = 0;
    ctx.localr = nullptr;
    ctx.memr = nullptr;
    ctx.funcr = nil;
    ctx.upvalr = nil;

    /* set canary = stack[STACK_DEPTH-1] */
    ctx.canary = ctx.stack+STACK_DEPTH-1;

    /* nothing is on stack */
    ctx.top = ctx.stack - 1;

    /* clear main stack and global */
    for(u32 i = 0; i<STACK_DEPTH; ++i) {
        ctx.stack[i] = nil;
        global[i] = nil;
    }
}

void vm::valinfo(var& val) {
    const auto p = reinterpret_cast<u64>(val.val.gcobj);
    switch(val.type) {
        case vm_none: std::clog << "| null |"; break;
        case vm_ret:  std::clog << "| pc   | 0x" << std::hex
                                << val.ret() << std::dec; break;
        case vm_addr: std::clog << "| addr | 0x" << std::hex
                                << reinterpret_cast<u64>(val.addr())
                                << std::dec; break;
        case vm_cnt:  std::clog << "| cnt  | " << val.cnt(); break;
        case vm_nil:  std::clog << "| nil  |"; break;
        case vm_num:  std::clog << "| num  | " << val.num(); break;
        case vm_str:  std::clog << "| str  | <0x" << std::hex << p
                                << "> " << rawstr(val.str(), 16)
                                << std::dec; break;
        case vm_func: std::clog << "| func | <0x" << std::hex << p
                                << "> entry:0x" << val.func().entry
                                << std::dec; break;
        case vm_upval:std::clog << "| upval| <0x" << std::hex << p
                                << std::dec << "> [" << val.upval().size
                                << " val]"; break;
        case vm_vec:  std::clog << "| vec  | <0x" << std::hex << p
                                << std::dec << "> [" << val.vec().size()
                                << " val]"; break;
        case vm_hash: std::clog << "| hash | <0x" << std::hex << p
                                << std::dec << "> {" << val.hash().size()
                                << " val}"; break;
        case vm_obj:  std::clog << "| obj  | <0x" << std::hex << p
                                << "> obj:0x"
                                << reinterpret_cast<u64>(val.obj().pointer)
                                << std::dec; break;
        case vm_co:   std::clog << "| co   | <0x" << std::hex << p
                                << std::dec << "> coroutine"; break;
        case vm_map:  std::clog << "| nmspc| <0x" << std::hex << p
                                << std::dec << "> namespace ["
                                << val.map().mapper.size() << " val]"; break;
        default:      std::clog << "| err  | <0x" << std::hex << p
                                << std::dec << "> unknown object"; break;
    }
    std::clog << "\n";
}

void vm::traceback() {
    var* bottom = ctx.stack;
    var* top = ctx.top;

    std::stack<u32> ret;
    for(var* i = bottom; i<=top; ++i) {
        if (i->type==vm_ret && i->ret()!=0) {
            ret.push(i->ret());
        }
    }
    ret.push(ctx.pc); // store the position program crashed

    std::clog << "trace back ("
              << (ngc.cort? "coroutine":"main")
              << ")\n";
    codestream::set(cnum, cstr, native.data(), files);
    for(u32 p = 0, same = 0, prev = 0xffffffff; !ret.empty(); prev = p, ret.pop()) {
        if ((p = ret.top())==prev) {
            ++same;
            continue;
        }
        if (same) {
            std::clog << "  0x" << std::hex
                      << std::setw(6) << std::setfill('0')
                      << prev << std::dec << "     "
                      << same << " same call(s)\n";
        }
        same = 0;
        std::clog << "  " << codestream(bytecode[p], p) << "\n";
    }
    // the first called place has no same calls
}

void vm::stackinfo(const u32 limit = 10) {
    var* top = ctx.top;
    var* bottom = ctx.stack;
    std::clog << "stack (0x" << std::hex << reinterpret_cast<u64>(bottom);
    std::clog << std::dec << ", limit " << limit << ", total ";
    std::clog << (top<bottom? 0:static_cast<i64>(top-bottom+1)) << ")\n";
    for(u32 i = 0; i<limit && top>=bottom; ++i, --top) {
        std::clog << "  0x" << std::hex
                  << std::setw(6) << std::setfill('0')
                  << static_cast<u64>(top-bottom) << std::dec
                  << "    ";
        valinfo(top[0]);
    }
}

void vm::reginfo() {
    std::clog << "registers (" << (ngc.cort? "coroutine":"main")
              << ")\n" << std::hex
              << "  [pc    ]    | pc   | 0x" << ctx.pc << "\n"
              << "  [global]    | addr | 0x"
              << reinterpret_cast<u64>(global) << "\n"
              << "  [local ]    | addr | 0x"
              << reinterpret_cast<u64>(ctx.localr) << "\n"
              << "  [memr  ]    | addr | 0x"
              << reinterpret_cast<u64>(ctx.memr) << "\n"
              << "  [canary]    | addr | 0x"
              << reinterpret_cast<u64>(ctx.canary) << "\n"
              << "  [top   ]    | addr | 0x"
              << reinterpret_cast<u64>(ctx.top) << "\n"
              << std::dec;
    std::clog << "  [funcr ]    "; valinfo(ctx.funcr);
    std::clog << "  [upval ]    "; valinfo(ctx.upvalr);
}

void vm::gstate() {
    if (!global_size || global[0].type==vm_none) {
        return;
    }
    std::clog << "global (0x" << std::hex
              << reinterpret_cast<u64>(global) << ")\n" << std::dec;
    for(usize i = 0; i<global_size; ++i) {
        std::clog << "  0x" << std::hex << std::setw(6)
                  << std::setfill('0') << i << std::dec
                  << "    ";
        valinfo(global[i]);
    }
}

void vm::lstate() {
    if (!ctx.localr || !ctx.funcr.func().local_size) {
        return;
    }
    const u32 lsize = ctx.funcr.func().local_size;
    std::clog << "local (0x" << std::hex << reinterpret_cast<u64>(ctx.localr)
              << " <+" << static_cast<u64>(ctx.localr-ctx.stack)
              << ">)\n" << std::dec;
    for(u32 i = 0; i<lsize; ++i) {
        std::clog << "  0x" << std::hex << std::setw(6)
                  << std::setfill('0') << i << std::dec
                  << "    ";
        valinfo(ctx.localr[i]);
    }
}

void vm::ustate() {
    if (ctx.funcr.type==vm_nil || ctx.funcr.func().upval.empty()) {
        return;
    }
    std::clog << "upvalue\n";
    auto& upval = ctx.funcr.func().upval;
    for(u32 i = 0; i<upval.size(); ++i) {
        std::clog << "  -> upval[" << i << "]:\n";
        auto& uv = upval[i].upval();
        for(u32 j = 0; j<uv.size; ++j) {
            std::clog << "     0x" << std::hex << std::setw(6)
                      << std::setfill('0') << j << std::dec
                      << " ";
            valinfo(uv[j]);
        }
    }
}

void vm::detail() {
    reginfo();
    gstate();
    lstate();
    ustate();
}

std::string vm::report_lack_arguments(u32 argc, const nas_func& func) const {
    auto result = std::string("lack argument(s): ");
    std::vector<std::string> argument_list = {};
    argument_list.resize(func.keys.size());
    for(const auto& i : func.keys) {
        argument_list[i.second-1] = i.first;
    }
    for(u32 i = argc; i<argument_list.size(); ++i) {
        result += argument_list[i];
        if (i!=argument_list.size()-1) {
            result += ", ";
        }
    }
    return result;
}

void vm::die(const std::string& str) {
    std::cerr << "[vm] error: " << str << "\n";
    traceback();
    stackinfo();

    // show verbose crash info
    if (verbose) {
        detail();
    }

    if (!ngc.cort) {
        // in main context, exit directly
        std::exit(1);
    } else {
        // in coroutine, shut down the coroutine and return to main context
        ctx.pc = 0; // mark coroutine 'dead'
        ngc.ctxreserve(); // switch context to main
        ctx.top[0] = nil; // generate return value 'nil'
    }
}

void vm::run(
    const codegen& gen,
    const linker& linker,
    const std::vector<std::string>& argv
) {
    init(gen.strs(), gen.nums(), gen.natives(),
         gen.codes(), gen.globals(), linker.get_file_list(), argv);
#ifndef _MSC_VER
    // using labels as values/computed goto
    const void* oprs[] = {
        &&vmexit, &&repl,   &&intl,   &&loadg,
        &&loadl,  &&loadu,  &&pnum,   &&pnil,
        &&pstr,   &&newv,   &&newh,   &&newf,
        &&happ,   &&para,   &&deft,   &&dyn,
        &&lnot,   &&usub,   &&bnot,   &&btor,
        &&btxor,  &&btand,  &&add,    &&sub,
        &&mul,    &&div,    &&lnk,    &&addc,
        &&subc,   &&mulc,   &&divc,   &&lnkc,
        &&addeq,  &&subeq,  &&muleq,  &&diveq,
        &&lnkeq,  &&bandeq, &&boreq,  &&bxoreq,
        &&addeqc, &&subeqc, &&muleqc, &&diveqc,
        &&lnkeqc, &&addecp, &&subecp, &&mulecp,
        &&divecp, &&lnkecp, &&meq,    &&eq,
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
    for(auto& i : gen.codes()) {
        code.push_back(oprs[i.op]);
        imm.push_back(i.num);
    }
    // goto the first operand
    goto *code[ctx.pc];
#else
    typedef void (vm::*nafunc)();
    const nafunc oprs[] = {
        nullptr,       &vm::o_repl,
        &vm::o_intl,   &vm::o_loadg,
        &vm::o_loadl,  &vm::o_loadu,
        &vm::o_pnum,   &vm::o_pnil,
        &vm::o_pstr,   &vm::o_newv,
        &vm::o_newh,   &vm::o_newf,
        &vm::o_happ,   &vm::o_para,
        &vm::o_deft,   &vm::o_dyn,
        &vm::o_lnot,   &vm::o_usub,
        &vm::o_bnot,   &vm::o_btor,
        &vm::o_btxor,  &vm::o_btand,
        &vm::o_add,    &vm::o_sub,
        &vm::o_mul,    &vm::o_div,
        &vm::o_lnk,    &vm::o_addc,
        &vm::o_subc,   &vm::o_mulc,
        &vm::o_divc,   &vm::o_lnkc,
        &vm::o_addeq,  &vm::o_subeq,
        &vm::o_muleq,  &vm::o_diveq,
        &vm::o_lnkeq,  &vm::o_bandeq,
        &vm::o_boreq,  &vm::o_bxoreq,
        &vm::o_addeqc, &vm::o_subeqc,
        &vm::o_muleqc, &vm::o_diveqc,
        &vm::o_lnkeqc, &vm::o_addecp,
        &vm::o_subecp, &vm::o_mulecp,
        &vm::o_divecp, &vm::o_lnkecp,
        &vm::o_meq,    &vm::o_eq,
        &vm::o_neq,    &vm::o_less,
        &vm::o_leq,    &vm::o_grt,
        &vm::o_geq,    &vm::o_lessc,
        &vm::o_leqc,   &vm::o_grtc,
        &vm::o_geqc,   &vm::o_pop,
        &vm::o_jmp,    &vm::o_jt,
        &vm::o_jf,     &vm::o_cnt,
        &vm::o_findex, &vm::o_feach,
        &vm::o_callg,  &vm::o_calll,
        &vm::o_upval,  &vm::o_callv,
        &vm::o_callvi, &vm::o_callh,
        &vm::o_callfv, &vm::o_callfh,
        &vm::o_callb,  &vm::o_slcbeg,
        &vm::o_slcend, &vm::o_slc,
        &vm::o_slc2,   &vm::o_mcallg,
        &vm::o_mcalll, &vm::o_mupval,
        &vm::o_mcallv, &vm::o_mcallh,
        &vm::o_ret
    };
    std::vector<nafunc> code;
    for(auto& i : gen.codes()) {
        code.push_back(oprs[i.op]);
        imm.push_back(i.num);
    }
    while(code[ctx.pc]) {
        (this->*code[ctx.pc])();
        if (ctx.top>=ctx.canary) {
            die("stack overflow");
        }
        ++ctx.pc;
    }
#endif

vmexit:
    if (verbose) {
        ngc.info();
    }
    imm.clear();
    if (!is_repl_mode) {
        ngc.clear();
    }
    return;

#ifndef _MSC_VER
// may cause stackoverflow
#define exec_check(op) {\
    op();\
    if (ctx.top<ctx.canary)\
        goto *code[++ctx.pc];\
    die("stack overflow");\
    goto *code[++ctx.pc];\
}
// do not cause stackoverflow
#define exec_nodie(op) {\
    op();\
    goto *code[++ctx.pc];\
}

repl:   exec_nodie(o_repl  ); // 0
intl:   exec_nodie(o_intl  ); // -0
loadg:  exec_nodie(o_loadg ); // -1
loadl:  exec_nodie(o_loadl ); // -1
loadu:  exec_nodie(o_loadu ); // -1
pnum:   exec_check(o_pnum  ); // +1
pnil:   exec_check(o_pnil  ); // +1
pstr:   exec_check(o_pstr  ); // +1
newv:   exec_check(o_newv  ); // +1-imm[pc]
newh:   exec_check(o_newh  ); // +1
newf:   exec_check(o_newf  ); // +1
happ:   exec_nodie(o_happ  ); // -1
para:   exec_nodie(o_para  ); // -0
deft:   exec_nodie(o_deft  ); // -1
dyn:    exec_nodie(o_dyn   ); // -0
lnot:   exec_nodie(o_lnot  ); // -0
usub:   exec_nodie(o_usub  ); // -0
bnot:   exec_nodie(o_bnot  ); // -0
btor:   exec_nodie(o_btor  ); // -1
btxor:  exec_nodie(o_btxor ); // -1
btand:  exec_nodie(o_btand ); // -1
add:    exec_nodie(o_add   ); // -1
sub:    exec_nodie(o_sub   ); // -1
mul:    exec_nodie(o_mul   ); // -1
div:    exec_nodie(o_div   ); // -1
lnk:    exec_nodie(o_lnk   ); // -1
addc:   exec_nodie(o_addc  ); // -0
subc:   exec_nodie(o_subc  ); // -0
mulc:   exec_nodie(o_mulc  ); // -0
divc:   exec_nodie(o_divc  ); // -0
lnkc:   exec_nodie(o_lnkc  ); // -0
addeq:  exec_nodie(o_addeq ); // -1
subeq:  exec_nodie(o_subeq ); // -1
muleq:  exec_nodie(o_muleq ); // -1
diveq:  exec_nodie(o_diveq ); // -1
lnkeq:  exec_nodie(o_lnkeq ); // -1
bandeq: exec_nodie(o_bandeq); // -1
boreq:  exec_nodie(o_boreq ); // -1
bxoreq: exec_nodie(o_bxoreq); // -1
addeqc: exec_nodie(o_addeqc); // -0
subeqc: exec_nodie(o_subeqc); // -0
muleqc: exec_nodie(o_muleqc); // -0
diveqc: exec_nodie(o_diveqc); // -0
lnkeqc: exec_nodie(o_lnkeqc); // -0
addecp: exec_nodie(o_addecp); // -1
subecp: exec_nodie(o_subecp); // -1
mulecp: exec_nodie(o_mulecp); // -1
divecp: exec_nodie(o_divecp); // -1
lnkecp: exec_nodie(o_lnkecp); // -1
meq:    exec_nodie(o_meq   ); // -1
eq:     exec_nodie(o_eq    ); // -1
neq:    exec_nodie(o_neq   ); // -1
less:   exec_nodie(o_less  ); // -1
leq:    exec_nodie(o_leq   ); // -1
grt:    exec_nodie(o_grt   ); // -1
geq:    exec_nodie(o_geq   ); // -1
lessc:  exec_nodie(o_lessc ); // -0
leqc:   exec_nodie(o_leqc  ); // -0
grtc:   exec_nodie(o_grtc  ); // -0
geqc:   exec_nodie(o_geqc  ); // -0
pop:    exec_nodie(o_pop   ); // -1
jmp:    exec_nodie(o_jmp   ); // -0
jt:     exec_nodie(o_jt    ); // -0
jf:     exec_nodie(o_jf    ); // -1
cnt:    exec_nodie(o_cnt   ); // -0
findex: exec_check(o_findex); // +1
feach:  exec_check(o_feach ); // +1
callg:  exec_check(o_callg ); // +1
calll:  exec_check(o_calll ); // +1
upval:  exec_check(o_upval ); // +1
callv:  exec_nodie(o_callv ); // -0
callvi: exec_nodie(o_callvi); // -0
callh:  exec_nodie(o_callh ); // -0
callfv: exec_nodie(o_callfv); // check in the function
callfh: exec_nodie(o_callfh); // check in the function
callb:  exec_nodie(o_callb ); // -0
slcbeg: exec_check(o_slcbeg); // +1
slcend: exec_nodie(o_slcend); // -1
slc:    exec_nodie(o_slc   ); // -1
slc2:   exec_nodie(o_slc2  ); // -2
mcallg: exec_check(o_mcallg); // +1
mcalll: exec_check(o_mcalll); // +1
mupval: exec_check(o_mupval); // +1
mcallv: exec_nodie(o_mcallv); // -0
mcallh: exec_nodie(o_mcallh); // -0
ret:    exec_nodie(o_ret   ); // -2
#endif
}

}
