#include "nasal_gc.h"

namespace nasal {

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
        if (value.type<=vm_type::vm_num ||
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
        if (value.type<=vm_type::vm_num ||
            value.val.gcobj->mark!=nas_val::gc_status::uncollected) {
            continue;
        }
        mark_var(bfs, value);
    }
    while(!bfs.empty()) {
        var value = bfs.back();
        bfs.pop_back();
        if (value.type<=vm_type::vm_num ||
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
        if (val.type>vm_type::vm_num) {
            bfs_queue.push_back(val);
        }
    }
    // scan now running context, this context maybe related to coroutine or main
    for(var* i = running_context->stack; i<=running_context->top; ++i) {
        if (i->type>vm_type::vm_num) {
            bfs_queue.push_back(*i);
        }
    }
    bfs_queue.push_back(running_context->funcr);
    bfs_queue.push_back(running_context->upvalr);
    bfs_queue.push_back(temp);

    if (!cort) {
        return;
    }

    // coroutine is running, so scan main process stack from mctx
    for(var* i = main_context.stack; i<=main_context.top; ++i) {
        if (i->type>vm_type::vm_num) {
            bfs_queue.push_back(*i);
        }
    }
    bfs_queue.push_back(main_context.funcr);
    bfs_queue.push_back(main_context.upvalr);
}

void gc::mark_var(std::vector<var>& bfs_queue, var& value) {
    value.val.gcobj->mark = nas_val::gc_status::found;
    switch(value.type) {
        case vm_type::vm_vec: mark_vec(bfs_queue, value.vec()); break;
        case vm_type::vm_hash: mark_hash(bfs_queue, value.hash()); break;
        case vm_type::vm_func: mark_func(bfs_queue, value.func()); break;
        case vm_type::vm_upval: mark_upval(bfs_queue, value.upval()); break;
        case vm_type::vm_ghost: mark_ghost(bfs_queue, value.ghost()); break;
        case vm_type::vm_co: mark_co(bfs_queue, value.co()); break;
        case vm_type::vm_map: mark_map(bfs_queue, value.map()); break;
        default: break;
    }
}

void gc::mark_vec(std::vector<var>& bfs_queue, nas_vec& vec) {
    for(auto& i : vec.elems) {
        if (i.type>vm_type::vm_num) {
            bfs_queue.push_back(i);
        }
    }
}

void gc::mark_hash(std::vector<var>& bfs_queue, nas_hash& hash) {
    for(auto& i : hash.elems) {
        if (i.second.type>vm_type::vm_num) {
            bfs_queue.push_back(i.second);
        }
    }
}

void gc::mark_func(std::vector<var>& bfs_queue, nas_func& function) {
    for(auto& i : function.local) {
        if (i.type>vm_type::vm_num) {
            bfs_queue.push_back(i);
        }
    }
    for(auto& i : function.upval) {
        bfs_queue.push_back(i);
    }
}

void gc::mark_upval(std::vector<var>& bfs_queue, nas_upval& upval) {
    for(auto& i : upval.elems) {
        if (i.type>vm_type::vm_num) {
            bfs_queue.push_back(i);
        }
    }
}

void gc::mark_ghost(std::vector<var>& bfs_queue, nas_ghost& ghost) {
    if (!ghost.gc_mark_function) {
        return;
    }
    ghost.gc_mark_function(ghost.pointer, &bfs_queue);
}

void gc::mark_co(std::vector<var>& bfs_queue, nas_co& co) {
    bfs_queue.push_back(co.ctx.funcr);
    bfs_queue.push_back(co.ctx.upvalr);
    for(var* i = co.ctx.stack; i<=co.ctx.top; ++i) {
        if (i->type>vm_type::vm_num) {
            bfs_queue.push_back(*i);
        }
    }
}

void gc::mark_map(std::vector<var>& bfs_queue, nas_map& mp) {
    for(const auto& i : mp.mapper) {
        if (i.second->type>vm_type::vm_num) {
            bfs_queue.push_back(*i.second);
        }
    }
}

void gc::sweep() {
    for(auto i : memory) {
        if (i->mark==nas_val::gc_status::uncollected) {
            i->clear();
            unused[static_cast<u8>(i->type)-static_cast<u8>(vm_type::vm_str)].push_back(i);
            i->mark = nas_val::gc_status::collected;
        } else if (i->mark==nas_val::gc_status::found) {
            i->mark = nas_val::gc_status::uncollected;
        }
    }
}

void gc::extend(const vm_type type) {
    const u8 index = static_cast<u8>(type)-static_cast<u8>(vm_type::vm_str);
    size[index] += incr[index];

    for(u32 i = 0; i<incr[index]; ++i) {
        // no need to check, will be killed if memory is not enough
        nas_val* tmp = new nas_val(type);

        // add to heap
        memory.push_back(tmp);
        unused[index].push_back(tmp);
    }
    // if incr[index] = 1, this will always be 1
    incr[index] = incr[index]+incr[index]/2;
}

void gc::init(
    const std::vector<std::string>& constant_strings,
    const std::vector<std::string>& argv
) {
    // initialize counters
    worktime = 0;
    for(u8 i = 0; i<gc_type_size; ++i) {
        size[i] = gcnt[i] = acnt[i] = 0;
    }

    // coroutine pointer set to nullptr
    cort = nullptr;

    // init constant strings
    strs.resize(constant_strings.size());
    for(u32 i = 0; i<strs.size(); ++i) {
        // incremental initialization, avoid memory leak in repl mode
        if (strs[i].is_str() && strs[i].str()==constant_strings[i]) {
            continue;
        }
        strs[i] = var::gcobj(new nas_val(vm_type::vm_str));
        strs[i].val.gcobj->unmutable = 1;
        strs[i].str() = constant_strings[i];
    }

    // record arguments
    env_argv.resize(argv.size());
    for(usize i = 0; i<argv.size(); ++i) {
        // incremental initialization, avoid memory leak in repl mode
        if (env_argv[i].is_str() && env_argv[i].str()==argv[i]) {
            continue;
        }
        env_argv[i] = var::gcobj(new nas_val(vm_type::vm_str));
        env_argv[i].val.gcobj->unmutable = 1;
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
        "ghost",
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

var gc::alloc(const vm_type type) {
    const u8 index = static_cast<u8>(type)-static_cast<u8>(vm_type::vm_str);
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

void gc::context_change(nas_co* co) {
    // store running state to main context
    main_context = *running_context;

    // restore coroutine context state
    *running_context = co->ctx;

    // set coroutine pointer
    cort = co;

    // set coroutine state to running
    cort->status = nas_co::status::running;
}

void gc::context_reserve() {
    // pc=0 means this coroutine is finished
    cort->status = running_context->pc?
        nas_co::status::suspended:
        nas_co::status::dead;

    // store running state to coroutine
    cort->ctx = *running_context;

    // restore main context state
    *running_context = main_context;

    // set coroutine pointer to nullptr
    cort = nullptr;
}

}
