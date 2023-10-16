#pragma once

#include "nasal.h"

#include <vector>
#include <unordered_map>

namespace nasal {

enum vm_type:u8 {
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
    vm_obj,      // ghost type
    vm_co,       // coroutine
    vm_map       // for globals and namespaces
};

// size of gc object type
const u32 gc_type_size = vm_map-vm_str+1;

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
    u8 type = vm_none;
    union {
        u32 ret;
        i64 cnt;
        f64 num;
        var* addr;
        nas_val* gcobj;
    } val;

private:
    var(u8 t, u32 pc) {type = t; val.ret = pc;}
    var(u8 t, i64 ct) {type = t; val.cnt = ct;}
    var(u8 t, f64 n) {type = t; val.num = n;}
    var(u8 t, var* p) {type = t; val.addr = p;}
    var(u8 t, nas_val* p) {type = t; val.gcobj = p;}

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
    bool objchk(const std::string&);

    // create new var object
    static var none();
    static var nil();
    static var ret(u32);
    static var cnt(i64);
    static var num(f64);
    static var gcobj(nas_val*);
    static var addr(var*);

    // get value
    var* addr();
    u32 ret();
    i64& cnt();
    f64 num();
    std::string& str();
    nas_vec& vec();
    nas_hash& hash();
    nas_func& func();
    nas_upval& upval();
    nas_ghost& obj();
    nas_co& co();
    nas_map& map();
};

struct nas_vec {
    std::vector<var> elems;

    // mark if this is printed, avoid stackoverflow
    bool printed;

    nas_vec():printed(false) {}
    usize size() const {return elems.size();}
    var get_val(const i32);
    var* get_mem(const i32);
};

struct nas_hash {
    std::unordered_map<std::string, var> elems;

    // mark if this is printed, avoid stackoverflow
    bool printed;

    nas_hash(): printed(false) {}
    usize size() const {return elems.size();}
    var get_val(const std::string&);
    var* get_mem(const std::string&);
};

struct nas_func {
    i32 dynamic_parameter_index; // dynamic parameter name index in hash.
    u32 entry; // pc will set to entry-1 to call this function
    u32 parameter_size; // used to load default parameters to a new function
    u32 local_size; // used to expand memory space for local values on stack
    std::vector<var> local; // local scope with default value(var)
    std::vector<var> upval; // closure

    // parameter table, u32 begins from 1
    std::unordered_map<std::string, u32> keys;

    nas_func():
        dynamic_parameter_index(-1), entry(0),
        parameter_size(0), local_size(0) {}
    void clear();
};

struct nas_upval {
public:
    /* on stack, use these variables */
    bool on_stack;
    u32 size;
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

public:
    std::string type_name;
    destructor destructor_function;
    void* pointer;

public:
    nas_ghost():
        type_name(""), destructor_function(nullptr), pointer(nullptr) {}
    ~nas_ghost() {clear();}
    void set(const std::string&, destructor, void*);
    void clear();

public:
    const std::string& get_ghost_name() const {
        return type_name;
    }
};

struct context {
    u32  pc = 0;
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
        ctx.stack = new var[STACK_DEPTH];
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

    nas_map() {}
    void clear() {
        mapper.clear();
    }

    var get_val(const std::string&);
    var* get_mem(const std::string&);
};

struct nas_val {
    enum class gc_status:u8 {
        uncollected = 0,   
        collected,
        found
    };

    gc_status mark;
    u8 type; // value type
    u8 unmutable; // used to mark if a string is unmutable
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

    nas_val(u8);
    ~nas_val();
    void clear();
};

std::ostream& operator<<(std::ostream&, nas_vec&);
std::ostream& operator<<(std::ostream&, nas_hash&);
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