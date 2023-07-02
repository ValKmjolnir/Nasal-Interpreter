#pragma once

#include "nasal_err.h"
#include "nasal_builtin.h"
#include "nasal_opcode.h"
#include "nasal_ast.h"
#include "ast_visitor.h"
#include "symbol_finder.h"
#include "nasal_parse.h"
#include "nasal_import.h"

#include <iomanip>
#include <list>
#include <stack>
#include <unordered_map>

#ifdef _MSC_VER
#pragma warning (disable:4244)
#pragma warning (disable:4267)
#endif

class codegen {
private:
    u16 fileindex;
    error& err;
    const std::string* file;
    std::stack<u32> in_iterloop;
    std::unordered_map<f64,u32> num_table;
    std::unordered_map<std::string,u32> str_table;
    std::vector<f64> num_res;
    std::vector<std::string> str_res;
    std::vector<opcode> code;
    std::list<std::vector<i32>> continue_ptr;
    std::list<std::vector<i32>> break_ptr;

    // symbol table
    // global : max STACK_DEPTH-1 values
    std::unordered_map<std::string,i32> global;
    // local  : max 32768 upvalues 65536 values
    // but in fact local scope also has less than STACK_DEPTH value
    std::list<std::unordered_map<std::string,i32>> local;

    void check_id_exist(identifier*);
    
    void die(const std::string& info, const span& loc) {
        err.err("code", loc, info);
    }

    void regist_num(const f64);
    void regist_str(const std::string&);
    void find_symbol(code_block*);
    void add_sym(const std::string&);
    i32 local_find(const std::string&);
    i32 global_find(const std::string&);
    i32 upvalue_find(const std::string&);

    void gen(u8, u32, u32);

    void num_gen(number_literal*);
    void str_gen(string_literal*);
    void bool_gen(bool_literal*);
    void vec_gen(vector_expr*);
    void hash_gen(hash_expr*);
    void func_gen(function*);
    void call_gen(call_expr*);
    void call_id(identifier*);
    void call_hash_gen(call_hash*);
    void call_vec(call_vector*);
    void call_func(call_function*);
    void mcall(expr*);
    void mcall_id(identifier*);
    void mcall_vec(call_vector*);
    void mcall_hash(call_hash*);
    void multi_def(definition_expr*);
    void single_def(definition_expr*);
    void def_gen(definition_expr*);
    void assignment_expression(assignment_expr*);
    void assignment_statement(assignment_expr*);
    void multi_assign_gen(multi_assign*);
    void cond_gen(condition_expr*);
    void loop_gen(expr*);
    void load_continue_break(i32, i32);
    void while_gen(while_expr*);
    void for_gen(for_expr*);
    void forei_gen(forei_expr*);
    void statement_generation(expr*);
    void or_gen(binary_operator*);
    void and_gen(binary_operator*);
    void unary_gen(unary_operator*);
    void binary_gen(binary_operator*);
    void trino_gen(ternary_operator*);
    void calc_gen(expr*);
    void block_gen(code_block*);
    void ret_gen(return_expr*);

public:
    codegen(error& e): fileindex(0), err(e), file(nullptr) {}
    const error& compile(parse&, linker&);
    void print();
    const std::vector<std::string>& strs() const {return str_res;}
    const std::vector<f64>& nums() const {return num_res;}
    const std::vector<opcode>& codes() const {return code;}
};
