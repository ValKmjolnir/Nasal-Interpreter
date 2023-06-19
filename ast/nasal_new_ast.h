#pragma once

#include "nasal.h"
#include "nasal_err.h"

#include <vector>
#include <unordered_map>

enum class expr_type:u32 {
    ast_null=0,      // null node
    ast_expr,        // expression node
    ast_block,       // expression block 
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

    ast_param,       // function parameter
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

    ast_ternary,
    ast_binary,
    ast_unary,
    ast_for,         // for keyword
    ast_forei,       // foreach or forindex loop
    ast_while,       // while
    ast_iter,        // iterator, used in forindex/foreach
    ast_cond,        // mark a sub-tree of conditional expression
    ast_if,          // if keyword
    ast_multi_id,    // multi identifiers sub-tree
    ast_def,         // definition
    ast_continue,    // continue keyword, only used in loop
    ast_break,       // break keyword, only used in loop
    ast_ret          // return keyword, only used in function block
};

class ast_visitor;
class hash_pair;
class parameter;
class slice_vector;
class multi_define;
class code_block;
class if_expr;

class ast_abstract_node {
public:
    virtual void accept(ast_visitor*) = 0;
};

class expr:public ast_abstract_node {
private:
    span nd_loc;
    expr_type nd_type;
public:
    expr(const span& location, expr_type node_type):
        nd_loc(location), nd_type(node_type) {}
    ~expr() = default;
    void set_begin(u32 line, u32 column) {
        nd_loc.begin_line = line;
        nd_loc.begin_column = column;
    }
    const span& get_location() const {return nd_loc;}
    expr_type get_type() const {return nd_type;}
    void update_location(const span& location) {
        nd_loc.end_line = location.end_line;
        nd_loc.end_column = location.end_column;
    }
    virtual void accept(ast_visitor*);
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
    std::string content;

public:
    string_literal(const span& location, const std::string& str):
        expr(location, expr_type::ast_str), content(str) {}
    ~string_literal() = default;
    virtual void accept(ast_visitor*) override;
};

class identifier:public expr {
private:
    std::string name;

public:
    identifier(const span& location, const std::string& str):
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
private:
    std::vector<expr*> elements;

public:
    vector_expr(const span& location):
        expr(location, expr_type::ast_vec) {}
    ~vector_expr();
    void add_element(expr* node) {elements.push_back(node);}
    std::vector<expr*>& get_elements() {return elements;}
    virtual void accept(ast_visitor*) override;
};

class hash_expr:public expr {
private:
    std::vector<hash_pair*> members;

public:
    hash_expr(const span& location):
        expr(location, expr_type::ast_hash) {}
    ~hash_expr();
    void add_member(hash_pair* node) {members.push_back(node);}
    std::vector<hash_pair*>& get_members() {return members;}
    virtual void accept(ast_visitor*) override;
};

class hash_pair:public expr {
private:
    std::string name;
    expr* element;

public:
    hash_pair(const span& location):
        expr(location, expr_type::ast_pair) {}
    ~hash_pair();
    void set_name(const std::string& field_name) {name = field_name;}
    void set_element(expr* node) {element = node;}
    const std::string& get_name() const {return name;}
    expr* get_element() {return element;}
    virtual void accept(ast_visitor*) override;
};

class function:public expr {
private:
    std::vector<parameter*> parameter_list;
    code_block* block;

public:
    function(const span& location):
        expr(location, expr_type::ast_func),
        block(nullptr) {}
    ~function();
    void add_parameter(parameter* node) {parameter_list.push_back(node);}
    void set_code_block(code_block* node) {block = node;}
    std::vector<parameter*>& get_parameter_list() {return parameter_list;}
    code_block* get_code_block() {return block;}
    virtual void accept(ast_visitor*) override;
};

class code_block:public expr {
private:
    std::vector<expr*> expressions;

public:
    code_block(const span& location):
        expr(location, expr_type::ast_block) {}
    ~code_block();
    void add_expression(expr* node) {expressions.push_back(node);}
    std::vector<expr*>& get_expressions() {return expressions;}
    virtual void accept(ast_visitor*) override;
};

