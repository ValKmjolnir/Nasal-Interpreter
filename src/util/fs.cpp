#ifndef _MSC_VER
#include <unistd.h>
#else
#include <io.h>
#endif

#ifdef _MSC_VER
#pragma warning (disable:4996)
#endif

#include <sys/stat.h>

#include "util/util.h"
#include "util/fs.h"
#include "nasal.h"

namespace nasal::fs {

path& path::operator/(const path& another) {
    this->file_system_path += util::is_windows()? "\\":"/";
    this->file_system_path += another.file_system_path;
    return *this;
}

bool exists(const path& file_path) {
#ifdef _MSC_VER
    #define F_OK 0 // fuck msc
#endif
    return access(file_path.c_str(), F_OK)==0;
}

bool is_regular(const path& file_path) {
#ifdef _MSC_VER
    #define S_ISREG(m) (((m)&0xF000)==0x8000)
#endif
    struct stat buffer;
    if (stat(file_path.c_str(), &buffer)!=0) {
        return false;
    }
    return S_ISREG(buffer.st_mode);
}

}