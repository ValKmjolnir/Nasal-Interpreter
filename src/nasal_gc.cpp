#include "nasal_gc.h"
#include "util/util.h"

namespace nasal {

void gc::do_mark_sweep() {
    count_mark_time();
    count_sweep_time();
}

void gc::count_mark_time() {
    if (in_incremental_sweep_stage) {
        return;
    }

    status.stamp();
    mark();
    status.elapsed_mark_time();

    in_incremental_sweep_stage = true;
    current_sweep_index = memory.size() - 1;
}

void gc::count_sweep_time() {
    status.stamp();
    sweep();
    status.elapsed_sweep_time();
}

void gc::mark() {
    std::vector<var> bfs;
    mark_context_root(bfs);

    // concurrent mark, experimental
    if (memory.size() > gc::concurrent_threshold() && bfs.size() > 16) {
        status.flag_concurrent_mark_triggered = true;
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

    // normal mark
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
    for(usize i = 0; i < main_context_global_size; ++i) {
        auto& val = main_context_global[i];
        if (val.type > vm_type::vm_num) {
            bfs_queue.push_back(val);
        }
    }
    // scan now running context, this context maybe related to coroutine or main
    for(var* i = running_context->stack; i <= running_context->top; ++i) {
        if (i->type > vm_type::vm_num) {
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
    for(var* i = main_context.stack; i <= main_context.top; ++i) {
        if (i->type > vm_type::vm_num) {
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
        if (i.type > vm_type::vm_num) {
            bfs_queue.push_back(i);
        }
    }
}

void gc::mark_hash(std::vector<var>& bfs_queue, nas_hash& hash) {
    for(auto& i : hash.elems) {
        if (i.second.type > vm_type::vm_num) {
            bfs_queue.push_back(i.second);
        }
    }
}

void gc::mark_func(std::vector<var>& bfs_queue, nas_func& function) {
    for(auto& i : function.local) {
        if (i.type > vm_type::vm_num) {
            bfs_queue.push_back(i);
        }
    }
    for(auto& i : function.upval) {
        bfs_queue.push_back(i);
    }
}

void gc::mark_upval(std::vector<var>& bfs_queue, nas_upval& upval) {
    for(auto& i : upval.elems) {
        if (i.type > vm_type::vm_num) {
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
        if (i->type > vm_type::vm_num) {
            bfs_queue.push_back(*i);
        }
    }
}

void gc::mark_map(std::vector<var>& bfs_queue, nas_map& mp) {
    for(const auto& i : mp.mapper) {
        if (i.second->type > vm_type::vm_num) {
            bfs_queue.push_back(*i.second);
        }
    }
}

void gc::sweep() {
    // if threshold is too small, too many allocated objects will be marked as "found"
    // objects with "found" will be marked to "uncollected" in the next gc cycle
    // this will cause memory wasting.
    const i64 threshold = 4096;
    for (i64 it = 0; it < threshold; ++it) {
        if (current_sweep_index - it < 0) {
            break;
        }
        auto i = memory[current_sweep_index - it];
        if (i->mark==nas_val::gc_status::uncollected) {
            unused[static_cast<u32>(i->type)-static_cast<u32>(vm_type::vm_str)].push_back(i);
            i->mark = nas_val::gc_status::collected;
        } else if (i->mark==nas_val::gc_status::found) {
            i->mark = nas_val::gc_status::uncollected;
        }
    }
    current_sweep_index -= threshold;
    if (current_sweep_index < 0) {
        in_incremental_sweep_stage = false;
        current_sweep_index = 0;
    }
}

void gc::extend(const vm_type type) {
    const u32 index = static_cast<u32>(type)-static_cast<u32>(vm_type::vm_str);
    status.object_size[index] += incr[index];

    for(u64 i = 0; i<incr[index]; ++i) {
        // no need to check, will be killed if memory is not enough
        nas_val* tmp = new nas_val(type);

        // add to heap
        memory.push_back(tmp);
        unused[index].push_back(tmp);
    }
    switch(type) {
        case vm_type::vm_str:
            total_object_count += incr[index] * sizeof(std::string); break;
        case vm_type::vm_vec:
            total_object_count += incr[index] * sizeof(nas_vec); break;
        case vm_type::vm_hash:
            total_object_count += incr[index] * sizeof(nas_hash); break;
        case vm_type::vm_func:
            total_object_count += incr[index] * sizeof(nas_func); break;
        case vm_type::vm_upval:
            total_object_count += incr[index] * sizeof(nas_upval); break;
        case vm_type::vm_ghost:
            total_object_count += incr[index] * sizeof(nas_ghost); break;
        case vm_type::vm_co:
            total_object_count += incr[index] * sizeof(nas_co); break;
        case vm_type::vm_map:
            total_object_count += incr[index] * sizeof(nas_map); break;
        default: break;
    }

    // if incr[index] = 1, this will always be 1
    incr[index] = incr[index] + incr[index];
}

void gc::init(const std::vector<std::string>& constant_strings,
              const std::vector<std::string>& argv) {
    // initialize gc status recorder
    status.init();

    // coroutine pointer set to nullptr
    cort = nullptr;

    // init constant strings
    strs.resize(constant_strings.size());
    for (u64 i = 0; i < strs.size(); ++i) {
        // incremental initialization, avoid memory leak in repl mode
        if (strs[i].is_str() && strs[i].str()==constant_strings[i]) {
            continue;
        }
        strs[i] = var::gcobj(new nas_val(vm_type::vm_str));
        strs[i].val.gcobj->immutable = 1;
        strs[i].str() = constant_strings[i];
        total_object_count += strs[i].str().size();
        total_object_count += sizeof(std::string);
    }

    // record arguments
    env_argv.resize(argv.size());
    for (u64 i = 0; i < argv.size(); ++i) {
        // incremental initialization, avoid memory leak in repl mode
        if (env_argv[i].is_str() && env_argv[i].str() == argv[i]) {
            continue;
        }
        env_argv[i] = var::gcobj(new nas_val(vm_type::vm_str));
        env_argv[i].val.gcobj->immutable = 1;
        env_argv[i].str() = argv[i];
        total_object_count += env_argv[i].str().size();
        total_object_count += sizeof(std::string);
    }
}

void gc::clear() {
    for(auto i : memory) {
        delete i;
    }
    memory.clear();
    for(u32 i = 0; i<GC_TYPE_SIZE; ++i) {
        unused[i].clear();
    }
    for(auto& i : strs) {
        delete i.val.gcobj;
    }
    strs.clear();
    env_argv.clear();
}

void gc::info() const {
    util::windows_code_page_manager wm;
    wm.set_utf8_output();

    using std::left;
    using std::setw;
    using std::setfill;
    using std::setprecision;

    const char* used_table_name[] = {
        "object type",
        "gc cycle",
        "alloc count",
        "object count",
        "detail",
        "time spend",
        "gc time",
        "avg time",
        "max gc",
        "max mark",
        "max sweep",
        nullptr
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
    for(u32 i = 0; i<GC_TYPE_SIZE; ++i) {
        len = std::to_string(status.gc_cycle_trigger_count[i]).length();
        indent = indent<len? len:indent;
        len = std::to_string(status.alloc_count[i]).length();
        indent = indent<len? len:indent;
        len = std::to_string(status.object_size[i]).length();
        indent = indent<len? len:indent;
    }
    auto indent_string = std::string("──");
    for(usize i = 0; i<indent; ++i) {
        indent_string += "─";
    }
    const auto first_line = "╭" + indent_string + "┬" +
                            indent_string + "┬" +
                            indent_string + "┬" +
                            indent_string + "╮";
    const auto second_line = "├" + indent_string + "┼" +
                             indent_string + "┼" +
                             indent_string + "┼" +
                             indent_string + "┤";
    const auto mid_line = "├" + indent_string + "┼" +
                          indent_string + "┴" +
                          indent_string + "┴" +
                          indent_string + "┤";
    const auto another_mid_line = "├" + indent_string + "┼" +
                                  indent_string + "─" +
                                  indent_string + "─" +
                                  indent_string + "┤";
    const auto last_line = "╰" + indent_string + "┴" +
                           indent_string + "─" +
                           indent_string + "─" +
                           indent_string + "╯";

    std::clog << "\n" << first_line << "\n";
    std::clog << "│ " << left << setw(indent) << setfill(' ') << "object type";
    std::clog << " │ " << left << setw(indent) << setfill(' ') << "gc cycle";
    std::clog << " │ " << left << setw(indent) << setfill(' ') << "alloc count";
    std::clog << " │ " << left << setw(indent) << setfill(' ') << "object count";
    std::clog << " │\n" << second_line << "\n" << std::internal;

    double total = 0;
    for(u8 i = 0; i<GC_TYPE_SIZE; ++i) {
        if (!status.gc_cycle_trigger_count[i] &&
            !status.alloc_count[i] &&
            !status.object_size[i]) {
            continue;
        }
        total += static_cast<f64>(status.gc_cycle_trigger_count[i]);
        std::clog << "│ " << left << setw(indent) << setfill(' ') << name[i];
        std::clog << " │ " << left << setw(indent) << setfill(' ') << status.gc_cycle_trigger_count[i];
        std::clog << " │ " << left << setw(indent) << setfill(' ') << status.alloc_count[i];
        std::clog << " │ " << left << setw(indent) << setfill(' ') << status.object_size[i];
        std::clog << " │\n" << std::internal;
    }
    std::clog << mid_line << "\n";

    const auto den = std::chrono::high_resolution_clock::duration::period::den;
    std::clog << "│ " << left << setw(indent) << setfill(' ') << "detail";
    std::clog << " │ " << left << setw(indent) << setfill(' ') << "time spend";
    std::clog << "   " << left << setw(indent) << setfill(' ') << " ";
    std::clog << "   " << left << setw(indent) << setfill(' ') << " ";
    std::clog << " │\n" << another_mid_line << "\n";

    std::clog << "│ " << left << setw(indent) << setfill(' ') << "gc time";
    std::clog << " │ " << setw(indent-3) << setprecision(4) << status.gc_time_ms() << " ms";
    std::clog << setw(indent*2+7) << " " << "│\n";

    std::clog << "│ " << left << setw(indent) << setfill(' ') << "avg time";
    std::clog << " │ " << setw(indent-3) << setprecision(4) << status.avg_time_ms() << " ms";
    std::clog << setw(indent*2+7) << " " << "│\n";

    std::clog << "│ " << left << setw(indent) << setfill(' ') << "avg mark";
    std::clog << " │ " << setw(indent-3) << setprecision(4) << status.avg_mark_time_ms() << " ms";
    std::clog << setw(indent*2+7) << " " << "│\n";

    std::clog << "│ " << left << setw(indent) << setfill(' ') << "avg sweep";
    std::clog << " │ " << setw(indent-3) << setprecision(4) << status.avg_sweep_time_ms() << " ms";
    std::clog << setw(indent*2+7) << " " << "│\n";

    std::clog << "│ " << left << setw(indent) << setfill(' ') << "max mark";
    std::clog << " │ " << setw(indent-3) << setprecision(4) << status.max_mark_time_ms() << " ms";
    std::clog << setw(indent*2+7) << " " << "│\n";

    std::clog << "│ " << left << setw(indent) << setfill(' ') << "max sweep";
    std::clog << " │ " << setw(indent-3) << setprecision(4) << status.max_sweep_time_ms() << " ms";
    std::clog << setw(indent*2+7) << " " << "│\n";

    std::clog << "│ " << left << setw(indent) << setfill(' ') << "concurrent";
    std::clog << " │ " << setw(indent)
              << (status.flag_concurrent_mark_triggered? "true":"false");
    std::clog << setw(indent*2+7) << " " << "│\n";

    std::clog << last_line << "\n" << std::internal;

    wm.restore_code_page();
}

var gc::alloc(const vm_type type) {
    const u32 index = static_cast<u32>(type)-static_cast<u32>(vm_type::vm_str);
    ++status.alloc_count[index];
    // if still in incremental sweep stage? do it
    // if not in incremental sweep stage, run a new gc cycle
    if (in_incremental_sweep_stage) {
        do_mark_sweep();
    } else if (unused[index].empty()) {
        ++status.gc_cycle_trigger_count[index];
        do_mark_sweep();
    }
    // if in incremental sweep stage, but the unused list is empty,
    // do it until the unused list has something
    while (unused[index].empty() && in_incremental_sweep_stage) {
        do_mark_sweep();
    }
    // after all gc stages, still get empty list, extend
    if (unused[index].empty()) {
        extend(type);
    }

    var ret = var::gcobj(unused[index].back());
    ret.val.gcobj->clear();

    // if incremental sweep stage, mark it as found
    // but be aware that it may be collected in next gc cycle
    ret.val.gcobj->mark = in_incremental_sweep_stage
        ? nas_val::gc_status::found
        : nas_val::gc_status::uncollected;
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
    // pc = 0 means this coroutine is finished
    cort->status = running_context->pc
        ? nas_co::status::suspended
        : nas_co::status::dead;

    // store running state to coroutine
    cort->ctx = *running_context;

    // restore main context state
    *running_context = main_context;

    // set coroutine pointer to nullptr
    cort = nullptr;
}

}
