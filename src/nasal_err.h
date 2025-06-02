#pragma once

#include <iostream>
#include <fstream>
#include <sstream> // MSVC need this to use std::getline
#include <cstring>
#include <vector>

#include "nasal.h"

namespace nasal {

struct span {
    u64 begin_line;
    u64 begin_column;
    u64 end_line;
    u64 end_column;
    std::string file;

    void dump_begin(std::ostream& out) const {
        out << file << ":" << begin_line << ":" << begin_column + 1;
    }
};

std::ostream& clear_screen(std::ostream&);
std::ostream& set_cursor(std::ostream&);
std::ostream& back_white(std::ostream&);
std::ostream& red(std::ostream&);
std::ostream& cyan(std::ostream&);
std::ostream& orange(std::ostream&);
std::ostream& white(std::ostream&);
std::ostream& reset(std::ostream&);

class filestream {
protected:
    std::string file;
    std::vector<std::string> res;

public:
    filestream(): file("") {}
    void load(const std::string&);
    const std::string& operator[](usize n) const {return res[n];}
    const auto& name() const {return file;}
    const auto& file_content() const {return res;}
    usize size() const {return res.size();}
};

class error: public filestream {
private:
    u32 cnt; // counter for errors

    std::string identation(usize len) {
        return std::string(len, ' ');
    }
    std::string leftpad(u64 num, usize len) {
        auto tmp = std::to_string(num);
        while (tmp.length()<len) {
            tmp = " "+tmp;
        }
        return tmp;
    }

public:
    error(): cnt(0) {}
    void err(const std::string&, const std::string&);
    void warn(const std::string&, const std::string&);
    void err(const std::string&, const span&, const std::string&);

    void chkerr() const {
        if (cnt) {
            std::exit(1);
        }
    }
    auto geterr() const { return cnt; }
};

}
