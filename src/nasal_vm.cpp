#include "nasal_vm.h"
#include "util/util.h"

namespace nasal {

void vm::vm_init_enrty(const std::vector<std::string>& strs,
                       const std::vector<f64>& nums,
                       const std::vector<nasal_builtin_table>& natives,
                       const std::vector<opcode>& code,
                       const std::unordered_map<std::string, u64>& global_symbol,
                       const std::vector<std::string>& filenames,
                       const std::vector<std::string>& argv) {
    const_number = nums.data();
    const_string = strs.data();
    bytecode = code.data();
    files = filenames.data();
    global_size = global_symbol.size();

    /* set native functions */
    native_function = natives;

    /* set context and global */
    if (!is_repl_mode || first_exec_flag) {
        context_and_global_init();
        first_exec_flag = false;
    }

    /* init gc */
    ngc.set(&ctx, global, global_size);
    ngc.init(strs, argv);

    /* init vm globals */
    auto map_instance = ngc.alloc(vm_type::vm_map);
    global[global_symbol.at("globals")] = map_instance;
    for(const auto& i : global_symbol) {
        map_instance.map().mapper[i.first] = global+i.second;
    }

    /* init vm arg */
    auto arg_instance = ngc.alloc(vm_type::vm_vec);
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

    /* set canary = stack[VM_STACK_DEPTH-1] */
    ctx.canary = ctx.stack+VM_STACK_DEPTH-1;

    /* nothing is on stack */
    ctx.top = ctx.stack - 1;

    /* clear main stack and global */
    for(u32 i = 0; i<VM_STACK_DEPTH; ++i) {
        ctx.stack[i] = nil;
        global[i] = nil;
    }
}

void vm::return_address_info(const var& val) {
    std::clog << "0x";
    std::clog << std::hex << val.ret() << std::dec;
}

void vm::memory_address_info(const var& val) {
    std::clog << "0x";
    std::clog << std::hex << reinterpret_cast<u64>(val.addr()) << std::dec;
}

void vm::raw_string_info(var& val) {
    std::clog << "\"" << util::rawstr(val.str(), 24) << "\"";
}

void vm::upvalue_info(var& val) {
    std::clog << "[" << val.upval().size << " val] ";
    if (val.upval().on_stack) {
        std::clog << "offset:0x" << std::hex;
        std::clog << reinterpret_cast<u64>(val.upval().stack_frame_offset);
        std::clog << std::dec;
    }
}

void vm::vector_value_info(var& val) {
    std::clog << "[" << val.vec().size() << " val]";
}

void vm::hash_value_info(var& val, const usize max_show_elems) {
    std::clog << "{";
    usize count = 0;
    for(const auto& i : val.hash().elems) {
        ++count;
        if (count>max_show_elems) {
            break;
        }

        std::clog << i.first;
        if (count!=val.hash().size()) {
            std::clog << ", ";
        }
    }
    if (val.hash().size()>max_show_elems) {
        std::clog << "...";
    }
    std::clog << "}";
}

void vm::ghost_type_info(var& val) {
    std::clog << "<object:" << val.ghost().type_name;
    std::clog << "@0x" << std::hex << val.ghost().pointer << ">" << std::dec;
}

void vm::coroutine_value_info(var& val) {
    std::clog << "[ ";
    switch(val.co().status) {
        case nas_co::status::dead: std::clog << "dead"; break;
        case nas_co::status::running: std::clog << "running"; break;
        case nas_co::status::suspended: std::clog << "suspended"; break;
    }
    std::clog << " ] @0x";
    std::clog << std::hex << reinterpret_cast<u64>(val.val.gcobj) << std::dec;
}

void vm::namespace_value_info(var& val, const usize max_show_elems) {
    std::clog << "{";
    usize count = 0;
    for(const auto& i : val.map().mapper) {
        ++count;
        if (count>max_show_elems) {
            break;
        }

        std::clog << i.first;
        if (count!=val.map().size()) {
            std::clog << ", ";
        }
    }
    if (val.map().size()>max_show_elems) {
        std::clog << "...";
    }
    std::clog << "}";
}

void vm::value_name_form(const var& val) {
    std::clog << "| ";
    switch(val.type) {
        case vm_type::vm_none:  std::clog << "null  "; break;
        case vm_type::vm_ret:   std::clog << "ret   "; break;
        case vm_type::vm_addr:  std::clog << "addr  "; break;
        case vm_type::vm_cnt:   std::clog << "cnt   "; break;
        case vm_type::vm_nil:   std::clog << "nil   "; break;
        case vm_type::vm_num:   std::clog << "num   "; break;
        case vm_type::vm_str:   std::clog << "str   "; break;
        case vm_type::vm_func:  std::clog << "func  "; break;
        case vm_type::vm_upval: std::clog << "upval "; break;
        case vm_type::vm_vec:   std::clog << "vec   "; break;
        case vm_type::vm_hash:  std::clog << "hash  "; break;
        case vm_type::vm_ghost: std::clog << "ghost "; break;
        case vm_type::vm_co:    std::clog << "co    "; break;
        case vm_type::vm_map:   std::clog << "map   "; break;
        default:                std::clog << "err   "; break;
    }
    std::clog << " | ";
}

void vm::value_info(var& val) {
    value_name_form(val);

    switch(val.type) {
        case vm_type::vm_none: break;
        case vm_type::vm_ret: return_address_info(val); break;
        case vm_type::vm_addr: memory_address_info(val); break;
        case vm_type::vm_cnt: std::clog << val.cnt(); break;
        case vm_type::vm_nil: break;
        case vm_type::vm_num: std::clog << val.num(); break;
        case vm_type::vm_str: raw_string_info(val); break;
        case vm_type::vm_func: std::clog << val.func(); break;
        case vm_type::vm_upval: upvalue_info(val); break;
        case vm_type::vm_vec: vector_value_info(val); break;
        case vm_type::vm_hash: hash_value_info(val, 4); break;
        case vm_type::vm_ghost: ghost_type_info(val); break;
        case vm_type::vm_co: coroutine_value_info(val); break;
        case vm_type::vm_map: namespace_value_info(val, 4); break;
        default: std::clog << "unknown"; break;
    }
    std::clog << "\n";
}

void vm::function_detail_info(const nas_func& func) {
    std::clog << "func@";
    std::clog << std::hex << reinterpret_cast<u64>(&func) << std::dec;
    
    std::vector<std::string> argument_list = {};
    argument_list.resize(func.keys.size());
    for(const auto& key : func.keys) {
        argument_list[key.second-1] = key.first;
    }

    std::clog << "(";
    for(const auto& key : argument_list) {
        std::clog << key;
        if (key != argument_list.back()) {
            std::clog << ", ";
        }
    }
    if (func.dynamic_parameter_index>=0) {
        std::clog << (argument_list.size()? ", ":"");
        std::clog << func.dynamic_parameter_name << "...";
    }
    std::clog << ") ";
    const auto& code = bytecode[func.entry];
    std::clog << "{ entry: " << files[code.fidx] << ":" << code.line << " }";
}

void vm::function_call_trace() {
    util::windows_code_page_manager cp;
    cp.set_utf8_output();

    var* bottom = ctx.stack;
    var* top = ctx.top;

    // generate trace back
    std::stack<const nas_func*> functions;
    std::stack<u64> callsite;

    // load call trace
    for(var* i = bottom; i<=top; ++i) {
        // i-1 is the callsite program counter of this function
        if (i->is_func() && i-1>=bottom && (i-1)->is_ret()) {
            functions.push(&i->func());
            callsite.push((i-1)->ret());
        }
    }

    // function register stores the latest-called function
    if (functions.empty() && !ctx.funcr.is_func()) {
        cp.restore_code_page();
        return;
    }

    std::clog << "\ncall trace ";
    std::clog << (ngc.cort? "(coroutine)":"(main)") << "\n";
    std::clog << "  crash occurred in\n       ";
    function_detail_info(ctx.funcr.func());
    std::clog << " at " << files[bytecode[ctx.pc].fidx] << ":";
    std::clog << bytecode[ctx.pc].line << "\n";

    // another condition may exist:
    // have ret pc on stack, but no function at the top of the ret pc
    for(var* i = top; i>=bottom; --i) {
        if (i->is_ret() && i->ret()!=callsite.top()) {
            std::clog << "  call ";
            function_detail_info(ctx.funcr.func());
            std::clog << " from ";
            std::clog << files[bytecode[i->ret()].fidx] << ":";
            std::clog << bytecode[i->ret()].line << "\n";
            break;
        }
    }

    const nas_func* last = nullptr;
    u64 last_callsite = SIZE_MAX;
    u64 same_count = 0;
    for(; !functions.empty() && !callsite.empty(); functions.pop(), callsite.pop()) {
        auto func = functions.top();
        auto place = callsite.top();

        if (last==func && last_callsite==place) {
            ++same_count;
            continue;
        } else if (same_count) {
            std::clog << "  `--> " << same_count << " same call(s)\n";
            same_count = 0;
        }
        
        last = func;
        last_callsite = place;

        // output called function
        std::clog << "  call ";
        function_detail_info(*func);

        // output callsite
        std::clog << " from ";
        std::clog << files[bytecode[place].fidx] << ":";
        std::clog << bytecode[place].line << "\n";
    }
    if (same_count) {
        std::clog << "  `--> " << same_count << " same call(s)\n";
    }

    cp.restore_code_page();
}

void vm::trace_back() {
    // generate trace back
    std::stack<u32> ret;
    for(var* i = ctx.stack; i<=ctx.top; ++i) {
        if (i->is_ret() && i->ret()!=0) {
            ret.push(i->ret());
        }
    }

    // store the position program crashed
    ret.push(ctx.pc);

    std::clog << "\nback trace ";
    std::clog << (ngc.cort? "(coroutine)":"(main)") << "\n";
    codestream::set(const_number, const_string, native_function.data(), files);

    for(u32 p = 0, same = 0, prev = 0xffffffff; !ret.empty(); prev = p, ret.pop()) {
        if ((p = ret.top())==prev) {
            ++same;
            continue;
        }
        if (same) {
            std::clog << "  0x" << std::hex
                      << std::setw(8) << std::setfill('0')
                      << prev << std::dec << "    "
                      << same << " same call(s)\n";
            same = 0;
        }
        std::clog << "  " << codestream(bytecode[p], p) << "\n";
    }
    // the first called place has no same calls
}

void vm::stack_info(const u64 limit = 16) {
    var* top = ctx.top;
    var* bottom = ctx.stack;
    const auto stack_address = reinterpret_cast<u64>(bottom);

    std::clog << "\nstack (0x" << std::hex << stack_address << std::dec;
    std::clog << ", limit " << limit << ", total ";
    std::clog << (top<bottom? 0:static_cast<i64>(top-bottom+1)) << ")\n";

    for(u32 i = 0; i<limit && top>=bottom; ++i, --top) {
        std::clog << "  0x" << std::hex
                  << std::setw(8) << std::setfill('0')
                  << static_cast<u64>(top-bottom) << std::dec
                  << "    ";
        value_info(top[0]);
    }
}

void vm::register_info() {
    std::clog << "\nregister (" << (ngc.cort? "coroutine":"main") << ")\n";
    std::clog << std::hex
              << "  [ pc     ]    | pc     | 0x" << ctx.pc << "\n"
              << "  [ global ]    | addr   | 0x"
              << reinterpret_cast<u64>(global) << "\n"
              << "  [ local  ]    | addr   | 0x"
              << reinterpret_cast<u64>(ctx.localr) << "\n"
              << "  [ memr   ]    | addr   | 0x"
              << reinterpret_cast<u64>(ctx.memr) << "\n"
              << "  [ canary ]    | addr   | 0x"
              << reinterpret_cast<u64>(ctx.canary) << "\n"
              << "  [ top    ]    | addr   | 0x"
              << reinterpret_cast<u64>(ctx.top) << "\n"
              << std::dec;
    std::clog << "  [ funcr  ]    "; value_info(ctx.funcr);
    std::clog << "  [ upval  ]    "; value_info(ctx.upvalr);
}

void vm::global_state() {
    if (!global_size || global[0].is_none()) {
        return;
    }
    std::clog << "\nglobal (0x" << std::hex
              << reinterpret_cast<u64>(global) << ")\n" << std::dec;
    for(usize i = 0; i<global_size; ++i) {
        std::clog << "  0x" << std::hex << std::setw(8)
                  << std::setfill('0') << i << std::dec
                  << "    ";
        value_info(global[i]);
    }
}

void vm::local_state() {
    if (!ctx.localr || !ctx.funcr.func().local_size) {
        return;
    }
    const u32 lsize = ctx.funcr.func().local_size;
    std::clog << "\nlocal (0x" << std::hex << reinterpret_cast<u64>(ctx.localr)
              << " <+" << static_cast<u64>(ctx.localr-ctx.stack)
              << ">)\n" << std::dec;
    for(u32 i = 0; i<lsize; ++i) {
        std::clog << "  0x" << std::hex << std::setw(8)
                  << std::setfill('0') << i << std::dec
                  << "    ";
        value_info(ctx.localr[i]);
    }
}

void vm::upvalue_state() {
    if (ctx.funcr.is_nil() || ctx.funcr.func().upval.empty()) {
        return;
    }
    std::clog << "\nupvalue\n";
    auto& upval = ctx.funcr.func().upval;
    for(u32 i = 0; i<upval.size(); ++i) {
        std::clog << "  -> upval[" << i << "]:\n";
        auto& uv = upval[i].upval();
        for(u32 j = 0; j<uv.size; ++j) {
            std::clog << "     0x" << std::hex << std::setw(8)
                      << std::setfill('0') << j << std::dec
                      << " ";
            value_info(uv[j]);
        }
    }
}

void vm::all_state_detail() {
    register_info();
    global_state();
    local_state();
    upvalue_state();
}

std::string vm::report_lack_arguments(u32 argc, const nas_func& func) const {
    auto result = std::string("lack argument(s) when calling function:\n  func(");
    std::vector<std::string> argument_list = {};
    argument_list.resize(func.keys.size());
    for(const auto& i : func.keys) {
        argument_list[i.second-1] = i.first;
    }
    for(u32 i = 0; i<argument_list.size(); ++i) {
        result += argument_list[i];
        if (i<argc) {
            result += "[get]";
        }
        if (i!=argument_list.size()-1) {
            result += ", ";
        }
    }
    if (func.dynamic_parameter_index>=0) {
        result += argument_list.size()? ", ":"";
        result += const_string[func.dynamic_parameter_index] + "[dynamic]";
    }
    result += ") ";
    std::stringstream out;
    const auto& code = bytecode[func.entry];
    out << "{ entry: " << files[code.fidx] << ":" << code.line << " }";
    out << " @ 0x" << std::hex << reinterpret_cast<u64>(&func) << std::dec;
    return result + out.str();
}

std::string vm::report_special_call_lack_arguments(var* local,
                                                   const nas_func& func) const {
    auto result = std::string("lack argument(s) when calling function:\n  func(");
    std::vector<std::string> argument_list = {};
    argument_list.resize(func.keys.size());
    for(const auto& i : func.keys) {
        argument_list[i.second-1] = i.first;
    }
    for(const auto& key : argument_list) {
        if (local[func.keys.at(key)].is_none()) {
            result += key + ", ";
        } else {
            result += key + "[get], ";
        }
    }
    result = result.substr(0, result.length()-2);
    result += ") ";
    std::stringstream out;
    const auto& code = bytecode[func.entry];
    out << "{ entry: " << files[code.fidx] << ":" << code.line << " }";
    out << " @ 0x" << std::hex << reinterpret_cast<u64>(&func) << std::dec;
    return result + out.str();
}

std::string vm::report_key_not_found(const std::string& not_found,
                                     const nas_hash& hash) const {
    auto result = "member \"" + not_found + "\" doesn't exist in hash {";
    for(const auto& i : hash.elems) {
        result += i.first + ", ";
    }
    if (hash.elems.size()) {
        result = result.substr(0, result.length()-2);
    }
    result += "}";
    return result;
}

std::string vm::report_out_of_range(f64 index, usize real_size) const {
    auto result = "index out of range: " + std::to_string(index);
    result += " but max size is " + std::to_string(real_size);
    if (!real_size) {
        return result;
    }
    result += ", index range is -" + std::to_string(real_size);
    result += "~" + std::to_string(real_size-1);
    return result;
}

std::string vm::type_name_string(const var& value) const {
    switch(value.type) {
        case vm_type::vm_none: return "none";
        case vm_type::vm_cnt: return "counter";
        case vm_type::vm_addr: return "address";
        case vm_type::vm_ret: return "program counter";
        case vm_type::vm_nil: return "nil";
        case vm_type::vm_num: return "number";
        case vm_type::vm_str: return "string";
        case vm_type::vm_vec: return "vector";
        case vm_type::vm_hash: return "hash";
        case vm_type::vm_func: return "function";
        case vm_type::vm_upval: return "upvalue";
        case vm_type::vm_ghost: return "ghost type";
        case vm_type::vm_co: return "coroutine";
        case vm_type::vm_map: return "namespace";
        default: break;
    }
    return "unknown";
}

void vm::die(const std::string& str) {
    std::cerr << "[vm] error: " << str << "\n";
    function_call_trace();
    trace_back();
    stack_info();

    // show verbose crash info
    if (verbose) {
        all_state_detail();
    }

    if (!ngc.cort) {
        // in main context, exit directly
        std::exit(1);
    } else {
        // in coroutine, shut down the coroutine and return to main context
        ctx.pc = 0; // mark coroutine 'dead'
        ngc.context_reserve(); // switch context to main
        ctx.top[0] = nil; // generate return value 'nil'
    }
}

void vm::run(const codegen& gen,
             const linker& linker,
             const std::vector<std::string>& argv) {
    vm_init_enrty(
        gen.strs(),
        gen.nums(),
        gen.natives(),
        gen.codes(),
        gen.globals(),
        linker.get_file_list(),
        argv
    );

#ifndef _MSC_VER
    // using labels as values/computed goto
    const void* oprs[] = {
        &&vmexit,
        &&repl,
        &&intl,
        &&loadg,
        &&loadl,
        &&loadu,
        &&dup,
        &&pnum,
        &&pnil,
        &&pstr,
        &&newv,
        &&newh,
        &&newf,
        &&happ,
        &&para,
        &&deft,
        &&dyn,
        &&lnot,
        &&usub,
        &&bnot,
        &&btor,
        &&btxor,
        &&btand,
        &&add,
        &&sub,
        &&mul,
        &&div,
        &&lnk,
        &&addc,
        &&subc,
        &&mulc,
        &&divc,
        &&lnkc,
        &&addeq,
        &&subeq,
        &&muleq,
        &&diveq,
        &&lnkeq,
        &&bandeq,
        &&boreq,
        &&bxoreq,
        &&addeqc,
        &&subeqc,
        &&muleqc,
        &&diveqc,
        &&lnkeqc,
        &&addecp,
        &&subecp,
        &&mulecp,
        &&divecp,
        &&lnkecp,
        &&meq,
        &&eq,
        &&neq,
        &&less,
        &&leq,
        &&grt,
        &&geq,
        &&lessc,
        &&leqc,
        &&grtc,
        &&geqc,
        &&pop,
        &&jmp,
        &&jt,
        &&jf,
        &&cnt,
        &&findex,
        &&feach,
        &&callg,
        &&calll,
        &&upval,
        &&callv,
        &&callvi,
        &&callh,
        &&callfv,
        &&callfh,
        &&callb,
        &&slcbeg,
        &&slcend,
        &&slc,
        &&slc2,
        &&mcallg,
        &&mcalll,
        &&mupval,
        &&mcallv,
        &&mcallh,
        &&ret
    };
    std::vector<const void*> code;
    for(const auto& i : gen.codes()) {
        code.push_back(oprs[i.op]);
        imm.push_back(i.num);
    }
    // goto the first operand
    goto *code[ctx.pc];
#else
    std::vector<nasal_vm_func> code;
    for(const auto& i : gen.codes()) {
        code.push_back(operand_function[i.op]);
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
// all nasal programs should end here
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
dup:    exec_check(o_dup   ); // +1
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
