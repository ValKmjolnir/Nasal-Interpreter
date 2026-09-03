#pragma once

#include "nasal.hpp"
#include "util/util.hpp"
#include "util/densemap.hpp"
#include "util/bit_cast.hpp"

#include <cstring>
#include <sstream>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

namespace nasal {

enum class vm_type: u8 {
    /* none-gc object */
    vm_none = 1, // error type, set to 1 to avoid 0x7ff8000000000000 quiet NaN
    vm_addr,     // var* address
    vm_ret,      // return address (program counter)
    vm_nil,      // nil
    vm_num,      // number

    /* gc object */
    vm_gcobj
};

enum class gc_type: u8 {
    gc_str,   // string
    gc_vec,   // vector
    gc_hash,  // hashmap (dict)
    gc_func,  // function (lambda)
    gc_upval, // upvalue
    gc_ghost, // ghost type
    gc_co,    // coroutine
    gc_map    // for globals and namespaces
};

// size of gc object type
const u32 GC_TYPE_SIZE = static_cast<u32>(gc_type::gc_map) + 1;

// basic types
struct nas_vec;   // vector
struct nas_hash;  // hashmap (dict)
struct nas_func;  // function (lambda)
struct nas_upval; // upvalue
struct nas_ghost; // objects
struct nas_co;    // coroutine
struct nas_map;   // mapper

// nas_val includes gc-managed types
struct nas_val {
    enum class gc_status: u8 {
        uncollected = 0,     // will be collected by sweep
        collected,           // already collected and put into unused list
        found,               // marked as referenced
        alloc_in_sweep_stage // mark it allocated in incremental sweep stage
                             // still scan in mark stage
                             // but do not sweep in this gc cycle
    };

    gc_status mark;
    gc_type type;   // value type
    bool immutable; // used to mark if a string is immutable
    union elem {
        std::string* str;
        nas_vec*     vec;
        nas_hash*    hash;
        nas_func*    func;
        nas_upval*   upval;
        nas_ghost*   obj;
        nas_co*      co;
        nas_map*     map;
    } ptr;

    nas_val(gc_type);
    ~nas_val();
    void clear();
};

#define QNAN_BASE ((u64)0x7ff8000000000000)
#define TYPE_MASK ((u64)0x0007000000000000)
#define PAYLOAD_MASK ((u64)0x0000ffffffffffff)

struct var {
private:
    u64 raw;

private:
    explicit constexpr var(u64 v): raw(v) {}
    static u64 make_tagged(vm_type t, u64 payload) {
        u64 type = static_cast<u64>(t);
        return QNAN_BASE | (type << 48) | (payload & PAYLOAD_MASK);
    }

public:
    var() { raw = make_tagged(vm_type::vm_none, 0); }
    var(const var&) = default;
    bool operator==(const var& nr) const { return raw == nr.raw; }
    bool operator!=(const var& nr) const { return raw != nr.raw; }

public:
    // create new var object
    static var none() { return var(make_tagged(vm_type::vm_none, 0)); }
    static var nil() { return var(make_tagged(vm_type::vm_nil, 0)); }
    static var ret(u64 pc) { return var(make_tagged(vm_type::vm_ret, pc)); }
    static var num(f64 n) { return var(bit_cast<u64>(n)); }
    static var gcobj(nas_val* p) {
        return var(make_tagged(vm_type::vm_gcobj, (u64)(std::uintptr_t)p));
    }
    static var addr(var* p) {
        return var(make_tagged(vm_type::vm_addr, (u64)(std::uintptr_t)p));
    }

public:
    // get value
    var* addr() const { return (var*)(raw & PAYLOAD_MASK); }
    u64 ret() const { return raw & PAYLOAD_MASK; }
    f64 num() const { return bit_cast<f64>(raw); }

public:
    nas_val* get_gcobj_ptr() const {
        return (nas_val*)(raw & PAYLOAD_MASK);
    }

