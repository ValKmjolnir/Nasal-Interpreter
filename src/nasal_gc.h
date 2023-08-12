#pragma once

// avoid MSVC warnings
#ifdef _MSC_VER
#pragma warning (disable:4244)
#pragma warning (disable:4267)
#pragma warning (disable:4102)
#endif

#ifndef _MSC_VER
#include <unistd.h>
#include <dirent.h>
#else
#include <io.h>
#include <direct.h>
#endif

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <iomanip>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <algorithm>
#include <thread>
#include <cstring>
#include <sstream>

#include "nasal.h"

enum vm_type:u8 {
    /* none-gc object */
    vm_none = 0,
    vm_cnt,
    vm_addr,
    vm_ret,
    vm_nil,
    vm_num,
    /* gc object */
    vm_str,
    vm_vec,
    vm_hash,
    vm_func,
    vm_upval,
    vm_obj,
    vm_co,
    vm_map // for globals and namespaces
};

const u32 gc_type_size = vm_map-vm_str+1;

struct nas_vec;   // vector
struct nas_hash;  // hashmap(dict)
struct nas_func;  // function(lambda)
struct nas_upval; // upvalue
struct nas_ghost; // objects
struct nas_co;    // coroutine
struct nas_map;   // mapper
struct nas_val;   // nas_val includes gc-managed types

struct var {
public:
    u8 type;
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
    f64 tonum();
    std::string tostr();
    bool objchk(const std::string&);

    // create new var object
    static var none();
    static var nil();
    static var ret(u32);
    static var cnt(i64);
    static var num(f64);
    static var gcobj(nas_val*);
    static var addr(var*);

    // get content
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
    i32 dpara; // dynamic parameter name index in hash.
    u32 entry; // pc will set to entry-1 to call this function
    u32 psize; // used to load default parameters to a new function
    u32 lsize; // used to expand memory space for local values on stack
    std::vector<var> local; // local scope with default value(var)
    std::vector<var> upval; // closure
    std::unordered_map<u32,u32> keys; // parameter table, u32 begins from 1

    nas_func(): dpara(-1), entry(0), psize(0), lsize(0) {}
    void clear();
};

struct nas_upval {
public:
    /* on stack, use these variables */
    bool onstk;
    u32 size;
    var* stk;

    /* not on stack, use this */
    std::vector<var> elems;

public:
    nas_upval(): onstk(true), size(0), stk(nullptr) {}

    var& operator[](usize n) {
        return onstk? stk[n]:elems[n];
    }

    void clear() {
        onstk = true;
        elems.clear();
        size = 0;
    }
};

struct nas_ghost {
private:
    using destructor=void (*)(void*);

public:
    std::string type_name;
    destructor dtor_ptr;
    void* ptr;

public:
    nas_ghost(): type_name(""), dtor_ptr(nullptr), ptr(nullptr) {}
    ~nas_ghost() {clear();}
    void set(const std::string&, destructor, void*);
    void clear();

public:
    const std::string& get_ghost_name() const {
        return type_name;
    }
};

struct context {
    u32  pc;
    var* localr;
    var* memr;
    var  funcr;
    var  upvalr;
    var* canary;
    var* stack;
    var* top;
};

struct nas_co {
    enum class status:u32 {
        suspended,
        running,
        dead
    };

    var stack[STACK_DEPTH];
    context ctx;
    status status;

    nas_co() {clear();}
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
    u8 unmut; // used to mark if a string is unmutable
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

struct gc {
    /* main context temporary storage */
    context mctx;

    /* runtime context */
    context* rctx = nullptr;
    nas_co* cort = nullptr; // running coroutine

    /*  temporary space used in native/module functions */
    var temp = nil;

    /* constants and memory pool */
    std::vector<var> strs;        // reserved address for const vm_str
    std::vector<var> env_argv;    // command line arguments
    std::vector<nas_val*> memory; // gc memory
    std::vector<nas_val*> unused[gc_type_size]; // gc free list

    /* heap increase size */
    u32 incr[gc_type_size] = {
        128, // vm_str
        128, // vm_vec
        64,  // vm_hash
        128, // vm_func
        256, // vm_upval
        16,  // vm_obj
        16,  // vm_co
        2,   // vm_map
    };

    /* values for analysis */
    u64 size[gc_type_size];
    u64 gcnt[gc_type_size];
    u64 acnt[gc_type_size];
    i64 worktime = 0;
    i64 max_time = 0;
    i64 max_mark_time = 0;
    i64 max_sweep_time = 0;

    gc(context* _ctx): rctx(_ctx) {}

private:
    /* gc functions */
    void do_mark_sweep();
    void mark();
    void concurrent_mark(std::vector<var>&, usize, usize);
    void mark_context_root(std::vector<var>&);
    void mark_var(std::vector<var>&, var&);
    void mark_vec(std::vector<var>&, nas_vec&);
    void mark_hash(std::vector<var>&, nas_hash&);
    void mark_func(std::vector<var>&, nas_func&);
    void mark_upval(std::vector<var>&, nas_upval&);
    void mark_co(std::vector<var>&, nas_co&);
    void mark_map(std::vector<var>&, nas_map&);
    void sweep();

public:
    void extend(u8);
    void init(const std::vector<std::string>&, const std::vector<std::string>&);
    void clear();
    void info() const;
    var alloc(const u8);
    void ctxchg(nas_co&);
    void ctxreserve();

public:
    var newstr(char c) {
        var s = alloc(vm_str);
        s.str() = c;
        return s;
    }

    var newstr(const char* buff) {
        var s=alloc(vm_str);
        s.str() = buff;
        return s;
    }

    var newstr(const std::string& buff) {
        var s=alloc(vm_str);
        s.str() = buff;
        return s;
    }
};

// use to print error log and return error value
var nas_err(const std::string&, const std::string&);

// module function type
typedef var (*module_func)(var*, usize, gc*);

// module function stores in tables with this type, end with {nullptr,nullptr}
struct module_func_info {
    const char* name;
    module_func fd;
};

// module function "get" type
typedef module_func_info* (*get_func_ptr)();
