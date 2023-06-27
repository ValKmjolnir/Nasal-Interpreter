#include "nasal_new_gc.h"

void filehandle_destructor(void* ptr) {
    if ((FILE*)ptr==stdin) {
        return;
    }
    fclose((FILE*)ptr);
}

void dir_entry_destructor(void* ptr) {
#ifndef _MSC_VER
    closedir((DIR*)ptr);
#else
    FindClose(ptr);
#endif
}

void dylib_destructor(void* ptr) {
#ifdef _WIN32
    FreeLibrary((HMODULE)ptr);
#else
    dlclose(ptr);
#endif
}

void func_addr_destructor(void* ptr) {}

var nas_vec::get_val(const i32 n) {
    i32 size=elems.size();
    if (n<-size || n>=size) {
        return var::none();
    }
    return elems[n>=0?n:n+size];
}

var* nas_vec::get_mem(const i32 n) {
    i32 size=elems.size();
    if (n<-size || n>=size) {
        return nullptr;
    }
    return &elems[n>=0?n:n+size];
}

std::ostream& operator<<(std::ostream& out, nas_vec& vec) {
    if (!vec.elems.size() || vec.printed) {
        out<<(vec.elems.size()?"[..]":"[]");
        return out;
    }
    vec.printed=true;
    usize iter=0,size=vec.elems.size();
    out<<'[';
    for(auto& i:vec.elems) {
        out<<i<<",]"[(++iter)==size];
    }
    vec.printed=false;
    return out;
}

var nas_hash::get_val(const std::string& key) {
    if (elems.count(key)) {
        return elems.at(key);
    } else if (!elems.count("parents")) {
        return var::none();
    }
    var ret=var::none();
    var val=elems.at("parents");
    if (val.type!=vm_vec) {
        return ret;
    }
    for(auto& i:val.vec().elems) {
        if (i.type==vm_hash) {
            ret=i.hash().get_val(key);
        }
        if (ret.type!=vm_none) {
            return ret;
        }
    }
    return ret;
}

var* nas_hash::get_mem(const std::string& key) {
    if (elems.count(key)) {
        return &elems.at(key);
    } else if (!elems.count("parents")) {
        return nullptr;
    }
    var* addr=nullptr;
    var val=elems.at("parents");
    if (val.type!=vm_vec) {
        return addr;
    }
    for(auto& i:val.vec().elems) {
        if (i.type==vm_hash) {
            addr=i.hash().get_mem(key);
        }
        if (addr) {
            return addr;
        }
    }
    return addr;
}

std::ostream& operator<<(std::ostream& out, nas_hash& hash) {
    if (!hash.elems.size() || hash.printed) {
        out<<(hash.elems.size()?"{..}":"{}");
        return out;
    }
    hash.printed=true;
    usize iter=0,size=hash.elems.size();
    out<<'{';
    for(auto& i:hash.elems) {
        out<<i.first<<':'<<i.second<<",}"[(++iter)==size];
    }
    hash.printed=false;
    return out;
}

void nas_func::clear() {
    dpara=-1;
    local.clear();
    upval.clear();
    keys.clear();
}

void nas_ghost::set(usize t, void* p, ghost_register_table* table) {
    type=t;
    ptr=p;
    ghost_type_table=table;
}

void nas_ghost::clear() {
    if (!ptr) {
        return;
    }
    ghost_type_table->destructor(type)(ptr);
    ptr=nullptr;
}

void nas_co::clear() {
    for(u32 i=0;i<STACK_DEPTH;++i) {
        stack[i]=var::nil();
    }
    ctx.pc=0;
    ctx.localr=nullptr;
    ctx.memr=nullptr;
    ctx.canary=stack+STACK_DEPTH-1;
    ctx.top=stack;
    ctx.funcr=var::nil();
    ctx.upvalr=var::nil();
    ctx.stack=stack;

    status=coroutine_status::suspended;
}

