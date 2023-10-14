#include "coroutine.h"

namespace nasal {

var builtin_cocreate(var* local, gc& ngc) {
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
    var func = local[1];
    if (func.type!=vm_func) {
        return nas_err(
            "coroutine::create",
            "must use a function to create coroutine"
        );
    }
    if (ngc.cort) {
        return nas_err(
            "coroutine::create",
            "cannot create another coroutine in a coroutine"
        );
    }
    var co = ngc.alloc(vm_co);
    nas_co& cort = co.co();
    cort.ctx.pc = func.func().entry-1;

    cort.ctx.top[0] = nil;
    cort.ctx.localr = cort.ctx.top+1;
    cort.ctx.top = cort.ctx.localr+func.func().local_size;
    cort.ctx.localr[0] = func.func().local[0];
    // store old upvalr on stack
    cort.ctx.top[0] = nil;
    cort.ctx.top++;
    // store old localr on stack
    cort.ctx.top[0] = var::addr((var*)nullptr);
    cort.ctx.top++;
    // store old pc on stack
    // set to zero to make op_ret recognizing this as coroutine function
    cort.ctx.top[0] = var::ret(0);

    // make sure the coroutine function can use correct upvalues
    cort.ctx.funcr = func;
    cort.status = nas_co::status::suspended;
    
    return co;
}

var builtin_coresume(var* local, gc& ngc) {
    if (ngc.cort) {
        return nas_err(
            "coroutine::resume",
            "cannot start another coroutine when one is running"
        );
    }
    var co = local[1];
    // return nil if is not a coroutine object
    if (co.type!=vm_co) {
        return nil;
    }
    // cannot resume a dead coroutine
    if (co.co().status==nas_co::status::dead) {
        return nil;
    }

    // change to coroutine context
    ngc.ctxchg(co.co());

    // fetch coroutine's stack top and return
    // so the coroutine's stack top in fact is not changed
    if (ngc.running_context->top[0].type==vm_ret) {
        // when first calling this coroutine, the stack top must be vm_ret
        return ngc.running_context->top[0];
    }

    // after first calling the coroutine, each time coroutine.yield triggered
    // a new space will be reserved on stack with value nil
    // so we could fill this place with args

    // the coroutine seems like coroutine.yield returns the value
    // but in fact coroutine.yield stop the coroutine
    // until main context calls the coroutine.resume
    return local[2];
}

var builtin_coyield(var* local, gc& ngc) {
    if (!ngc.cort) {
        return nas_err("coroutine::yield", "no coroutine is running");
    }

    // this will set to main stack top
    ngc.ctxreserve();
    
    // then this will return value to main's stack top[0]
    // the procedure seems like coroutine.resume returns the value
    // but in fact coroutine.resume stop the main context
    // until coroutine calls the coroutine.yield
    return local[1];
}

var builtin_costatus(var* local, gc& ngc) {
    var co = local[1];
    if (co.type!=vm_co) {
        return ngc.newstr("error");
    }
    switch(co.co().status) {
        case nas_co::status::suspended: return ngc.newstr("suspended");
        case nas_co::status::running:   return ngc.newstr("running");
        case nas_co::status::dead:      return ngc.newstr("dead");
    }
    return nil;
}

var builtin_corun(var* local, gc& ngc) {
    return ngc.cort? one:zero;
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
