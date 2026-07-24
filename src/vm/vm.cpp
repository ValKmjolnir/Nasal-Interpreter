#include "vm/vm.hpp"
#include "util/util.hpp"
#include "code/codestream.hpp"

namespace nasal {

void vm::vm_init_entry(const std::vector<std::string>& strs,
                       const std::vector<f64>& nums,
                       const std::vector<nasal_builtin_info>& natives,
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
    auto map_instance = ngc.alloc(gc_type::gc_map);
    global_symbol_name.resize(global_symbol.size());
    global[global_symbol.at("globals")] = map_instance;
    for (const auto& i : global_symbol) {
        map_instance.map().mapper[i.first] = global + i.second;
        global_symbol_name[i.second] = i.first;
    }

    /* init vm arg */
    auto arg_instance = ngc.alloc(gc_type::gc_vec);
    global[global_symbol.at("arg")] = arg_instance;
    arg_instance.vec().elems = ngc.env_argv;
}

void vm::context_and_global_init() {
    /* clear context status */
    ctx.clear();

    /* clear main stack and global */
    for (u32 i = 0; i < VM_STACK_DEPTH; ++i) {
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
    for (const auto& i : val.hash().elems) {
        ++count;
        if (count > max_show_elems) {
            break;
        }

        std::clog << i.first;
        if (count != val.hash().size()) {
            std::clog << ", ";
        }
    }
    if (val.hash().size() > max_show_elems) {
        std::clog << "...";
    }
    std::clog << "}";
}

void vm::coroutine_value_info(var& val) {
    std::clog << "[ ";
    switch (val.co().status) {
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
    for (const auto& i : val.map().mapper) {
        ++count;
        if (count > max_show_elems) {
            break;
        }

        std::clog << i.first;
        if (count != val.map().size()) {
            std::clog << ", ";
        }
    }
    if (val.map().size() > max_show_elems) {
        std::clog << "...";
    }
    std::clog << "}";
}

void vm::value_name_form(const var& val) {
    std::clog << "| ";
    switch (val.type) {
        case vm_type::vm_none: std::clog << "null "; break;
        case vm_type::vm_ret:  std::clog << "ret  "; break;
        case vm_type::vm_addr: std::clog << "addr "; break;
        case vm_type::vm_nil:  std::clog << "nil  "; break;
        case vm_type::vm_num:  std::clog << "num  "; break;
        case vm_type::vm_gcobj:
            switch (val.val.gcobj->type) {
                case gc_type::gc_str:   std::clog << "str  "; break;
                case gc_type::gc_func:  std::clog << "func "; break;
                case gc_type::gc_upval: std::clog << "upval"; break;
                case gc_type::gc_vec:   std::clog << "vec  "; break;
                case gc_type::gc_hash:  std::clog << "hash "; break;
                case gc_type::gc_ghost: std::clog << "ghost"; break;
                case gc_type::gc_co:    std::clog << "co   "; break;
                case gc_type::gc_map:   std::clog << "map  "; break;
            } break;
        default:               std::clog << "err  "; break;
    }
    std::clog << " | ";
}

void vm::value_info(var& val) {
    value_name_form(val);

    switch (val.type) {
        case vm_type::vm_none: break;
        case vm_type::vm_ret: return_address_info(val); break;
        case vm_type::vm_addr: memory_address_info(val); break;
        case vm_type::vm_nil: break;
        case vm_type::vm_num: std::clog << val.num(); break;
        case vm_type::vm_gcobj:
            switch (val.val.gcobj->type) {
                case gc_type::gc_str: raw_string_info(val); break;
                case gc_type::gc_func: std::clog << val.func(); break;
                case gc_type::gc_upval: upvalue_info(val); break;
                case gc_type::gc_vec: vector_value_info(val); break;
                case gc_type::gc_hash: hash_value_info(val, 4); break;
                case gc_type::gc_ghost: std::clog << val.ghost(); break;
                case gc_type::gc_co: coroutine_value_info(val); break;
                case gc_type::gc_map: namespace_value_info(val, 4); break;
            } break;
        default: std::clog << "unknown"; break;
    }
    std::clog << "\n";
}

void vm::function_detail_info(const nas_func& func) {
    std::clog << "func ";

    std::vector<std::string> argument_list = {};
    argument_list.resize(func.keys.size());
    for (const auto& key : func.keys) {
        argument_list[key.second-1] = key.first;
    }

    std::clog << "(";
    for (const auto& key : argument_list) {
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
    // no function is called when error ocurred
    if (!ctx.funcr.is_func()) {
        return;
    }

    util::windows_code_page_manager cp;
    cp.set_utf8_output();

    var* bottom = ctx.stack;
    var* top = ctx.top;

    // generate trace back
    std::vector<const nas_func*> functions;
    std::vector<u64> callsite;

    var* prev_func = &ctx.funcr;
    functions.push_back(&prev_func->func());
    for (var* i = top; i >= bottom; i--) {
        // +-------+------------------+
        // | ret   | 0x3bf            | <-- i + 1 (should not be 0, except coroutine)
        // +-------+------------------+
        // | addr  | 0x7ff5f61ae020   | <-- i
        // +-------+------------------+
        // | upval | ...              | <-- i - 1
        // +-------+------------------+
        // | locals| ...              |
        // +-------+------------------+
        // | func  | function         | <-- i - 1 - prev_func->local_size - 1
        // +-------+------------------+
        if (i + 1 <= top && i[0].is_addr() && i[1].is_ret()) {
            auto r_addr = i[1].ret();
            callsite.push_back(r_addr);
            i--;
            i -= prev_func->func().local_size;
            i--;
            if (i >= bottom && i[0].is_func()) {
                prev_func = i;
                functions.push_back(&prev_func->func());
            }
        }
    }

    std::clog << "\ncall trace ";
    std::clog << (ngc.cort? "(coroutine)":"(main)") << "\n";
    std::clog << "  crash occurred at\n       ";
    function_detail_info(ctx.funcr.func());
    std::clog << " at " << files[bytecode[ctx.pc].fidx] << ":";
    std::clog << bytecode[ctx.pc].line << "\n";

    if (callsite.empty()) {
        cp.restore_code_page();
        return;
    }

    const nas_func* prev = nullptr;
    u64 prev_addr = 0;
    u64 same_call_count = 0;
    for (int i = 0; i < functions.size(); ++i) {
        if (functions[i] == prev && callsite[i] == prev_addr) {
            same_call_count++;
            continue;
        } else if (same_call_count) {
            std::clog << "  `--> " << same_call_count << " same call(s)\n";
            same_call_count = 0;
        }

        // in coroutine
        if (callsite[i] == 0 && ngc.cort) {
            std::clog << "  call by coroutine\n";
            break;
        }

        std::clog << "  call ";
        function_detail_info(*functions[i]);
        auto r_addr = callsite[i];
        std::clog << " from " << files[bytecode[r_addr].fidx] << ":";
        std::clog << bytecode[r_addr].line << "\n";

        prev = functions[i];
        prev_addr = r_addr;
    }
    if (same_call_count) {
        std::clog << "  `--> " << same_call_count << " same call(s)\n";
    }

    cp.restore_code_page();
}

void vm::trace_back() {
    // generate trace back
    std::stack<u64> ret;
    for (var* i = ctx.stack; i <= ctx.top; ++i) {
        if (i->is_ret() && i->ret() != 0) {
            ret.push(i->ret());
        }
    }

    // store the position program crashed
    ret.push(ctx.pc);

    std::clog << "\nback trace ";
    std::clog << (ngc.cort ? "(coroutine)" : "(main)") << "\n";
    auto cs = codestream(
        const_number,
        const_string,
        global_symbol_name,
        native_function.data(),
        files
    );

    for (u64 p = 0, same = 0, prev = 0xffffffff; !ret.empty(); prev = p, ret.pop()) {
        if ((p = ret.top()) == prev) {
            ++same;
            continue;
        } else if (same) {
            std::clog << "  0x" << std::hex
                      << std::setw(8) << std::setfill('0')
                      << prev << std::dec << "    "
                      << same << " same call(s)\n";
            same = 0;
        }
        std::clog << "  " << cs.create(bytecode[p], p) << "\n";
    }
    // the first called place has no same calls
}

void vm::stack_info(const u64 limit) {
    var* top = ctx.top;
    var* bottom = ctx.stack;
    const auto stack_address = reinterpret_cast<u64>(bottom);

    std::clog << "\nstack (0x" << std::hex << stack_address << std::dec;
    std::clog << ", limit " << limit << ", total ";
    std::clog << (top<bottom? 0:static_cast<i64>(top-bottom+1)) << ")\n";

    for (u32 i = 0; i<limit && top>=bottom; ++i, --top) {
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
              << "  [ pc     ]    | pc    | 0x" << ctx.pc << "\n"
              << "  [ global ]    | addr  | 0x"
              << reinterpret_cast<u64>(global) << "\n"
              << "  [ local  ]    | addr  | 0x"
              << reinterpret_cast<u64>(ctx.localr) << "\n"
              << "  [ memr   ]    | addr  | 0x"
              << reinterpret_cast<u64>(ctx.memr) << "\n"
              << "  [ canary ]    | addr  | 0x"
              << reinterpret_cast<u64>(ctx.canary) << "\n"
              << "  [ top    ]    | addr  | 0x"
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
    for (usize i = 0; i<global_size; ++i) {
        std::clog << "  0x" << std::hex << std::setw(8)
                  << std::setfill('0') << static_cast<u64>(i) << std::dec
                  << "    ";
        auto name = global_symbol_name[i];
        if (name.length()>=10) {
            name = name.substr(0, 7) + "...";
        } else {

        }
        std::clog << "| " << std::left << std::setw(10)
                  << std::setfill(' ') << name << " "
                  << std::internal;
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
    for (u32 i = 0; i<lsize; ++i) {
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
    for (u32 i = 0; i<upval.size(); ++i) {
        std::clog << "  -> upval[" << i << "]:\n";
        auto& uv = upval[i].upval();
        for (u32 j = 0; j<uv.size; ++j) {
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
    for (const auto& i : func.keys) {
        argument_list[i.second-1] = i.first;
    }
    for (u32 i = 0; i<argument_list.size(); ++i) {
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
    for (const auto& i : func.keys) {
        argument_list[i.second-1] = i.first;
    }
    for (const auto& key : argument_list) {
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
    for (const auto& i : hash.elems) {
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
    switch (value.type) {
        case vm_type::vm_none: return "none";
        case vm_type::vm_addr: return "address";
        case vm_type::vm_ret:  return "program counter";
        case vm_type::vm_nil:  return "nil";
        case vm_type::vm_num:  return "number";
        case vm_type::vm_gcobj:
            switch (value.val.gcobj->type) {
                case gc_type::gc_str:   return "string";
                case gc_type::gc_vec:   return "vector";
                case gc_type::gc_hash:  return "hash";
                case gc_type::gc_func:  return "function";
                case gc_type::gc_upval: return "upvalue";
                case gc_type::gc_ghost: return "ghost type";
                case gc_type::gc_co:    return "coroutine";
                case gc_type::gc_map:   return "namespace";
            } break;
        default: break;
    }
    return "unknown";
}

void vm::die(const std::string& str) {
    const auto& file = files[bytecode[ctx.pc].fidx];
    const auto line = bytecode[ctx.pc].line;
    std::cerr << "[vm] error occurred at " << file << ":" << line << ": ";
    std::cerr << str << "\n";
    function_call_trace();

    // trace back contains bytecode info, dump in verbose mode
    if (verbose) {
        trace_back();
    }

    // verbose will dump more values on stack
    if (verbose) {
        stack_info(64);
    }

    // show verbose crash info
    if (verbose) {
        all_state_detail();
    }

    if (!ngc.cort) {
        if (!verbose) {
            std::cerr << "\n[vm] use <-d> for detailed crash info.\n\n";
        }
        // in main context, exit directly
        std::exit(1);
    }

    // in coroutine, shut down the coroutine and return to main context
    ctx.pc = 0; // mark coroutine 'dead'
    ngc.context_reserve(); // switch context to main
    ctx.top[0] = nil; // generate return value 'nil'
}

bool vm::boolify(const var& val) {
    if (val.is_num()) {
        return val.num();
    } else if (val.is_str()) {
        const f64 num = util::str_to_num(val.str().c_str());
        return std::isnan(num)? !val.str().empty() : num;
    } else if (val.is_vec()) {
        return val.vec().size() > 0;
    } else if (val.is_hash()) {
        return val.hash().size() > 0;
    } else if (val.is_func() || val.is_ghost() || val.is_coroutine()) {
        return true;
    } else if (val.is_map()) {
        return val.map().size() > 0;
    }
    return false;
}

void vm::set_frame(const nas_func& func, var* local) {
    ctx.top[0] = ctx.upvalr;
    (++ctx.top)[0] = var::addr(ctx.localr);
    (++ctx.top)[0] = var::ret(ctx.pc); // rewrite top with vm_ret
    ctx.pc = func.entry - 1;
    ctx.localr = local;
    ctx.upvalr = nil;
}

void vm::o_repl() {
    // reserved for repl mode stack top value output
    if (allow_repl_output) {
        std::cout << ctx.top[0] << "\n";
    }
}

void vm::o_intl() {
    ctx.top[0].func().local.resize(imm[ctx.pc], nil);
    ctx.top[0].func().local_size = imm[ctx.pc];
}

void vm::o_loadg() {
    global[imm[ctx.pc]] = (ctx.top--)[0];
}

void vm::o_loadl() {
    ctx.localr[imm[ctx.pc]] = (ctx.top--)[0];
}

void vm::o_loadu() {
    ctx.funcr.func().upval[(imm[ctx.pc]>>16) & 0xffff]
                    .upval()[imm[ctx.pc] & 0xffff] = (ctx.top--)[0];
}

void vm::o_dup() {
    ctx.top[1] = ctx.top[0];
    ++ctx.top;
}

void vm::o_pnum() {
    (++ctx.top)[0] = var::num(const_number[imm[ctx.pc]]);
}

void vm::o_pnil() {
    (++ctx.top)[0] = nil;
}

void vm::o_pstr() {
    (++ctx.top)[0] = ngc.strs[imm[ctx.pc]];
}

void vm::o_newv() {
    var newv = ngc.alloc(gc_type::gc_vec);
    auto size = imm[ctx.pc];
    // use top -= imm[pc] - 1 here will cause error if imm[pc] is 0
    ctx.top = ctx.top - size + 1;
    if (size) {
        auto& vec = newv.vec().elems;
        vec.resize(size);
        memcpy(vec.data(), ctx.top, size * sizeof(var));
    }
    ctx.top[0] = newv;
}

void vm::o_newh() {
    (++ctx.top)[0] = ngc.alloc(gc_type::gc_hash);
}

void vm::o_newf() {
    (++ctx.top)[0] = ngc.alloc(gc_type::gc_func);
    auto& func = ctx.top[0].func();
    func.entry = imm[ctx.pc];
    func.parameter_size = 1;

    /* this means you create a new function in local scope */
    if (ctx.localr) {
        // copy upval scope list from upper level function
        func.upval = ctx.funcr.func().upval;

        // function created in the same local scope shares same closure
        var upval = (ctx.upvalr.is_nil())
            ? ngc.alloc(gc_type::gc_upval)
            : ctx.upvalr;
        // if no upval scope exists, now it's time to create one
        if (ctx.upvalr.is_nil()) {
            upval.upval().size = ctx.funcr.func().local_size;
            upval.upval().stack_frame_offset = ctx.localr;
            ctx.upvalr = upval;
        }

        func.upval.push_back(upval);
    }
}

void vm::o_happ() {
    ctx.top[-1].hash().elems[const_string[imm[ctx.pc]]] = ctx.top[0];
    --ctx.top;
}

void vm::o_para() {
    auto& func = ctx.top[0].func();
    // func->size has 1 place reserved for "me"
    func.keys[const_string[imm[ctx.pc]]] = func.parameter_size;
    func.local[func.parameter_size++] = var::none();
}

void vm::o_default() {
    var val = ctx.top[0];
    auto& func = (--ctx.top)[0].func();
    // func->size has 1 place reserved for "me"
    func.keys[const_string[imm[ctx.pc]]] = func.parameter_size;
    func.local[func.parameter_size++] = val;
}

void vm::o_dyn() {
    ctx.top[0].func().dynamic_parameter_index = imm[ctx.pc];
    ctx.top[0].func().dynamic_parameter_name = const_string[imm[ctx.pc]];
}

void vm::o_lnot() {
    var val = ctx.top[0];
    if (val.is_nil()) {
        ctx.top[0] = one;
    } else if (val.is_num()) {
        ctx.top[0] = val.num() ? zero : one;
    } else if (val.is_str()) {
        const auto num = util::str_to_num(val.str().c_str());
        if (std::isnan(num)) {
            ctx.top[0] = var::num(static_cast<f64>(val.str().empty()));
        } else {
            ctx.top[0] = num ? zero : one;
        }
    } else {
        die("cannot do not-operation on " + type_name_string(val));
    }
}

void vm::o_usub() {
    ctx.top[0] = var::num(-ctx.top[0].to_num());
}

void vm::o_bnot() {
    ctx.top[0] = var::num(~static_cast<int32_t>(ctx.top[0].num()));
}

void vm::o_btor() {
    ctx.top[-1] = var::num(
        static_cast<i32>(ctx.top[-1].to_num()) |
        static_cast<i32>(ctx.top[0].to_num())
    );
    --ctx.top;
}

void vm::o_btxor() {
    ctx.top[-1] = var::num(
        static_cast<i32>(ctx.top[-1].to_num()) ^
        static_cast<i32>(ctx.top[0].to_num())
    );
    --ctx.top;
}

void vm::o_btand() {
    ctx.top[-1] = var::num(
        static_cast<i32>(ctx.top[-1].to_num()) &
        static_cast<i32>(ctx.top[0].to_num())
    );
    --ctx.top;
}

#define op_calc(type)\
    ctx.top[-1] = var::num(ctx.top[-1].to_num() type ctx.top[0].to_num());\
    --ctx.top;

void vm::o_add() { op_calc(+); }
void vm::o_sub() { op_calc(-); }
void vm::o_mul() { op_calc(*); }
void vm::o_div() { op_calc(/); }
void vm::o_lnk() {
    // concat two vectors into one
    if (ctx.top[-1].is_vec() && ctx.top[0].is_vec()) {
        ngc.temp = ngc.alloc(gc_type::gc_vec);
        ngc.temp.vec().elems.reserve(
            ctx.top[-1].vec().elems.size() +
            ctx.top[0].vec().elems.size()
        );
        for (auto& i : ctx.top[-1].vec().elems) {
            ngc.temp.vec().elems.push_back(i);
        }
        for (auto& i : ctx.top[0].vec().elems) {
            ngc.temp.vec().elems.push_back(i);
        }
        ctx.top[-1] = ngc.temp;
        ngc.temp = nil;
        --ctx.top;
        return;
    }
    // concat strings
    ctx.top[-1] = ngc.alloc_str(ctx.top[-1].to_str() + ctx.top[0].to_str());
    --ctx.top;
}

#define op_calc_const(type)\
    ctx.top[0] = var::num(ctx.top[0].to_num() type const_number[imm[ctx.pc]]);

void vm::o_addc() { op_calc_const(+); }
void vm::o_subc() { op_calc_const(-); }
void vm::o_mulc() { op_calc_const(*); }
void vm::o_divc() { op_calc_const(/); }
void vm::o_lnkc() {
    ctx.top[0] = ngc.alloc_str(ctx.top[0].to_str() + const_string[imm[ctx.pc]]);
}

// top[0] stores the value of memr[0], to avoid being garbage-collected
// so when the calculation ends, top -= 1, then top -= imm[pc]
// because this return value is meaningless if on stack when imm[pc] = 1
// like this: func { a += c; }(); the result of 'a+c' will no be used later, imm[pc] = 1
// but if b += a += c; the result of 'a + c' will be used later, imm[pc] = 0
#define op_calc_eq(type)\
    ctx.top[-1] = ctx.memr[0] = var::num(\
        ctx.memr[0].to_num() type ctx.top[-1].to_num()\
    );\
    ctx.memr = nullptr;\
    ctx.top -= imm[ctx.pc] + 1;

void vm::o_addeq() { op_calc_eq(+); }
void vm::o_subeq() { op_calc_eq(-); }
void vm::o_muleq() { op_calc_eq(*); }
void vm::o_diveq() { op_calc_eq(/); }
void vm::o_lnkeq() {
    // concat two vectors into one
    if (ctx.top[-1].is_vec() && ctx.memr[0].is_vec()) {
        ngc.temp = ngc.alloc(gc_type::gc_vec);
        for (auto i : ctx.memr[0].vec().elems) {
            ngc.temp.vec().elems.push_back(i);
        }
        for (auto i : ctx.top[-1].vec().elems) {
            ngc.temp.vec().elems.push_back(i);
        }
        ctx.top[-1] = ctx.memr[0] = ngc.temp;
        ngc.temp = nil;
        ctx.memr = nullptr;
        ctx.top -= imm[ctx.pc]+1;
        return;
    }

    ctx.top[-1] = ctx.memr[0] = ngc.alloc_str(
        ctx.memr[0].to_str() + ctx.top[-1].to_str()
    );
    ctx.memr = nullptr;
    ctx.top -= imm[ctx.pc] + 1;
}

void vm::o_bandeq() {
    ctx.top[-1] = ctx.memr[0] = var::num(
        static_cast<i32>(ctx.memr[0].to_num())&
        static_cast<i32>(ctx.top[-1].to_num())
    );
    ctx.memr = nullptr;
    ctx.top -= imm[ctx.pc]+1;
}

void vm::o_boreq() {
    ctx.top[-1] = ctx.memr[0] = var::num(
        static_cast<i32>(ctx.memr[0].to_num())|
        static_cast<i32>(ctx.top[-1].to_num())
    );
    ctx.memr = nullptr;
    ctx.top -= imm[ctx.pc]+1;
}

void vm::o_bxoreq() {
    ctx.top[-1] = ctx.memr[0] = var::num(
        static_cast<i32>(ctx.memr[0].to_num()) ^
        static_cast<i32>(ctx.top[-1].to_num())
    );
    ctx.memr = nullptr;
    ctx.top -= imm[ctx.pc] + 1;
}

// top[0] stores the value of memr[0], to avoid being garbage-collected
// so when the calculation ends, top -= imm[pc] >> 31
// because this return value is meaningless if on stack when imm[pc] >> 31 = 1
// like this: func { a += 1; }(); the result of 'a + 1' will no be used later, imm[pc] >> 31 = 1
// but if b += a += 1; the result of 'a + 1' will be used later, imm[pc] >> 31 = 0
#define op_calc_eq_const(type)\
    ctx.top[0] = ctx.memr[0] = var::num(\
        ctx.memr[0].to_num() type const_number[imm[ctx.pc]]\
    );\
    ctx.memr = nullptr;

void vm::o_addeqc() { op_calc_eq_const(+); }
void vm::o_subeqc() { op_calc_eq_const(-); }
void vm::o_muleqc() { op_calc_eq_const(*); }
void vm::o_diveqc() { op_calc_eq_const(/); }
void vm::o_lnkeqc() {
    ctx.top[0] = ctx.memr[0] = ngc.alloc_str(
        ctx.memr[0].to_str() + const_string[imm[ctx.pc]]
    );
    ctx.memr = nullptr;
}

#define op_calc_eq_const_and_pop(type)\
    ctx.top[0] = ctx.memr[0] = var::num(\
        ctx.memr[0].to_num() type const_number[imm[ctx.pc]]\
    );\
    ctx.memr = nullptr;\
    --ctx.top;

void vm::o_addecp() { op_calc_eq_const_and_pop(+); }
void vm::o_subecp() { op_calc_eq_const_and_pop(-); }
void vm::o_mulecp() { op_calc_eq_const_and_pop(*); }
void vm::o_divecp() { op_calc_eq_const_and_pop(/); }
void vm::o_lnkecp() {
    ctx.top[0] = ctx.memr[0] = ngc.alloc_str(
        ctx.memr[0].to_str() + const_string[imm[ctx.pc]]
    );
    ctx.memr = nullptr;
    --ctx.top;
}

void vm::o_meq() {
    // pop old memr[0] and replace it
    // the reason why we should get memr and push the old value on stack
    // is that when lnkeq/lnkeqc is called, there will be
    // a new gc object vm_str which is returned by gc::alloc
    // this may cause gc, so we should temporarily put it on stack
    ctx.memr[0] = ctx.top[-1];
    ctx.memr = nullptr;
    ctx.top -= imm[ctx.pc]+1;
}

void vm::o_eq() {
    var val2 = ctx.top[0];
    var val1 = (--ctx.top)[0];
    if (val1.is_nil() && val2.is_nil()) {
        ctx.top[0] = one;
    } else if (val1.is_str() && val2.is_str()) {
        ctx.top[0] = (val1.str() == val2.str()) ? one : zero;
    } else if ((val1.is_num() || val2.is_num())
        && !val1.is_nil() && !val2.is_nil()) {
        ctx.top[0] = (val1.to_num() == val2.to_num()) ? one : zero;
    } else {
        ctx.top[0] = (val1==val2)? one : zero;
    }
}

void vm::o_neq() {
    var val2 = ctx.top[0];
    var val1 = (--ctx.top)[0];
    if (val1.is_nil() && val2.is_nil()) {
        ctx.top[0] = zero;
    } else if (val1.is_str() && val2.is_str()) {
        ctx.top[0] = (val1.str() != val2.str()) ? one : zero;
    } else if ((val1.is_num() || val2.is_num())
        && !val1.is_nil() && !val2.is_nil()) {
        ctx.top[0] = (val1.to_num() != val2.to_num()) ? one : zero;
    } else {
        ctx.top[0] = (val1!=val2)? one : zero;
    }
}

#define op_cmp(type)\
    --ctx.top;\
    ctx.top[0] = (ctx.top[0].to_num() type ctx.top[1].to_num()) ? one : zero;

void vm::o_less() { op_cmp(<); }
void vm::o_leq() { op_cmp(<=); }
void vm::o_grt() { op_cmp(>); }
void vm::o_geq() { op_cmp(>=); }

#define op_cmp_const(type)\
    ctx.top[0] = (ctx.top[0].to_num() type const_number[imm[ctx.pc]]) ? one : zero;

void vm::o_lessc() { op_cmp_const(<); }
void vm::o_leqc() { op_cmp_const(<=); }
void vm::o_grtc() { op_cmp_const(>); }
void vm::o_geqc() { op_cmp_const(>=); }

void vm::o_pop() {
    --ctx.top;
}

void vm::o_jmp() {
    ctx.pc = imm[ctx.pc] - 1;
}

void vm::o_jt() {
    // jump true needs to reserve the result on stack
    // because conditional expression in nasal has return value
    if (boolify(ctx.top[0])) {
        ctx.pc = imm[ctx.pc] - 1;
    }
}

void vm::o_jf() {
    // jump false doesn't need to reserve result
    if (!boolify(ctx.top[0])) {
        ctx.pc = imm[ctx.pc] - 1;
    }
    --ctx.top;
}

void vm::o_cnt() {
    if (!ctx.top[0].is_vec()) {
        die("must use vector in forindex/foreach but get " +
            type_name_string(ctx.top[0])
        );
        return;
    }
    (++ctx.top)[0] = var::num(-1.0f);
}

void vm::o_findex() {
    ctx.top[0] = var::num(ctx.top[0].num() + 1.0f);
    if ((usize)ctx.top[0].num() >= ctx.top[-1].vec().size()) {
        ctx.pc = imm[ctx.pc] - 1;
        return;
    }
    ctx.top[1] = ctx.top[0];
    ++ctx.top;
}

void vm::o_feach() {
    ctx.top[0] = var::num(ctx.top[0].num() + 1.0f);
    auto& ref = ctx.top[-1].vec().elems;
    if ((usize)ctx.top[0].num() >= ref.size()) {
        ctx.pc = imm[ctx.pc]-1;
        return;
    }
    ctx.top[1] = ref[ctx.top[0].num()];
    ++ctx.top;
}

void vm::o_callg() {
    // get main stack directly
    (++ctx.top)[0] = global[imm[ctx.pc]];
}

void vm::o_calll() {
    (++ctx.top)[0] = ctx.localr[imm[ctx.pc]];
}

void vm::o_upval() {
    (++ctx.top)[0] = ctx.funcr.func()
        .upval[(imm[ctx.pc] >> 16) & 0xffff]
        .upval()[imm[ctx.pc] & 0xffff];
}

void vm::o_callv() {
    var val = ctx.top[0];
    var vec = (--ctx.top)[0];
    if (vec.is_vec()) {
        ctx.top[0] = vec.vec().get_value(val.to_num());
        if (ctx.top[0].is_none()) {
            die(report_out_of_range(val.to_num(), vec.vec().size()));
            return;
        }
    } else if (vec.is_hash()) {
        if (!val.is_str()) {
            die("must use string as the key but get " + type_name_string(val));
            return;
        }
        ctx.top[0] = vec.hash().get_value(val.str());
        if (ctx.top[0].is_none()) {
            die(report_key_not_found(val.str(), vec.hash()));
            return;
        } else if (ctx.top[0].is_func()) {
            ctx.top[0].func().local[0] = val; // 'me'
        }
    } else if (vec.is_str()) {
        const auto& str = vec.str();
        i32 num = val.to_num();
        i32 len = str.length();
        if (num < -len || num >= len) {
            die(report_out_of_range(num, str.size()));
            return;
        }
        ctx.top[0] = var::num(
            static_cast<f64>(static_cast<u8>(str[num>=0? num : num + len]))
        );
    } else if (vec.is_map()) {
        if (!val.is_str()) {
            die("must use string as the key but get " + type_name_string(val));
            return;
        }
        ctx.top[0] = vec.map().get_value(val.str());
        if (ctx.top[0].is_none()) {
            die("cannot find symbol \"" + val.str() + "\"");
            return;
        }
    } else {
        die("must get element from vector/hash/string, but get " + type_name_string(vec));
        return;
    }
}

void vm::o_callvi() {
    var val = ctx.top[0];
    if (!val.is_vec()) {
        die("must use a vector but get " + type_name_string(val));
        return;
    }
    // cannot use operator[], because this may cause overflow
    (++ctx.top)[0] = val.vec().get_value(imm[ctx.pc]);
    if (ctx.top[0].is_none()) {
        die(report_out_of_range(imm[ctx.pc], val.vec().size()));
        return;
    }
}

void vm::o_callh() {
    var val = ctx.top[0];
    if (!val.is_hash() && !val.is_map()) {
        die("must get element from hash, but get " + type_name_string(val));
        return;
    }

    const auto& str = const_string[imm[ctx.pc]];
    if (val.is_hash()) {
        ctx.top[0] = val.hash().get_value(str);
    } else {
        ctx.top[0] = val.map().get_value(str);
    }

    // report key not found if get_value returns none
    if (ctx.top[0].is_none()) {
        val.is_hash()
            ? die(report_key_not_found(str, val.hash()))
            : die("cannot find symbol \"" + str + "\"");
        return;
    }

    // if get function from hash, set 'me'
    if (ctx.top[0].is_func() && val.is_hash()) {
        ctx.top[0].func().local[0] = val; // 'me'
    }
}

void vm::o_callfv() {
    const auto argc = imm[ctx.pc]; // arguments counter
    var* local = ctx.top - argc + 1; // arguments begin address
    if (!local[-1].is_func()) {
        die("must call a function but get " + type_name_string(local[-1]));
        return;
    }
    const auto& func = local[-1].func();

    // swap funcr with local[-1]
    var tmp = local[-1];
    local[-1] = ctx.funcr;
    ctx.funcr = tmp;
    (++ctx.func_top)[0] = {
        tmp,
        bytecode[ctx.pc].fidx,
        bytecode[ctx.pc].line
    };

    // top-argc+lsize(local) +1(old pc) +1(old localr) +1(old upvalr)
    if (ctx.top - argc + func.local_size + 3 >= ctx.canary) {
        die("stack overflow");
        return;
    }
    // parameter size is func->psize-1, 1 is reserved for "me"
    const u64 parameter_size = func.parameter_size - 1;
    if (argc < parameter_size && func.local[argc + 1].is_none()) {
        die(report_lack_arguments(argc, func));
        return;
    }

    // load dynamic argument, default nil, for better performance
    var dynamic = nil;
    if (func.dynamic_parameter_index >= 0 || parameter_size < argc) {
        // load dynamic argument in 2 situations:
        //   1. function has dynamic parameter
        //   2. function has more arguments than parameters
        //      - load arguments to default dynamic argument "arg"
        //      - located at stack + 1
        dynamic = ngc.alloc(gc_type::gc_vec);
        for (u64 i = parameter_size; i < argc; ++i) {
            dynamic.vec().elems.push_back(local[i]);
        }
    }

    // should reset stack top after allocating vector
    // because this may cause gc
    // then all the available values the vector needs
    // are all outside the stack top and may be
    // collected incorrectly
    ctx.top = local + func.local_size;

    // use (std::min) to avoid compilation error in MSVC
    // MSVC windows.h uses macro std::min
    const u64 min_size = (std::min)(parameter_size, argc);

    // load arguments
    for (u64 i = min_size; i >= 1; --i) {
        local[i] = local[i - 1];
    }
    local[0] = func.local[0]; // load "me"

    // load local scope & default arguments
    for (u64 i = min_size + 1; i < func.local_size; ++i) {
        local[i] = func.local[i];
    }

    // load dynamic argument
    local[func.dynamic_parameter_index >= 0
            ? parameter_size + 1
            : func.local_size - 1] = dynamic;

    set_frame(func, local);
}

void vm::o_callfh() {
    const auto& hash = ctx.top[0].hash().elems;
    if (!ctx.top[-1].is_func()) {
        die("must call a function but get " + type_name_string(ctx.top[-1]));
        return;
    }
    const auto& func = ctx.top[-1].func();
    var tmp = ctx.top[-1];
    ctx.top[-1] = ctx.funcr;
    ctx.funcr = tmp;
    (++ctx.func_top)[0] = {
        tmp,
        bytecode[ctx.pc].fidx,
        bytecode[ctx.pc].line
    };

    // top -1(hash) +lsize(local) +1(old pc) +1(old localr) +1(old upvalr)
    if (ctx.top+func.local_size+2>= ctx.canary) {
        die("stack overflow");
        return;
    }
    // dynamic parameter is not allowed in this kind of function call
    if (func.dynamic_parameter_index>=0) {
        die("special call cannot use dynamic argument \"" +
            const_string[func.dynamic_parameter_index] + "\""
        );
        return;
    }

    var* local = ctx.top;
    ctx.top += func.local_size;
    for (u32 i = 0; i<func.local_size; ++i) {
        local[i] = func.local[i];
    }

    bool lack_arguments_flag = false;
    for (const auto& i : func.keys) {
        const auto& key = i.first;
        if (hash.count(key)) {
            local[i.second] = hash.at(key);
        } else if (local[i.second].is_none()) {
            lack_arguments_flag = true;
        }
    }
    if (lack_arguments_flag) {
        die(report_special_call_lack_arguments(local, func));
        return;
    }

    set_frame(func, local);
}

void vm::o_callb() {
    // reserve place for native function return,
    // this code is written for coroutine
    (++ctx.top)[0] = nil;

    // set file list into ctx
    ctx.files = files;

    // if running a native function about coroutine
    // (top) will be set to another context.top, instead of main_context.top
    auto function_pointer = native_function[imm[ctx.pc]].func;
    var result = (*function_pointer)(&ctx, &ngc);

    // so we use tmp variable to store this return value
    // and set it to top[0] later
    ctx.top[0] = result;

    // if get none, this means errors occurred when calling this native function
    if (ctx.top[0].is_none()) {
        die("error occurred in native function");
        return;
    }
}

void vm::o_slcbeg() {
    // +--------------+
    // | slice_vector | <-- top[0]
    // +--------------+
    // | resource_vec | <-- top[-1]
    // +--------------+
    (++ctx.top)[0] = ngc.alloc(gc_type::gc_vec);
    if (!ctx.top[-1].is_vec()) {
        die("must slice a vector but get " + type_name_string(ctx.top[-1]));
        return;
    }
}

void vm::o_slcend() {
    ctx.top[-1] = ctx.top[0];
    --ctx.top;
}

void vm::o_slc() {
    var val = (ctx.top--)[0];
    var res = ctx.top[-1].vec().get_value(val.to_num());
    if (res.is_none()) {
        die(report_out_of_range(val.to_num(), ctx.top[-1].vec().size()));
        return;
    }
    ctx.top[0].vec().elems.push_back(res);
}

void vm::o_slc2() {
    var val2 = (ctx.top--)[0];
    var val1 = (ctx.top--)[0];
    const auto& ref = ctx.top[-1].vec().elems;
    auto& aim = ctx.top[0].vec().elems;

    vm_type type1 = val1.type;
    vm_type type2 = val2.type;
    i32 num1 = val1.to_num();
    i32 num2 = val2.to_num();
    i32 size = ref.size();
    if (val1.is_nil() && val2.is_nil()) {
        num1 = 0;
        num2 = size-1;
    } else if (val1.is_nil() && type2!=vm_type::vm_nil) {
        num1 = num2<0? -size:0;
    } else if (type1!=vm_type::vm_nil && val2.is_nil()) {
        num2 = num1<0? -1:size-1;
    }

    if (num1<-size || num1>=size || num2<-size || num2>=size) {
        die("index " + std::to_string(num1) + ":" +
            std::to_string(num2) + " out of range, real size is " +
            std::to_string(size)
        );
        return;
    } else if (num1<=num2) {
        for (i32 i = num1; i<=num2; ++i) {
            aim.push_back(i>=0? ref[i]:ref[i+size]);
        }
    }
}

void vm::o_mcallg() {
    ctx.memr = global+imm[ctx.pc];
    (++ctx.top)[0] = ctx.memr[0];
    // push value in this memory space on stack
    // to avoid being garbage collected
}

void vm::o_mcalll() {
    ctx.memr = ctx.localr+imm[ctx.pc];
    (++ctx.top)[0] = ctx.memr[0];
    // push value in this memory space on stack
    // to avoid being garbage collected
}

void vm::o_mupval() {
    ctx.memr = &(
        ctx.funcr.func()
           .upval[(imm[ctx.pc]>>16) & 0xffff]
           .upval()[imm[ctx.pc] & 0xffff]
    );
    (++ctx.top)[0] = ctx.memr[0];
    // push value in this memory space on stack
    // to avoid being garbage collected
}

void vm::o_mcallv() {
    var val = ctx.top[0];     // index
    var vec = (--ctx.top)[0]; // mcall vector, reserved on stack to avoid gc
    if (vec.is_vec()) {
        ctx.memr = vec.vec().get_memory(val.to_num());
        if (!ctx.memr) {
            die(report_out_of_range(val.to_num(), vec.vec().size()));
            return;
        }
    } else if (vec.is_hash()) { // do mcallh but use the mcallv way
        if (!val.is_str()) {
            die("must use string as the key but get " + type_name_string(val));
            return;
        }
        auto& ref = vec.hash();
        const auto& str = val.str();
        ctx.memr = ref.get_memory(str);
        if (!ctx.memr) {
            ref.elems[str] = nil;
            ctx.memr = ref.get_memory(str);
        }
    } else if (vec.is_map()) {
        if (!val.is_str()) {
            die("must use string as the key but get " + type_name_string(val));
            return;
        }
        auto& ref = vec.map();
        const auto& str = val.str();
        ctx.memr = ref.get_memory(str);
        if (!ctx.memr) {
            die("cannot find symbol \"" + str + "\"");
        }
    } else {
        die("cannot get memory space in this type");
        return;
    }
}

void vm::o_mcallh() {
    // mcall hash, reserved on stack to avoid gc, so do not do ctx.top--
    var hash = ctx.top[0];
    if (!hash.is_hash() && !hash.is_map()) {
        die("must call a hash/namespace but get " + type_name_string(hash));
        return;
    }

    const auto& key = const_string[imm[ctx.pc]];

    // map is for nasal namespace type, for example `globals`
    if (hash.is_map()) {
        ctx.memr = hash.map().get_memory(key);
        if (!ctx.memr) {
            die("cannot find symbol \"" + key + "\"");
        }
        return;
    }

    // call hash member
    auto& ref = hash.hash();
    ctx.memr = ref.get_memory(key);
    // create a new key if not exists
    if (!ctx.memr) {
        ref.elems[key] = nil;
        ctx.memr = ref.get_memory(key);
    }
}

void vm::o_ret() {
    // return statement outside any other functions
    // directly exit the program
    if (ctx.funcr.is_nil()) {
        std::exit(0);
    }
/*  +-------------+
*   | return value| <- top[0]
*   +-------------+
*   | old pc      | <- top[-1]
*   +-------------+
*   | old localr  | <- top[-2]
*   +-------------+
*   | old upvalr  | <- top[-3]
*   +-------------+
*   | local scope |
*   +-------------+ <- local pointer stored in localr
*   | old funcr   | <- old function stored in funcr
*   +-------------+
*/
    var  ret   = ctx.top[0];
    var* local = ctx.localr;
    var  func  = ctx.funcr;
    var  up    = ctx.upvalr;

    ctx.pc     = ctx.top[-1].ret();
    ctx.localr = ctx.top[-2].addr();
    ctx.upvalr = ctx.top[-3];

    ctx.top = local-1;
    ctx.funcr = ctx.top[0];
    ctx.top[0] = ret; // rewrite func with returned value
    ctx.func_top--;

    // synchronize upvalue
    if (up.is_upval()) {
        up.upval().move_from_stack();
    }

    // cannot use gc.cort to judge,
    // because there maybe another function call inside but return here
    // coroutine function ends with setting pc to 0
    if (!ctx.pc) {
        ngc.context_reserve();
    }
}

void vm::run(const codegen& gen,
             const resource_manager& resm,
             const std::vector<std::string>& argv) {
    vm_init_entry(
        gen.strs(),
        gen.nums(),
        gen.natives(),
        gen.codes(),
        gen.globals(),
        resm.get_ordered_file_list(),
        argv
    );

#ifndef _MSC_VER

// interrupt check macro for computed goto mode.
#define CHECK_INTERRUPT { \
    if (interrupt_ptr && interrupt_ptr->load()) { \
        throw std::runtime_error("VM execution interrupted by timeout"); \
    } \
}

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
    code.reserve(gen.codes().size());
    imm.reserve(gen.codes().size());
    for (const auto& i : gen.codes()) {
        code.push_back(oprs[i.op]);
        imm.push_back(i.num);
    }
    CHECK_INTERRUPT;
    // goto the first operand
    goto *code[ctx.pc];
#else
    std::vector<nasal_vm_func> code;
    code.reserve(gen.codes().size());
    imm.reserve(gen.codes().size());
    for (const auto& i : gen.codes()) {
        code.push_back(operand_function[i.op]);
        imm.push_back(i.num);
    }
    while (code[ctx.pc]) {
        if (interrupt_ptr && interrupt_ptr->load()) {
            throw std::runtime_error("VM execution interrupted by timeout");
        }
        (this->*code[ctx.pc])();
        if (ctx.top >= ctx.canary) {
            die("stack overflow");
        }
        ++ctx.pc;
    }
#endif
// all nasal programs should end here
vmexit:
    if (verbose) {
        ngc.status.dump_info();
    }
    imm.clear();
    if (!is_repl_mode) {
        ngc.clear();
    }
    return;

#ifndef _MSC_VER
// IR which may cause stackoverflow
#define exec_check(op) {\
    op();\
    CHECK_INTERRUPT;\
    if (ctx.top < ctx.canary)\
        goto *code[++ctx.pc];\
    die("stack overflow");\
    goto *code[++ctx.pc];\
}
// IR which does not cause stackoverflow
#define exec_nodie(op) {\
    op();\
    CHECK_INTERRUPT;\
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
deft:   exec_nodie(o_default); // -1
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
