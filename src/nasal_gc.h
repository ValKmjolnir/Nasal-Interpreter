#pragma once

// avoid MSVC warnings
#ifdef _MSC_VER
#pragma warning (disable:4244)
#pragma warning (disable:4267)
#pragma warning (disable:4102)
#endif

#include <iomanip>
#include <vector>
#include <chrono>
#include <thread>
#include <cstring>
#include <sstream>

#include "nasal.h"
#include "nasal_type.h"

namespace nasal {

struct gc {
    /* main context temporary storage */
    context main_context;

    /* global storage */
    var* main_context_global = nullptr;
    usize main_context_global_size = 0;

    /* runtime context */
    context* running_context = nullptr;
    nas_co* cort = nullptr; // running coroutine

    /*  temporary space used in native/module functions */
    var temp = nil;

    /* constants and memory pool */
    std::vector<var> strs = {};        // reserved address for const vm_str
    std::vector<var> env_argv = {};    // command line arguments
    std::vector<nas_val*> memory;      // gc memory
    std::vector<nas_val*> unused[gc_type_size]; // gc free list

    /* heap increase size */
    u64 incr[gc_type_size] = {
        128, // vm_str
        128, // vm_vec
        64,  // vm_hash
        256, // vm_func
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

    void set(context* _ctx, var* _global, usize _size) {
        running_context = _ctx;
        main_context_global = _global;
        main_context_global_size = _size;
    }

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
    void mark_ghost(std::vector<var>&, nas_ghost&);
    void mark_co(std::vector<var>&, nas_co&);
    void mark_map(std::vector<var>&, nas_map&);
    void sweep();

public:
    void extend(const vm_type);
    void init(const std::vector<std::string>&, const std::vector<std::string>&);
    void clear();
    void info() const;
    var alloc(const vm_type);
    void context_change(nas_co*);
    void context_reserve();

public:
    var newstr(char c) {
        var s = alloc(vm_type::vm_str);
        s.str() = c;
        return s;
    }

    var newstr(const char* buff) {
        var s = alloc(vm_type::vm_str);
        s.str() = std::string(buff);
        return s;
    }

    var newstr(const std::string& buff) {
        var s = alloc(vm_type::vm_str);
        s.str() = buff;
        return s;
    }
};

// module function type
typedef var (*module_func)(var*, usize, gc*);

// module function stores in tables with this type, end with {nullptr,nullptr}
struct module_func_info {
    const char* name;
    module_func fd;
};

// module function "get" type
typedef module_func_info* (*get_func_ptr)();


// avoid error loading function bug in MSVC version nasal.exe
#ifdef _MSC_VER
    // and fuck MSVC again
    #define NASAL_EXTERN extern "C" __declspec(dllexport)
#else
    #define NASAL_EXTERN extern "C"
#endif

}
