#include "nasal_gc.h"

namespace nasal {

var nas_vec::get_val(const i32 n) {
    i32 size = elems.size();
    if (n<-size || n>=size) {
        return var::none();
    }
    return elems[n>=0? n:n+size];
}

var* nas_vec::get_mem(const i32 n) {
    i32 size = elems.size();
    if (n<-size || n>=size) {
        return nullptr;
    }
    return &elems[n>=0? n:n+size];
}

std::ostream& operator<<(std::ostream& out, nas_vec& vec) {
    if (!vec.elems.size() || vec.printed) {
        out << (vec.elems.size()? "[..]":"[]");
        return out;
    }
    vec.printed = true;
    usize iter = 0, size = vec.elems.size();
    out << "[";
    for(auto& i:vec.elems) {
        out << i << ",]"[(++iter)==size];
    }
    vec.printed = false;
    return out;
}

var nas_hash::get_val(const std::string& key) {
    if (elems.count(key)) {
        return elems.at(key);
    } else if (!elems.count("parents")) {
        return var::none();
    }
    var ret = var::none();
    var val = elems.at("parents");
    if (val.type!=vm_vec) {
        return ret;
    }
    for(auto& i : val.vec().elems) {
        if (i.type==vm_hash) {
            ret = i.hash().get_val(key);
        }
        if (ret.type!=vm_none) {
            return ret;
        }
    }
    return ret;
}

var* nas_hash::get_mem(const std::string& key) {
    if (elems.count(key)) {
        return &elems.at(key);
    } else if (!elems.count("parents")) {
        return nullptr;
    }
    var* addr = nullptr;
    var val = elems.at("parents");
    if (val.type!=vm_vec) {
        return addr;
    }
    for(auto& i : val.vec().elems) {
        if (i.type==vm_hash) {
            addr = i.hash().get_mem(key);
        }
        if (addr) {
            return addr;
        }
    }
    return addr;
}

std::ostream& operator<<(std::ostream& out, nas_hash& hash) {
    if (!hash.elems.size() || hash.printed) {
        out << (hash.elems.size()? "{..}":"{}");
        return out;
    }
    hash.printed = true;
    usize iter = 0, size = hash.elems.size();
    out << "{";
    for(auto& i : hash.elems) {
        out << i.first << ":" << i.second << ",}"[(++iter)==size];
    }
    hash.printed = false;
    return out;
}

void nas_func::clear() {
    dpara = -1;
    local.clear();
    upval.clear();
    keys.clear();
}

void nas_ghost::set(
    const std::string& ghost_type_name,
    destructor destructor_pointer,
    void* ghost_pointer) {
    type_name = ghost_type_name;
    dtor_ptr = destructor_pointer;
    ptr = ghost_pointer;
}

void nas_ghost::clear() {
    // do nothing if pointer is null
    if (!ptr) {
        return;
    }

    // do clear pointer if destructor function pointer is null
    if (!dtor_ptr) {
        type_name = "";
        ptr = nullptr;
        return;
    }

    // do destruction
    dtor_ptr(ptr);
    type_name = "";
    ptr = nullptr;
    dtor_ptr = nullptr;
}

std::ostream& operator<<(std::ostream& out, const nas_ghost& ghost) {
    out << "<object " << ghost.get_ghost_name();
    out << " at 0x" << std::hex;
    out << reinterpret_cast<u64>(ghost.ptr) << std::dec << ">";
    return out;
}

void nas_co::clear() {
    if (!ctx.stack) {
        return;
    }
    for(u32 i = 0; i<STACK_DEPTH; ++i) {
        ctx.stack[i] = var::nil();
    }

    ctx.pc = 0;
    ctx.localr = nullptr;
    ctx.memr = nullptr;
    ctx.canary = ctx.stack+STACK_DEPTH-1;
    ctx.top = ctx.stack;
    ctx.funcr = var::nil();
    ctx.upvalr = var::nil();

    status = status::suspended;
}

std::ostream& operator<<(std::ostream& out, const nas_co& co) {
    out << "<coroutine at 0x" << std::hex;
    out << reinterpret_cast<u64>(&co) << std::dec << ">";
    return out;
}

var nas_map::get_val(const std::string& key) {
    if (mapper.count(key)) {
        return *mapper.at(key);
    }
    return var::none();
}

var* nas_map::get_mem(const std::string& key) {
    if (mapper.count(key)) {
        return mapper.at(key);
    }
    return nullptr;
}

std::ostream& operator<<(std::ostream& out, nas_map& mp) {
    if (!mp.mapper.size() || mp.printed) {
        out << (mp.mapper.size()? "{..}":"{}");
        return out;
    }
    mp.printed = true;
    usize iter = 0, size = mp.mapper.size();
    out << "{";
    for(auto& i : mp.mapper) {
        out << i.first << ":" << *i.second << ",}"[(++iter)==size];
    }
    mp.printed = false;
    return out;
}

nas_val::nas_val(u8 val_type) {
    mark = gc_status::collected;
    type = val_type;
    unmut = 0;
    switch(val_type) {
        case vm_str: ptr.str = new std::string; break;
        case vm_vec: ptr.vec = new nas_vec; break;
        case vm_hash: ptr.hash = new nas_hash; break;
        case vm_func: ptr.func = new nas_func; break;
        case vm_upval: ptr.upval = new nas_upval; break;
        case vm_obj: ptr.obj = new nas_ghost; break;
        case vm_co: ptr.co = new nas_co; break;
        case vm_map: ptr.map = new nas_map; break;
    }
}

nas_val::~nas_val() {
    switch(type) {
        case vm_str:  delete ptr.str;  break;
        case vm_vec:  delete ptr.vec;  break;
        case vm_hash: delete ptr.hash; break;
        case vm_func: delete ptr.func; break;
        case vm_upval:delete ptr.upval;break;
        case vm_obj:  delete ptr.obj;  break;
        case vm_co:   delete ptr.co;   break;
        case vm_map:  delete ptr.map;  break;
    }
    type=vm_nil;
}

void nas_val::clear() {
    switch(type) {
        case vm_str:  ptr.str->clear();       break;
        case vm_vec:  ptr.vec->elems.clear(); break;
        case vm_hash: ptr.hash->elems.clear();break;
        case vm_func: ptr.func->clear();      break;
        case vm_upval:ptr.upval->clear();     break;
        case vm_obj:  ptr.obj->clear();       break;
        case vm_co:   ptr.co->clear();        break;
        case vm_map:  ptr.map->clear();       break;
    }
}

f64 var::tonum() {
    return type!=vm_str? val.num:str2num(str().c_str());
}

std::string var::tostr() {
    if (type==vm_str) {
        return str();
    } else if (type==vm_num) {
        std::string tmp=std::to_string(num());
        tmp.erase(tmp.find_last_not_of('0')+1, std::string::npos);
        tmp.erase(tmp.find_last_not_of('.')+1, std::string::npos);
        return tmp;
    }
    return "";
}

std::ostream& operator<<(std::ostream& out, var& ref) {
    switch(ref.type) {
        case vm_none: out << "undefined";   break;
        case vm_nil:  out << "nil";         break;
        case vm_num:  out << ref.val.num;   break;
        case vm_str:  out << ref.str();     break;
        case vm_vec:  out << ref.vec();     break;
        case vm_hash: out << ref.hash();    break;
        case vm_func: out << "func(..) {..}"; break;
        case vm_obj:  out << ref.obj();     break;
        case vm_co:   out << ref.co();      break;
        case vm_map:  out << ref.map();     break;
    }
    return out;
}

bool var::objchk(const std::string& name) {
    return type==vm_obj && obj().type_name==name && obj().ptr;
}

var var::none() {
    return {vm_none, static_cast<u32>(0)};
}

var var::nil() {
    return {vm_nil, static_cast<u32>(0)};
}

var var::ret(u32 pc) {
    return {vm_ret, pc};
}

var var::cnt(i64 n) {
    return {vm_cnt, n};
}

var var::num(f64 n) {
    return {vm_num, n};
}

var var::gcobj(nas_val* p) {
    return {p->type, p};
}

var var::addr(var* p) {
    return {vm_addr, p};
}

var* var::addr() {
    return val.addr;
}

u32 var::ret() {
    return val.ret;
}

i64& var::cnt() {
    return val.cnt;
}

f64 var::num() {
    return val.num;
}

std::string& var::str() {
    return *val.gcobj->ptr.str;
}

nas_vec& var::vec() {
    return *val.gcobj->ptr.vec;
}

nas_hash& var::hash() {
    return *val.gcobj->ptr.hash;
}

nas_func& var::func() {
    return *val.gcobj->ptr.func;
}

nas_upval& var::upval() {
    return *val.gcobj->ptr.upval;
}

nas_ghost& var::obj() {
    return *val.gcobj->ptr.obj;
}

nas_co& var::co() {
    return *val.gcobj->ptr.co;
}

nas_map& var::map() {
    return *val.gcobj->ptr.map;
}

void gc::do_mark_sweep() {
    using clk = std::chrono::high_resolution_clock;
    auto begin = clk::now();
    mark();
    auto mark_end = clk::now();
    sweep();
    auto sweep_end = clk::now();
    
    auto total_time = (sweep_end-begin).count();
    auto mark_time = (mark_end-begin).count();
    auto sweep_time = (sweep_end-mark_end).count();
    worktime += total_time;
    max_time = max_time<total_time? total_time:max_time;
    max_mark_time = max_mark_time<mark_time? mark_time:max_mark_time;
    max_sweep_time = max_sweep_time<sweep_time? sweep_time:max_sweep_time;
}

void gc::mark() {
    std::vector<var> bfs;
    mark_context_root(bfs);
    if (memory.size()>8192 && bfs.size()>4) {
        usize size = bfs.size();
        std::thread t0(&gc::concurrent_mark, this, std::ref(bfs), 0, size/4);
        std::thread t1(&gc::concurrent_mark, this, std::ref(bfs), size/4, size/2);
        std::thread t2(&gc::concurrent_mark, this, std::ref(bfs), size/2, size/4*3);
        std::thread t3(&gc::concurrent_mark, this, std::ref(bfs), size/4*3, size);
        t0.join();
        t1.join();
        t2.join();
        t3.join();
        return;
    }
    
    while(!bfs.empty()) {
        var value = bfs.back();
        bfs.pop_back();
        if (value.type<=vm_num ||
            value.val.gcobj->mark!=nas_val::gc_status::uncollected) {
            continue;
        }
        mark_var(bfs, value);
    }
}

void gc::concurrent_mark(std::vector<var>& vec, usize begin, usize end) {
    std::vector<var> bfs;
    for(auto i = begin; i<end; ++i) {
        var value = vec[i];
        if (value.type<=vm_num ||
            value.val.gcobj->mark!=nas_val::gc_status::uncollected) {
            continue;
        }
        mark_var(bfs, value);
    }
    while(!bfs.empty()) {
        var value = bfs.back();
        bfs.pop_back();
        if (value.type<=vm_num ||
            value.val.gcobj->mark!=nas_val::gc_status::uncollected) {
            continue;
        }
        mark_var(bfs, value);
    }
}

void gc::mark_context_root(std::vector<var>& bfs_queue) {
    // scan global
    for(usize i = 0; i<main_context_global_size; ++i) {
        auto& val = main_context_global[i];
        if (val.type>vm_num) {
            bfs_queue.push_back(val);
        }
    }
    // scan now running context, this context maybe related to coroutine or main
    for(var* i = rctx->stack; i<=rctx->top; ++i) {
        if (i->type>vm_num) {
            bfs_queue.push_back(*i);
        }
    }
    bfs_queue.push_back(rctx->funcr);
    bfs_queue.push_back(rctx->upvalr);
    bfs_queue.push_back(temp);

    if (!cort) {
        return;
    }

    // coroutine is running, so scan main process stack from mctx
    for(var* i = mctx.stack; i<=mctx.top; ++i) {
        if (i->type>vm_num) {
            bfs_queue.push_back(*i);
        }
    }
    bfs_queue.push_back(mctx.funcr);
    bfs_queue.push_back(mctx.upvalr);
}

void gc::mark_var(std::vector<var>& bfs_queue, var& value) {
    value.val.gcobj->mark = nas_val::gc_status::found;
    switch(value.type) {
        case vm_vec: mark_vec(bfs_queue, value.vec()); break;
        case vm_hash: mark_hash(bfs_queue, value.hash()); break;
        case vm_func: mark_func(bfs_queue, value.func()); break;
        case vm_upval: mark_upval(bfs_queue, value.upval()); break;
        case vm_co: mark_co(bfs_queue, value.co()); break;
        case vm_map: mark_map(bfs_queue, value.map()); break;
        default: break;
    }
}

void gc::mark_vec(std::vector<var>& bfs_queue, nas_vec& vec) {
    for(auto& i : vec.elems) {
        if (i.type>vm_num) {
            bfs_queue.push_back(i);
        }
    }
}

void gc::mark_hash(std::vector<var>& bfs_queue, nas_hash& hash) {
    for(auto& i : hash.elems) {
        if (i.second.type>vm_num) {
            bfs_queue.push_back(i.second);
        }
    }
}

void gc::mark_func(std::vector<var>& bfs_queue, nas_func& function) {
    for(auto& i : function.local) {
        if (i.type>vm_num) {
            bfs_queue.push_back(i);
        }
    }
    for(auto& i : function.upval) {
        bfs_queue.push_back(i);
    }
}

void gc::mark_upval(std::vector<var>& bfs_queue, nas_upval& upval) {
    for(auto& i : upval.elems) {
        if (i.type>vm_num) {
            bfs_queue.push_back(i);
        }
    }
}

void gc::mark_co(std::vector<var>& bfs_queue, nas_co& co) {
    bfs_queue.push_back(co.ctx.funcr);
    bfs_queue.push_back(co.ctx.upvalr);
    for(var* i = co.ctx.stack; i<=co.ctx.top; ++i) {
        if (i->type>vm_num) {
            bfs_queue.push_back(*i);
        }
    }
}

void gc::mark_map(std::vector<var>& bfs_queue, nas_map& mp) {
    for(const auto& i : mp.mapper) {
        if (i.second->type>vm_num) {
            bfs_queue.push_back(*i.second);
        }
    }
}

void gc::sweep() {
    for(auto i : memory) {
        if (i->mark==nas_val::gc_status::uncollected) {
            i->clear();
            unused[i->type-vm_str].push_back(i);
            i->mark = nas_val::gc_status::collected;
        } else if (i->mark==nas_val::gc_status::found) {
            i->mark = nas_val::gc_status::uncollected;
        }
    }
}

void gc::extend(u8 type) {
    const u8 index = type-vm_str;
    size[index] += incr[index];

    for(u32 i = 0; i<incr[index]; ++i) {
        // no need to check, will be killed if memory is not enough
        nas_val* tmp = new nas_val(type);

        // add to heap
        memory.push_back(tmp);
        unused[index].push_back(tmp);
    }

    incr[index] = incr[index]+incr[index]/2;
}

void gc::init(
    const std::vector<std::string>& s, const std::vector<std::string>& argv) {
    // initialize function register
    rctx->funcr = nil;
    worktime = 0;

    // initialize counters
    for(u8 i = 0; i<gc_type_size; ++i) {
        size[i] = gcnt[i] = acnt[i] = 0;
    }

    // coroutine pointer set to nullptr
    cort = nullptr;

    // init constant strings
    strs.resize(s.size());
    for(u32 i = 0; i<strs.size(); ++i) {
        strs[i]=var::gcobj(new nas_val(vm_str));
        strs[i].val.gcobj->unmut = 1;
        strs[i].str() = s[i];
    }

    // record arguments
    env_argv.resize(argv.size());
    for(usize i = 0; i<argv.size(); ++i) {
        env_argv[i] = var::gcobj(new nas_val(vm_str));
        env_argv[i].val.gcobj->unmut = 1;
        env_argv[i].str() = argv[i];
    }
}

void gc::clear() {
    for(auto i : memory) {
        delete i;
    }
    memory.clear();
    for(u8 i = 0; i<gc_type_size; ++i) {
        unused[i].clear();
    }
    for(auto& i : strs) {
        delete i.val.gcobj;
    }
    strs.clear();
    env_argv.clear();
}

void gc::info() const {

    using std::left;
    using std::setw;
    using std::setfill;

    const char* used_table_name[] = {
        "object type", "gc count", "alloc count", "memory size",
        "detail", "time spend", "gc time", "avg time", "max gc",
        "max mark", "max sweep", nullptr
    };
    const char* name[] = {
        "string",
        "vector",
        "hashmap",
        "function",
        "upvalue",
        "object",
        "coroutine",
        "namespace",
        nullptr
    };

    usize indent = 0, len = 0;
    for(usize i = 0; used_table_name[i]; ++i) {
        len = std::string(used_table_name[i]).length();
        indent = indent<len? len:indent;
    }
    for(usize i = 0; name[i]; ++i) {
        len = std::string(name[i]).length();
        indent = indent<len? len:indent;
    }
    for(u32 i = 0; i<gc_type_size; ++i) {
        len = std::to_string(gcnt[i]).length();
        indent = indent<len? len:indent;
        len = std::to_string(acnt[i]).length();
        indent = indent<len? len:indent;
        len = std::to_string(size[i]).length();
        indent = indent<len? len:indent;
    }
    auto indent_string = std::string("--");
    for(usize i = 0; i<indent; ++i) {
        indent_string += "-";
    }
    auto last_line = indent_string + "+" +
        indent_string + "-" + indent_string + "-" + indent_string;
    indent_string = indent_string + "+" +
        indent_string + "+" + indent_string + "+" + indent_string;

    std::clog << "\n" << indent_string << "\n";
    std::clog << " " << left << setw(indent) << setfill(' ') << "object type";
    std::clog << " | " << left << setw(indent) << setfill(' ') << "gc count";
    std::clog << " | " << left << setw(indent) << setfill(' ') << "alloc count";
    std::clog << " | " << left << setw(indent) << setfill(' ') << "memory size";
    std::clog << "\n" << indent_string << "\n";

    double total = 0;
    for(u8 i = 0; i<gc_type_size; ++i) {
        if (!gcnt[i] && !acnt[i] && !size[i]) {
            continue;
        }
        total += gcnt[i];
        std::clog << " " << left << setw(indent) << setfill(' ') << name[i];
        std::clog << " | " << left << setw(indent) << setfill(' ') << gcnt[i];
        std::clog << " | " << left << setw(indent) << setfill(' ') << acnt[i];
        std::clog << " | " << left << setw(indent) << setfill(' ') << size[i];
        std::clog << "\n";
    }
    std::clog << indent_string << "\n";

    auto den = std::chrono::high_resolution_clock::duration::period::den;
    std::clog << " " << left << setw(indent) << setfill(' ') << "detail";
    std::clog << " | " << left << setw(indent) << setfill(' ') << "time spend";
    std::clog << " | " << left << setw(indent) << setfill('x') << "x";
    std::clog << " | " << left << setw(indent) << setfill('x') << "x";
    std::clog << "\n" << indent_string << "\n";

    std::clog << " " << left << setw(indent) << setfill(' ') << "gc time";
    std::clog << " | " << worktime*1.0/den*1000 << " ms\n";
    std::clog << " " << left << setw(indent) << setfill(' ') << "avg time";
    std::clog << " | " << worktime*1.0/den*1000/total << " ms\n";
    std::clog << " " << left << setw(indent) << setfill(' ') << "max gc";
    std::clog << " | " << max_time*1.0/den*1000 << " ms\n";
    std::clog << " " << left << setw(indent) << setfill(' ') << "max mark";
    std::clog << " | " << max_mark_time*1.0/den*1000 << " ms\n";
    std::clog << " " << left << setw(indent) << setfill(' ') << "max sweep";
    std::clog << " | " << max_sweep_time*1.0/den*1000 << " ms\n";
    std::clog << last_line << "\n";
}

var gc::alloc(u8 type) {
    const u8 index = type-vm_str;
    ++acnt[index];
    if (unused[index].empty()) {
        ++gcnt[index];
        do_mark_sweep();
    }
    if (unused[index].empty()) {
        extend(type);
    }
    var ret = var::gcobj(unused[index].back());
    ret.val.gcobj->mark = nas_val::gc_status::uncollected;
    unused[index].pop_back();
    return ret;
}

void gc::ctxchg(nas_co& co) {
    // store running state to main context
    mctx = *rctx;

    // restore coroutine context state
    *rctx = co.ctx;

    // set coroutine pointer
    cort = &co;

    // set coroutine state to running
    cort->status = nas_co::status::running;
}

void gc::ctxreserve() {
    // pc=0 means this coroutine is finished
    cort->status = rctx->pc?
        nas_co::status::suspended:
        nas_co::status::dead;

    // store running state to coroutine
    cort->ctx = *rctx;

    // restore main context state
    *rctx = mctx;

    // set coroutine pointer to nullptr
    cort = nullptr;
}

var nas_err(const std::string& error_function_name, const std::string& info) {
    std::cerr << "[vm] " << error_function_name << ": " << info << "\n";
    return var::none();
}

}
