#pragma once

#include "nasal.h"
#include "nasal_gc.h"
#include "nasal_builtin.h"

#include <sys/stat.h>

#ifndef _MSC_VER
#include <unistd.h>
#else
#include <io.h>
#endif

#ifdef _MSC_VER
#define F_OK 0 // fuck msc
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

extern nasal_builtin_table io_lib_native[];

}