    // get gc object reference
    std::string& str() { return *get_gcobj_ptr()->ptr.str; }
    nas_vec& vec() { return *get_gcobj_ptr()->ptr.vec; }
    nas_hash& hash() { return *get_gcobj_ptr()->ptr.hash; }
    nas_func& func() { return *get_gcobj_ptr()->ptr.func; }
    nas_upval& upval() { return *get_gcobj_ptr()->ptr.upval; }
    nas_ghost& ghost() { return *get_gcobj_ptr()->ptr.obj; }
    nas_co& co() { return *get_gcobj_ptr()->ptr.co; }
    nas_map& map() { return *get_gcobj_ptr()->ptr.map; }

public:
    // get const gc object
    const std::string& str() const { return *get_gcobj_ptr()->ptr.str; }
    const nas_vec& vec() const { return *get_gcobj_ptr()->ptr.vec; }
    const nas_hash& hash() const { return *get_gcobj_ptr()->ptr.hash; }
    const nas_func& func() const { return *get_gcobj_ptr()->ptr.func; }
    const nas_upval& upval() const { return *get_gcobj_ptr()->ptr.upval; }
    const nas_ghost& ghost() const { return *get_gcobj_ptr()->ptr.obj; }
    const nas_co& co() const { return *get_gcobj_ptr()->ptr.co; }
    const nas_map& map() const { return *get_gcobj_ptr()->ptr.map; }

public:
    vm_type type() const {
        if (is_num()) {
            return vm_type::vm_num;
        }
        return static_cast<vm_type>((raw & TYPE_MASK) >> 48);
    }
    bool is_none() const { return type() == vm_type::vm_none; }
    bool is_addr() const { return type() == vm_type::vm_addr; }
    bool is_ret() const { return type() == vm_type::vm_ret; }
    bool is_nil() const { return type() == vm_type::vm_nil; }
    bool is_num() const {
        return ((raw & QNAN_BASE) != QNAN_BASE) || // not NaN
               (raw & TYPE_MASK) == 0; // if is NaN, should be quiet NaN (e.g. std::nan(""))
    }
    bool is_gcobj() const {
        return type() == vm_type::vm_gcobj;
    }
    bool is_str() const {
        return type() == vm_type::vm_gcobj &&
               get_gcobj_ptr()->type == gc_type::gc_str;
    }
    bool is_vec() const {
        return type() == vm_type::vm_gcobj &&
               get_gcobj_ptr()->type == gc_type::gc_vec;
    }
    bool is_hash() const {
        return type() == vm_type::vm_gcobj &&
               get_gcobj_ptr()->type == gc_type::gc_hash;
    }
    bool is_func() const {
        return type() == vm_type::vm_gcobj &&
               get_gcobj_ptr()->type == gc_type::gc_func;
    }
    bool is_upval() const {
        return type() == vm_type::vm_gcobj &&
               get_gcobj_ptr()->type == gc_type::gc_upval;
    }
    bool is_ghost() const {
        return type() == vm_type::vm_gcobj &&
               get_gcobj_ptr()->type == gc_type::gc_ghost;
    }
    bool is_coroutine() const {
        return type() == vm_type::vm_gcobj &&
               get_gcobj_ptr()->type == gc_type::gc_co;
    }
    bool is_map() const {
        return type() == vm_type::vm_gcobj &&
               get_gcobj_ptr()->type == gc_type::gc_map;
    }

public:
    // convert to number
    f64 to_num() const {
        if (is_num()) {
            return num();
        }
        if (is_str()) {
            return util::str_to_num(str().c_str());
        }
        return 0.0;
    }
    // convert to string
    std::string to_str();
    bool ghost_object_check(const std::string&) const;
    friend std::ostream& operator<<(std::ostream&, var&);
};

struct nas_vec {
    std::vector<var> elems;

    // mark if this is printed, avoid stack overflow
    bool printed = false;