class parameter:public expr {
public:
    enum class param_type {
        normal_parameter,
        default_parameter,
        dynamic_parameter
    };

private:
    param_type type;
    identifier* name;
    expr* default_value;

public:
    parameter(const span& location):
        expr(location, expr_type::ast_param),
        name(nullptr), default_value(nullptr) {}
    ~parameter();
    void set_parameter_type(param_type pt) {type = pt;}
    void set_parameter_name(identifier* node) {name = node;}
    void set_default_value(expr* node) {default_value = node;}
    virtual void accept(ast_visitor*) override;
};

class ternary_operator:public expr {
private:
    expr* condition;
    expr* left;
    expr* right;

public:
    ternary_operator(const span& location):
        expr(location, expr_type::ast_ternary),
        condition(nullptr), left(nullptr), right(nullptr) {}
    ~ternary_operator();
    virtual void accept(ast_visitor*) override;
};

class binary_operator:public expr {
public:
    enum class binary_type {
        add,
        sub,
        mult,
        div,
        concat,
        cmpeq,
        cmpneq,
        less,
        leq,
        grt,
        geq,
        bitwise_or,
        bitwise_xor,
        bitwise_and
    };

private:
    binary_type type;
    expr* left;
    expr* right;

public:
    binary_operator(const span& location):
        expr(location, expr_type::ast_binary),
        left(nullptr), right(nullptr) {}
    ~binary_operator();
    void set_type(binary_type operator_type) {type = operator_type;}
    void set_left(expr* node) {left = node;}
    void set_right(expr* node) {right = node;}
    virtual void accept(ast_visitor*) override;
};

class unary_operator:public expr {
public:
    enum class unary_type {
        negative,
        logical_not,
        bitwise_not
    };

private:
    unary_type type;
    expr* value;

public:
    unary_operator(const span& location):
        expr(location, expr_type::ast_unary),
        value(nullptr) {}
    ~unary_operator();
    void set_type(unary_type operator_type) {type = operator_type;}
    void set_value(expr* node) {value = node;}
    virtual void accept(ast_visitor*) override;
};

class call_hash:public expr {
private:
    std::string field;

public:
    call_hash(const span& location, const std::string& name):
        expr(location, expr_type::ast_callh),
        field(name) {}
    ~call_hash();
    virtual void accept(ast_visitor*) override;
};

class call_vector:public expr {
private:
    std::vector<slice_vector*> calls;

public:
    call_vector(const span& location):
        expr(location, expr_type::ast_callv) {}
    ~call_vector();
    void add_slice(slice_vector* node) {calls.push_back(node);}
    std::vector<slice_vector*>& get_slices() {return calls;}
    virtual void accept(ast_visitor*) override;
};

class call_function:public expr {
private:
    std::vector<expr*> args;

public:
    call_function(const span& location):
        expr(location, expr_type::ast_callf) {}
    ~call_function();
    void add_argument(expr* node) {args.push_back(node);}
    std::vector<expr*>& get_argument() {return args;}
    virtual void accept(ast_visitor*) override;
};

class slice_vector:public expr {
private:
    expr* begin;
    expr* end;

public:
    slice_vector(const span& location):
        expr(location, expr_type::ast_subvec),
        begin(nullptr), end(nullptr) {}
    ~slice_vector();
    void set_begin(expr* node) {begin = node;}
    void set_end(expr* node) {end = node;}
    virtual void accept(ast_visitor*) override;
};

class definition:public expr {
private:
    identifier* variable;
    multi_define* variables;
    expr* value;

public:
    definition(const span& location):
        expr(location, expr_type::ast_def),
        variable(nullptr), variables(nullptr), value(nullptr) {}
    ~definition();
    virtual void accept(ast_visitor*) override;
};

class multi_define:public expr {
private:
    std::vector<expr*> variables;

public:
    multi_define(const span& location):
        expr(location, expr_type::ast_multi_id) {}
    ~multi_define();
    virtual void accept(ast_visitor*) override;
};

class while_expr:public expr {
private:
    expr* condition;
    code_block* block;

public:
    while_expr(const span& location):
        expr(location, expr_type::ast_while),
        condition(nullptr), block(nullptr) {}
    ~while_expr();
    void set_condition(expr* node) {condition = node;}
    void set_code_block (code_block* node) {block = node;}
    expr* get_condition() {return condition;}
    code_block* get_code_block() {return block;}
    virtual void accept(ast_visitor*) override;
};

