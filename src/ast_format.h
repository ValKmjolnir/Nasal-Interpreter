#pragma once

#include "ast_visitor.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <sstream>
#include <vector>

namespace nasal {

class ast_format: public ast_visitor {
private:
    std::ofstream out;
    std::vector<std::string> indent;

private:
    void push_indent() {
        indent.push_back("  ");
    }
    void pop_indent() {
        if (indent.size()) {
            indent.pop_back();
        }
    }
    void dump_indent() {
        for (const auto& i : indent) {
            out << i;
        }
    }
    void dump_formating_node_info(expr* n, const char* name) {
        std::cout << "  formating " << name << " @ 0x";
        std::cout << std::hex << n << std::dec << "\n";
    }
    bool need_dump_semi(expr* n) {
        switch (n->get_type()) {
            case expr_type::ast_use:
            case expr_type::ast_null:
            case expr_type::ast_nil:
            case expr_type::ast_num:
            case expr_type::ast_str:
            case expr_type::ast_bool:
            case expr_type::ast_vec:
            case expr_type::ast_hash:
            case expr_type::ast_call: return true;
            case expr_type::ast_def: {
                auto dn = reinterpret_cast<definition_expr*>(n);
                if (dn->get_value() &&
                    dn->get_value()->get_type() == expr_type::ast_func) {
                    return false;
                }
                return true;
            }
            case expr_type::ast_assign: {
                auto dn = reinterpret_cast<assignment_expr*>(n);
                if (dn->get_right() &&
                    dn->get_right()->get_type() == expr_type::ast_func) {
                    return false;
                }
                return true;
            }
            case expr_type::ast_ret: {
                auto dn = reinterpret_cast<return_expr*>(n);
                if (dn->get_value() &&
                    dn->get_value()->get_type() == expr_type::ast_func) {
                    return false;
                }
                return true;
            }
            default: break;
        }

        return false;
    }

public:
    bool visit_use_stmt(use_stmt*) override;
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
    bool visit_null_access(null_access*) override;
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
    ast_format(const std::string output_file): out(output_file) {
        if (out.fail()) {
            throw std::runtime_error("can't open file: " + output_file);
        }
    }

    void do_format(code_block* root) {
        std::cout << "nasal-format is not stable right now, ";
        std::cout << "take care of source code!\n";
        dump_formating_node_info(root, "program root");
        bool is_use_statement = true;
        for (auto i : root->get_expressions()) {
            if (is_use_statement && i->get_type() != expr_type::ast_use) {
                is_use_statement = false;
                out << "\n";
            }

            i->accept(this);
            if (need_dump_semi(i)) {
                out << ";\n";
            } else {
                out << "\n";
            }
        }
    }
};

}
