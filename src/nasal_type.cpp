#include "nasal_type.h"

namespace nasal {

var nas_vec::get_value(const i32 n) {
    i32 size = elems.size();
    if (n<-size || n>=size) {
        return var::none();
    }
    return elems[n>=0? n:n+size];
}

var* nas_vec::get_memory(const i32 n) {
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

var nas_hash::get_value(const std::string& key) {
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
            ret = i.hash().get_value(key);
        }
        if (ret.type!=vm_none) {
            return ret;
        }
    }
    return ret;
}

var* nas_hash::get_memory(const std::string& key) {
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
            addr = i.hash().get_memory(key);
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
    dynamic_parameter_index = -1;
    local.clear();
    upval.clear();
    keys.clear();
}

void nas_ghost::set(
    const std::string& ghost_type_name,
    destructor destructor_pointer,
    void* ghost_pointer) {
    type_name = ghost_type_name;
    destructor_function = destructor_pointer;
    pointer = ghost_pointer;
}

void nas_ghost::clear() {
    // do nothing if pointer is null
    if (!pointer) {
        return;
    }

    // do clear pointer if destructor function pointer is null
    if (!destructor_function) {
        type_name = "";
        pointer = nullptr;
        return;
    }

    // do destruction
    destructor_function(pointer);
    type_name = "";
    pointer = nullptr;
    destructor_function = nullptr;
}

std::ostream& operator<<(std::ostream& out, const nas_ghost& ghost) {
    out << "<object " << ghost.get_ghost_name();
    out << " at 0x" << std::hex;
    out << reinterpret_cast<u64>(ghost.pointer) << std::dec << ">";
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

var nas_map::get_value(const std::string& key) {
    if (mapper.count(key)) {
        return *mapper.at(key);
    }
    return var::none();
}

var* nas_map::get_memory(const std::string& key) {
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
    unmutable = 0;
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

f64 var::to_num() {
    return type!=vm_str? val.num:str2num(str().c_str());
}

std::string var::to_str() {
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

bool var::object_check(const std::string& name) {
    return type==vm_obj && obj().type_name==name && obj().pointer;
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

var nas_err(const std::string& error_function_name, const std::string& info) {
    std::cerr << "[vm] " << error_function_name << ": " << info << "\n";
    return var::none();
}

}