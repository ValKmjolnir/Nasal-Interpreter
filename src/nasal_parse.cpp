#include "nasal_ast.h"
#include "nasal_parse.h"
#include "util/util.h"

namespace nasal {

const error& parse::compile(const lexer& lexer) {
    toks = lexer.result().data();
    ptr = in_func_depth = in_loop_depth = 0;

    root = new code_block(toks[0].loc);

    while(!lookahead(tok::tk_eof)) {
        root->add_expression(expression());
        if (lookahead(tok::tk_semi)) {
            match(tok::tk_semi);
        } else if (need_semi_check(root->get_expressions().back()) &&
                   !lookahead(tok::tk_eof)) {
            // the last expression can be recognized without semi
            die(prevspan, "expected \";\" after this token");
        }
    }
    update_location(root);
    return err;
}

void parse::easter_egg() {
    // do you remember this text drawing in old versions?
    std::clog
    << "              _,,,_                                          \n"
    << "            .'     `'.                                       \n"
    << "           /     ____ \\      Fucking Nasal Parser           \n"
    << "           |  .-'_  _\\/    /                                \n"
    << "           \\_/   a  a|    /                                 \n"
    << "           (,`     \\ |         .----.                       \n"
    << "            |     -' |        /|     '--.                    \n"
    << "             \\   '=  /        ||    ]|   `-.                \n"
    << "             /`-.__.'         ||    ]|    ::|                \n"
    << "          .-'`-.__ \\__        ||    ]|    ::|               \n"
    << "         /        ``  `.      ||    ]|    ::|                \n"
    << "       _ |     \\     \\  \\     \\|    ]|   .-'             \n"
    << "      / \\|      \\    |   \\     L.__  .--'(                \n"
    << "     |   |\\      `.  |    \\  ,---|_      \\---------,      \n"
    << "     |   | '.      './\\    \\/ .--._|=-    |_      /|       \n"
    << "     |   \\   '.     `'.'. /`\\/ .-'          '.   / |       \n"
    << "     |   |     `'.     `;-:-;`)|             |-./  |         \n"
    << "     |   /_       `'--./_  ` )/'-------------')/)  |         \n"
    << "     \\   | `\"\"\"\"----\"`\\//`\"\"`/,===..'`````````/ (  |\n"
    << "      |  |            / `---` `==='          /   ) |         \n"
    << "      /  \\           /                      /   (  |        \n"
    << "     |    '------.  |'--------------------'|     ) |         \n"
    << "      \\           `-|                      |    /  |        \n"
    << "       `--...,______|                      |   (   |         \n"
    << "              | |   |                      |    ) ,|         \n"
    << "              | |   |                      |   ( /||         \n"
    << "              | |   |                      |   )/ `\"        \n"
    << "             /   \\  |                      |  (/            \n"
    << "           .' /I\\ '.|                      |  /)            \n"
    << "        .-'_.'/ \\'. |                      | /              \n"
    << "        ```  `\"\"\"` `| .-------------------.||             \n"
    << "                    `\"`                   `\"`              \n";
}

void parse::die(const span& loc, const std::string& info) {
    err.err("parse", loc, info);
}

void parse::next() {
    if (lookahead(tok::tk_eof)) {
        return;
    }
    ++ptr;
}

void parse::match(tok type, const char* info) {
    if (!lookahead(type)) {
        if (info) {
            die(thisspan, info);
            return;
        }
        switch(type) {
            case tok::tk_num: die(thisspan, "expected number"); break;
            case tok::tk_str: die(thisspan, "expected string"); break;
            case tok::tk_id: die(thisspan, "expected identifier"); break;
            default:
                die(thisspan,
                    "expected \"" + token_name_mapper.at(type)+"\""
                );
                break;
        }
        return;
    }
    next();
}

bool parse::lookahead(tok type) {
    return toks[ptr].type==type;
}

bool parse::is_call(tok type) {
    return type==tok::tk_lcurve || type==tok::tk_lbracket ||
           type==tok::tk_dot || type==tok::tk_quesdot;
}

bool parse::check_comma(const tok* panic_set) {
    for(u32 i = 0; panic_set[i]!=tok::tk_null; ++i) {
        if (lookahead(panic_set[i])) {
            die(prevspan, "expected \",\" between scalars");
            return true;
        }
    }
    return false;
}

bool parse::check_tuple() {
    u64 check_ptr = ptr, curve = 1, bracket = 0, brace = 0;
    while(toks[++check_ptr].type!=tok::tk_eof && curve) {
        switch(toks[check_ptr].type) {
            case tok::tk_lcurve:   ++curve;   break;
            case tok::tk_lbracket: ++bracket; break;
            case tok::tk_lbrace:   ++brace;   break;
            case tok::tk_rcurve:   --curve;   break;
            case tok::tk_rbracket: --bracket; break;
            case tok::tk_rbrace:   --brace;   break;
            default: break;
        }
        if (curve==1 && !bracket && !brace &&
            toks[check_ptr].type==tok::tk_comma) {
            return true;
        }
    }
    return false;
}

bool parse::check_func_end(expr* node) {
    // avoid error parse caused nullptr return value
    if (!node) {
        return true;
    }
    const auto type = node->get_type();
    if (type==expr_type::ast_func) {
        return true;
    } else if (type==expr_type::ast_def) {
        return check_func_end(
            reinterpret_cast<definition_expr*>(node)->get_value()
        );
    } else if (type==expr_type::ast_assign) {
        return check_func_end(
            reinterpret_cast<assignment_expr*>(node)->get_right()
        );
    }
    return false;
}

bool parse::check_in_curve_multi_definition() {
    // we do not allow syntax like:
    //   func {}(var a, b, c)
    // but we still allow syntax like:
    //   func {}(var a = 1)
    // in fact, this syntax is not recommended
    if (!lookahead(tok::tk_lcurve) || toks[ptr+1].type!=tok::tk_var) {
        return false;
    }
    return toks[ptr+2].type==tok::tk_id && toks[ptr+3].type==tok::tk_comma;
}

bool parse::check_special_call() {
    // special call means like this: function_name(a:1, b:2, c:3);
    u64 check_ptr = ptr, curve = 1, bracket = 0, brace = 0;
    while(toks[++check_ptr].type!=tok::tk_eof && curve) {
        switch(toks[check_ptr].type) {
            case tok::tk_lcurve:   ++curve;  break;
            case tok::tk_lbracket: ++bracket;break;
            case tok::tk_lbrace:   ++brace;  break;
            case tok::tk_rcurve:   --curve;  break;
            case tok::tk_rbracket: --bracket;break;
            case tok::tk_rbrace:   --brace;  break;
            default: break;
        }
        // m?1:0 will be recognized as normal parameter
        if (curve==1 && !bracket && !brace &&
            toks[check_ptr].type==tok::tk_quesmark) {
            return false;
        }
        if (curve==1 && !bracket && !brace &&
            toks[check_ptr].type==tok::tk_colon) {
            return true;
        }
    }
    return false;
}

bool parse::need_semi_check(expr* node) {
    // avoid error parse caused nullptr return value
    if (!node) {
        return true;
    }
    auto type = node->get_type();
    if (type==expr_type::ast_for ||
        type==expr_type::ast_forei ||
        type==expr_type::ast_while ||
        type==expr_type::ast_cond) {
        return false;
    }
    return !check_func_end(node);
}

void parse::update_location(expr* node) {
    if (!ptr) {
        return;
    }
    node->update_location(toks[ptr-1].loc);
}

use_stmt* parse::use_stmt_gen() {
    auto node = new use_stmt(toks[ptr].loc);
    match(tok::tk_use);
    node->add_path(id());
    while(lookahead(tok::tk_dot)) {
        match(tok::tk_dot);
        node->add_path(id());
    }
    update_location(node);
    return node;
}

null_expr* parse::null() {
    return new null_expr(toks[ptr].loc);
}

nil_expr* parse::nil() {
    return new nil_expr(toks[ptr].loc);
}

number_literal* parse::num() {
    auto node = new number_literal(
        toks[ptr].loc,
        util::str_to_num(toks[ptr].str.c_str()),
        toks[ptr].str
    );
    match(tok::tk_num);
    return node;
}

string_literal* parse::str() {
    auto node = new string_literal(toks[ptr].loc, toks[ptr].str);
    match(tok::tk_str);
    return node;
}

identifier* parse::id() {
    auto node = new identifier(toks[ptr].loc, toks[ptr].str);
    match(tok::tk_id);
    return node;
}

bool_literal* parse::bools() {
    auto node = new bool_literal(toks[ptr].loc, toks[ptr].str=="true");
    if (lookahead(tok::tk_true)) {
        match(tok::tk_true);
    } else {
        match(tok::tk_false);
    }
    return node;
}

vector_expr* parse::vec() {
    // panic set for this token is not ','
    // this is the FIRST set of calculation
    // array end with tok::tk_null = 0
    const tok panic[] = {
        tok::tk_id, tok::tk_str, tok::tk_num, tok::tk_true,
        tok::tk_false, tok::tk_not, tok::tk_sub, tok::tk_nil,
        tok::tk_func, tok::tk_var, tok::tk_lcurve, tok::tk_floater,
        tok::tk_lbrace, tok::tk_lbracket, tok::tk_null
    };
    auto node = new vector_expr(toks[ptr].loc);
    match(tok::tk_lbracket);
    while(!lookahead(tok::tk_rbracket)) {
        node->add_element(calc());
        if (lookahead(tok::tk_comma)) {
            match(tok::tk_comma);
        } else if (lookahead(tok::tk_eof)) {
            break;
        } else if (!lookahead(tok::tk_rbracket) && !check_comma(panic)) {
            break;
        }
    }
    update_location(node);
    match(tok::tk_rbracket, "expected \"]\" when generating vector");
    return node;
}

hash_expr* parse::hash() {
    auto node = new hash_expr(toks[ptr].loc);
    match(tok::tk_lbrace);
    while(!lookahead(tok::tk_rbrace)) {
        node->add_member(pair());
        if (lookahead(tok::tk_comma)) {
            match(tok::tk_comma);
        } else if (lookahead(tok::tk_id) || lookahead(tok::tk_str)) {
            // first set of hashmember
            die(prevspan, "expected \",\" between hash members");
        } else {
            break;
        }
    }
    update_location(node);
    match(tok::tk_rbrace, "expected \"}\" when generating hash");
    return node;
}

hash_pair* parse::pair() {
    auto node = new hash_pair(toks[ptr].loc);
    if (lookahead(tok::tk_id)) {
        node->set_name(toks[ptr].str);
        match(tok::tk_id);
    } else if (lookahead(tok::tk_str)) {
        node->set_name(toks[ptr].str);
        match(tok::tk_str);
    } else {
        match(tok::tk_id, "expected hashmap key");
    }
    match(tok::tk_colon);
    node->set_value(calc());
    update_location(node);
    return node;
}

function* parse::func() {
    ++in_func_depth;
    auto node = new function(toks[ptr].loc);
    match(tok::tk_func);
    if (lookahead(tok::tk_lcurve)) {
        params(node);
    }
    node->set_code_block(expression_block());
    --in_func_depth;
    update_location(node);
    return node;
}

void parse::params(function* func_node) {
    match(tok::tk_lcurve);
    while(!lookahead(tok::tk_rcurve)) {
        auto param = new parameter(toks[ptr].loc);
        param->set_parameter_name(toks[ptr].str);
        match(tok::tk_id);
        if (lookahead(tok::tk_eq)) {
            match(tok::tk_eq);
            param->set_parameter_type(parameter::kind::default_parameter);
            param->set_default_value(calc());
        } else if (lookahead(tok::tk_ellipsis)) {
            match(tok::tk_ellipsis);
            param->set_parameter_type(parameter::kind::dynamic_parameter);
        } else {
            param->set_parameter_type(parameter::kind::normal_parameter);
        }
        update_location(param);
        func_node->add_parameter(param);
        if (lookahead(tok::tk_comma)) {
            match(tok::tk_comma);
        } else if (lookahead(tok::tk_id)) { // first set of identifier
            die(prevspan, "expected \",\" between identifiers");
        } else {
            break;
        }
    }
    update_location(func_node);
    match(tok::tk_rcurve, "expected \")\" after parameter list");
    return;
}

expr* parse::lcurve_expr() {
    if (toks[ptr+1].type==tok::tk_var)
        return definition();
    return check_tuple()? multi_assignment():calc();
}

expr* parse::expression() {
    tok type=toks[ptr].type;
    if ((type==tok::tk_brk || type==tok::tk_cont) && !in_loop_depth) {
        die(thisspan, "must use break/continue in loops");
    }
    if (type==tok::tk_ret && !in_func_depth) {
        die(thisspan, "must use return in functions");
    }
    switch(type) {
        case tok::tk_use: return use_stmt_gen();
        case tok::tk_nil:
        case tok::tk_num:
        case tok::tk_str:
        case tok::tk_id:
        case tok::tk_true:
        case tok::tk_false:
        case tok::tk_func:
        case tok::tk_lbracket:
        case tok::tk_lbrace:
        case tok::tk_sub:
        case tok::tk_floater:
        case tok::tk_not: return calc();
        case tok::tk_var: return definition();
        case tok::tk_lcurve: return lcurve_expr();
        case tok::tk_for:
        case tok::tk_forindex:
        case tok::tk_foreach:
        case tok::tk_while: return loop();
        case tok::tk_if: return cond();
        case tok::tk_cont: return continue_expression();
        case tok::tk_brk: return break_expression();
        case tok::tk_ret: return return_expression();
        case tok::tk_semi: break;
        default:
            die(thisspan, "incorrect token <"+toks[ptr].str+">");
            next();
            break;
    }

    // unreachable
    return new null_expr(toks[ptr].loc);
}

code_block* parse::expression_block() {
    if (lookahead(tok::tk_eof)) {
        die(thisspan, "expected expression block");
        return new code_block(toks[ptr].loc);
    }
    auto node = new code_block(toks[ptr].loc);
    if (lookahead(tok::tk_lbrace)) {
        match(tok::tk_lbrace);
        while(!lookahead(tok::tk_rbrace) && !lookahead(tok::tk_eof)) {
            node->add_expression(expression());
            if (lookahead(tok::tk_semi)) {
                match(tok::tk_semi);
            } else if (need_semi_check(node->get_expressions().back()) &&
                       !lookahead(tok::tk_rbrace)) {
                // the last expression can be recognized without semi
                die(prevspan, "expected \";\" after this token");
            }
        }
        match(tok::tk_rbrace, "expected \"}\" when generating expressions");
    } else {
        node->add_expression(expression());
        if (lookahead(tok::tk_semi)) {
            match(tok::tk_semi);
        }
    }
    update_location(node);
    return node;
}

expr* parse::calc() {
    auto node = bitwise_or();
    if (lookahead(tok::tk_quesmark)) {
        // trinocular calculation
        auto tmp = new ternary_operator(toks[ptr].loc);
        match(tok::tk_quesmark);
        tmp->set_condition(node);
        tmp->set_left(calc());
        match(tok::tk_colon);
        tmp->set_right(calc());
        node = tmp;
    } else if (tok::tk_eq<=toks[ptr].type && toks[ptr].type<=tok::tk_lnkeq) {
        auto tmp = new assignment_expr(toks[ptr].loc);
        switch(toks[ptr].type) {
            case tok::tk_eq: tmp->set_assignment_type(assignment_expr::kind::equal); break;
            case tok::tk_addeq: tmp->set_assignment_type(assignment_expr::kind::add_equal); break;
            case tok::tk_subeq: tmp->set_assignment_type(assignment_expr::kind::sub_equal); break;
            case tok::tk_multeq: tmp->set_assignment_type(assignment_expr::kind::mult_equal); break;
            case tok::tk_diveq: tmp->set_assignment_type(assignment_expr::kind::div_equal); break;
            case tok::tk_lnkeq: tmp->set_assignment_type(assignment_expr::kind::concat_equal); break;
            default: break;
        }
        tmp->set_left(node);
        match(toks[ptr].type);
        tmp->set_right(calc());
        node = tmp;
    } else if (toks[ptr].type==tok::tk_btandeq ||
               toks[ptr].type==tok::tk_btoreq ||
               toks[ptr].type==tok::tk_btxoreq) {
        auto tmp = new assignment_expr(toks[ptr].loc);
        switch(toks[ptr].type) {
            case tok::tk_btandeq: tmp->set_assignment_type(assignment_expr::kind::bitwise_and_equal); break;
            case tok::tk_btoreq: tmp->set_assignment_type(assignment_expr::kind::bitwise_or_equal); break;
            case tok::tk_btxoreq: tmp->set_assignment_type(assignment_expr::kind::bitwise_xor_equal); break;
            default: break;
        }
        tmp->set_left(node);
        match(toks[ptr].type);
        tmp->set_right(calc());
        node = tmp;
    }
    update_location(node);
    return node;
}

expr* parse::bitwise_or() {
    auto node = bitwise_xor();
    while(lookahead(tok::tk_btor)) {
        auto tmp = new binary_operator(toks[ptr].loc);
        tmp->set_operator_type(binary_operator::kind::bitwise_or);
        tmp->set_left(node);
        match(tok::tk_btor);
        tmp->set_right(bitwise_xor());
        update_location(tmp);
        node = tmp;
    }
    update_location(node);
    return node;
}

expr* parse::bitwise_xor() {
    auto node = bitwise_and();
    while(lookahead(tok::tk_btxor)) {
        auto tmp = new binary_operator(toks[ptr].loc);
        tmp->set_operator_type(binary_operator::kind::bitwise_xor);
        tmp->set_left(node);
        match(tok::tk_btxor);
        tmp->set_right(bitwise_and());
        update_location(tmp);
        node = tmp;
    }
    update_location(node);
    return node;
}

expr* parse::bitwise_and() {
    auto node = or_expr();
    while(lookahead(tok::tk_btand)) {
        auto tmp = new binary_operator(toks[ptr].loc);
        tmp->set_operator_type(binary_operator::kind::bitwise_and);
        tmp->set_left(node);
        match(tok::tk_btand);
        tmp->set_right(or_expr());
        update_location(tmp);
        node = tmp;
    }
    update_location(node);
    return node;
}

expr* parse::or_expr() {
    auto node = and_expr();
    while(lookahead(tok::tk_or)) {
        auto tmp = new binary_operator(toks[ptr].loc);
        tmp->set_operator_type(binary_operator::kind::condition_or);
        tmp->set_left(node);
        match(tok::tk_or);
        tmp->set_right(and_expr());
        update_location(tmp);
        node = tmp;
    }
    update_location(node);
    return node;
}

expr* parse::and_expr() {
    auto node = cmp_expr();
    while(lookahead(tok::tk_and)) {
        auto tmp = new binary_operator(toks[ptr].loc);
        tmp->set_operator_type(binary_operator::kind::condition_and);
        tmp->set_left(node);
        match(tok::tk_and);
        tmp->set_right(cmp_expr());
        update_location(tmp);
        node = tmp;
    }
    update_location(node);
    return node;
}

expr* parse::cmp_expr() {
    auto node = null_chain_expr();
    while(tok::tk_cmpeq<=toks[ptr].type && toks[ptr].type<=tok::tk_geq) {
        auto tmp = new binary_operator(toks[ptr].loc);
        switch(toks[ptr].type) {
            case tok::tk_cmpeq: tmp->set_operator_type(binary_operator::kind::cmpeq); break;
            case tok::tk_neq: tmp->set_operator_type(binary_operator::kind::cmpneq); break;
            case tok::tk_less: tmp->set_operator_type(binary_operator::kind::less); break;
            case tok::tk_leq: tmp->set_operator_type(binary_operator::kind::leq); break;
            case tok::tk_grt: tmp->set_operator_type(binary_operator::kind::grt); break;
            case tok::tk_geq: tmp->set_operator_type(binary_operator::kind::geq); break;
            default: break;
        }
        tmp->set_left(node);
        match(toks[ptr].type);
        tmp->set_right(null_chain_expr());
        update_location(tmp);
        node = tmp;
    }
    update_location(node);
    return node;
}

expr* parse::null_chain_expr() {
    auto node = additive_expr();
    while(lookahead(tok::tk_quesques)) {
        auto tmp = new binary_operator(toks[ptr].loc);
        tmp->set_operator_type(binary_operator::kind::null_chain);
        tmp->set_left(node);
        match(tok::tk_quesques);
        tmp->set_right(additive_expr());
        update_location(tmp);
        node = tmp;
    }
    update_location(node);
    return node;
}

expr* parse::additive_expr() {
    auto node = multive_expr();
    while(lookahead(tok::tk_add) ||
          lookahead(tok::tk_sub) ||
          lookahead(tok::tk_floater)) {
        auto tmp = new binary_operator(toks[ptr].loc);
        switch(toks[ptr].type) {
            case tok::tk_add: tmp->set_operator_type(binary_operator::kind::add); break;
            case tok::tk_sub: tmp->set_operator_type(binary_operator::kind::sub); break;
            case tok::tk_floater: tmp->set_operator_type(binary_operator::kind::concat); break;
            default: break;
        }
        tmp->set_left(node);
        match(toks[ptr].type);
        tmp->set_right(multive_expr());
        update_location(tmp);
        node = tmp;
    }
    update_location(node);
    return node;
}

expr* parse::multive_expr() {
    expr* node=(lookahead(tok::tk_sub) ||
                lookahead(tok::tk_not) ||
                lookahead(tok::tk_floater))? unary():scalar();
    while(lookahead(tok::tk_mult) || lookahead(tok::tk_div)) {
        auto tmp = new binary_operator(toks[ptr].loc);
        if (lookahead(tok::tk_mult)) {
            tmp->set_operator_type(binary_operator::kind::mult);
        } else {
            tmp->set_operator_type(binary_operator::kind::div);
        }
        tmp->set_left(node);
        match(toks[ptr].type);
        tmp->set_right(
            (lookahead(tok::tk_sub) ||
             lookahead(tok::tk_not) ||
             lookahead(tok::tk_floater))? unary():scalar()
        );
        update_location(tmp);
        node = tmp;
    }
    update_location(node);
    return node;
}

unary_operator* parse::unary() {
    auto node = new unary_operator(toks[ptr].loc);
    switch(toks[ptr].type) {
        case tok::tk_sub:
            node->set_operator_type(unary_operator::kind::negative);
            match(tok::tk_sub);
            break;
        case tok::tk_not:
            node->set_operator_type(unary_operator::kind::logical_not);
            match(tok::tk_not);
            break;
        case tok::tk_floater:
            node->set_operator_type(unary_operator::kind::bitwise_not);
            match(tok::tk_floater);
            break;
        default: break;
    }
    node->set_value(
        (lookahead(tok::tk_sub) ||
         lookahead(tok::tk_not) ||
         lookahead(tok::tk_floater))? unary():scalar()
    );
    update_location(node);
    return node;
}

expr* parse::scalar() {
    expr* node = nullptr;
    if (lookahead(tok::tk_nil)) {
        node = nil();
        match(tok::tk_nil);
    } else if (lookahead(tok::tk_num)) {
        node = num();
    } else if (lookahead(tok::tk_str)) {
        node = str();
    } else if (lookahead(tok::tk_id)) {
        node = id();
    } else if (lookahead(tok::tk_true) || lookahead(tok::tk_false)) {
        node = bools();
    } else if (lookahead(tok::tk_func)) {
        node = func();
    } else if (lookahead(tok::tk_lbracket)) {
        node = vec();
    } else if (lookahead(tok::tk_lbrace)) {
        node = hash();
    } else if (lookahead(tok::tk_lcurve)) {
        const auto& loc = toks[ptr].loc;
        match(tok::tk_lcurve);
        node = calc();
        node->set_begin(loc.begin_line, loc.begin_column);
        update_location(node);
        match(tok::tk_rcurve);
    } else if (lookahead(tok::tk_var)) {
        match(tok::tk_var);
        auto def_node = new definition_expr(toks[ptr].loc);
        def_node->set_identifier(id());
        match(tok::tk_eq);
        def_node->set_value(calc());
        node = def_node;
    } else {
        die(thisspan, "expected scalar");
        return null();
    }
    // check call and avoid ambiguous syntax:
    //   var f = func(){}
    //   (var a, b, c) = (1, 2, 3);
    // will be incorrectly recognized like:
    //   var f = func(){}(var a, b, c)
    if (is_call(toks[ptr].type) && !check_in_curve_multi_definition()) {
        auto call_node = new call_expr(toks[ptr].loc);
        call_node->set_first(node);
        while(is_call(toks[ptr].type)) {
            call_node->add_call(call_scalar());
        }
        node = call_node;
    }
    update_location(node);
    return node;
}

call* parse::call_scalar() {
    switch(toks[ptr].type) {
        case tok::tk_lcurve:   return callf(); break;
        case tok::tk_lbracket: return callv(); break;
        case tok::tk_dot:      return callh(); break;
        case tok::tk_quesdot:  return null_access_call(); break;
        default: break;
    }
    // unreachable
    return new call(toks[ptr].loc, expr_type::ast_null);
}

call_hash* parse::callh() {
    const auto& begin_loc = toks[ptr].loc;
    match(tok::tk_dot);
    auto node = new call_hash(begin_loc, toks[ptr].str);
    update_location(node);
    match(tok::tk_id, "expected hashmap key"); // get key
    return node;
}

null_access* parse::null_access_call() {
    const auto& begin_loc = toks[ptr].loc;
    match(tok::tk_quesdot);
    auto node = new null_access(begin_loc, toks[ptr].str);
    update_location(node);
    match(tok::tk_id, "expected hashmap key"); // get key
    return node;
}

call_vector* parse::callv() {
    // panic set for this token is not ','
    // this is the FIRST set of subvec
    // array end with tok::tk_null = 0
    const tok panic[] = {
        tok::tk_id, tok::tk_str, tok::tk_num, tok::tk_true,
        tok::tk_false, tok::tk_not, tok::tk_sub, tok::tk_nil,
        tok::tk_func, tok::tk_var, tok::tk_lcurve, tok::tk_floater,
        tok::tk_lbrace, tok::tk_lbracket, tok::tk_colon, tok::tk_null
    };
    auto node = new call_vector(toks[ptr].loc);
    match(tok::tk_lbracket);
    while(!lookahead(tok::tk_rbracket)) {
        node->add_slice(subvec());
        if (lookahead(tok::tk_comma)) {
            match(tok::tk_comma);
        } else if (lookahead(tok::tk_eof)) {
            break;
        } else if (!lookahead(tok::tk_rbracket) && !check_comma(panic)) {
            break;
        }
    }
    if (node->get_slices().size()==0) {
        die(thisspan, "expected index value");
    }
    update_location(node);
    match(tok::tk_rbracket, "expected \"]\" when calling vector");
    return node;
}

call_function* parse::callf() {
    // panic set for this token is not ','
    // this is the FIRST set of calculation/hashmember
    // array end with tok::tk_null = 0
    const tok panic[] = {
        tok::tk_id, tok::tk_str, tok::tk_num, tok::tk_true,
        tok::tk_false, tok::tk_not, tok::tk_sub, tok::tk_nil,
        tok::tk_func, tok::tk_var, tok::tk_lcurve, tok::tk_floater,
        tok::tk_lbrace, tok::tk_lbracket, tok::tk_null
    };
    auto node = new call_function(toks[ptr].loc);
    bool special_call=check_special_call();
    match(tok::tk_lcurve);
    while(!lookahead(tok::tk_rcurve)) {
        node->add_argument(special_call?pair():calc());
        if (lookahead(tok::tk_comma))
            match(tok::tk_comma);
        else if (lookahead(tok::tk_eof))
            break;
        else if (!lookahead(tok::tk_rcurve) && !check_comma(panic))
            break;
    }
    update_location(node);
    match(tok::tk_rcurve, "expected \")\" when calling function");
    return node;
}

slice_vector* parse::subvec() {
    auto node = new slice_vector(toks[ptr].loc);
    node->set_begin(lookahead(tok::tk_colon)?nil():calc());
    if (lookahead(tok::tk_colon)) {
        match(tok::tk_colon);
        node->set_end(
            (lookahead(tok::tk_comma) || lookahead(tok::tk_rbracket))?
                nil():
                calc()
        );
    }
    update_location(node);
    return node;
}

expr* parse::definition() {
    auto node = new definition_expr(toks[ptr].loc);
    if (lookahead(tok::tk_var)) {
        match(tok::tk_var);
        switch(toks[ptr].type) {
            case tok::tk_id: node->set_identifier(id());break;
            case tok::tk_lcurve: node->set_multi_define(outcurve_def());break;
            default: die(thisspan, "expected identifier");break;
        }
    } else if (lookahead(tok::tk_lcurve)) {
        node->set_multi_define(incurve_def());
    }
    match(tok::tk_eq);
    if (lookahead(tok::tk_lcurve)) {
        check_tuple()?
            node->set_tuple(multi_scalar()):
            node->set_value(calc());
    } else {
        node->set_value(calc());
    }
    update_location(node);
    return node;
}

multi_identifier* parse::incurve_def() {
    const auto& loc = toks[ptr].loc;
    match(tok::tk_lcurve);
    match(tok::tk_var);
    auto node = multi_id();
    update_location(node);
    match(tok::tk_rcurve);
    node->set_begin(loc.begin_line, loc.begin_column);
    return node;
}

multi_identifier* parse::outcurve_def() {
    const auto& loc = toks[ptr].loc;
    match(tok::tk_lcurve);
    auto node = multi_id();
    update_location(node);
    match(tok::tk_rcurve);
    node->set_begin(loc.begin_line, loc.begin_column);
    return node;
}

multi_identifier* parse::multi_id() {
    auto node = new multi_identifier(toks[ptr].loc);
    while(!lookahead(tok::tk_eof)) {
        // only identifier is allowed here
        node->add_var(id());
        if (lookahead(tok::tk_comma)) {
            match(tok::tk_comma);
        } else if (lookahead(tok::tk_id)) { // first set of identifier
            die(prevspan, "expected \",\" between identifiers");
        } else {
            break;
        }
    }
    update_location(node);
    return node;
}

tuple_expr* parse::multi_scalar() {
    // if check_call_memory is true,
    // we will check if value called here can reach a memory space
    const tok panic[] = {
        tok::tk_id, tok::tk_str, tok::tk_num, tok::tk_true,
        tok::tk_false, tok::tk_not, tok::tk_sub, tok::tk_nil,
        tok::tk_func, tok::tk_var, tok::tk_lcurve, tok::tk_floater,
        tok::tk_lbrace, tok::tk_lbracket, tok::tk_null
    };
    auto node = new tuple_expr(toks[ptr].loc);
    match(tok::tk_lcurve);
    while(!lookahead(tok::tk_rcurve)) {
        node->add_element(calc());
        if (lookahead(tok::tk_comma)) {
            match(tok::tk_comma);
        } else if (lookahead(tok::tk_eof)) {
            break;
        } else if (!lookahead(tok::tk_rcurve) && !check_comma(panic)) {
            break;
        }
    }
    update_location(node);
    match(tok::tk_rcurve, "expected \")\" after multi-scalar");
    return node;
}

multi_assign* parse::multi_assignment() {
    auto node = new multi_assign(toks[ptr].loc);
    node->set_tuple(multi_scalar());
    match(tok::tk_eq);
    if (lookahead(tok::tk_eof)) {
        die(thisspan, "expected value list");
        return node;
    }
    if (lookahead(tok::tk_lcurve)) {
        node->set_value(check_tuple()?multi_scalar():calc());
    } else {
        node->set_value(calc());
    }
    update_location(node);
    return node;
}

expr* parse::loop() {
    ++in_loop_depth;
    expr* node = nullptr;
    switch(toks[ptr].type) {
        case tok::tk_while:   node = while_loop(); break;
        case tok::tk_for:     node = for_loop();   break;
        case tok::tk_forindex:
        case tok::tk_foreach: node = forei_loop(); break;
        default:
            die(thisspan, "unreachable");
            node = null();
            break;
    }
    --in_loop_depth;
    return node;
}

while_expr* parse::while_loop() {
    auto node = new while_expr(toks[ptr].loc);
    match(tok::tk_while);
    match(tok::tk_lcurve);
    node->set_condition(calc());
    match(tok::tk_rcurve);
    node->set_code_block(expression_block());
    update_location(node);
    return node;
}

for_expr* parse::for_loop() {
    auto node = new for_expr(toks[ptr].loc);
    match(tok::tk_for);
    match(tok::tk_lcurve);

    // first expression
    if (lookahead(tok::tk_eof)) {
        die(thisspan, "expected definition");
    }
    if (lookahead(tok::tk_semi)) {
        node->set_initial(null());
    } else if (lookahead(tok::tk_var)) {
        node->set_initial(definition());
    } else if (lookahead(tok::tk_lcurve)) {
        node->set_initial(lcurve_expr());
    } else {
        node->set_initial(calc());
    }
    match(tok::tk_semi, "expected \";\" in for(;;)");

    // conditional expression
    if (lookahead(tok::tk_eof)) {
        die(thisspan, "expected conditional expr");
    }
    if (lookahead(tok::tk_semi)) {
        node->set_condition(null());
    } else {
        node->set_condition(calc());
    }
    match(tok::tk_semi, "expected \";\" in for(;;)");

    //after loop expression
    if (lookahead(tok::tk_eof)) {
        die(thisspan, "expected calculation");
    }
    if (lookahead(tok::tk_rcurve)) {
        node->set_step(null());
    } else {
        node->set_step(calc());
    }
    match(tok::tk_rcurve);
    node->set_code_block(expression_block());
    update_location(node);
    return node;
}

forei_expr* parse::forei_loop() {
    auto node = new forei_expr(toks[ptr].loc);
    switch(toks[ptr].type) {
        case tok::tk_forindex:
            node->set_loop_type(forei_expr::kind::forindex);
            match(tok::tk_forindex);
            break;
        case tok::tk_foreach: 
            node->set_loop_type(forei_expr::kind::foreach);
            match(tok::tk_foreach);
            break;
        default: break;
    }
    match(tok::tk_lcurve);
    // first expression
    // foreach/forindex must have an iterator to loop through
    if (!lookahead(tok::tk_var) && !lookahead(tok::tk_id)) {
        die(thisspan, "expected iterator");
    }
    node->set_iterator(iter_gen());
    match(tok::tk_semi, "expected \";\" in foreach/forindex(iter;vector)");
    if (lookahead(tok::tk_eof)) {
        die(thisspan, "expected vector");
    }
    node->set_value(calc());
    match(tok::tk_rcurve);
    node->set_code_block(expression_block());
    update_location(node);
    return node;
}

iter_expr* parse::iter_gen() {
    auto node = new iter_expr(toks[ptr].loc);
    // definition
    if (lookahead(tok::tk_var)) {
        match(tok::tk_var);
        node->set_name(id());
        node->set_is_definition(true);
        update_location(node);
        return node;
    }

    // single symbol call
    auto id_node = id();
    if (!is_call(toks[ptr].type)) {
        node->set_name(id_node);
        update_location(node);
        return node;
    }

    // call expression
    auto tmp = new call_expr(id_node->get_location());
    tmp->set_first(id_node);
    while(is_call(toks[ptr].type)) {
        tmp->add_call(call_scalar());
    }
    node->set_call(tmp);
    update_location(node);
    return node;
}

condition_expr* parse::cond() {
    auto node = new condition_expr(toks[ptr].loc);

    // generate if
    auto ifnode = new if_expr(toks[ptr].loc);
    match(tok::tk_if);
    match(tok::tk_lcurve);
    ifnode->set_condition(calc());
    match(tok::tk_rcurve);
    ifnode->set_code_block(expression_block());
    update_location(ifnode);
    node->set_if_statement(ifnode);

    // generate elsif
    while(lookahead(tok::tk_elsif)) {
        auto elsifnode = new if_expr(toks[ptr].loc);
        match(tok::tk_elsif);
        match(tok::tk_lcurve);
        elsifnode->set_condition(calc());
        match(tok::tk_rcurve);
        elsifnode->set_code_block(expression_block());
        update_location(elsifnode);
        node->add_elsif_statement(elsifnode);
    }

    // generate else
    if (lookahead(tok::tk_else)) {
        auto elsenode = new if_expr(toks[ptr].loc);
        match(tok::tk_else);
        elsenode->set_code_block(expression_block());
        update_location(elsenode);
        node->set_else_statement(elsenode);
    }
    update_location(node);
    return node;
}

continue_expr* parse::continue_expression() {
    auto node = new continue_expr(toks[ptr].loc);
    match(tok::tk_cont);
    return node;
}

break_expr* parse::break_expression() {
    auto node = new break_expr(toks[ptr].loc);
    match(tok::tk_brk);
    return node;
}

return_expr* parse::return_expression() {
    auto node = new return_expr(toks[ptr].loc);
    match(tok::tk_ret);
    tok type = toks[ptr].type;
    if (type==tok::tk_nil || type==tok::tk_num ||
        type==tok::tk_str || type==tok::tk_id ||
        type==tok::tk_func || type==tok::tk_sub ||
        type==tok::tk_not || type==tok::tk_lcurve ||
        type==tok::tk_lbracket || type==tok::tk_lbrace ||
        type==tok::tk_true || type==tok::tk_false) {
        node->set_value(calc());
    } else {
        node->set_value(nil());
    }
    update_location(node);
    return node;
}

}
