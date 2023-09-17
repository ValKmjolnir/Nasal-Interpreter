#pragma once

#include "nasal.h"
#include "nasal_gc.h"
#include "nasal_builtin.h"

#include <sys/stat.h>

#ifdef _MSC_VER
#define F_OK 0 // fuck msc
#endif

namespace nasal {

void filehandle_destructor(void*);

var builtin_readfile(var*, gc&);
var builtin_fout(var*, gc&);
var builtin_exists(var*, gc&);
var builtin_open(var*, gc&);
var builtin_close(var*, gc&);
var builtin_read(var*, gc&);
var builtin_write(var*, gc&);
var builtin_seek(var*, gc&);
var builtin_tell(var*, gc&);
var builtin_readln(var*, gc&);
var builtin_stat(var*, gc&);
var builtin_eof(var*, gc&);

extern nasal_builtin_table io_lib_native[];

}
