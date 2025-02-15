#include "natives/io_lib.h"
#include "util/fs.h"

#include <fstream>
#include <sys/stat.h>

namespace nasal {

const auto file_type_name = "nasal::FILE";

void filehandle_destructor(void* ptr) {
    fclose(static_cast<FILE*>(ptr));
}

var builtin_readfile(context* ctx, gc* ngc) {
    auto filename = ctx->localr[1];
    if (!filename.is_str()) {
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
    if (!filename.is_str()) {
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
    if (!filename.is_str()) {
        return zero;
    }
    return fs::exists(filename.str())? one:zero;
}

var builtin_open(context* ctx, gc* ngc) {
    auto local = ctx->localr;
    auto name = local[1];
    auto mode = local[2];
    if (!name.is_str()) {
        return nas_err("io::open", "\"filename\" must be string");
    }
    if (!mode.is_str()) {
        return nas_err("io::open", "\"mode\" must be string");
    }
    auto file_descriptor = fopen(name.str().c_str(), mode.str().c_str());
    // if failed to open, just return nil for check
    if (!file_descriptor) {
        return nil;
    }
    var return_object = ngc->alloc(vm_type::vm_ghost);
    return_object.ghost().set(
        file_type_name, filehandle_destructor, nullptr, file_descriptor
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
    if (!buffer.is_str() || buffer.val.gcobj->immutable) {
        return nas_err("io::read", "\"buf\" must be mutable string");
    }
    if (!length.is_num()) {
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
        file_descriptor.ghost().get<FILE>()
    );
    buffer.str() = temp_buffer;
    buffer.val.gcobj->immutable = true;
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
    if (!source.is_str()) {
        return nas_err("io::write", "\"str\" must be string");
    }
    return var::num(static_cast<f64>(fwrite(
        source.str().c_str(), 1, source.str().length(),
        file_descriptor.ghost().get<FILE>()
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
        file_descriptor.ghost().get<FILE>(),
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
        ftell(file_descriptor.ghost().get<FILE>())
    ));
}

var builtin_readln(context* ctx, gc* ngc) {
    auto file_descriptor = ctx->localr[1];
    if (!file_descriptor.object_check(file_type_name)) {
        return nas_err("io::readln", "not a valid filehandle");
    }
    auto result = ngc->alloc(vm_type::vm_str);
    char c;
    while((c = fgetc(file_descriptor.ghost().get<FILE>()))!=EOF) {
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
    if (!name.is_str()) {
        return nas_err("io::stat", "\"filename\" must be string");
    }
    struct stat buffer;
    // if failed to stat, return nil
    if (stat(name.str().c_str(), &buffer) < 0) {
        return nil;
    }
    auto result = ngc->alloc(vm_type::vm_vec);
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
        feof(file_descriptor.ghost().get<FILE>())
    ));
}

var builtin_stdin(context* ctx, gc* ngc) {
    auto file_descriptor = ngc->alloc(vm_type::vm_ghost);
    file_descriptor.ghost().set(file_type_name, nullptr, nullptr, stdin);
    return file_descriptor;
}

var builtin_stdout(context* ctx, gc* ngc) {
    auto file_descriptor = ngc->alloc(vm_type::vm_ghost);
    file_descriptor.ghost().set(file_type_name, nullptr, nullptr, stdout);
    return file_descriptor;
}

var builtin_stderr(context* ctx, gc* ngc) {
    auto file_descriptor = ngc->alloc(vm_type::vm_ghost);
    file_descriptor.ghost().set(file_type_name, nullptr, nullptr, stderr);
    return file_descriptor;
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
    {"__stdin", builtin_stdin},
    {"__stdout", builtin_stdout},
    {"__stderr", builtin_stderr},
    {nullptr, nullptr}
};

}
