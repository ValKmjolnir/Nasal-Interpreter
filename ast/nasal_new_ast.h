#pragma once

#include "nasal.h"
#include "nasal_err.h"

enum class expr_type:u32 {
    ast_null=0,      // null node
    ast_expr,        // expression node
    ast_root,        // mark the root node of ast
    ast_block,       // expression block 
    ast_file,        // used to store which file the sub-tree is on, only used in main block
    ast_nil,         // nil keyword
    ast_num,         // number, basic value type
    ast_str,         // string, basic value type
    ast_id,          // identifier
    ast_bool,        // bools
    ast_func,        // func keyword
    ast_hash,        // hash, basic value type
    ast_vec,         // vector, basic value type
    ast_pair,        // pair of key and value in hashmap
    ast_call,        // mark a sub-tree of calling an identifier
    ast_callh,       // id.name
    ast_callv,       // id[index]
    ast_callf,       // id()
    ast_subvec,      // id[index:index]
    ast_params,      // mark a sub-tree of function parameters
    ast_default,     // default parameter
    ast_dynamic,     // dynamic parameter
    ast_and,         // and keyword
    ast_or,          // or keyword
    ast_equal,       // =
    ast_addeq,       // +=
    ast_subeq,       // -=
    ast_multeq,      // *=
    ast_diveq,       // /=
    ast_lnkeq,       // ~=
    ast_btandeq,     // &=
    ast_btoreq,      // |=
    ast_btxoreq,     // ^=
    ast_cmpeq,       // ==
    ast_neq,         // !=
    ast_less,        // <
    ast_leq,         // <=
    ast_grt,         // >
    ast_geq,         // >=
    ast_add,         // +
    ast_sub,         // -
    ast_mult,        // *
    ast_div,         // /
    ast_link,        // ~
    ast_neg,         // unary -
    ast_lnot,        // unary !
    ast_bnot,        // unary ~ bitwise not
    ast_bitor,       // bitwise or
    ast_bitxor,      // bitwise xor
    ast_bitand,      // bitwise and
    ast_trino,       // ?:
    ast_for,         // for keyword
    ast_forindex,    // forindex keyword
    ast_foreach,     // foreach keyword
    ast_while,       // while
    ast_iter,        // iterator, used in forindex/foreach
    ast_cond,        // mark a sub-tree of conditional expression
    ast_if,          // if keyword
    ast_elsif,       // elsif keyword
    ast_else,        // else keyword
    ast_multi_id,    // multi identifiers sub-tree
    ast_tuple,       // tuple, only used in multiple assignment
    ast_def,         // definition
    ast_multi_assign,// multi assignment sub-tree
    ast_continue,    // continue keyword, only used in loop
    ast_break,       // break keyword, only used in loop
    ast_ret          // return keyword, only used in function block
};

class ast_visitor;

class expr {
private:
    span nd_loc;
    expr_type nd_type;
public:
    expr(const span& location, expr_type node_type):
        nd_loc(location), nd_type(node_type) {}
    ~expr() = default;
    virtual void accept(ast_visitor*) = 0;
};

class null_expr:public expr {
public:
    null_expr(const span& location):
        expr(location, expr_type::ast_null) {}
    ~null_expr() = default;
    virtual void accept(ast_visitor*) override;
};

class nil_expr:public expr {
public:
    nil_expr(const span& location):
        expr(location, expr_type::ast_nil) {}
    ~nil_expr() = default;
    virtual void accept(ast_visitor*) override;
};

class number_literal:public expr {
private:
    f64 number;

public:
    number_literal(const span& location, const f64 num):
        expr(location, expr_type::ast_num), number(num) {}
    ~number_literal() = default;
    virtual void accept(ast_visitor*) override;
};

class string_literal:public expr {
private:
    string content;

public:
    string_literal(const span& location, const string& str):
        expr(location, expr_type::ast_str), content(str) {}
    ~string_literal() = default;
    virtual void accept(ast_visitor*) override;
};

class identifier:public expr {
private:
    string name;

public:
    identifier(const span& location, const string& str):
        expr(location, expr_type::ast_id), name(str) {}
    ~identifier() = default;
    virtual void accept(ast_visitor*) override;
};

class bool_literal:public expr {
private:
    bool flag;

public:
    bool_literal(const span& location, const bool bool_flag):
        expr(location, expr_type::ast_bool), flag(bool_flag) {}
    ~bool_literal() = default;
    virtual void accept(ast_visitor*) override;
};

class vector_expr:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class hash_expr:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class hash_pair:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class function:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class parameter:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class ternary_operator:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class binary_operator:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class unary_operator:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class call_expr:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class call_hash:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class call_vector:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class call_function:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class slice_vector:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class definition:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class multi_define:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class while_expr:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class for_expr:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class foreach_expr:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class forindex_expr:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class condition_expr:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class if_expr:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class continue_expr:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class break_expr:public expr {
public:
    virtual void accept(ast_visitor*) override;
};

class return_expr:public expr {
public:
    virtual void accept(ast_visitor*) override;
};
