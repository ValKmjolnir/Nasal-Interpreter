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

#include "nasal_new_header.h"

enum vm_type:u8 {
    /* none-gc object */
    vm_none=0,
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
    vm_co
};

const u32 gc_type_size=vm_co-vm_str+1;

enum class coroutine_status:u32 {
    suspended,
    running,
    dead
};

enum class gc_status:u8 {
    uncollected=0,   
    collected,
    found
};

struct nas_vec;   // vector
struct nas_hash;  // hashmap(dict)
struct nas_func;  // function(lambda)
struct nas_upval; // upvalue
struct nas_ghost; // objects
struct nas_co;    // coroutine
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
    var(u8 t, u32 pc) {type=t;val.ret=pc;}
    var(u8 t, i64 ct) {type=t;val.cnt=ct;}
    var(u8 t, f64 n) {type=t;val.num=n;}
    var(u8 t, var* p) {type=t;val.addr=p;}
    var(u8 t, nas_val* p) {type=t;val.gcobj=p;}

public:
    var() = default;
    var(const var&) = default;
    bool operator==(const var& nr) const {
        return type==nr.type && val.gcobj==nr.val.gcobj;
    }
    bool operator!=(const var& nr) const {
        return type!=nr.type || val.gcobj!=nr.val.gcobj;
    }
    friend std::ostream& operator<<(std::ostream&, var&);

    // number and string can be translated to each other
    f64 tonum();
    std::string tostr();
    bool objchk(usize);

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

    nas_hash():printed(false) {}
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
        onstk=true;
        elems.clear();
        size=0;
    }
};

void filehandle_destructor(void*);
void dir_entry_destructor(void*);
void dylib_destructor(void*);
void func_addr_destructor(void*);

struct ghost_register_table {
private:
    using dtor=void (*)(void*);

private:
    std::unordered_map<std::string, usize> mapper;
    std::vector<std::string> ghost_name;
    std::vector<dtor> destructors;

public:
    // reserved ghost type only for native functions
    usize ghost_file;
    usize ghost_dir;
    usize ghost_dylib;
    usize ghost_faddr;

public:
    ghost_register_table() {
        ghost_file = register_ghost_type("file", filehandle_destructor);
        ghost_dir = register_ghost_type("dir", dir_entry_destructor);
        ghost_dylib = register_ghost_type("dylib", dylib_destructor);
        ghost_faddr = register_ghost_type("faddr", func_addr_destructor);
    }

    bool exists(const std::string& name) const {
        return mapper.count(name);
    }

    usize get_ghost_type_index(const std::string& name) const {
        return mapper.at(name);
    }

    const std::string& get_ghost_name(usize index) const {
        return ghost_name.at(index);
    }

    usize register_ghost_type(const std::string& name, dtor ptr) {
        if (mapper.count(name)) {
            std::cerr<<"nasal_gc.h: ghost_register_table::register_ghost_type: ";
            std::cerr<<"ghost type \""<<name<<"\" already exists.\n";
            std::exit(1);
        }
        auto res=destructors.size();
        mapper[name]=res;
        ghost_name.push_back(name);
        destructors.push_back(ptr);
        return res;
    }

    dtor destructor(usize index) {
        return destructors.at(index);
    }
};

struct nas_ghost {
public:
    usize type;
    void* ptr;

private:
    ghost_register_table* ghost_type_table;

public:
    nas_ghost(): type(0), ptr(nullptr), ghost_type_table(nullptr) {}
    ~nas_ghost() {clear();}
    void set(usize, void*, ghost_register_table*);
    void clear();

public:
    friend std::ostream& operator<<(std::ostream& out, nas_ghost& ghost) {
        out<<"<object "<<ghost.ghost_type_table->get_ghost_name(ghost.type);
        out<<" at 0x"<<std::hex<<(u64)ghost.ptr<<std::dec<<">";
        return out;
    }

    const std::string& get_ghost_name() const {
        return ghost_type_table->get_ghost_name(type);
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
    var stack[STACK_DEPTH];
    context ctx;
    coroutine_status status;

    nas_co() {clear();}
    void clear();
};

struct nas_val {
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
    } ptr;

    nas_val(u8);
    ~nas_val();
    void clear();
};

std::ostream& operator<<(std::ostream&, nas_vec&);
std::ostream& operator<<(std::ostream&, nas_hash&);
std::ostream& operator<<(std::ostream&, var&);

const var zero = var::num(0);
const var one = var::num(1);
const var nil = var::nil();

struct gc {
    ghost_register_table global_ghost_type_table;
    /* main context temporary storage */
    context mctx;

    /* runtime context */
    context* rctx = nullptr;
    nas_co* cort = nullptr; // running coroutine

    /*  temporary space used in builtin/module functions */
    var temp=nil;

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
        16   // vm_co
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
    void mark();
    void mark_context(std::vector<var>&);
    void mark_var(std::vector<var>&, var&);
    void mark_vec(std::vector<var>&, nas_vec&);
    void mark_hash(std::vector<var>&, nas_hash&);
    void mark_func(std::vector<var>&, nas_func&);
    void mark_upval(std::vector<var>&, nas_upval&);
    void mark_co(std::vector<var>&, nas_co&);
    void sweep();

public:
    void extend(u8);
    void init(const std::vector<std::string>&, const std::vector<std::string>&);
    void clear();
    void info();
    var alloc(const u8);
    void ctxchg(nas_co&);
    void ctxreserve();

public:
    var newstr(char c) {
        var s=alloc(vm_str);
        s.str()=c;
        return s;
    }

    var newstr(const char* buff) {
        var s=alloc(vm_str);
        s.str()=buff;
        return s;
    }

    var newstr(const std::string& buff) {
        var s=alloc(vm_str);
        s.str()=buff;
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
typedef module_func_info* (*get_func_ptr)(ghost_register_table*);
