#pragma once

#include "nasal.h"
#include "nasal_err.h"

enum class ast_node_type:u32 {
    ast_null=0,      // null node
    ast_stmt,
    ast_expr,
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

class ast_node {
private:
    span nd_loc;
    ast_node_type nd_type;
public:
    ast_node(const span& location,ast_node_type node_type):
        nd_loc(location), nd_type(node_type) {}
    ~ast_node() = default;
    virtual bool accept(ast_visitor*) = 0;
};

class stmt:public ast_node {
public:
    stmt(const span& location,ast_node_type node_type):
        ast_node(location, node_type) {}
    ~stmt() = default;
    virtual bool accept(ast_visitor*);
};

class expr:public ast_node {
private:

public:
    expr(const span& location,ast_node_type node_type):
        ast_node(location, node_type) {}
    ~expr() = default;
    virtual bool accept(ast_visitor*);
};

class ast_visitor {
public:
    virtual void visit_stmt(stmt*);
    virtual void visit_expr(expr*);
};