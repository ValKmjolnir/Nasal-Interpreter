#pragma once

#include "error/error.hpp"
#include "code/opcode.hpp"
#include "code/compilation.hpp"
#include "ast/ast.hpp"
#include "ast/visitor.hpp"
#include "ast/symbol_finder.hpp"
#include "util/resource_manager.hpp"
#include "natives/registry.hpp"

#include <iomanip>
#include <list>
#include <stack>
#include <unordered_map>
#include <unordered_set>

#ifdef _MSC_VER
#pragma warning (disable:4244)
#pragma warning (disable:4267)
#endif

namespace nasal {

class codegen {
private:
    error& err;
    compilation& comp;
    const resource_manager& resm;

    // repl output flag, will generate op_repl to output stack top value if true
    bool flag_need_repl_output = false;

    // used for generate pop in return expression
    std::vector<u32> in_foreach_loop_level;

    // used to store jmp operands index, to fill the jump address back
    std::list<std::vector<u64>> continue_ptr;
    std::list<std::vector<u64>> break_ptr;

    // nasal namespace
    // stores all global symbols of each file
    std::unordered_map<std::string, std::unordered_set<std::string>> nasal_namespace;

    // local  : max 32768 upvalues 65536 values
    // but in fact local scope also has less than VM_STACK_DEPTH value
    std::list<std::unordered_map<std::string, u32>> local;

private:
    void check_id_exist(identifier*);

    void die(const std::string& info, expr* node) {
        err.err("code", node->get_location(), info);
    }

    void find_symbol(code_block*);
    void regist_symbol(const std::string&);
    i32 local_symbol_find(const std::string&);
    i32 global_symbol_find(const std::string&);
    i32 upvalue_symbol_find(const std::string&);

    void emit(u8, u32, const span&);

    void number_gen(number_literal*);
    void string_gen(string_literal*);
    void bool_gen(bool_literal*);
    void vector_gen(vector_expr*);
    void hash_gen(hash_expr*);
    const_value const_inner_gen(expr*);
    const_value const_gen(expr*);
    void check_parameter_list(function*);
    u64 func_gen(function*, bool);
    void call_gen(call_expr*);
    void call_identifier(identifier*);
    void call_hash_gen(call_hash*);
    void null_access_gen(null_access*);
    void call_vector_gen(call_vector*);
    void call_func_gen(call_function*);
    void mcall(expr*);
    void mcall_identifier(identifier*);
    void mcall_vec(call_vector*);
    void mcall_hash(call_hash*);
    void multi_def(definition_expr*);
    void single_def(definition_expr*);
    void definition_gen(definition_expr*);
    void assignment_expression(assignment_expr*);
    void gen_assignment_equal_statement(assignment_expr*);
    void replace_left_assignment_with_load(const span&);
    void assignment_statement(assignment_expr*);
    void multi_assign_gen(multi_assign*);
    void cond_gen(condition_expr*);
    void loop_gen(expr*);
    void load_continue_break(u64, u64);
    void while_gen(while_expr*);
    void for_gen(for_expr*);
    void forei_gen(forei_expr*);
    void statement_generation(expr*);
    void or_gen(binary_operator*);
    void and_gen(binary_operator*);
    void unary_gen(unary_operator*);
    void binary_gen(binary_operator*);
    void null_chain_gen(binary_operator*);
    void trino_gen(ternary_operator*);
    void calc_gen(expr*);
    void repl_mode_info_output_gen(expr*);
    void block_gen(code_block*);
    void ret_gen(return_expr*);

public:
    codegen(error& e, compilation& c, const resource_manager& r):
        err(e), comp(c), resm(r) {}
    const error& compile(code_block*, bool);
    void print(std::ostream&);
    void symbol_dump(std::ostream&) const;
};

}
