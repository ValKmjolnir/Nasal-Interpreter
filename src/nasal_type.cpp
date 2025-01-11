#include "nasal_type.h"
#include "util/util.h"

#include <cstring>
#include <sstream>

namespace nasal {

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
    }
    if (!elems.count("parents")) {
        return var::none();
    }

    auto ret = var::none();
    auto& val = elems.at("parents");
    if (!val.is_vec()) {
        return ret;
    }
    for(auto& i : val.vec().elems) {
        if (i.is_hash()) {
            ret = i.hash().get_value(key);
        }
        if (!ret.is_none()) {
            return ret;
        }
    }
    return ret;
}

var* nas_hash::get_memory(const std::string& key) {
    if (elems.count(key)) {
        return &elems.at(key);
    }
    if (!elems.count("parents")) {
        return nullptr;
    }

    var* addr = nullptr;
    var& val = elems.at("parents");
    if (!val.is_vec()) {
        return addr;
    }
    for(auto& i : val.vec().elems) {
        // recursively search key in `parents`
        if (i.is_hash()) {
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

    // mark print, to avoid infinite recursion
    hash.printed = true;

    static const char* sep[] = {", ", "}"};
    usize iter = 0, size = hash.elems.size();
    out << "{";
    for(auto& i : hash.elems) {
        out << i.first << ": " << i.second << sep[(++iter)==size];
    }

    // restore flag
    hash.printed = false;
    return out;
}

std::ostream& operator<<(std::ostream& out, nas_func& func) {
    out << "func(";
    
    std::vector<std::string> argument_list = {};
    argument_list.resize(func.keys.size());
    for(const auto& key : func.keys) {
        argument_list[key.second-1] = key.first;
    }

    for(const auto& key : argument_list) {
        out << key;
        if (key != argument_list.back()) {
            out << ", ";
        }
    }
    if (func.dynamic_parameter_index>=0) {
        out << (argument_list.size()? ", ":"");
        out << func.dynamic_parameter_name << "...";
    }

    out << ") {..}";
    return out;
}

void nas_func::clear() {
    dynamic_parameter_index = -1;
    local.clear();
    upval.clear();
    keys.clear();
}

void nas_ghost::set(const std::string& ghost_type_name,
                    destructor destructor_pointer,
                    marker gc_marker_pointer,
                    void* ghost_pointer) {
    type_name = ghost_type_name;
    destructor_function = destructor_pointer;
    gc_mark_function = gc_marker_pointer;
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
    gc_mark_function = nullptr;
}

std::ostream& operator<<(std::ostream& out, const nas_ghost& ghost) {
    out << "<" << ghost.get_ghost_name();
    out << "@0x" << std::hex << ghost.convert<u64>() << std::dec << ">";
    return out;
}

void nas_co::clear() {
    if (!ctx.stack) {
        return;
    }
    for(u32 i = 0; i<VM_STACK_DEPTH; ++i) {
        ctx.stack[i] = var::nil();
    }

    ctx.pc = 0;
    ctx.localr = nullptr;
    ctx.memr = nullptr;
    ctx.canary = ctx.stack+VM_STACK_DEPTH-1;
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

    // mark print, to avoid infinite recursion
    mp.printed = true;

    static const char* sep[] = {", ", "}"};
    usize iter = 0, size = mp.mapper.size();
    out << "{";
    for(auto& i : mp.mapper) {
        out << i.first << ": " << *i.second << sep[(++iter)==size];
    }

    // restore flag
    mp.printed = false;
    return out;
}

nas_val::nas_val(vm_type val_type) {
    mark = gc_status::collected;
    type = val_type;
    immutable = 0;
    switch(val_type) {
        case vm_type::vm_str:   ptr.str = new std::string; break;
        case vm_type::vm_vec:   ptr.vec = new nas_vec;     break;
        case vm_type::vm_hash:  ptr.hash = new nas_hash;   break;
        case vm_type::vm_func:  ptr.func = new nas_func;   break;
        case vm_type::vm_upval: ptr.upval = new nas_upval; break;
        case vm_type::vm_ghost: ptr.obj = new nas_ghost;   break;
        case vm_type::vm_co:    ptr.co = new nas_co;       break;
        case vm_type::vm_map:   ptr.map = new nas_map;     break;
        default: break;
    }
}

nas_val::~nas_val() {
    switch(type) {
        case vm_type::vm_str:   delete ptr.str;   break;
        case vm_type::vm_vec:   delete ptr.vec;   break;
        case vm_type::vm_hash:  delete ptr.hash;  break;
        case vm_type::vm_func:  delete ptr.func;  break;
        case vm_type::vm_upval: delete ptr.upval; break;
        case vm_type::vm_ghost: delete ptr.obj;   break;
        case vm_type::vm_co:    delete ptr.co;    break;
        case vm_type::vm_map:   delete ptr.map;   break;
        default: break;
    }
    type = vm_type::vm_nil;
}

void nas_val::clear() {
    switch(type) {
        case vm_type::vm_str:   ptr.str->clear();        break;
        case vm_type::vm_vec:   ptr.vec->elems.clear();  break;
        case vm_type::vm_hash:  ptr.hash->elems.clear(); break;
        case vm_type::vm_func:  ptr.func->clear();       break;
        case vm_type::vm_upval: ptr.upval->clear();      break;
        case vm_type::vm_ghost: ptr.obj->clear();        break;
        case vm_type::vm_co:    ptr.co->clear();         break;
        case vm_type::vm_map:   ptr.map->clear();        break;
        default: break;
    }
}

std::string var::to_str() {
    if (type==vm_type::vm_str) {
        return str();
    } else if (type==vm_type::vm_num) {
        auto tmp = std::to_string(num());
        tmp.erase(tmp.find_last_not_of('0') + 1, std::string::npos);
        tmp.erase(tmp.find_last_not_of('.') + 1, std::string::npos);
        return tmp;
    }

    std::stringstream ss;
    ss << *this;
    return ss.str();
}

std::ostream& operator<<(std::ostream& out, var& ref) {
    switch(ref.type) {
        case vm_type::vm_none:  out << "undefined"; break;
        case vm_type::vm_nil:   out << "nil";       break;
        case vm_type::vm_num:   out << ref.val.num; break;
        case vm_type::vm_str:   out << ref.str();   break;
        case vm_type::vm_vec:   out << ref.vec();   break;
        case vm_type::vm_hash:  out << ref.hash();  break;
        case vm_type::vm_func:  out << ref.func();  break;
        case vm_type::vm_ghost: out << ref.ghost(); break;
        case vm_type::vm_co:    out << ref.co();    break;
        case vm_type::vm_map:   out << ref.map();   break;
        default: break;
    }
    return out;
}

}