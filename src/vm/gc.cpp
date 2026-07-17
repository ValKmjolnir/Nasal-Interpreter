#include "vm/gc.hpp"

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
    std::queue<var> bfs;
    mark_context_root(bfs);

    while (!bfs.empty()) {
        var value = bfs.front();
        bfs.pop();
        if (value.type != vm_type::vm_gcobj) {
            continue;
        }
        if (value.val.gcobj->mark != nas_val::gc_status::uncollected &&
            value.val.gcobj->mark != nas_val::gc_status::alloc_in_sweep_stage) {
            continue;
        }
        mark_var(bfs, value);
    }
}

void gc::mark_context_root(std::queue<var>& bfs_queue) {
    // scan global
    for (usize i = 0; i < main_context_global_size; ++i) {
        auto& val = main_context_global[i];
        if (val.type > vm_type::vm_num) {
            bfs_queue.push(val);
        }
    }
    // scan now running context, this context maybe related to coroutine or main
    for (var* i = running_context->stack; i <= running_context->top; ++i) {
        if (i->type > vm_type::vm_num) {
            bfs_queue.push(*i);
        }
    }
    bfs_queue.push(running_context->funcr);
    bfs_queue.push(running_context->upvalr);
    bfs_queue.push(temp);

    if (!cort) {
        return;
    }

    // coroutine is running, so scan main process stack from mctx
    for (var* i = main_context.stack; i <= main_context.top; ++i) {
        if (i->type > vm_type::vm_num) {
            bfs_queue.push(*i);
        }
    }
    bfs_queue.push(main_context.funcr);
    bfs_queue.push(main_context.upvalr);
}

void gc::mark_var(std::queue<var>& bfs_queue, var& value) {
    value.val.gcobj->mark = nas_val::gc_status::found;
    switch (value.val.gcobj->type) {
        case gc_type::gc_vec: mark_vec(bfs_queue, value.vec()); break;
        case gc_type::gc_hash: mark_hash(bfs_queue, value.hash()); break;
        case gc_type::gc_func: mark_func(bfs_queue, value.func()); break;
        case gc_type::gc_upval: mark_upval(bfs_queue, value.upval()); break;
        case gc_type::gc_ghost: mark_ghost(bfs_queue, value.ghost()); break;
        case gc_type::gc_co: mark_co(bfs_queue, value.co()); break;
        case gc_type::gc_map: mark_map(bfs_queue, value.map()); break;
        default: break;
    }
}

void gc::mark_vec(std::queue<var>& bfs_queue, nas_vec& vec) {
    for (auto& i : vec.elems) {
        if (i.type > vm_type::vm_num) {
            bfs_queue.push(i);
        }
    }
}

void gc::mark_hash(std::queue<var>& bfs_queue, nas_hash& hash) {
    for (auto& i : hash.elems) {
        if (i.second.type > vm_type::vm_num) {
            bfs_queue.push(i.second);
        }
    }
}

void gc::mark_func(std::queue<var>& bfs_queue, nas_func& function) {
    for (auto& i : function.local) {
        if (i.type > vm_type::vm_num) {
            bfs_queue.push(i);
        }
    }
    for (auto& i : function.upval) {
        bfs_queue.push(i);
    }
}

void gc::mark_upval(std::queue<var>& bfs_queue, nas_upval& upval) {
    if (upval.on_stack) {
        for (u64 i = 0; i < upval.size; ++i) {
            if (upval.stack_frame_offset[i].type > vm_type::vm_num) {
                bfs_queue.push(upval.stack_frame_offset[i]);
            }
        }
        return;
    }

    for (auto& i : upval.elems) {
        if (i.type > vm_type::vm_num) {
            bfs_queue.push(i);
        }
    }
}

void gc::mark_ghost(std::queue<var>& bfs_queue, nas_ghost& ghost) {
    if (!ghost.gc_mark_function) {
        return;
    }
    ghost.gc_mark_function(ghost.pointer, &bfs_queue);
}

void gc::mark_co(std::queue<var>& bfs_queue, nas_co& co) {
    bfs_queue.push(co.ctx.funcr);
    bfs_queue.push(co.ctx.upvalr);
    for (var* i = co.ctx.stack; i <= co.ctx.top; ++i) {
        if (i->type > vm_type::vm_num) {
            bfs_queue.push(*i);
        }
    }
}

