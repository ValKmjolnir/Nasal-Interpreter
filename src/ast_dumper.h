#pragma once

#include "ast_visitor.h"

#include <iostream>
#include <cstring>
#include <sstream>

class ast_dumper:public ast_visitor {
private:
    std::vector<std::string> indent;
    void push_indent() {
        if (indent.size()) {
            if (indent.back()=="|--") {
                indent.back() = "|  ";
            } else if (indent.back()=="+--") {
                indent.back() = "   ";
            }
        }
        indent.push_back("|--");
    }
    void pop_indent() {indent.pop_back();}
    void set_last() {indent.back() = "+--";}
    void dump_indent() {
        if (indent.size() && indent.back()=="|  ") {
            indent.back() = "|--";
        }
        for(const auto& i : indent) {
            std::cout << i;
        }
    }
    std::string format_location(const span& location) {
        std::stringstream ss;
        ss << " -> ";
        ss << location.file << ":";
        ss << location.begin_line << ":" << location.begin_column + 1;
        ss << "\n";
        return ss.str();
    }

public:
    bool visit_null_expr(null_expr*) override;
    bool visit_nil_expr(nil_expr*) override;
    bool visit_number_literal(number_literal*) override;
    bool visit_string_literal(string_literal*) override;
    bool visit_identifier(identifier*) override;
    bool visit_bool_literal(bool_literal*) override;
    bool visit_vector_expr(vector_expr*) override;
    bool visit_hash_expr(hash_expr*) override;
    bool visit_hash_pair(hash_pair*) override;
    bool visit_function(function*) override;
    bool visit_code_block(code_block*) override;
    bool visit_parameter(parameter*) override;
    bool visit_ternary_operator(ternary_operator*) override;
    bool visit_binary_operator(binary_operator*) override;
    bool visit_unary_operator(unary_operator*) override;
    bool visit_call_expr(call_expr*) override;
    bool visit_call_hash(call_hash*) override;
    bool visit_call_vector(call_vector*) override;
    bool visit_call_function(call_function*) override;
    bool visit_slice_vector(slice_vector*) override;
    bool visit_definition_expr(definition_expr*) override;
    bool visit_assignment_expr(assignment_expr*) override;
    bool visit_multi_identifier(multi_identifier*) override;
    bool visit_tuple_expr(tuple_expr*) override;
    bool visit_multi_assign(multi_assign*) override;
    bool visit_while_expr(while_expr*) override;
    bool visit_for_expr(for_expr*) override;
    bool visit_iter_expr(iter_expr*) override;
    bool visit_forei_expr(forei_expr*) override;
    bool visit_condition_expr(condition_expr*) override;
    bool visit_if_expr(if_expr*) override;
    bool visit_continue_expr(continue_expr*) override;
    bool visit_break_expr(break_expr*) override;
    bool visit_return_expr(return_expr*) override;

public:
    void dump(code_block* root) {
        root->accept(this);
    }
};