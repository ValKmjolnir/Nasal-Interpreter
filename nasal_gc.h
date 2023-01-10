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

const u32 gc_tsize=vm_co-vm_str+1;

struct nas_vec;  // vector
struct nas_hash; // hashmap(dict)
struct nas_func; // function(lambda)
struct nas_upval;// upvalue
struct nas_obj;  // special objects
struct nas_co;   // coroutine
struct nas_val;  // nas_val includes gc-managed types

struct var {
    u8 type;
    union {
        u32 ret;
        i64 cnt;
        f64 num;
        var* addr;
        nas_val* gcobj;
    } val;

    var() = default;
    var(const var&) = default;
    bool operator==(const var& nr) const {return type==nr.type && val.gcobj==nr.val.gcobj;}
    bool operator!=(const var& nr) const {return type!=nr.type || val.gcobj!=nr.val.gcobj;}

    // number and string can be translated to each other
    f64 tonum();
    string tostr();
    friend std::ostream& operator<<(std::ostream&,var&);
    bool objchk(u32);

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

    nas_func():dpara(-1),entry(0),psize(0),lsize(0) {}
    void clear();
};

struct nas_upval {
    // if on stack, use these three variables
    bool onstk;
    u32 size;
    var* stk;

    // if not on stack, use this
    std::vector<var> elems;

    nas_upval() {onstk=true;stk=nullptr;size=0;}
    var& operator[](usize n) {return onstk? stk[n]:elems[n];}
    void clear() {onstk=true;elems.clear();size=0;}
};

struct nas_obj {
    enum obj:u32 {
        file=1,
        dir,
        dylib,
        faddr,
        unsafe
    };
    /* RAII constructor */
    /* new object is initialized when creating */
    u32 type;
    void* ptr;
private:
    void file_dtor() {
        fclose((FILE*)ptr);
    }
    void dir_dtor() {
#ifndef _MSC_VER
        closedir((DIR*)ptr);
#else
        FindClose(ptr);
#endif
    }
    void dylib_dtor() {
#ifdef _WIN32
        FreeLibrary((HMODULE)ptr);
#else
        dlclose(ptr);
#endif
    }
public:
    nas_obj():type(0),ptr(nullptr) {}
    ~nas_obj() {clear();}
    void set(u32 t=0,void* p=nullptr) {
        type=t;
        ptr=p;
    }
    void clear() {
        if (!ptr) {
            return;
        }
        switch(type) {
            case obj::file:  file_dtor(); break;
            case obj::dir:   dir_dtor();  break;
            case obj::dylib: dylib_dtor();break;
            default: break;
        }
        ptr=nullptr;
    }
};

struct nas_co {
    enum costat:u32{
        suspended,
        running,
        dead
    };

    // calculation stack
    var  stack[STACK_DEPTH];
    
    u32  pc;
    var* top;
    var* canary=stack+STACK_DEPTH-1;
    var* localr;
    var* memr;
    var  funcr;
    var  upvalr;

    u32 status;
    nas_co() {clear();}
    void clear() {
        for(u32 i=0;i<STACK_DEPTH;++i) {
            stack[i]=var::nil();
        }
        pc=0;
        localr=nullptr;
        memr=nullptr;
        top=stack;
        status=nas_co::suspended;
        funcr=var::nil();
        upvalr=var::nil();
    }
};

struct nas_val {
    enum status:u8 {
        uncollected=0,   
        collected,
        found
    };

    u8 mark; // mark if it is visited by gc or collected by gc
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

std::ostream& operator<<(std::ostream& out,nas_vec& vec) {
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
    } else if (elems.count("parents")) {
        var ret=var::none();
        var val=elems["parents"];
        if (val.type==vm_vec) {
            for(auto& i:val.vec().elems) {
                if (i.type==vm_hash) {
                    ret=i.hash().get_val(key);
                }
                if (ret.type!=vm_none) {
                    return ret;
                }
            }
        }
    }
    return var::none();
}

var* nas_hash::get_mem(const string& key) {
    if (elems.count(key)) {
        return &elems.at(key);
    } else if (elems.count("parents")) {
        var* addr=nullptr;
        var val=elems["parents"];
        if (val.type==vm_vec) {
            for(auto& i:val.vec().elems) {
                if (i.type==vm_hash) {
                    addr=i.hash().get_mem(key);
                }
                if (addr) {
                    return addr;
                }
            }
        }
    }
    return nullptr;
}

std::ostream& operator<<(std::ostream& out,nas_hash& hash) {
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

nas_val::nas_val(u8 val_type) {
    mark=status::collected;
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
        tmp.erase(tmp.find_last_not_of('0')+1,string::npos);
        tmp.erase(tmp.find_last_not_of('.')+1,string::npos);
        return tmp;
    }
    return "";
}

