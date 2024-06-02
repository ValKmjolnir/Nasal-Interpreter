#pragma once

#include "nasal.h"

#include <cstring>
#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_map>

namespace nasal {

enum class vm_type: u8 {
    /* none-gc object */
    vm_none = 0, // error type
    vm_cnt,      // counter for forindex/foreach loop
    vm_addr,     // var* address
    vm_ret,      // return addres(program counter) 
    vm_nil,      // nil
    vm_num,      // number

    /* gc object */
    vm_str,      // string
    vm_vec,      // vector
    vm_hash,     // hashmap(dict)
    vm_func,     // function(lambda)
    vm_upval,    // upvalue
    vm_ghost,    // ghost type
    vm_co,       // coroutine
    vm_map,      // for globals and namespaces

    /* mark type range */
    vm_type_size_max
};

// size of gc object type
const u32 gc_type_size =
    static_cast<u32>(vm_type::vm_type_size_max) -
    static_cast<u32>(vm_type::vm_str);

// basic types
struct nas_vec;   // vector
struct nas_hash;  // hashmap(dict)
struct nas_func;  // function(lambda)
struct nas_upval; // upvalue
struct nas_ghost; // objects
struct nas_co;    // coroutine
struct nas_map;   // mapper

// union type
struct nas_val;   // nas_val includes gc-managed types

struct var {
public:
    vm_type type = vm_type::vm_none;
    union {
        u64 ret;
        i64 cnt;
        f64 num;
        var* addr;
        nas_val* gcobj;
    } val;

private:
    var(vm_type t, u64 pc) {type = t; val.ret = pc;}
    var(vm_type t, i64 ct) {type = t; val.cnt = ct;}
    var(vm_type t, f64 n) {type = t; val.num = n;}
    var(vm_type t, var* p) {type = t; val.addr = p;}
    var(vm_type t, nas_val* p) {type = t; val.gcobj = p;}

public:
    var() = default;
    var(const var&) = default;
    bool operator==(const var& nr) const {
        return type==nr.type && val.gcobj==nr.val.gcobj;
    }
    bool operator!=(const var& nr) const {
        return type!=nr.type || val.gcobj!=nr.val.gcobj;
    }

    // number and string can be translated to each other
    f64 to_num();
    std::string to_str();
    bool object_check(const std::string&);

public:
    // create new var object
    static var none();
    static var nil();
    static var ret(u64);
    static var cnt(i64);
    static var num(f64);
    static var gcobj(nas_val*);
    static var addr(var*);

public:
    // get value
    var* addr();
    u64 ret() const;
    i64& cnt();
    f64 num() const;
    std::string& str();
    nas_vec& vec();
    nas_hash& hash();
    nas_func& func();
    nas_upval& upval();
    nas_ghost& ghost();
    nas_co& co();
    nas_map& map();

public:
    bool is_none() const { return type==vm_type::vm_none; }
    bool is_cnt() const { return type==vm_type::vm_cnt; }
    bool is_addr() const { return type==vm_type::vm_addr; }
    bool is_ret() const { return type==vm_type::vm_ret; }
    bool is_nil() const { return type==vm_type::vm_nil; }
    bool is_num() const { return type==vm_type::vm_num; }
    bool is_str() const { return type==vm_type::vm_str; }
    bool is_vec() const { return type==vm_type::vm_vec; }
    bool is_hash() const { return type==vm_type::vm_hash; }
    bool is_func() const { return type==vm_type::vm_func; }
    bool is_upval() const { return type==vm_type::vm_upval; }
    bool is_ghost() const { return type==vm_type::vm_ghost; }
    bool is_coroutine() const { return type==vm_type::vm_co; }
    bool is_map() const { return type==vm_type::vm_map; }
};

struct nas_vec {
    std::vector<var> elems;

    // mark if this is printed, avoid stack overflow
    bool printed = false;