    auto size() const { return elems.size(); }
    var get_value(const i32 index) {
        auto size = static_cast<i32>(elems.size());
        if (index < -size || index >= size) {
            return var::none();
        }
        return elems[index >= 0 ? index : index + size];
    }
    var* get_memory(const i32 index) {
        auto size = static_cast<i32>(elems.size());
        if (index < -size || index >= size) {
            return nullptr;
        }
        return &elems[index >= 0 ? index : index + size];
    }
    friend std::ostream& operator<<(std::ostream&, nas_vec&);
};

struct nas_hash {
    util::densemap<std::string, var> elems;

    // mark if this is printed, avoid stack overflow
    bool printed = false;

    auto size() const { return elems.size(); }
    var get_value(const std::string&);
    var* get_memory(const std::string&);
    friend std::ostream& operator<<(std::ostream&, nas_hash&);
};

struct nas_func {
    i64 dynamic_parameter_index; // dynamic param position
    u64 entry; // pc will set to entry-1 to call this function
    u32 parameter_size; // used to load default parameters to a new function
    u64 local_size; // used to expand memory space for local values on stack
    std::vector<var> local; // local scope with default value(var)
    std::vector<var> upval; // closure

    // parameter table, u32 begins from 1
    util::densemap<std::string, u32> param_index_map;

    // dynamic parameter name
    std::string dynamic_parameter_name;

    nas_func():
        dynamic_parameter_index(-1), entry(0),
        parameter_size(0), local_size(0),
        dynamic_parameter_name("") {}
    void clear();
    friend std::ostream& operator<<(std::ostream&, nas_func&);
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
        return on_stack ? stack_frame_offset[n] : elems[n];
    }

    void clear() {
        on_stack = true;
        elems.clear();
        size = 0;
        stack_frame_offset = nullptr;
    }

    void move_from_stack() {
        on_stack = false;
        elems.resize(size);
        for (u64 i = 0; i < size; ++i) {
            elems[i] = stack_frame_offset[i];
        }
    }
};

struct nas_ghost {
private:
    using destructor = void (*)(void*);
    using marker = void (*)(void*, std::queue<var>*);

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
    friend std::ostream& operator<<(std::ostream&, const nas_ghost&);

public:
    const auto& get_ghost_name() const { return type_name; }

public:
    template<typename T>
    T* get() { return static_cast<T*>(pointer); }
    template<typename T>
    T convert() const { return reinterpret_cast<T>(pointer); }
};

struct callsite {
    var caller;
    u64 file_index = 0;
    u64 line = 0;
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

    callsite* func_stack = nullptr;
    callsite* func_top = nullptr;

    const std::string* files = nullptr;

    void ctor() {
        stack = new var[VM_STACK_DEPTH];
        func_stack = new callsite[VM_STACK_DEPTH];
    }
    void dtor() {
        delete[] stack;
        delete[] func_stack;
    }
    void clear() {
        /* set canary and program counter */
        pc = 0;
        localr = nullptr;
        memr = nullptr;
        funcr = var::nil();
        upvalr = var::nil();

        /* set canary = stack[VM_STACK_DEPTH-1] */
        canary = stack + VM_STACK_DEPTH - 1;

        /* nothing is on stack */
        top = stack;
        func_top = func_stack - 1;

        /* clear main stack */
        for (u32 i = 0; i < VM_STACK_DEPTH; ++i) {
            stack[i] = var::nil();
        }
    }
};

struct nas_co {
    enum class status:u32 {
        suspended,
        running,
        dead
    };

    context ctx;
    status status;

    nas_co() { ctx.ctor(); }
    ~nas_co() { ctx.dtor(); }
    void clear() { ctx.clear(); status = status::suspended; }
    friend std::ostream& operator<<(std::ostream&, const nas_co&);
};

struct nas_map {
    bool printed = false;
    util::densemap<std::string, var*> mapper;

public:
    void clear() {
        mapper.clear();
    }
    auto size() const { return mapper.size(); }

    var get_value(const std::string&);
    var* get_memory(const std::string&);
    friend std::ostream& operator<<(std::ostream&, nas_map&);
};

const var zero = var::num(0);
const var one = var::num(1);
const var nil = var::nil();

// use to print error log and return error value
var nas_err(const std::string& func, const std::string& info);

}