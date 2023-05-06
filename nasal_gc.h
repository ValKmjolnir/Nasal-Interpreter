#pragma once

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

#include "nasal.h"
#include "nasal_err.h"

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

enum class obj_type:u32 {
    null=0,
    file=1,
    dir,
    dylib,
    faddr
};

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

struct nas_vec;  // vector
struct nas_hash; // hashmap(dict)
struct nas_func; // function(lambda)
struct nas_upval;// upvalue
struct nas_obj;  // special objects
struct nas_co;   // coroutine
struct nas_val;  // nas_val includes gc-managed types

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
    bool operator==(const var& nr) const {return type==nr.type && val.gcobj==nr.val.gcobj;}
    bool operator!=(const var& nr) const {return type!=nr.type || val.gcobj!=nr.val.gcobj;}
    friend std::ostream& operator<<(std::ostream&, var&);

    // number and string can be translated to each other
    f64 tonum();
    string tostr();
    bool objchk(obj_type);

    // create new var object
    static var none();
    static var nil();
    static var ret(u32);
    static var cnt(i64);
    static var num(f64);
    static var gcobj(nas_val*);
    static var addr(var*);

    // get content
    var*       addr();
    u32        ret ();
    i64&       cnt ();
    f64        num ();
    string&    str ();
    nas_vec&   vec ();
    nas_hash&  hash();
    nas_func&  func();
    nas_upval& upval();
    nas_obj&   obj ();
    nas_co&    co  ();
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
    std::unordered_map<string,var> elems;

    // mark if this is printed, avoid stackoverflow
    bool printed;

    nas_hash():printed(false) {}
    usize size() const {return elems.size();}
    var get_val(const string&);
    var* get_mem(const string&);
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
    /* on stack, use these variables */
    bool onstk;
    u32 size;
    var* stk;

    /* not on stack, use this */
    std::vector<var> elems;

    nas_upval() {onstk=true;stk=nullptr;size=0;}
    var& operator[](usize n) {return onstk? stk[n]:elems[n];}
    void clear() {onstk=true;elems.clear();size=0;}
};

struct nas_obj {
    obj_type type;
    void* ptr;

private:
    /* RAII constructor, new object is initialized when creating */
    void file_dtor();
    void dir_dtor();
    void dylib_dtor();

public:
    nas_obj(): type(obj_type::null), ptr(nullptr) {}
    ~nas_obj() {clear();}
    void set(obj_type, void*);
    void clear();
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
        string*    str;
        nas_vec*   vec;
        nas_hash*  hash;
        nas_func*  func;
        nas_upval* upval;
        nas_obj*   obj;
        nas_co*    co;
    } ptr;

    nas_val(u8);
    ~nas_val();
    void clear();
};

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

