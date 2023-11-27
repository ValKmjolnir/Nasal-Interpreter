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
}

struct token {
    token_type type;
    std::string content;
};

class json {
private:
    usize parse_error = 0;
    std::string text = "";
    usize line = 1;
    usize ptr = 0;
    token this_token;

private:
    std::string var_generate(var&);
    std::string vector_generate(nas_vec&);
    std::string hash_generate(nas_hash&);
    std::string map_generate(nas_map&);

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
    var vector_member(gc*);
    var vector_object_generate(gc*);
    void hash_member(nas_hash&, gc*);
    var hash_object_generate(gc*);

public:
    std::string stringify(var&);
    var parse(const std::string&, gc*);
    bool has_error() const { return parse_error; }
};

std::string json::var_generate(var& value) {
    switch(value.type) {
        case vm_type::vm_num: {
            std::stringstream out;
            out << value.num();
            return out.str();
        }
        case vm_type::vm_str: return "\"" + value.str() + "\"";
        case vm_type::vm_vec: return vector_generate(value.vec());
        case vm_type::vm_hash: return hash_generate(value.hash());
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

std::string json::map_generate(nas_map& nmap) {
    std::string out = "{";
    for(auto& i : nmap.mapper) {
        out += "\"" + i.first + "\":";
        out += var_generate(*i.second) + ",";
    }
    if (out.back()==',') {
        out.pop_back();
    }
    out += "}";
    return out;
}

std::string json::stringify(var& object) {
    if (object.is_vec()) {
        return vector_generate(object.vec());
    } else if (object.is_hash()) {
        return hash_generate(object.hash());
    } else if (object.is_map()) {
        return map_generate(object.map());
    }
    return "[]";
}

void json::next() {
    while(ptr<text.length() && !check(text[ptr])) {
        if (text[ptr]=='\n') {
            ++line;
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
        std::cerr << "json::parse: line " << line << ": expect ";
        std::cerr << get_content(type) << " but get `";
        std::cerr << this_token.content << "`.\n";
        ++parse_error;
    }
    next();
    return;
}

var json::vector_member(gc* ngc) {
    auto result = nil;
    if (this_token.type==token_type::tok_lbrace) {
        result = hash_object_generate(ngc);   
    } else if (this_token.type==token_type::tok_lbrkt) {
        result = vector_object_generate(ngc);
    } else if (this_token.type==token_type::tok_str) {
        result = ngc->newstr(this_token.content);
        next();
    } else if (this_token.type==token_type::tok_num) {
        result = var::num(str2num(this_token.content.c_str()));
        next();
    }
    return result;
}

var json::vector_object_generate(gc* ngc) {
    auto vect_object = ngc->alloc(vm_type::vm_vec);
    auto& vec = vect_object.vec().elems;
    ngc->temp = vect_object;
    match(token_type::tok_lbrkt);
    vec.push_back(vector_member(ngc));
    while(this_token.type==token_type::tok_comma) {
        match(token_type::tok_comma);
        vec.push_back(vector_member(ngc));
    }
    match(token_type::tok_rbrkt);
    ngc->temp = nil;
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
    ngc->temp = hash_object; // cause problem
    match(token_type::tok_lbrace);
    hash_member(hash_object.hash(), ngc);
    while(this_token.type==token_type::tok_comma) {
        match(token_type::tok_comma);
        hash_member(hash_object.hash(), ngc);
    }
    match(token_type::tok_rbrace);
    ngc->temp = nil;
    return hash_object;
}

var json::parse(const std::string& input, gc* ngc) {
    usize parse_error = 0;
    usize line = 1;
    usize ptr = 0;
    this_token = {token_type::tok_eof, ""};

    if (input.empty()) {
        std::cerr << "json::parse: empty string.\n";
        ++parse_error;
        return nil;
    }
    text = input;
    next();
    if (this_token.type==token_type::tok_lbrkt) {
        return vector_object_generate(ngc);
    } else {
        return hash_object_generate(ngc);
    }
    return nil;
}

var stringify(var* args, usize size, gc* ngc) {
    auto object = args[0];
    if (!object.is_vec() && !object.is_hash() && !object.is_map()) {
        return nas_err("json::stringify", "must use hashmap or vector");
    }
    return ngc->newstr(json().stringify(object));
}

var parse(var* args, usize size, gc* ngc) {
    auto input = args[0];
    if (!input.is_str()) {
        return nas_err("json::parse", "must use string");
    }
    json instance;
    auto result = instance.parse(input.str(), ngc);
    if (instance.has_error()) {
        return nas_err("json::parse", "parse error");
    }
    return result;
}

module_func_info func_tbl[] = {
    {"stringify", stringify},
    {"parse", parse},
    {nullptr, nullptr}
};

extern "C" module_func_info* get() {
    return func_tbl;
}

}