#pragma once

#include "nasal.h"
#include "nasal_gc.h"
#include "natives/builtin.h"

#ifndef _MSC_VER
#include <unistd.h>
#else
#include <io.h>
#endif

namespace nasal {

void filehandle_destructor(void*);

var builtin_readfile(context*, gc*);
var builtin_fout(context*, gc*);
var builtin_exists(context*, gc*);
var builtin_open(context*, gc*);
var builtin_close(context*, gc*);
var builtin_read(context*, gc*);
var builtin_write(context*, gc*);
var builtin_seek(context*, gc*);
var builtin_tell(context*, gc*);
var builtin_readln(context*, gc*);
var builtin_stat(context*, gc*);
var builtin_eof(context*, gc*);
var builtin_stdin(context*, gc*);
var builtin_stdout(context*, gc*);
var builtin_stderr(context*, gc*);

extern nasal_builtin_table io_lib_native[];

}