var nas_hash::get_val(const string& key) {
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

var* nas_hash::get_mem(const string& key) {
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

void nas_obj::set(obj_type t, void* p) {
    type=t;
    ptr=p;
}

void nas_obj::clear() {
    if (!ptr) {
        return;
    }
    switch(type) {
        case obj_type::file:  file_dtor(); break;
        case obj_type::dir:   dir_dtor();  break;
        case obj_type::dylib: dylib_dtor();break;
        default: break;
    }
    ptr=nullptr;
}

void nas_obj::file_dtor() {
    if ((FILE*)ptr==stdin) {
        return;
    }
    fclose((FILE*)ptr);
}

void nas_obj::dir_dtor() {
#ifndef _MSC_VER
    closedir((DIR*)ptr);
#else
    FindClose(ptr);
#endif
}

void nas_obj::dylib_dtor() {
#ifdef _WIN32
    FreeLibrary((HMODULE)ptr);
#else
    dlclose(ptr);
#endif
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
        case vm_str:  ptr.str=new string;     break;
        case vm_vec:  ptr.vec=new nas_vec;    break;
        case vm_hash: ptr.hash=new nas_hash;  break;
        case vm_func: ptr.func=new nas_func;  break;
        case vm_upval:ptr.upval=new nas_upval;break;
        case vm_obj:  ptr.obj=new nas_obj;    break;
        case vm_co:   ptr.co=new nas_co;      break;
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

string var::tostr() {
    if (type==vm_str) {
        return str();
    } else if (type==vm_num) {
        string tmp=std::to_string(num());
        tmp.erase(tmp.find_last_not_of('0')+1, string::npos);
        tmp.erase(tmp.find_last_not_of('.')+1, string::npos);
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
        case vm_obj:  out<<"<object>";    break;
        case vm_co:   out<<"<coroutine>"; break;
    }
    return out;
}

bool var::objchk(obj_type objtype) {
    return type==vm_obj && obj().type==objtype && obj().ptr;
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

var*       var::addr () {return val.addr;             }
u32        var::ret  () {return val.ret;              }
i64&       var::cnt  () {return val.cnt;              }
f64        var::num  () {return val.num;              }
string&    var::str  () {return *val.gcobj->ptr.str;  }
nas_vec&   var::vec  () {return *val.gcobj->ptr.vec;  }
nas_hash&  var::hash () {return *val.gcobj->ptr.hash; }
nas_func&  var::func () {return *val.gcobj->ptr.func; }
nas_upval& var::upval() {return *val.gcobj->ptr.upval;}
nas_obj&   var::obj  () {return *val.gcobj->ptr.obj;  }
nas_co&    var::co   () {return *val.gcobj->ptr.co;   }

const var zero=var::num(0);
const var one =var::num(1);
const var nil =var::nil();

struct gc {
    /* main context temporary storage */
    context mctx;

    /* runtime context */
    context* rctx;
    nas_co* cort=nullptr; // running coroutine

    /*  temporary space used in builtin/module functions */
    var temp=nil;

    /* constants and memory pool */
    std::vector<var> strs;        // reserved address for const vm_str
    std::vector<var> env_argv;    // command line arguments
    std::vector<nas_val*> memory; // gc memory
    std::vector<nas_val*> unused[gc_type_size]; // gc free list

    /* heap increase size */
    u32 incr[gc_type_size]={
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
    i64 worktime=0;

    gc(context* _ctx): rctx(_ctx) {}

private:
    /* gc functions */
    void mark();
    void mark_context(std::vector<var>&);
    void mark_var(std::vector<var>&, var&);
    inline void mark_vec(std::vector<var>&, nas_vec&);
    inline void mark_hash(std::vector<var>&, nas_hash&);
    inline void mark_func(std::vector<var>&, nas_func&);
    inline void mark_upval(std::vector<var>&, nas_upval&);
    inline void mark_co(std::vector<var>&, nas_co&);
    void sweep();

public:
    void extend(u8);
    void init(const std::vector<string>&, const std::vector<string>&);
    void clear();
    void info();
    var alloc(const u8);
    var newstr(char);
    var newstr(const char*);
    var newstr(const string&);
    void ctxchg(nas_co&);
    void ctxreserve();
};

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
    u8 index=type-vm_str;
    size[index]+=incr[index];

    for(u32 i=0;i<incr[index];++i) {
        nas_val* tmp=new nas_val(type);

        if (!tmp) {
            std::cerr<<"failed to allocate new memory\n";
            std::exit(-1);
        }

        // add to heap
        memory.push_back(tmp);
        unused[index].push_back(tmp);
    }

    incr[index]*=2;
}

void gc::init(const std::vector<string>& s, const std::vector<string>& argv) {
    // initialize function register
    rctx->funcr=nil;
    worktime=0;

    // initialize counters
    for(u8 i=0;i<gc_type_size;++i) {
        size[i]=gcnt[i]=acnt[i]=0;
    }

    // coroutine pointer set to nullpre
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

var gc::newstr(char c) {
    var s=alloc(vm_str);
    s.str()=c;
    return s;
}

var gc::newstr(const char* buff) {
    var s=alloc(vm_str);
    s.str()=buff;
    return s;
}

var gc::newstr(const string& buff) {
    var s=alloc(vm_str);
    s.str()=buff;
    return s;
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

// use to print error log and return error value
var nas_err(const string& err_f, const string& info) {
    std::cerr<<"[vm] "<<err_f<<": "<<info<<"\n";
    return var::none();
}

// module function type
typedef var (*mod)(var*, usize, gc*);

// module function stores in tables with this type, end with {nullptr,nullptr}
struct mod_func {
    const char* name;
    mod fd;
};

// module function "get" type
typedef mod_func* (*getptr)();
