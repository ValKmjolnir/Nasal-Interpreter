#pragma once

#include "nasal_new_header.h"
#include "nasal_new_gc.h"

#ifndef _MSC_VER
#include <unistd.h>
#include <dirent.h>
#else
#pragma warning (disable:4566) // i know i'm using utf-8, fuck you
#pragma warning (disable:4244)
#pragma warning (disable:4267)
#pragma warning (disable:4996)
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1
#include <io.h>
#include <direct.h>
#endif

#include <sstream>
#include <cmath>
#include <thread>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <sys/wait.h>
#endif

#if defined __APPLE__
#include <crt_externs.h>
#define environ (*_NSGetEnviron())
#endif

var builtin_print(var*, gc&);
var builtin_println(var*, gc&);
var builtin_exit(var*, gc&);
var builtin_abort(var*, gc&);
var builtin_append(var*, gc&);
var builtin_setsize(var*, gc&);
var builtin_system(var*, gc&);
var builtin_input(var*, gc&);
var builtin_readfile(var*, gc&);
var builtin_fout(var*, gc&);
var builtin_split(var*, gc&);
var builtin_rand(var*, gc&);
var builtin_id(var*, gc&);
var builtin_int(var*, gc&);
var builtin_floor(var*, gc&);
var builtin_num(var*, gc&);
var builtin_pop(var*, gc&);
var builtin_str(var*, gc&);
var builtin_size(var*, gc&);
var builtin_u32xor(var*, gc&);
var builtin_u32and(var*, gc&);
var builtin_u32or(var*, gc&);
var builtin_u32nand(var*, gc&);
var builtin_u32not(var*, gc&);
var builtin_pow(var*, gc&);
var builtin_sin(var*, gc&);
var builtin_cos(var*, gc&);
var builtin_tan(var*, gc&);
var builtin_exp(var*, gc&);
var builtin_lg(var*, gc&);
var builtin_ln(var*, gc&);
var builtin_sqrt(var*, gc&);
var builtin_atan2(var*, gc&);
var builtin_isnan(var*, gc&);
var builtin_time(var*, gc&);
var builtin_contains(var*, gc&);
var builtin_delete(var*, gc&);
var builtin_keys(var*, gc&);
var builtin_die(var*, gc&);
var builtin_find(var*, gc&);
var builtin_type(var*, gc&);
var builtin_substr(var*, gc&);
var builtin_streq(var*, gc&);
var builtin_left(var*, gc&);
var builtin_right(var*, gc&);
var builtin_cmp(var*, gc&);
var builtin_chr(var*, gc&);
var builtin_char(var*, gc&);
var builtin_values(var*, gc&);
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
var builtin_fld(var*, gc&);
var builtin_sfld(var*, gc&);
var builtin_setfld(var*, gc&);
var builtin_buf(var*, gc&);
var builtin_sleep(var*, gc&);
var builtin_pipe(var*, gc&);
var builtin_fork(var*, gc&);
var builtin_waitpid(var*, gc&);
var builtin_opendir(var*, gc&);
var builtin_readdir(var*, gc&);
var builtin_closedir(var*, gc&);
var builtin_chdir(var*, gc&);
var builtin_environ(var*, gc&);
var builtin_getcwd(var*, gc&);
var builtin_getenv(var*, gc&);
var builtin_dlopen(var*, gc&);
var builtin_dlclose(var*, gc&);
var builtin_dlcallv(var*, gc&);
var builtin_dlcall(var*, gc&);
var builtin_platform(var*, gc&);
var builtin_arch(var*, gc&);
// md5 related functions
std::string tohex(u32);
std::string md5(const std::string&);
var builtin_md5(var*, gc&);
var builtin_cocreate(var*, gc&);
var builtin_coresume(var*, gc&);
var builtin_coyield(var*, gc&);
var builtin_costatus(var*, gc&);
var builtin_corun(var*, gc&);
var builtin_millisec(var*, gc&);
var builtin_sysargv(var*, gc&);
var builtin_gcextend(var*, gc&);
var builtin_logtime(var*, gc&);
var builtin_ghosttype(var*, gc&);