void gc::mark_map(std::queue<var>& bfs_queue, nas_map& mp) {
    for (const auto& i : mp.mapper) {
        if (i.second->type > vm_type::vm_num) {
            bfs_queue.push(*i.second);
        }
    }
}

void gc::sweep() {
    // if threshold is too small, too many allocated objects will be marked as "found"
    // objects with "found" will be marked to "uncollected" in the next gc cycle
    // this will cause memory wasting.
    const i64 threshold = status.calc_sweep_threshold();
    for (i64 it = 0; it < threshold; ++it) {
        const auto index = current_sweep_index - it;
        if (index < 0) {
            break;
        }
        auto i = memory[index];
        if (i->mark == nas_val::gc_status::uncollected) {
            unused[static_cast<u32>(i->type) - static_cast<u32>(gc_type::gc_str)].push_back(i);
            i->mark = nas_val::gc_status::collected;
        } else if (i->mark == nas_val::gc_status::found ||
                   i->mark == nas_val::gc_status::alloc_in_sweep_stage) {
            i->mark = nas_val::gc_status::uncollected;
        }
    }

    current_sweep_index -= threshold;
    if (current_sweep_index < 0) {
        in_incremental_sweep_stage = false;
        current_sweep_index = 0;
    }
}

void gc::extend(const gc_type type) {
    const u32 index = static_cast<u32>(type);
    status.object_size[index] += incr[index];

    for (u64 i = 0; i < incr[index]; ++i) {
        // no need to check, will be killed if memory is not enough
        nas_val* tmp = new nas_val(type);

        // add to heap
        memory.push_back(tmp);
        unused[index].push_back(tmp);
    }
    switch (type) {
        case gc_type::gc_str:
            total_object_count += incr[index] * sizeof(std::string); break;
        case gc_type::gc_vec:
            total_object_count += incr[index] * sizeof(nas_vec); break;
        case gc_type::gc_hash:
            total_object_count += incr[index] * sizeof(nas_hash); break;
        case gc_type::gc_func:
            total_object_count += incr[index] * sizeof(nas_func); break;
        case gc_type::gc_upval:
            total_object_count += incr[index] * sizeof(nas_upval); break;
        case gc_type::gc_ghost:
            total_object_count += incr[index] * sizeof(nas_ghost); break;
        case gc_type::gc_co:
            total_object_count += incr[index] * sizeof(nas_co); break;
        case gc_type::gc_map:
            total_object_count += incr[index] * sizeof(nas_map); break;
        default: break;
    }

    incr[index] = incr[index] + incr[index];
    if (incr[index] > max_incr[index]) {
        incr[index] = max_incr[index];
    }
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
        if (strs[i].is_str() && strs[i].str() == constant_strings[i]) {
            continue;
        }
        strs[i] = var::gcobj(new nas_val(gc_type::gc_str));
        strs[i].val.gcobj->immutable = true;
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
        env_argv[i] = var::gcobj(new nas_val(gc_type::gc_str));
        env_argv[i].val.gcobj->immutable = true;
        env_argv[i].str() = argv[i];
        total_object_count += env_argv[i].str().size();
        total_object_count += sizeof(std::string);
    }
}

void gc::clear() {
    for (auto i : memory) {
        delete i;
    }
    memory.clear();
    for (u32 i = 0; i < GC_TYPE_SIZE; ++i) {
        unused[i].clear();
    }
    for (auto& i : strs) {
        delete i.val.gcobj;
    }
    strs.clear();
    env_argv.clear();
}

var gc::alloc(const gc_type type) {
    const u32 index = static_cast<u32>(type);
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

    // if incremental sweep stage, mark it with special state.
    // to avoid miss-marking objects inside them, because mark stage
    // will skip scanning objects with "found" mark
    // if we mark it with "found" mark, objects inside it will not be marked
    // so they may be treated as unreferenced, which is not expected.
    // be aware that it will be collected in next gc cycle
    ret.val.gcobj->mark = in_incremental_sweep_stage
        ? nas_val::gc_status::alloc_in_sweep_stage
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
