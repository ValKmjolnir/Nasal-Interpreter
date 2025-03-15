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
#include <iostream>

#include "nasal.h"
#include "nasal_type.h"
#include "util/gc_stat.h"

namespace nasal {

struct free_list {
    std::vector<nas_val*> elem[GC_TYPE_SIZE];

    auto& operator[](i64 index) {
        return elem[index];
    }
};

struct gc {
    /* main context temporary storage */
    context main_context;

    /* global storage */
    var* main_context_global = nullptr;
    usize main_context_global_size = 0;

    /* runtime context */
    context* running_context = nullptr;
    nas_co* cort = nullptr;         // running coroutine

    /*  temporary space used in native / module functions */
    var temp = nil;

    /* constants and memory pool */
    std::vector<var> strs = {};     // reserved address for const vm_str
    std::vector<var> env_argv = {}; // command line arguments
    std::vector<nas_val*> memory;   // gc memory
    free_list unused;               // gc free list

    /* heap increase size */
    u64 incr[GC_TYPE_SIZE] = {
        256, // vm_str
        256, // vm_vec
        256, // vm_hash
        256, // vm_func
        256, // vm_upval
        4,   // vm_obj
        4,   // vm_co
        1,   // vm_map
    };

    // total object count
    u64 total_object_count = 0;

    /* values for analysis */
    gc_stat status;

    bool in_incremental_sweep_stage = false;
    i64 current_sweep_index = 0;

    void set(context* _ctx, var* _global, usize _size) {
        running_context = _ctx;
        main_context_global = _global;
        main_context_global_size = _size;
    }

private:
    /* gc functions */
    void do_mark_sweep();
    void count_mark_time();
    void count_sweep_time();
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

    static const auto concurrent_threshold() {
        return UINT16_MAX * 16;
    }

public:
    void extend(const vm_type);
    void init(const std::vector<std::string>&, const std::vector<std::string>&);
    void clear();
    void info() const;
    var alloc(const vm_type);
    void context_change(nas_co*);
    void context_reserve();

public:
    f64 get_gc_time_ms() const {
        return status.gc_time_ms();
    }

    // not very accurate
    f64 get_total_memory() const {
        return total_object_count * 3.5 / 1024.0 / 1024.0;
    }

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

// module function stores in tables with this type, end with {nullptr, nullptr}
struct module_func_info {
    const char* name;
    module_func fd;
};

// module function "get" type
typedef module_func_info* (*get_func_ptr)();

}