// register builtin function's name and it's address here in this table below
// this table must end with {nullptr,nullptr}
struct {
    const char* name;
    var (*func)(var*,gc&);
} builtin[]= {
    {"__print",   builtin_print   },
    {"__println", builtin_println },
    {"__exit",    builtin_exit    },
    {"__abort",   builtin_abort   },
    {"__append",  builtin_append  },
    {"__setsize", builtin_setsize },
    {"__system",  builtin_system  },
    {"__input",   builtin_input   },
    {"__readfile",builtin_readfile},
    {"__fout",    builtin_fout    },
    {"__split",   builtin_split   },
    {"__rand",    builtin_rand    },
    {"__id",      builtin_id      },
    {"__int",     builtin_int     },
    {"__floor",   builtin_floor   },
    {"__num",     builtin_num     },
    {"__pop",     builtin_pop     },
    {"__str",     builtin_str     },
    {"__size",    builtin_size    },
    {"__u32xor",  builtin_u32xor  },
    {"__u32and",  builtin_u32and  },
    {"__u32or",   builtin_u32or   },
    {"__u32nand", builtin_u32nand },
    {"__u32not",  builtin_u32not  },
    {"__pow",     builtin_pow     },
    {"__sin",     builtin_sin     },
    {"__cos",     builtin_cos     },
    {"__tan",     builtin_tan     },
    {"__exp",     builtin_exp     },
    {"__lg",      builtin_lg      },
    {"__ln",      builtin_ln      },
    {"__sqrt",    builtin_sqrt    },
    {"__atan2",   builtin_atan2   },
    {"__isnan",   builtin_isnan   },
    {"__time",    builtin_time    },
    {"__contains",builtin_contains},
    {"__delete",  builtin_delete  },
    {"__keys",    builtin_keys    },
    {"__die",     builtin_die     },
    {"__find",    builtin_find    },
    {"__type",    builtin_type    },
    {"__substr",  builtin_substr  },
    {"__streq",   builtin_streq   },
    {"__left",    builtin_left    },
    {"__right",   builtin_right   },
    {"__cmp",     builtin_cmp     },
    {"__chr",     builtin_chr     },
    {"__char",    builtin_char    },
    {"__values",  builtin_values  },
    {"__exists",  builtin_exists  },
    {"__open",    builtin_open    },
    {"__close",   builtin_close   },
    {"__read",    builtin_read    },
    {"__write",   builtin_write   },
    {"__seek",    builtin_seek    },
    {"__tell",    builtin_tell    },
    {"__readln",  builtin_readln  },
    {"__stat",    builtin_stat    },
    {"__eof",     builtin_eof     },
    {"__fld",     builtin_fld     },
    {"__sfld",    builtin_sfld    },
    {"__setfld",  builtin_setfld  },
    {"__buf",     builtin_buf     },
    {"__sleep",   builtin_sleep   },
    {"__pipe",    builtin_pipe    },
    {"__fork",    builtin_fork    },
    {"__waitpid", builtin_waitpid },
    {"__opendir", builtin_opendir },
    {"__readdir", builtin_readdir },
    {"__closedir",builtin_closedir},
    {"__chdir",   builtin_chdir   },
    {"__environ", builtin_environ },
    {"__getcwd",  builtin_getcwd  },
    {"__getenv",  builtin_getenv  },
    {"__dlopen",  builtin_dlopen  },
    {"__dlclose", builtin_dlclose },
    {"__dlcallv", builtin_dlcallv },
    {"__dlcall",  builtin_dlcall  },
    {"__platform",builtin_platform},
    {"__arch",    builtin_arch    },
    {"__md5",     builtin_md5     },
    {"__cocreate",builtin_cocreate},
    {"__coresume",builtin_coresume},
    {"__coyield", builtin_coyield },
    {"__costatus",builtin_costatus},
    {"__corun"   ,builtin_corun   },
    {"__millisec",builtin_millisec},
    {"__sysargv", builtin_sysargv },
    {"__gcextd",  builtin_gcextend},
    {"__logtime", builtin_logtime },
    {"__ghosttype", builtin_ghosttype},
    {nullptr,     nullptr         }
};
