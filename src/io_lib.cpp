#include "io_lib.h"

namespace nasal {

const auto file_type_name = "file";

void filehandle_destructor(void* ptr) {
    if ((FILE*)ptr==stdin) {
        return;
    }
    fclose((FILE*)ptr);
}

var builtin_readfile(var* local, gc& ngc) {
    var val = local[1];
    if (val.type!=vm_str) {
        return nas_err("io::readfile", "\"filename\" must be string");
    }
    std::ifstream in(val.str(), std::ios::binary);
    std::stringstream rd;
    if (!in.fail()) {
        rd << in.rdbuf();
    }
    return ngc.newstr(rd.str());
}

var builtin_fout(var* local, gc& ngc) {
    var val = local[1];
    var str = local[2];
    if (val.type!=vm_str) {
        return nas_err("io::fout", "\"filename\" must be string");
    }
    std::ofstream out(val.str());
    if (out.fail()) {
        return nas_err("io::fout", "cannot open <"+val.str()+">");
    }
    out << str;
    return nil;
}

var builtin_exists(var* local, gc& ngc) {
    if (local[1].type!=vm_str) {
        return zero;
    }
    return access(local[1].str().c_str(), F_OK)!=-1?one:zero;
}

var builtin_open(var* local, gc& ngc) {
    var name = local[1];
    var mode = local[2];
    if (name.type!=vm_str) {
        return nas_err("open", "\"filename\" must be string");
    }
    if (mode.type!=vm_str) {
        return nas_err("open", "\"mode\" must be string");
    }
    FILE* res = fopen(name.str().c_str(), mode.str().c_str());
    if (!res) {
        return nas_err("open", "failed to open file <"+name.str()+">");
    }
    var ret = ngc.alloc(vm_obj);
    ret.obj().set(file_type_name, filehandle_destructor, res);
    return ret;
}

var builtin_close(var* local, gc& ngc) {
    var fd = local[1];
    if (!fd.objchk(file_type_name)) {
        return nas_err("close", "not a valid filehandle");
    }
    fd.obj().clear();
    return nil;
}

var builtin_read(var* local, gc& ngc) {
    var fd = local[1];
    var buf = local[2];
    var len = local[3];
    if (!fd.objchk(file_type_name)) {
        return nas_err("read", "not a valid filehandle");
    }
    if (buf.type!=vm_str || buf.val.gcobj->unmut) {
        return nas_err("read", "\"buf\" must be mutable string");
    }
    if (len.type!=vm_num) {
        return nas_err("read", "\"len\" must be number");
    }
    if (len.num()<=0 || len.num()>=(1<<30)) {
        return nas_err("read", "\"len\" less than 1 or too large");
    }
    char* buff = new char[(usize)len.num()+1];
    if (!buff) {
        return nas_err("read", "malloc failed");
    }
    f64 res = fread(buff,1,len.num(), (FILE*)fd.obj().ptr);
    buf.str() = buff;
    buf.val.gcobj->unmut = true;
    delete []buff;
    return var::num(res);
}

var builtin_write(var* local, gc& ngc) {
    var fd = local[1];
    var str = local[2];
    if (!fd.objchk(file_type_name)) {
        return nas_err("write", "not a valid filehandle");
    }
    if (str.type!=vm_str) {
        return nas_err("write", "\"str\" must be string");
    }
    return var::num((f64)fwrite(str.str().c_str(), 1, str.str().length(), (FILE*)fd.obj().ptr));
}

var builtin_seek(var* local, gc& ngc) {
    var fd = local[1];
    var pos = local[2];
    var whence = local[3];
    if (!fd.objchk(file_type_name)) {
        return nas_err("seek", "not a valid filehandle");
    }
    return var::num((f64)fseek((FILE*)fd.obj().ptr, pos.num(), whence.num()));
}

var builtin_tell(var* local, gc& ngc) {
    var fd = local[1];
    if (!fd.objchk(file_type_name)) {
        return nas_err("tell", "not a valid filehandle");
    }
    return var::num((f64)ftell((FILE*)fd.obj().ptr));
}

var builtin_readln(var* local, gc& ngc) {
    var fd = local[1];
    if (!fd.objchk(file_type_name)) {
        return nas_err("readln", "not a valid filehandle");
    }
    var str = ngc.alloc(vm_str);
    char c;
    while((c = fgetc((FILE*)fd.obj().ptr))!=EOF) {
        if (c=='\r') {
            continue;
        }
        if (c=='\n') {
            return str;
        }
        str.str() += c;
    }
    if (str.str().length()) {
        return str;
    }
    return nil;
}

var builtin_stat(var* local, gc& ngc) {
    var name = local[1];
    if (name.type!=vm_str) {
        return nas_err("stat", "\"filename\" must be string");
    }
    struct stat buf;
    if (stat(name.str().c_str(),&buf)<0) {
        return nas_err("stat", "failed to open file <"+name.str()+">");
    }
    var ret = ngc.alloc(vm_vec);
    ret.vec().elems = {
        var::num((f64)buf.st_dev),
        var::num((f64)buf.st_ino),
        var::num((f64)buf.st_mode),
        var::num((f64)buf.st_nlink),
        var::num((f64)buf.st_uid),
        var::num((f64)buf.st_gid),
        var::num((f64)buf.st_rdev),
        var::num((f64)buf.st_size),
        var::num((f64)buf.st_atime),
        var::num((f64)buf.st_mtime),
        var::num((f64)buf.st_ctime)
    };
    return ret;
}

var builtin_eof(var* local, gc& ngc) {
    var fd = local[1];
    if (!fd.objchk(file_type_name)) {
        return nas_err("readln", "not a valid filehandle");
    }
    return var::num((f64)feof((FILE*)fd.obj().ptr));
}

nasal_builtin_table io_lib_native[] = {
    {"__readfile", builtin_readfile},
    {"__fout", builtin_fout},
    {"__exists", builtin_exists},
    {"__open", builtin_open},
    {"__close", builtin_close},
    {"__read", builtin_read},
    {"__write", builtin_write},
    {"__seek", builtin_seek},
    {"__tell", builtin_tell},
    {"__readln", builtin_readln},
    {"__stat", builtin_stat},
    {"__eof", builtin_eof},
    {nullptr, nullptr}
};

}