    usize size() const {return elems.size();}
    var get_value(const i32);
    var* get_memory(const i32);
};

struct nas_hash {
    std::unordered_map<std::string, var> elems;

    // mark if this is printed, avoid stack overflow
    bool printed = false;

    usize size() const {return elems.size();}
    var get_value(const std::string&);
    var* get_memory(const std::string&);
};

struct nas_func {
    i64 dynamic_parameter_index; // dynamic parameter name index in hash.
    u64 entry; // pc will set to entry-1 to call this function
    u32 parameter_size; // used to load default parameters to a new function
    u64 local_size; // used to expand memory space for local values on stack
    std::vector<var> local; // local scope with default value(var)
    std::vector<var> upval; // closure

    // parameter table, u32 begins from 1
    std::unordered_map<std::string, u32> keys;

    // dynamic parameter name
    std::string dynamic_parameter_name;

    nas_func():
        dynamic_parameter_index(-1), entry(0),
        parameter_size(0), local_size(0),
        dynamic_parameter_name("") {}
    void clear();
};

struct nas_upval {
public:
    /* on stack, use these variables */
    bool on_stack;
    u64 size;
    var* stack_frame_offset;

    /* not on stack, use this */
    std::vector<var> elems;

public:
    nas_upval(): on_stack(true), size(0), stack_frame_offset(nullptr) {}

    var& operator[](usize n) {
        return on_stack? stack_frame_offset[n]:elems[n];
    }

    void clear() {
        on_stack = true;
        elems.clear();
        size = 0;
    }
};

struct nas_ghost {
private:
    using destructor = void (*)(void*);
    using marker = void (*)(void*, std::vector<var>*);

public:
    std::string type_name;
    destructor destructor_function;
    marker gc_mark_function;
    void* pointer;

public:
    nas_ghost():
        type_name(""), destructor_function(nullptr),
        gc_mark_function(nullptr), pointer(nullptr) {}
    ~nas_ghost() { clear(); }
    void set(const std::string&, destructor, marker, void*);
    void clear();

public:
    const auto& get_ghost_name() const { return type_name; }
};

struct context {
    u64  pc = 0;
    var* localr = nullptr;
    var* memr = nullptr;
    var  funcr = var::nil();
    var  upvalr = var::nil();
    var* canary = nullptr;
    var* stack = nullptr;
    var* top = nullptr;
};

struct nas_co {
    enum class status:u32 {
        suspended,
        running,
        dead
    };

    context ctx;
    status status;

    nas_co() {
        ctx.stack = new var[VM_STACK_DEPTH];
        clear();
    }
    ~nas_co() {
        delete[] ctx.stack;
    }
    void clear();
};

struct nas_map {
    bool printed = false;
    std::unordered_map<std::string, var*> mapper;

    void clear() {
        mapper.clear();
    }

    var get_value(const std::string&);
    var* get_memory(const std::string&);
};

struct nas_val {
    enum class gc_status: u8 {
        uncollected = 0,   
        collected,
        found
    };

    gc_status mark;
    vm_type type; // value type
    u8 immutable; // used to mark if a string is immutable
    union {
        std::string* str;
        nas_vec*   vec;
        nas_hash*  hash;
        nas_func*  func;
        nas_upval* upval;
        nas_ghost* obj;
        nas_co*    co;
        nas_map*   map;
    } ptr;

    nas_val(vm_type);
    ~nas_val();
    void clear();
};

std::ostream& operator<<(std::ostream&, nas_vec&);
std::ostream& operator<<(std::ostream&, nas_hash&);
std::ostream& operator<<(std::ostream&, nas_func&);
std::ostream& operator<<(std::ostream&, nas_map&);
std::ostream& operator<<(std::ostream&, const nas_ghost&);
std::ostream& operator<<(std::ostream&, const nas_co&);
std::ostream& operator<<(std::ostream&, var&);

const var zero = var::num(0);
const var one = var::num(1);
const var nil = var::nil();

// use to print error log and return error value
var nas_err(const std::string&, const std::string&);

}