class for_expr:public expr {
private:
    expr* initializing;
    expr* condition;
    expr* step;
    code_block* block;

public:
    for_expr(const span& location):
        expr(location, expr_type::ast_for),
        initializing(nullptr), condition(nullptr),
        step(nullptr), block(nullptr) {}
    ~for_expr();
    void set_initial(expr* node) {initializing = node;}
    void set_condition(expr* node) {condition = node;}
    void set_step(expr* node) {step = node;}
    void set_code_block(code_block* node) {block = node;}
    expr* get_initial() {return initializing;}
    expr* get_condition() {return condition;}
    expr* get_step() {return step;}
    code_block* get_code_block() {return block;}
    virtual void accept(ast_visitor*) override;
};

class iter_expr:public expr {
private:
    identifier* name;
    expr* value;

public:
    iter_expr(const span& location):
        expr(location, expr_type::ast_iter),
        name(nullptr), value(nullptr) {}
    ~iter_expr();
    void set_name(identifier* node) {name = node;}
    void set_value(expr* node) {value = node;}
    identifier* get_name() {return name;}
    expr* get_value() {return value;}
    virtual void accept(ast_visitor*) override;
};

class forei_expr:public expr {
public:
    enum class forei_loop_type {
        foreach,
        forindex
    };

private:
    forei_loop_type type;
    iter_expr* iterator;
    expr* vector_node;
    code_block* block;

public:
    forei_expr(const span& location):
        expr(location, expr_type::ast_forei),
        type(forei_loop_type::foreach), iterator(nullptr),
        vector_node(nullptr), block(nullptr) {}
    ~forei_expr();
    void set_type(forei_loop_type ft) {type = ft;}
    void set_iterator(iter_expr* node) {iterator = node;}
    void set_value(expr* node) {vector_node = node;}
    void set_code_block(code_block* node) {block = node;}
    iter_expr* get_iterator() {return iterator;}
    expr* get_value() {return vector_node;}
    code_block* get_code_block() {return block;}
    virtual void accept(ast_visitor*) override;
};

class condition_expr:public expr {
private:
    if_expr* if_stmt;
    std::vector<if_expr*> elsif_stmt;
    if_expr* else_stmt;

public:
    condition_expr(const span& location):
        expr(location, expr_type::ast_cond),
        if_stmt(nullptr), else_stmt(nullptr) {}
    ~condition_expr();
    void set_if_statement(if_expr* node) {if_stmt = node;}
    void add_elsif_statement(if_expr* node) {elsif_stmt.push_back(node);}
    void set_else_statement(if_expr* node) {else_stmt = node;}
    if_expr* get_if_statement() {return if_stmt;}
    std::vector<if_expr*>& get_elsif_stataments() {return elsif_stmt;}
    if_expr* get_else_statement() {return else_stmt;}
    virtual void accept(ast_visitor*) override;
};

class if_expr:public expr {
private:
    expr* condition;
    code_block* block;

public:
    if_expr(const span& location):
        expr(location, expr_type::ast_if),
        condition(nullptr), block(nullptr) {}
    ~if_expr();
    void set_condition(expr* node) {condition = node;}
    void set_code_block(code_block* node) {block = node;}
    expr* get_condition() {return condition;}
    code_block* get_code_block() {return block;}
    virtual void accept(ast_visitor*) override;
};

class continue_expr:public expr {
public:
    continue_expr(const span& location):
        expr(location, expr_type::ast_continue) {}
    ~continue_expr() = default;
    virtual void accept(ast_visitor*) override;
};

class break_expr:public expr {
public:
    break_expr(const span& location):
        expr(location, expr_type::ast_break) {}
    ~break_expr() = default;
    virtual void accept(ast_visitor*) override;
};

class return_expr:public expr {
private:
    expr* value;

public:
    return_expr(const span& location):
        expr(location, expr_type::ast_ret),
        value(nullptr) {}
    ~return_expr();
    void set_value(expr* node) {value = node;}
    expr* get_value() {return value;}
    virtual void accept(ast_visitor*) override;
};