nas_val::nas_val(u8 val_type) {
    mark=gc_status::collected;
    type=val_type;
    unmut=0;
    switch(val_type) {
        case vm_str: ptr.str=new std::string; break;
        case vm_vec: ptr.vec=new nas_vec; break;
        case vm_hash: ptr.hash=new nas_hash; break;
        case vm_func: ptr.func=new nas_func; break;
        case vm_upval: ptr.upval=new nas_upval; break;
        case vm_obj: ptr.obj=new nas_ghost; break;
        case vm_co: ptr.co=new nas_co; break;
    }
}

nas_val::~nas_val() {
    switch(type) {
        case vm_str:  delete ptr.str;  break;
        case vm_vec:  delete ptr.vec;  break;
        case vm_hash: delete ptr.hash; break;
        case vm_func: delete ptr.func; break;
        case vm_upval:delete ptr.upval;break;
        case vm_obj:  delete ptr.obj;  break;
        case vm_co:   delete ptr.co;   break;
    }
    type=vm_nil;
}

void nas_val::clear() {
    switch(type) {
        case vm_str:  ptr.str->clear();       break;
        case vm_vec:  ptr.vec->elems.clear(); break;
        case vm_hash: ptr.hash->elems.clear();break;
        case vm_func: ptr.func->clear();      break;
        case vm_upval:ptr.upval->clear();     break;
        case vm_obj:  ptr.obj->clear();       break;
        case vm_co:   ptr.co->clear();        break;
    }
}

f64 var::tonum() {
    return type!=vm_str? val.num:str2num(str().c_str());
}

std::string var::tostr() {
    if (type==vm_str) {
        return str();
    } else if (type==vm_num) {
        std::string tmp=std::to_string(num());
        tmp.erase(tmp.find_last_not_of('0')+1, std::string::npos);
        tmp.erase(tmp.find_last_not_of('.')+1, std::string::npos);
        return tmp;
    }
    return "";
}

std::ostream& operator<<(std::ostream& out, var& ref) {
    switch(ref.type) {
        case vm_none: out<<"undefined";   break;
        case vm_nil:  out<<"nil";         break;
        case vm_num:  out<<ref.val.num;   break;
        case vm_str:  out<<ref.str();     break;
        case vm_vec:  out<<ref.vec();     break;
        case vm_hash: out<<ref.hash();    break;
        case vm_func: out<<"func(..) {..}";break;
        case vm_obj:  out<<ref.obj();     break;
        case vm_co:   out<<"<coroutine>"; break;
    }
    return out;
}

bool var::objchk(usize obj_type) {
    return type==vm_obj && obj().type==obj_type && obj().ptr;
}

var var::none() {
    return {vm_none, (u32)0};
}

var var::nil() {
    return {vm_nil, (u32)0};
}

var var::ret(u32 pc) {
    return {vm_ret, pc};
}

var var::cnt(i64 n) {
    return {vm_cnt, n};
}

var var::num(f64 n) {
    return {vm_num, n};
}

var var::gcobj(nas_val* p) {
    return {p->type, p};
}

var var::addr(var* p) {
    return {vm_addr, p};
}

var* var::addr() {
    return val.addr;
}

u32 var::ret() {
    return val.ret;
}

i64& var::cnt() {
    return val.cnt;
}

f64 var::num() {
    return val.num;
}

std::string& var::str() {
    return *val.gcobj->ptr.str;
}

nas_vec& var::vec() {
    return *val.gcobj->ptr.vec;
}

nas_hash& var::hash() {
    return *val.gcobj->ptr.hash;
}

nas_func& var::func() {
    return *val.gcobj->ptr.func;
}

nas_upval& var::upval() {
    return *val.gcobj->ptr.upval;
}

nas_ghost& var::obj() {
    return *val.gcobj->ptr.obj;
}

nas_co& var::co() {
    return *val.gcobj->ptr.co;
}