std::ostream& operator<<(std::ostream& out,var& ref) {
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

bool var::objchk(u32 objtype) {
    return type==vm_obj && obj().type==objtype && obj().ptr;
}

var var::none() {
    var t;
    t.type=vm_none;
    return t;
}

var var::nil() {
    var t;
    t.type=vm_nil;
    t.val.num=0;
    return t;
}

var var::ret(u32 pc) {
    var t;
    t.type=vm_ret;
    t.val.ret=pc;
    return t;
}

var var::cnt(i64 n) {
    var t;
    t.type=vm_cnt;
    t.val.cnt=n;
    return t;
}

var var::num(f64 n) {
    var t;
    t.type=vm_num;
    t.val.num=n;
    return t;
}

var var::gcobj(nas_val* p) {
    var t;
    t.type=p->type; // use nas_val::type directly
    t.val.gcobj=p;
    return t;
}

var var::addr(var* p) {
    var t;
    t.type=vm_addr;
    t.val.addr=p;
    return t;
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
    /* main context */
    struct {
        u32  pc;
        var* top;
        var* localr;
        var* memr;
        var  funcr;
        var  upvalr;
        var* canary;
        var* stack;
    } mctx;
    
    /* runtime context */
    u32&  pc;     // program counter
    var*& localr; // local scope register
    var*& memr;   // used for mem_call
    var&  funcr;  // function register
    var&  upvalr; // upvalue register
    var*& canary; // avoid stackoverflow
    var*& top;    // stack top
    var*  stack;  // stack pointer
    nas_co* cort; // running coroutine

    /* native function used */
    var temp; // temporary place used in builtin/module functions

    /* constants and memory pool */
    std::vector<var> strs;        // reserved address for const vm_str
    std::vector<var> env_argv;    // command line arguments
    std::vector<nas_val*> memory; // gc memory
    std::vector<nas_val*> unused[gc_tsize]; // gc free list

    // heap increase size
    u32 incr[gc_tsize]={
        128, // vm_str
        128, // vm_vec
        64,  // vm_hash
        128, // vm_func
        256, // vm_upval
        16,  // vm_obj
        16   // vm_co
    };

    /* values for analysis */
    u64 size[gc_tsize];
    u64 gcnt[gc_tsize];
    u64 acnt[gc_tsize];
    i64 worktime;

    gc(u32& _pc, var*& _localr, var*& _memr, var& _funcr,
       var& _upvalr, var*& _canary, var*& _top, var* _stk):
        pc(_pc),localr(_localr),memr(_memr),funcr(_funcr),upvalr(_upvalr),
        canary(_canary),top(_top),stack(_stk),cort(nullptr),temp(nil),
        worktime(0) {}
    void mark();
    void sweep();
    void extend(u8);
    void init(const std::vector<string>&,const std::vector<string>&);
    void clear();
    void info();
    var alloc(const u8);
    var newstr(char);
    var newstr(const char*);
    var newstr(const string&);
    void ctxchg(nas_co&);
    void ctxreserve();
};

/* gc functions */
void gc::mark() {
    std::vector<var> bfs;
    // scan coroutine process stack when coroutine ptr is not null
    // scan main process stack when coroutine ptr is null
    // this scan process must execute because when running coroutine,
    // the nas_co related to it will not update it's context(like `top`) until the coroutine suspends or exits.
    for(var* i=stack;i<=top;++i) {
        bfs.push_back(*i);
    }
    bfs.push_back(funcr);
    bfs.push_back(upvalr);
    bfs.push_back(temp);

    // if coroutine is running, scan main process stack from mctx
    if (cort) {
        for(var* i=mctx.stack;i<=mctx.top;++i) {
            bfs.push_back(*i);
        }
        bfs.push_back(mctx.funcr);
        bfs.push_back(mctx.upvalr);
    }
    
    while(!bfs.empty()) {
        var tmp=bfs.back();
        bfs.pop_back();
        if (tmp.type<=vm_num || tmp.val.gcobj->mark) {
            continue;
        }
        tmp.val.gcobj->mark=nas_val::status::found;
        switch(tmp.type) {
            case vm_vec:
                for(auto& i:tmp.vec().elems) {
                    bfs.push_back(i);
                }
                break;
            case vm_hash:
                for(auto& i:tmp.hash().elems) {
                    bfs.push_back(i.second);
                }
                break;
            case vm_func:
                for(auto& i:tmp.func().local) {
                    bfs.push_back(i);
                }
                for(auto& i:tmp.func().upval) {
                    bfs.push_back(i);
                }
                break;
            case vm_upval:
                for(auto& i:tmp.upval().elems) {
                    bfs.push_back(i);
                }
                break;
            case vm_co:
                bfs.push_back(tmp.co().funcr);
                bfs.push_back(tmp.co().upvalr);
                for(var* i=tmp.co().stack;i<=tmp.co().top;++i) {
                    bfs.push_back(*i);
                }
                break;
        }
    }
}

void gc::sweep() {
    for(auto i:memory) {
        if (i->mark==nas_val::status::uncollected) {
            i->clear();
            unused[i->type-vm_str].push_back(i);
            i->mark=nas_val::status::collected;
        } else if (i->mark==nas_val::status::found) {
            i->mark=nas_val::status::uncollected;
        }
    }
}

void gc::extend(u8 type) {
    u8 index=type-vm_str;
    size[index]+=incr[index];
    for(u32 i=0;i<incr[index];++i) {
        nas_val* tmp=new nas_val(type);

        // failed to allocate new memory
        if (!tmp) {
            std::exit(-1);
        }

        // add to heap
        memory.push_back(tmp);
        unused[index].push_back(tmp);
    }
    incr[index]*=2;
}

void gc::init(const std::vector<string>& s,const std::vector<string>& argv) {
    // initialize function register
    funcr=nil;
    worktime=0;

    // initialize counters
    for(u8 i=0;i<gc_tsize;++i) {
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
    for(u8 i=0;i<gc_tsize;++i) {
        unused[i].clear();
    }
    for(auto& i:strs) {
        delete i.val.gcobj;
    }
    strs.clear();
    env_argv.clear();
}

void gc::info() {
    const char* name[]={"str  ","vec  ","hash ","func ","upval","obj  ","co   "};
    std::clog<<"\ngarbage collector info (gc count|alloc count|memory size)\n";
    u32 maxlen=0;
    for(u8 i=0;i<gc_tsize;++i) {
        u32 len=std::to_string(gcnt[i]).length();
        maxlen=maxlen<len?len:maxlen;
        len=std::to_string(acnt[i]).length();
        maxlen=maxlen<len?len:maxlen;
        len=std::to_string(size[i]).length();
        maxlen=maxlen<len?len:maxlen;
    }
    double total=0;
    for(u8 i=0;i<gc_tsize;++i) {
        if (gcnt[i] || acnt[i] || size[i]) {
            total+=gcnt[i];
            std::clog<<" "<<name[i]<<" | "<<std::left<<std::setw(maxlen)<<std::setfill(' ')<<gcnt[i];
            std::clog<<" | "<<std::left<<std::setw(maxlen)<<std::setfill(' ')<<acnt[i];
            std::clog<<" | "<<std::left<<std::setw(maxlen)<<std::setfill(' ')<<size[i]<<"\n";
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
    const u8 index=type-vm_str;
    ++acnt[index];
    if (unused[index].empty()) {
        ++gcnt[index];
        auto begin=std::chrono::high_resolution_clock::now();
        mark();
        sweep();
        worktime+=(std::chrono::high_resolution_clock::now()-begin).count();
    }
    if (unused[index].empty()) {
        extend(type);
    }
    var ret=var::gcobj(unused[index].back());
    ret.val.gcobj->mark=nas_val::status::uncollected;
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

void gc::ctxchg(nas_co& ctx) {
    // store running state to main context
    mctx.pc=pc;
    mctx.top=top;
    mctx.localr=localr;
    mctx.memr=memr;
    mctx.funcr=funcr;
    mctx.upvalr=upvalr;
    mctx.canary=canary;
    mctx.stack=stack;

    // restore coroutine context state
    pc=ctx.pc;
    top=ctx.top;
    localr=ctx.localr;
    memr=ctx.memr;
    funcr=ctx.funcr;
    upvalr=ctx.upvalr;
    canary=ctx.canary;
    stack=ctx.stack;

    // set coroutine pointer
    cort=&ctx;

    // set coroutine state to running
    cort->status=nas_co::running;
}

void gc::ctxreserve() {
    // pc=0 means this coroutine is finished
    cort->status=pc? nas_co::suspended:nas_co::dead;

    // store running state to coroutine
    cort->pc=pc;
    cort->localr=localr;
    cort->memr=memr;
    cort->funcr=funcr;
    cort->upvalr=upvalr;
    cort->canary=canary;
    cort->top=top;

    // restore main context state
    pc=mctx.pc;
    localr=mctx.localr;
    memr=mctx.memr;
    funcr=mctx.funcr;
    upvalr=mctx.upvalr;
    canary=mctx.canary;
    top=mctx.top;
    stack=mctx.stack;

    // set coroutine pointer to nullptr
    cort=nullptr;
}

// use to print error log and return error value
var nas_err(const string& err_f,const string& info) {
    std::cerr<<"[vm] "<<err_f<<": "<<info<<"\n";
    return var::none();
}

typedef var (*mod)(var*,usize,gc*); // module function type
typedef struct {
    const char* name;
    mod fd;
} mod_func; // module function stores in tables with this type, end with {nullptr,nullptr}
typedef mod_func* (*getptr)(); // module function "get" type
