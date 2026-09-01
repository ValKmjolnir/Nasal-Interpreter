#pragma once

#include <string>
#include <iostream>

#include "util/type_alias.hpp"

namespace nasal {

struct func_info;
class compilation;

class const_value {
public:
    enum class type {
        CONST_NIL,
        CONST_NUM,
        CONST_STR,
        CONST_FUNC
    };

private:
    type type_;
    u64 index_;

private:
    const_value(type t, u64 i) : type_(t), index_(i) {}

public:
    const_value() : type_(type::CONST_NIL), index_(0) {}
    static const_value nil() { return const_value(); }
    static const_value num(u64 i) { return const_value(type::CONST_NUM, i); }
    static const_value str(u64 i) { return const_value(type::CONST_STR, i); }
    static const_value func(u64 i) { return const_value(type::CONST_FUNC, i); }
    void dump(std::ostream&, const compilation&) const;
};

}
