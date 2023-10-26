#include "io_lib.h"

namespace nasal {

const auto file_type_name = "file";

void filehandle_destructor(void* ptr) {
    if (static_cast<FILE*>(ptr)==stdin) {
        return;
    }
    fclose(static_cast<FILE*>(ptr));
}

var builtin_readfile(context* ctx, gc* ngc) {
    auto filename = ctx->localr[1];
    if (filename.type!=vm_str) {
        return nas_err("io::readfile", "\"filename\" must be string");
    }
    std::ifstream in(filename.str(), std::ios::binary);
    std::stringstream rd;
    if (!in.fail()) {
        rd << in.rdbuf();
    }
    return ngc->newstr(rd.str());
}

var builtin_fout(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    auto filename = local[1];
    auto source = local[2];
    if (filename.type!=vm_str) {
        return nas_err("io::fout", "\"filename\" must be string");
    }
    std::ofstream out(filename.str());
    if (out.fail()) {
        return nas_err("io::fout", "cannot open <" + filename.str() + ">");
    }
    out << source;
    return nil;
}

var builtin_exists(context* ctx, gc* ngc) {
    auto filename = ctx->localr[1];
    if (filename.type!=vm_str) {
        return zero;
    }
    return access(filename.str().c_str(), F_OK)!=-1? one:zero;
}

var builtin_open(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    auto name = local[1];
    auto mode = local[2];
    if (name.type!=vm_str) {
        return nas_err("io::open", "\"filename\" must be string");
    }
    if (mode.type!=vm_str) {
        return nas_err("io::open", "\"mode\" must be string");
    }
    auto file_descriptor = fopen(name.str().c_str(), mode.str().c_str());
    if (!file_descriptor) {
        return nas_err("io::open", "failed to open file <" + name.str() + ">");
    }
    var return_object = ngc->alloc(vm_obj);
    return_object.ghost().set(
        file_type_name, filehandle_destructor, file_descriptor
    );
    return return_object;
}

var builtin_close(context* ctx, gc* ngc) {
    var file_descriptor = ctx->localr[1];
    if (!file_descriptor.object_check(file_type_name)) {
        return nas_err("io::close", "not a valid filehandle");
    }
    file_descriptor.ghost().clear();
    return nil;
}

var builtin_read(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    auto file_descriptor = local[1];
    auto buffer = local[2];
    auto length = local[3];
    if (!file_descriptor.object_check(file_type_name)) {
        return nas_err("io::read", "not a valid filehandle");
    }
    if (buffer.type!=vm_str || buffer.val.gcobj->unmutable) {
        return nas_err("io::read", "\"buf\" must be mutable string");
    }
    if (length.type!=vm_num) {
        return nas_err("io::read", "\"len\" must be number");
    }
    if (length.num()<=0 || length.num()>=(1<<30)) {
        return nas_err("io::read", "\"len\" less than 1 or too large");
    }
    auto temp_buffer = new char[static_cast<usize>(length.num())+1];
    if (!temp_buffer) {
        return nas_err("io::read", "malloc failed");
    }
    auto read_size = fread(
        temp_buffer, 1, length.num(),
        static_cast<FILE*>(file_descriptor.ghost().pointer)
    );
    buffer.str() = temp_buffer;
    buffer.val.gcobj->unmutable = true;
    delete []temp_buffer;
    return var::num(read_size);
}

var builtin_write(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    auto file_descriptor = local[1];
    auto source = local[2];
    if (!file_descriptor.object_check(file_type_name)) {
        return nas_err("io::write", "not a valid filehandle");
    }
    if (source.type!=vm_str) {
        return nas_err("io::write", "\"str\" must be string");
    }
    return var::num(static_cast<f64>(fwrite(
        source.str().c_str(), 1, source.str().length(),
        static_cast<FILE*>(file_descriptor.ghost().pointer)
    )));
}

var builtin_seek(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    auto file_descriptor = local[1];
    auto position = local[2];
    auto whence = local[3];
    if (!file_descriptor.object_check(file_type_name)) {
        return nas_err("io::seek", "not a valid filehandle");
    }
    return var::num(static_cast<f64>(fseek(
        static_cast<FILE*>(file_descriptor.ghost().pointer),
        position.num(),
        whence.num()
    )));
}

var builtin_tell(context* ctx, gc* ngc) {
    auto file_descriptor = ctx->localr[1];
    if (!file_descriptor.object_check(file_type_name)) {
        return nas_err("io::tell", "not a valid filehandle");
    }
    return var::num(static_cast<f64>(
        ftell(static_cast<FILE*>(file_descriptor.ghost().pointer))
    ));
}

var builtin_readln(context* ctx, gc* ngc) {
    auto file_descriptor = ctx->localr[1];
    if (!file_descriptor.object_check(file_type_name)) {
        return nas_err("io::readln", "not a valid filehandle");
    }
    auto result = ngc->alloc(vm_str);
    char c;
    while((c = fgetc(static_cast<FILE*>(file_descriptor.ghost().pointer)))!=EOF) {
        if (c=='\r') {
            continue;
        }
        if (c=='\n') {
            return result;
        }
        result.str().push_back(c);
    }
    if (result.str().length()) {
        return result;
    }
    return nil;
}

var builtin_stat(context* ctx, gc* ngc) {
    auto name = ctx->localr[1];
    if (name.type!=vm_str) {
        return nas_err("io::stat", "\"filename\" must be string");
    }
    struct stat buffer;
    if (stat(name.str().c_str(), &buffer)<0) {
        return nas_err("io::stat", "failed to open file <" + name.str() + ">");
    }
    auto result = ngc->alloc(vm_vec);
    result.vec().elems = {
        var::num(static_cast<f64>(buffer.st_dev)),
        var::num(static_cast<f64>(buffer.st_ino)),
        var::num(static_cast<f64>(buffer.st_mode)),
        var::num(static_cast<f64>(buffer.st_nlink)),
        var::num(static_cast<f64>(buffer.st_uid)),
        var::num(static_cast<f64>(buffer.st_gid)),
        var::num(static_cast<f64>(buffer.st_rdev)),
        var::num(static_cast<f64>(buffer.st_size)),
        var::num(static_cast<f64>(buffer.st_atime)),
        var::num(static_cast<f64>(buffer.st_mtime)),
        var::num(static_cast<f64>(buffer.st_ctime))
    };
    return result;
}

var builtin_eof(context* ctx, gc* ngc) {
    auto file_descriptor = ctx->localr[1];
    if (!file_descriptor.object_check(file_type_name)) {
        return nas_err("io::readln", "not a valid filehandle");
    }
    return var::num(static_cast<f64>(
        feof(static_cast<FILE*>(file_descriptor.ghost().pointer))
    ));
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
