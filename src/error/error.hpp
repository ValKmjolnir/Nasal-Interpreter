#pragma once

#include <iostream>
#include <fstream>
#include <sstream> // MSVC need this to use std::getline
#include <cstring>
#include <vector>

#include "nasal.hpp"
#include "error/span.hpp"
#include "util/filestream.hpp"

namespace nasal {

class error {
private:
    u32 cnt; // counter for errors
    filestream fls;

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
    void load(const std::string& f) {
        fls.load(f);
    }
    void chkerr() const {
        if (cnt) {
            std::exit(1);
        }
    }
    auto geterr() const { return cnt; }
};

}