void gc::mark() {
    std::vector<var> bfs;
    mark_context(bfs);
    
    while(!bfs.empty()) {
        var value=bfs.back();
        bfs.pop_back();
        if (value.type<=vm_num ||
            value.val.gcobj->mark!=gc_status::uncollected) {
            continue;
        }
        mark_var(bfs, value);
    }
}

void gc::mark_context(std::vector<var>& bfs_queue) {

    // scan now running context, this context maybe related to coroutine or main
    for(var* i=rctx->stack;i<=rctx->top;++i) {
        bfs_queue.push_back(*i);
    }
    bfs_queue.push_back(rctx->funcr);
    bfs_queue.push_back(rctx->upvalr);
    bfs_queue.push_back(temp);

    if (!cort) {
        return;
    }

    // coroutine is running, so scan main process stack from mctx
    for(var* i=mctx.stack;i<=mctx.top;++i) {
        bfs_queue.push_back(*i);
    }
    bfs_queue.push_back(mctx.funcr);
    bfs_queue.push_back(mctx.upvalr);
}

void gc::mark_var(std::vector<var>& bfs_queue, var& value) {
    value.val.gcobj->mark=gc_status::found;
    switch(value.type) {
        case vm_vec: mark_vec(bfs_queue, value.vec()); break;
        case vm_hash: mark_hash(bfs_queue, value.hash()); break;
        case vm_func: mark_func(bfs_queue, value.func()); break;
        case vm_upval: mark_upval(bfs_queue, value.upval()); break;
        case vm_co: mark_co(bfs_queue, value.co()); break;
        default: break;
    }
}

void gc::mark_vec(std::vector<var>& bfs_queue, nas_vec& vec) {
    for(auto& i:vec.elems) {
        bfs_queue.push_back(i);
    }
}

void gc::mark_hash(std::vector<var>& bfs_queue, nas_hash& hash) {
    for(auto& i:hash.elems) {
        bfs_queue.push_back(i.second);
    }
}

void gc::mark_func(std::vector<var>& bfs_queue, nas_func& function) {
    for(auto& i:function.local) {
        bfs_queue.push_back(i);
    }
    for(auto& i:function.upval) {
        bfs_queue.push_back(i);
    }
}

void gc::mark_upval(std::vector<var>& bfs_queue, nas_upval& upval) {
    for(auto& i:upval.elems) {
        bfs_queue.push_back(i);
    }
}

void gc::mark_co(std::vector<var>& bfs_queue, nas_co& co) {
    bfs_queue.push_back(co.ctx.funcr);
    bfs_queue.push_back(co.ctx.upvalr);
    for(var* i=co.stack;i<=co.ctx.top;++i) {
        bfs_queue.push_back(*i);
    }
}

void gc::sweep() {
    for(auto i:memory) {
        if (i->mark==gc_status::uncollected) {
            i->clear();
            unused[i->type-vm_str].push_back(i);
            i->mark=gc_status::collected;
        } else if (i->mark==gc_status::found) {
            i->mark=gc_status::uncollected;
        }
    }
}

void gc::extend(u8 type) {
    const u8 index=type-vm_str;
    size[index]+=incr[index];

    for(u32 i=0;i<incr[index];++i) {
        nas_val* tmp=new(std::nothrow) nas_val(type);

        if (!tmp) {
            std::cerr<<"nasal_gc.h: gc::extend: ";
            std::cerr<<"failed to allocate memory\n";
            std::exit(1);
        }

        // add to heap
        memory.push_back(tmp);
        unused[index].push_back(tmp);
    }

    incr[index]=incr[index]+incr[index]/2;
}

