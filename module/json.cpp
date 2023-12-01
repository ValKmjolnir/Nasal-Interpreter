#include "../src/nasal.h"
#include "../src/nasal_type.h"
#include "../src/nasal_gc.h"

#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>

namespace nasal {

enum class token_type {
    tok_eof,
    tok_lbrace,
    tok_rbrace,
    tok_lbrkt,
    tok_rbrkt,
    tok_comma,
    tok_colon,
    tok_str,
    tok_num,
    tok_id,
    tok_bool
};

std::string get_content(token_type type) {
    switch(type) {
        case token_type::tok_eof: return "eof";
        case token_type::tok_lbrace: return "`{`";
        case token_type::tok_rbrace: return "`}`";
        case token_type::tok_lbrkt: return "`[`";
        case token_type::tok_rbrkt: return "`]`";
        case token_type::tok_comma: return "`,`";
        case token_type::tok_colon: return "`:`";
        case token_type::tok_str: return "string";
        case token_type::tok_num: return "number";
        case token_type::tok_id: return "identifier";
        case token_type::tok_bool: return "boolean";
    }
    // unreachable
    return "";
}

struct token {
    token_type type;
    std::string content;
};

class json {
private:
    std::string text = "";
    usize line = 1;
    usize ptr = 0;
    token this_token;
    var temp_stack = nil;

private:
    std::string var_generate(var&);
    std::string vector_generate(nas_vec&);
    std::string hash_generate(nas_hash&);

private:
    bool is_num(char c) {
        return std::isdigit(c);
    }
    bool is_id(char c) {
        return std::isalpha(c) || c=='_';
    }
    bool check(char c) {
        return c=='{' || c=='}' || c=='[' || c==']' ||
               c==':' || c==',' || c=='"' || c=='\'' ||
               is_num(c) || is_id(c);
    }
    void next();
    void match(token_type);
    void vector_member(nas_vec&, gc*);
    var vector_object_generate(gc*);
    void hash_member(nas_hash&, gc*);
    var hash_object_generate(gc*);
    void check_eof();
    static std::string& error_info() {
        static std::string info = "";
        return info;
    }

public:
    std::string stringify(var&);
    var parse(const std::string&, gc*);
    static const std::string& get_error() { return error_info(); }
};

std::string json::var_generate(var& value) {
    switch(value.type) {
        case vm_type::vm_num: {
            std::stringstream out;
            out << value.num();
            if (std::isnan(value.num())) {
                error_info() += "json::stringify: cannot generate number nan\n";
            }
            if (std::isinf(value.num())) {
                error_info() += "json::stringify: cannot generate number inf\n";
            }
            return out.str();
        }
        case vm_type::vm_str: return "\"" + value.str() + "\"";
        case vm_type::vm_vec: return vector_generate(value.vec());
        case vm_type::vm_hash: return hash_generate(value.hash());
        case vm_type::vm_func:
            error_info() += "json::stringify: cannot generate function\n"; break;
        case vm_type::vm_ghost:
            error_info() += "json::stringify: cannot generate ghost type\n"; break;
        case vm_type::vm_map:
            error_info() += "json::stringify: cannot generate namespace type\n"; break;
        default: break;
    }
    return "\"undefined\"";
}

std::string json::vector_generate(nas_vec& vect) {
    std::string out = "[";
    for(auto& i : vect.elems) {
        out += var_generate(i) + ",";
    }
    if (out.back()==',') {
        out.pop_back();
    }
    out += "]";
    return out;
}

std::string json::hash_generate(nas_hash& hash) {
    std::string out = "{";
    for(auto& i : hash.elems) {
        out += "\"" + i.first + "\":";
        out += var_generate(i.second) + ",";
    }
    if (out.back()==',') {
        out.pop_back();
    }
    out += "}";
    return out;
}

std::string json::stringify(var& object) {
    error_info() = "";
    if (object.is_vec()) {
        return vector_generate(object.vec());
    } else if (object.is_hash()) {
        return hash_generate(object.hash());
    }
    return "[]";
}

void json::next() {
    while(ptr<text.length() && !check(text[ptr])) {
        if (text[ptr]=='\n') {
            ++line;
        } else if (text[ptr]!=' ' && text[ptr]!='\t') {
            error_info() += "json::parse: line " + std::to_string(line);
            error_info() += ": invalid character `";
            error_info() += text[ptr];
            error_info() += "`\n";
        }
        ++ptr;
    }
    if (ptr>=text.length()) {
        this_token = {token_type::tok_eof, "eof"};
        return;
    }
    auto c = text[ptr];
    switch(c) {
        case '{': this_token = {token_type::tok_lbrace, "{"}; ++ptr; return;
        case '}': this_token = {token_type::tok_rbrace, "}"}; ++ptr; return;
        case '[': this_token = {token_type::tok_lbrkt, "["}; ++ptr; return;
        case ']': this_token = {token_type::tok_rbrkt, "]"}; ++ptr; return;
        case ',': this_token = {token_type::tok_comma, ","}; ++ptr; return;
        case ':': this_token = {token_type::tok_colon, ":"}; ++ptr; return;
        default: break;
    }
    if (is_num(c)) {
        auto temp = std::string(1, c);
        ++ptr;
        while(ptr<text.length() && (
            is_num(text[ptr]) ||
            text[ptr]=='.' ||
            text[ptr]=='e' ||
            text[ptr]=='-' ||
            text[ptr]=='+')) {
            temp += text[ptr];
            ++ptr;
        }
        --ptr;
        this_token = {token_type::tok_num, temp};
    } else if (is_id(c)) {
        auto temp = std::string(1, c);
        ++ptr;
        while(ptr<text.length() && (is_id(text[ptr]) || is_num(text[ptr]))) {
            temp += text[ptr];
            ++ptr;
        }
        --ptr;
        if (temp=="true" || temp=="false") {
            this_token = {token_type::tok_bool, temp};
        } else {
            this_token = {token_type::tok_id, temp};
        }
    } else if (c=='"' || c=='\'') {
        auto begin = c;
        auto temp = std::string("");
        ++ptr;
        while(ptr<text.length() && text[ptr]!=begin) {
            temp += text[ptr];
            ++ptr;
            if (text[ptr-1]=='\\' && ptr<text.length()) {
                temp += text[ptr];
                ++ptr;
            }
        }
        this_token = {token_type::tok_str, temp};
    }
    ++ptr;
    return;
}

void json::match(token_type type) {
    if (this_token.type!=type) {
        error_info() += "json::parse: line " + std::to_string(line);
        error_info() += ": expect " + get_content(type) + " but get `";
        error_info() += this_token.content + "`.\n";
    }
    next();
    return;
}

void json::vector_member(nas_vec& vec, gc* ngc) {
    if (this_token.type==token_type::tok_lbrace) {
        vec.elems.push_back(hash_object_generate(ngc));   
    } else if (this_token.type==token_type::tok_lbrkt) {
        vec.elems.push_back(vector_object_generate(ngc));
    } else if (this_token.type==token_type::tok_str) {
        vec.elems.push_back(ngc->newstr(this_token.content));
        next();
    } else if (this_token.type==token_type::tok_num) {
        vec.elems.push_back(var::num(str2num(this_token.content.c_str())));
        next();
    }
}

var json::vector_object_generate(gc* ngc) {
    auto vect_object = ngc->alloc(vm_type::vm_vec);
    temp_stack.vec().elems.push_back(vect_object);
    match(token_type::tok_lbrkt);
    vector_member(vect_object.vec(), ngc);
    while(this_token.type==token_type::tok_comma) {
        match(token_type::tok_comma);
        vector_member(vect_object.vec(), ngc);
    }
    match(token_type::tok_rbrkt);
    temp_stack.vec().elems.pop_back();
    return vect_object;
}

void json::hash_member(nas_hash& hash, gc* ngc) {
    const auto name = this_token.content;
    if (this_token.type==token_type::tok_rbrace) {
        return;
    }
    if (this_token.type==token_type::tok_str) {
        match(token_type::tok_str);
    } else {
        match(token_type::tok_id);
    }
    match(token_type::tok_colon);
    if (this_token.type==token_type::tok_lbrace) {
        hash.elems.insert({name, hash_object_generate(ngc)});   
    } else if (this_token.type==token_type::tok_lbrkt) {
        hash.elems.insert({name, vector_object_generate(ngc)});
    } else if (this_token.type==token_type::tok_str ||
        this_token.type==token_type::tok_bool) {
        hash.elems.insert({name, ngc->newstr(this_token.content)});
        next();
    } else if (this_token.type==token_type::tok_num) {
        hash.elems.insert({name, var::num(str2num(this_token.content.c_str()))});
        next();
    }
}

var json::hash_object_generate(gc* ngc) {
    auto hash_object = ngc->alloc(vm_type::vm_hash);
    temp_stack.vec().elems.push_back(hash_object);
    match(token_type::tok_lbrace);
    hash_member(hash_object.hash(), ngc);
    while(this_token.type==token_type::tok_comma) {
        match(token_type::tok_comma);
        hash_member(hash_object.hash(), ngc);
    }
    match(token_type::tok_rbrace);
    temp_stack.vec().elems.pop_back();
    return hash_object;
}

void json::check_eof() {
    next();
    if (this_token.type==token_type::tok_eof) {
        return;
    }
    while (this_token.type!=token_type::tok_eof) {
        error_info() += "json::parse: line " + std::to_string(line);
        error_info() += ": expect " + get_content(token_type::tok_eof);
        error_info() += " but get `" + this_token.content + "`.\n";
        next();
    }
}

var json::parse(const std::string& input, gc* ngc) {
    usize parse_error = 0;
    usize line = 1;
    usize ptr = 0;
    this_token = {token_type::tok_eof, ""};
    error_info() = "";

    if (input.empty()) {
        error_info() += "json::parse: empty string.\n";
        ++parse_error;
        return nil;
    }
    text = input;
    next();
    if (this_token.type==token_type::tok_lbrkt) {
        ngc->temp = temp_stack = ngc->alloc(vm_type::vm_vec);
        auto result = vector_object_generate(ngc);
        check_eof();
        ngc->temp = nil;
        temp_stack = nil;
        return result;
    } else {
        ngc->temp = temp_stack = ngc->alloc(vm_type::vm_vec);
        auto result = hash_object_generate(ngc);
        check_eof();
        ngc->temp = nil;
        temp_stack = nil;
        return result;
    }
    return nil;
}

var stringify(var* args, usize size, gc* ngc) {
    auto object = args[0];
    if (!object.is_vec() && !object.is_hash()) {
        return nas_err("json::stringify", "must use hashmap or vector");
    }
    return ngc->newstr(json().stringify(object));
}

var parse(var* args, usize size, gc* ngc) {
    auto input = args[0];
    if (!input.is_str()) {
        return nas_err("json::parse", "must use string");
    }
    return json().parse(input.str(), ngc);
}

var get_error(var* args, usize size, gc* ngc) {
    return ngc->newstr(json::get_error());
}

module_func_info func_tbl[] = {
    {"stringify", stringify},
    {"parse", parse},
    {"get_error", get_error},
    {nullptr, nullptr}
};

extern "C" module_func_info* get() {
    return func_tbl;
}

}