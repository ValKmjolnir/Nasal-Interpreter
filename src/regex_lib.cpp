#include "regex_lib.h"

namespace nasal {

var builtin_regex_match(context* ctx, gc* ngc) {
    auto source = ctx->localr[1];
    auto reg_str = ctx->localr[2];
    if (!source.is_str()) {
        return nas_err("regex::match", "\"src\" must be a string");
    }
    if (!reg_str.is_str()) {
        return nas_err("regex::match", "\"reg\" must be a format string");
    }
    try {
        auto res = std::regex_match(source.str(), std::regex(reg_str.str()));
        return res? one:zero;
    } catch(const std::regex_error& e) {
        return nas_err("regex::match", e.what());
    }
    return zero;
}

var builtin_regex_search(context* ctx, gc* ngc) {
    auto source = ctx->localr[1];
    auto reg_str = ctx->localr[2];
    if (!source.is_str()) {
        return nas_err("regex::search", "\"src\" must be a string");
    }
    if (!reg_str.is_str()) {
        return nas_err("regex::search", "\"reg\" must be a format string");
    }
    try {
        auto res = std::regex_search(source.str(), std::regex(reg_str.str()));
        return res? one:zero;
    } catch(const std::regex_error& e) {
        return nas_err("regex::search", e.what());
    }
    return nil;
}

var builtin_regex_replace(context* ctx, gc* ngc) {
    auto source = ctx->localr[1];
    auto reg_str = ctx->localr[2];
    auto fmt = ctx->localr[3];
    if (!source.is_str()) {
        return nas_err("regex::replace", "\"src\" must be a string");
    }
    if (!reg_str.is_str()) {
        return nas_err("regex::replace", "\"reg\" must be a format string");
    }
    if (!fmt.is_str()) {
        return nas_err("regex::replace", "\"fmt\" must be a format string");
    }
    try {
        auto res = std::regex_replace(
            source.str(),
            std::regex(reg_str.str()),
            fmt.str()
        );
        return ngc->newstr(res);
    } catch(const std::regex_error& e) {
        return nas_err("regex::replace", e.what());
    }
    return ngc->newstr(source.str());
}

var builtin_regex_match_all(context* ctx, gc* ngc) {
    auto source = ctx->localr[1];
    auto reg_str = ctx->localr[2];
    if (!source.is_str()) {
        return nas_err("regex::match_all", "\"src\" must be a string");
    }
    if (!reg_str.is_str()) {
        return nas_err("regex::match_all", "\"reg\" must be a format string");
    }
    auto res = ngc->temp = ngc->alloc(vm_type::vm_vec);
    try {
        const auto& src = source.str();
        auto words_regex = std::regex(reg_str.str());
        auto begin = std::sregex_iterator(src.begin(), src.end(), words_regex);
        auto end = std::sregex_iterator();
        for (auto i = begin; i!=end; ++i) {
            res.vec().elems.push_back(ngc->newstr((*i).str()));
        }
    } catch(const std::regex_error& e) {
        return nas_err("regex::match_all", e.what());
    }
    return res;
}

nasal_builtin_table regex_lib_native[] = {
    {"__regex_match", builtin_regex_match},
    {"__regex_search", builtin_regex_search},
    {"__regex_replace", builtin_regex_replace},
    {"__regex_match_all", builtin_regex_match_all},
    {nullptr, nullptr}
};

}