void gc::init(
    const std::vector<std::string>& s, const std::vector<std::string>& argv) {
    // initialize function register
    rctx->funcr=nil;
    worktime=0;

    // initialize counters
    for(u8 i=0;i<gc_type_size;++i) {
        size[i]=gcnt[i]=acnt[i]=0;
    }

    // coroutine pointer set to nullptr
    cort=nullptr;

    // init constant strings
    strs.resize(s.size());
    for(u32 i=0;i<strs.size();++i) {
        strs[i]=var::gcobj(new nas_val(vm_str));
        strs[i].val.gcobj->unmut=1;
        strs[i].str()=s[i];
    }

    // record arguments
    env_argv.resize(argv.size());
    for(usize i=0;i<argv.size();++i) {
        env_argv[i]=var::gcobj(new nas_val(vm_str));
        env_argv[i].val.gcobj->unmut=1;
        env_argv[i].str()=argv[i];
    }
}

void gc::clear() {
    for(auto i:memory) {
        delete i;
    }
    memory.clear();
    for(u8 i=0;i<gc_type_size;++i) {
        unused[i].clear();
    }
    for(auto& i:strs) {
        delete i.val.gcobj;
    }
    strs.clear();
    env_argv.clear();
}

void gc::info() {
    using std::left;
    using std::setw;
    using std::setfill;
    const char* name[]={"str  ","vec  ","hash ","func ","upval","obj  ","co   "};
    std::clog<<"\ngc info (gc count|alloc count|memory size)\n";

    usize ident=0;
    for(u8 i=0;i<gc_type_size;++i) {
#ifndef _MSC_VER
        usize len=std::max({
            std::to_string(gcnt[i]).length(),
            std::to_string(acnt[i]).length(),
            std::to_string(size[i]).length()
        });
#else // VS is a piece of shit
        usize len=std::to_string(gcnt[i]).length();
        ident=ident<len?len:ident;
        len=std::to_string(acnt[i]).length();
        ident=ident<len?len:ident;
        len=std::to_string(size[i]).length();
#endif
        ident=ident<len?len:ident;
    }

    double total=0;
    for(u8 i=0;i<gc_type_size;++i) {
        if (gcnt[i] || acnt[i] || size[i]) {
            total+=gcnt[i];
            std::clog<<" "<<name[i];
            std::clog<<" | "<<left<<setw(ident)<<setfill(' ')<<gcnt[i];
            std::clog<<" | "<<left<<setw(ident)<<setfill(' ')<<acnt[i];
            std::clog<<" | "<<left<<setw(ident)<<setfill(' ')<<size[i];
            std::clog<<"\n";
        }
    }

    double sec=worktime*1.0/1000000000; // seconds
    std::clog<<" time  | "<<(sec<0.1? sec*1000:sec)<<(sec<0.1? " ms\n":" s\n");
    if (total) {
        std::clog<<" avg   | "<<sec/total*1000<<" ms\n";
    }
    std::clog<<"\n";
}

var gc::alloc(u8 type) {
    using clk=std::chrono::high_resolution_clock;
    const u8 index=type-vm_str;
    ++acnt[index];
    if (unused[index].empty()) {
        ++gcnt[index];
        auto begin=clk::now();
        mark();
        sweep();
        worktime+=(clk::now()-begin).count();
    }
    if (unused[index].empty()) {
        extend(type);
    }
    var ret=var::gcobj(unused[index].back());
    ret.val.gcobj->mark=gc_status::uncollected;
    unused[index].pop_back();
    return ret;
}

void gc::ctxchg(nas_co& co) {
    // store running state to main context
    mctx=*rctx;

    // restore coroutine context state
    *rctx=co.ctx;

    // set coroutine pointer
    cort=&co;

    // set coroutine state to running
    cort->status=coroutine_status::running;
}

void gc::ctxreserve() {
    // pc=0 means this coroutine is finished
    cort->status=rctx->pc?
        coroutine_status::suspended:
        coroutine_status::dead;

    // store running state to coroutine
    cort->ctx=*rctx;

    // restore main context state
    *rctx=mctx;

    // set coroutine pointer to nullptr
    cort=nullptr;
}

var nas_err(const std::string& error_function_name, const std::string& info) {
    std::cerr<<"[vm] "<<error_function_name<<": "<<info<<"\n";
    return var::none();
}