#include "coroutine.h"

namespace nasal {

var builtin_cocreate(context* ctx, gc* ngc) {
    // ```
    // +-------------+
    // | old pc      | <- top[0]
    // +-------------+
    // | old localr  | <- top[-1]
    // +-------------+
    // | old upvalr  | <- top[-2]
    // +-------------+
    // | local scope |
    // | ...         |
    // +-------------+ <- local pointer stored in localr
    // | old funcr   | <- old function stored in funcr
    // +-------------+
    // ```
    auto coroutine_function = ctx->localr[1];
    if (coroutine_function.type!=vm_func) {
        return nas_err(
            "coroutine::create",
            "must use a function to create coroutine"
        );
    }
    if (ngc->cort) {
        return nas_err(
            "coroutine::create",
            "cannot create another coroutine in a coroutine"
        );
    }
    auto coroutine_object = ngc->alloc(vm_co);
    auto& coroutine = coroutine_object.co();
    coroutine.ctx.pc = coroutine_function.func().entry-1;

    coroutine.ctx.top[0] = nil;
    coroutine.ctx.localr = coroutine.ctx.top+1;
    coroutine.ctx.top = coroutine.ctx.localr +
                        coroutine_function.func().local_size;
    coroutine.ctx.localr[0] = coroutine_function.func().local[0];

    // store old upvalr on stack
    coroutine.ctx.top[0] = nil;
    coroutine.ctx.top++;

    // store old localr on stack
    coroutine.ctx.top[0] = var::addr((var*)nullptr);
    coroutine.ctx.top++;

    // store old pc on stack
    // set to zero to make op_ret recognizing this as coroutine function
    coroutine.ctx.top[0] = var::ret(0);

    // make sure the coroutine function can use correct upvalues
    coroutine.ctx.funcr = coroutine_function;
    coroutine.status = nas_co::status::suspended;

    return coroutine_object;
}

var builtin_coresume(context* ctx, gc* ngc) {
    if (ngc->cort) {
        return nas_err(
            "coroutine::resume",
            "cannot start another coroutine when one is running"
        );
    }
    auto main_local_frame = ctx->localr;
    auto coroutine_object = main_local_frame[1];
    // return nil if is not a coroutine object or coroutine exited
    if (coroutine_object.type!=vm_co ||
        coroutine_object.co().status==nas_co::status::dead) {
        return nil;
    }

    // change to coroutine context
    ngc->context_change(&coroutine_object.co());

    // fetch coroutine's stack top and return
    // then coroutine's stack top will catch this return value
    // so the coroutine's stack top in fact is not changed
    if (ngc->running_context->top[0].type==vm_ret) {
        // when first calling this coroutine, the stack top must be vm_ret
        return ngc->running_context->top[0];
    }

    // after first calling the coroutine, each time coroutine.yield triggered
    // a new space will be reserved on stack with value nil
    // so we could fill this place with args

    // the coroutine seems like coroutine.yield returns the value
    // but in fact coroutine.yield stop the coroutine
    // until main context calls the coroutine.resume
    return main_local_frame[2];
}

var builtin_coyield(context* ctx, gc* ngc) {
    if (!ngc->cort) {
        return nas_err("coroutine::yield", "no coroutine is running");
    }
    // get coroutine local frame
    auto coroutine_local_frame = ctx->localr;

    // vm context will set to main context
    ngc->context_reserve();

    // then this will return value to main's stack top[0]
    // the procedure seems like coroutine.resume returns the value
    // but in fact coroutine.resume stop the main context
    // until coroutine calls the coroutine.yield
    return coroutine_local_frame[1];
}

var builtin_costatus(context* ctx, gc* ngc) {
    auto coroutine_object = ctx->localr[1];
    if (coroutine_object.type!=vm_co) {
        return ngc->newstr("error");
    }
    switch(coroutine_object.co().status) {
        case nas_co::status::suspended: return ngc->newstr("suspended");
        case nas_co::status::running:   return ngc->newstr("running");
        case nas_co::status::dead:      return ngc->newstr("dead");
    }
    return nil;
}

var builtin_corun(context* ctx, gc* ngc) {
    return ngc->cort? one:zero;
}

nasal_builtin_table coroutine_native[] = {
    {"__cocreate", builtin_cocreate},
    {"__coresume", builtin_coresume},
    {"__coyield", builtin_coyield},
    {"__costatus", builtin_costatus},
    {"__corun", builtin_corun},
    {nullptr, nullptr}
};

}
