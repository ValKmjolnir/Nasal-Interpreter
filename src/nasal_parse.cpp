#include "nasal_ast.h"
#include "nasal_parse.h"

namespace nasal {

const error& parse::compile(const lexer& lexer) {
    toks=lexer.result().data();
    ptr=in_func=in_loop=0;

    root = new code_block(toks[0].loc);

    while(!lookahead(tok::eof)) {
        root->add_expression(expression());
        if (lookahead(tok::semi)) {
            match(tok::semi);
        } else if (need_semi_check(root->get_expressions().back()) && !lookahead(tok::eof)) {
            // the last expression can be recognized without semi
            die(prevspan, "expected \";\" after this token");
        }
    }
    update_location(root);
    return err;
}

void parse::easter_egg() {
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
    if (lookahead(tok::eof)) {
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
            case tok::num:die(thisspan, "expected number");    break;
            case tok::str:die(thisspan, "expected string");    break;
            case tok::id: die(thisspan, "expected identifier");break;
            default:      die(thisspan, "expected \""+tokname.at(type)+"\""); break;
        }
        return;
    }
    next();
}

bool parse::lookahead(tok type) {
    return toks[ptr].type==type;
}

bool parse::is_call(tok type) {
    return type==tok::lcurve || type==tok::lbracket || type==tok::dot;
}

bool parse::check_comma(const tok* panic_set) {
    for(u32 i=0;panic_set[i]!=tok::null;++i) {
        if (lookahead(panic_set[i])) {
            die(prevspan, "expected \",\" between scalars");
            return true;
        }
    }
    return false;
}

bool parse::check_tuple() {
    u64 check_ptr = ptr, curve = 1, bracket = 0, brace = 0;
    while(toks[++check_ptr].type!=tok::eof && curve) {
        switch(toks[check_ptr].type) {
            case tok::lcurve:   ++curve;   break;
            case tok::lbracket: ++bracket; break;
            case tok::lbrace:   ++brace;   break;
            case tok::rcurve:   --curve;   break;
            case tok::rbracket: --bracket; break;
            case tok::rbrace:   --brace;   break;
            default: break;
        }
        if (curve==1 && !bracket && !brace && toks[check_ptr].type==tok::comma) {
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
    auto type = node->get_type();
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
    if (!lookahead(tok::lcurve) || toks[ptr+1].type!=tok::var) {
        return false;
    }
    return toks[ptr+2].type==tok::id && toks[ptr+3].type==tok::comma;
}

bool parse::check_special_call() {
    // special call means like this: function_name(a:1,b:2,c:3);
    u64 check_ptr = ptr, curve = 1, bracket = 0, brace = 0;
    while(toks[++check_ptr].type!=tok::eof && curve) {
        switch(toks[check_ptr].type) {
            case tok::lcurve:   ++curve;  break;
            case tok::lbracket: ++bracket;break;
            case tok::lbrace:   ++brace;  break;
            case tok::rcurve:   --curve;  break;
            case tok::rbracket: --bracket;break;
            case tok::rbrace:   --brace;  break;
            default: break;
        }
        // m?1:0 will be recognized as normal parameter
        if (curve==1 && !bracket && !brace && toks[check_ptr].type==tok::quesmark) {
            return false;
        }
        if (curve==1 && !bracket && !brace && toks[check_ptr].type==tok::colon) {
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
    match(tok::use);
    node->add_path(id());
    while(lookahead(tok::dot)) {
        match(tok::dot);
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
    auto node = new number_literal(toks[ptr].loc,
        str_to_num(toks[ptr].str.c_str()));
    match(tok::num);
    return node;
}

string_literal* parse::str() {
    auto node = new string_literal(toks[ptr].loc, toks[ptr].str);
    match(tok::str);
    return node;
}

identifier* parse::id() {
    auto node = new identifier(toks[ptr].loc, toks[ptr].str);
    match(tok::id);
    return node;
}

bool_literal* parse::bools() {
    auto node = new bool_literal(toks[ptr].loc, toks[ptr].str=="true");
    if (lookahead(tok::tktrue)) {
        match(tok::tktrue);
    } else {
        match(tok::tkfalse);
    }
    return node;
}

vector_expr* parse::vec() {
    // panic set for this token is not ','
    // this is the FIRST set of calculation
    // array end with tok::null=0
    const tok panic[]={
        tok::id,tok::str,tok::num,tok::tktrue,
        tok::tkfalse,tok::opnot,tok::sub,tok::tknil,
        tok::func,tok::var,tok::lcurve,tok::floater,
        tok::lbrace,tok::lbracket,tok::null
    };
    auto node = new vector_expr(toks[ptr].loc);
    match(tok::lbracket);
    while(!lookahead(tok::rbracket)) {
        node->add_element(calc());
        if (lookahead(tok::comma)) {
            match(tok::comma);
        } else if (lookahead(tok::eof)) {
            break;
        } else if (!lookahead(tok::rbracket) && !check_comma(panic)) {
            break;
        }
    }
    update_location(node);
    match(tok::rbracket, "expected \"]\" when generating vector");
    return node;
}

hash_expr* parse::hash() {
    auto node = new hash_expr(toks[ptr].loc);
    match(tok::lbrace);
    while(!lookahead(tok::rbrace)) {
        node->add_member(pair());
        if (lookahead(tok::comma)) {
            match(tok::comma);
        } else if (lookahead(tok::id) || lookahead(tok::str)) { // first set of hashmember
            die(prevspan, "expected \",\" between hash members");
        } else {
            break;
        }
    }
    update_location(node);
    match(tok::rbrace, "expected \"}\" when generating hash");
    return node;
}

hash_pair* parse::pair() {
    auto node = new hash_pair(toks[ptr].loc);
    if (lookahead(tok::id)) {
        node->set_name(toks[ptr].str);
        match(tok::id);
    } else if (lookahead(tok::str)) {
        node->set_name(toks[ptr].str);
        match(tok::str);
    } else {
        match(tok::id, "expected hashmap key");
    }
    match(tok::colon);
    node->set_value(calc());
    update_location(node);
    return node;
}

function* parse::func() {
    ++in_func;
    auto node = new function(toks[ptr].loc);
    match(tok::func);
    if (lookahead(tok::lcurve)) {
        params(node);
    }
    node->set_code_block(expression_block());
    --in_func;
    update_location(node);
    return node;
}

void parse::params(function* func_node) {
    match(tok::lcurve);
    while(!lookahead(tok::rcurve)) {
        auto param = new parameter(toks[ptr].loc);
        param->set_parameter_name(toks[ptr].str);
        match(tok::id);
        if (lookahead(tok::eq)) {
            match(tok::eq);
            param->set_parameter_type(parameter::param_type::default_parameter);
            param->set_default_value(calc());
        } else if (lookahead(tok::ellipsis)) {
            match(tok::ellipsis);
            param->set_parameter_type(parameter::param_type::dynamic_parameter);
        } else {
            param->set_parameter_type(parameter::param_type::normal_parameter);
        }
        update_location(param);
        func_node->add_parameter(param);
        if (lookahead(tok::comma)) {
            match(tok::comma);
        } else if (lookahead(tok::id)) { // first set of identifier
            die(prevspan, "expected \",\" between identifiers");
        } else {
            break;
        }
    }
    update_location(func_node);
    match(tok::rcurve, "expected \")\" after parameter list");
    return;
}

expr* parse::lcurve_expr() {
    if (toks[ptr+1].type==tok::var)
        return definition();
    return check_tuple()? multi_assignment():calc();
}

expr* parse::expression() {
    tok type=toks[ptr].type;
    if ((type==tok::brk || type==tok::cont) && !in_loop) {
        die(thisspan, "must use break/continue in loops");
    }
    if (type==tok::ret && !in_func) {
        die(thisspan, "must use return in functions");
    }
    switch(type) {
        case tok::use: return use_stmt_gen();
        case tok::tknil:
        case tok::num:
        case tok::str:
        case tok::id:
        case tok::tktrue:
        case tok::tkfalse:
        case tok::func:
        case tok::lbracket:
        case tok::lbrace:
        case tok::sub:
        case tok::floater:
        case tok::opnot:   return calc();
        case tok::var:     return definition();
        case tok::lcurve:  return lcurve_expr();
        case tok::rfor:
        case tok::forindex:
        case tok::foreach:
        case tok::rwhile:  return loop();
        case tok::rif:     return cond();
        case tok::cont:    return continue_expression();
        case tok::brk:     return break_expression();
        case tok::ret:     return return_expression();
        case tok::semi:    break;
        default:
            die(thisspan, "incorrect token <"+toks[ptr].str+">");
            next();
            break;
    }

    // unreachable
    return new null_expr(toks[ptr].loc);
}

code_block* parse::expression_block() {
    if (lookahead(tok::eof)) {
        die(thisspan, "expected expression block");
        return new code_block(toks[ptr].loc);
    }
    auto node = new code_block(toks[ptr].loc);
    if (lookahead(tok::lbrace)) {
        match(tok::lbrace);
        while(!lookahead(tok::rbrace) && !lookahead(tok::eof)) {
            node->add_expression(expression());
            if (lookahead(tok::semi)) {
                match(tok::semi);
            } else if (need_semi_check(node->get_expressions().back()) && !lookahead(tok::rbrace)) {
                // the last expression can be recognized without semi
                die(prevspan, "expected \";\" after this token");
            }
        }
        match(tok::rbrace, "expected \"}\" when generating expressions");
    } else {
        node->add_expression(expression());
        if (lookahead(tok::semi)) {
            match(tok::semi);
        }
    }
    update_location(node);
    return node;
}

expr* parse::calc() {
    auto node = bitwise_or();
    if (lookahead(tok::quesmark)) {
        // trinocular calculation
        auto tmp = new ternary_operator(toks[ptr].loc);
        match(tok::quesmark);
        tmp->set_condition(node);
        tmp->set_left(calc());
        match(tok::colon);
        tmp->set_right(calc());
        node = tmp;
    } else if (tok::eq<=toks[ptr].type && toks[ptr].type<=tok::lnkeq) {
        auto tmp = new assignment_expr(toks[ptr].loc);
        switch(toks[ptr].type) {
            case tok::eq: tmp->set_assignment_type(assignment_expr::assign_type::equal); break;
            case tok::addeq: tmp->set_assignment_type(assignment_expr::assign_type::add_equal); break;
            case tok::subeq: tmp->set_assignment_type(assignment_expr::assign_type::sub_equal); break;
            case tok::multeq: tmp->set_assignment_type(assignment_expr::assign_type::mult_equal); break;
            case tok::diveq: tmp->set_assignment_type(assignment_expr::assign_type::div_equal); break;
            case tok::lnkeq: tmp->set_assignment_type(assignment_expr::assign_type::concat_equal); break;
            default: break;
        }
        tmp->set_left(node);
        match(toks[ptr].type);
        tmp->set_right(calc());
        node = tmp;
    } else if (toks[ptr].type==tok::btandeq || toks[ptr].type==tok::btoreq || toks[ptr].type==tok::btxoreq) {
        auto tmp = new assignment_expr(toks[ptr].loc);
        switch(toks[ptr].type) {
            case tok::btandeq: tmp->set_assignment_type(assignment_expr::assign_type::bitwise_and_equal); break;
            case tok::btoreq: tmp->set_assignment_type(assignment_expr::assign_type::bitwise_or_equal); break;
            case tok::btxoreq: tmp->set_assignment_type(assignment_expr::assign_type::bitwise_xor_equal); break;
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
    while(lookahead(tok::btor)) {
        auto tmp = new binary_operator(toks[ptr].loc);
        tmp->set_operator_type(binary_operator::binary_type::bitwise_or);
        tmp->set_left(node);
        match(tok::btor);
        tmp->set_right(bitwise_xor());
        update_location(tmp);
        node = tmp;
    }
    update_location(node);
    return node;
}

expr* parse::bitwise_xor() {
    auto node = bitwise_and();
    while(lookahead(tok::btxor)) {
        auto tmp = new binary_operator(toks[ptr].loc);
        tmp->set_operator_type(binary_operator::binary_type::bitwise_xor);
        tmp->set_left(node);
        match(tok::btxor);
        tmp->set_right(bitwise_and());
        update_location(tmp);
        node = tmp;
    }
    update_location(node);
    return node;
}

expr* parse::bitwise_and() {
    auto node = or_expr();
    while(lookahead(tok::btand)) {
        auto tmp = new binary_operator(toks[ptr].loc);
        tmp->set_operator_type(binary_operator::binary_type::bitwise_and);
        tmp->set_left(node);
        match(tok::btand);
        tmp->set_right(or_expr());
        update_location(tmp);
        node = tmp;
    }
    update_location(node);
    return node;
}

expr* parse::or_expr() {
    auto node = and_expr();
    while(lookahead(tok::opor)) {
        auto tmp = new binary_operator(toks[ptr].loc);
        tmp->set_operator_type(binary_operator::binary_type::condition_or);
        tmp->set_left(node);
        match(tok::opor);
        tmp->set_right(and_expr());
        update_location(tmp);
        node = tmp;
    }
    update_location(node);
    return node;
}

expr* parse::and_expr() {
    auto node = cmp_expr();
    while(lookahead(tok::opand)) {
        auto tmp = new binary_operator(toks[ptr].loc);
        tmp->set_operator_type(binary_operator::binary_type::condition_and);
        tmp->set_left(node);
        match(tok::opand);
        tmp->set_right(cmp_expr());
        update_location(tmp);
        node = tmp;
    }
    update_location(node);
    return node;
}

expr* parse::cmp_expr() {
    auto node = additive_expr();
    while(tok::cmpeq<=toks[ptr].type && toks[ptr].type<=tok::geq) {
        auto tmp = new binary_operator(toks[ptr].loc);
        switch(toks[ptr].type) {
            case tok::cmpeq: tmp->set_operator_type(binary_operator::binary_type::cmpeq); break;
            case tok::neq: tmp->set_operator_type(binary_operator::binary_type::cmpneq); break;
            case tok::less: tmp->set_operator_type(binary_operator::binary_type::less); break;
            case tok::leq: tmp->set_operator_type(binary_operator::binary_type::leq); break;
            case tok::grt: tmp->set_operator_type(binary_operator::binary_type::grt); break;
            case tok::geq: tmp->set_operator_type(binary_operator::binary_type::geq); break;
            default: break;
        }
        tmp->set_left(node);
        match(toks[ptr].type);
        tmp->set_right(additive_expr());
        update_location(tmp);
        node = tmp;
    }
    update_location(node);
    return node;
}

expr* parse::additive_expr() {
    auto node = multive_expr();
    while(lookahead(tok::add) || lookahead(tok::sub) || lookahead(tok::floater)) {
        auto tmp = new binary_operator(toks[ptr].loc);
        switch(toks[ptr].type) {
            case tok::add: tmp->set_operator_type(binary_operator::binary_type::add); break;
            case tok::sub: tmp->set_operator_type(binary_operator::binary_type::sub); break;
            case tok::floater: tmp->set_operator_type(binary_operator::binary_type::concat); break;
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
    expr* node=(lookahead(tok::sub) || lookahead(tok::opnot) || lookahead(tok::floater))?unary():scalar();
    while(lookahead(tok::mult) || lookahead(tok::div)) {
        auto tmp = new binary_operator(toks[ptr].loc);
        if (lookahead(tok::mult)) {
            tmp->set_operator_type(binary_operator::binary_type::mult);
        } else {
            tmp->set_operator_type(binary_operator::binary_type::div);
        }
        tmp->set_left(node);
        match(toks[ptr].type);
        tmp->set_right((lookahead(tok::sub) || lookahead(tok::opnot) || lookahead(tok::floater))?unary():scalar());
        update_location(tmp);
        node = tmp;
    }
    update_location(node);
    return node;
}

unary_operator* parse::unary() {
    auto node = new unary_operator(toks[ptr].loc);
    switch(toks[ptr].type) {
        case tok::sub:
            node->set_operator_type(unary_operator::unary_type::negative);
            match(tok::sub);
            break;
        case tok::opnot:
            node->set_operator_type(unary_operator::unary_type::logical_not);
            match(tok::opnot);
            break;
        case tok::floater:
            node->set_operator_type(unary_operator::unary_type::bitwise_not);
            match(tok::floater);
            break;
        default: break;
    }
    node->set_value((lookahead(tok::sub) || lookahead(tok::opnot) || lookahead(tok::floater))?unary():scalar());
    update_location(node);
    return node;
}

expr* parse::scalar() {
    expr* node = nullptr;
    if (lookahead(tok::tknil)) {
        node = nil();
        match(tok::tknil);
    } else if (lookahead(tok::num)) {
        node = num();
    } else if (lookahead(tok::str)) {
        node = str();
    } else if (lookahead(tok::id)) {
        node = id();
    } else if (lookahead(tok::tktrue) || lookahead(tok::tkfalse)) {
        node = bools();
    } else if (lookahead(tok::func)) {
        node = func();
    } else if (lookahead(tok::lbracket)) {
        node = vec();
    } else if (lookahead(tok::lbrace)) {
        node = hash();
    } else if (lookahead(tok::lcurve)) {
        const auto& loc = toks[ptr].loc;
        match(tok::lcurve);
        node = calc();
        node->set_begin(loc.begin_line, loc.begin_column);
        update_location(node);
        match(tok::rcurve);
    } else if (lookahead(tok::var)) {
        match(tok::var);
        auto def_node = new definition_expr(toks[ptr].loc);
        def_node->set_identifier(id());
        match(tok::eq);
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
        case tok::lcurve:   return callf(); break;
        case tok::lbracket: return callv(); break;
        case tok::dot:      return callh(); break;
        default: break;
    }
    // unreachable
    return new call(toks[ptr].loc, expr_type::ast_null);
}

call_hash* parse::callh() {
    const auto& begin_loc = toks[ptr].loc;
    match(tok::dot);
    auto node = new call_hash(begin_loc, toks[ptr].str);
    update_location(node);
    match(tok::id, "expected hashmap key"); // get key
    return node;
}

call_vector* parse::callv() {
    // panic set for this token is not ','
    // this is the FIRST set of subvec
    // array end with tok::null=0
    const tok panic[]={
        tok::id,tok::str,tok::num,tok::tktrue,
        tok::tkfalse,tok::opnot,tok::sub,tok::tknil,
        tok::func,tok::var,tok::lcurve,tok::floater,
        tok::lbrace,tok::lbracket,tok::colon,tok::null
    };
    auto node = new call_vector(toks[ptr].loc);
    match(tok::lbracket);
    while(!lookahead(tok::rbracket)) {
        node->add_slice(subvec());
        if (lookahead(tok::comma)) {
            match(tok::comma);
        } else if (lookahead(tok::eof)) {
            break;
        } else if (!lookahead(tok::rbracket) && !check_comma(panic)) {
            break;
        }
    }
    if (node->get_slices().size()==0) {
        die(thisspan, "expected index value");
    }
    update_location(node);
    match(tok::rbracket, "expected \"]\" when calling vector");
    return node;
}

call_function* parse::callf() {
    // panic set for this token is not ','
    // this is the FIRST set of calculation/hashmember
    // array end with tok::null=0
    const tok panic[]={
        tok::id,tok::str,tok::num,tok::tktrue,
        tok::tkfalse,tok::opnot,tok::sub,tok::tknil,
        tok::func,tok::var,tok::lcurve,tok::floater,
        tok::lbrace,tok::lbracket,tok::null
    };
    auto node = new call_function(toks[ptr].loc);
    bool special_call=check_special_call();
    match(tok::lcurve);
    while(!lookahead(tok::rcurve)) {
        node->add_argument(special_call?pair():calc());
        if (lookahead(tok::comma))
            match(tok::comma);
        else if (lookahead(tok::eof))
            break;
        else if (!lookahead(tok::rcurve) && !check_comma(panic))
            break;
    }
    update_location(node);
    match(tok::rcurve, "expected \")\" when calling function");
    return node;
}

slice_vector* parse::subvec() {
    auto node = new slice_vector(toks[ptr].loc);
    node->set_begin(lookahead(tok::colon)?nil():calc());
    if (lookahead(tok::colon)) {
        match(tok::colon);
        node->set_end((lookahead(tok::comma) || lookahead(tok::rbracket))?nil():calc());
    }
    update_location(node);
    return node;
}

expr* parse::definition() {
    auto node = new definition_expr(toks[ptr].loc);
    if (lookahead(tok::var)) {
        match(tok::var);
        switch(toks[ptr].type) {
            case tok::id: node->set_identifier(id());break;
            case tok::lcurve: node->set_multi_define(outcurve_def());break;
            default: die(thisspan, "expected identifier");break;
        }
    } else if (lookahead(tok::lcurve)) {
        node->set_multi_define(incurve_def());
    }
    match(tok::eq);
    if (lookahead(tok::lcurve)) {
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
    const auto& loc=toks[ptr].loc;
    match(tok::lcurve);
    match(tok::var);
    auto node = multi_id();
    update_location(node);
    match(tok::rcurve);
    node->set_begin(loc.begin_line, loc.begin_column);
    return node;
}

multi_identifier* parse::outcurve_def() {
    const auto& loc=toks[ptr].loc;
    match(tok::lcurve);
    auto node = multi_id();
    update_location(node);
    match(tok::rcurve);
    node->set_begin(loc.begin_line, loc.begin_column);
    return node;
}

multi_identifier* parse::multi_id() {
    auto node = new multi_identifier(toks[ptr].loc);
    while(!lookahead(tok::eof)) {
        // only identifier is allowed here
        node->add_var(id());
        if (lookahead(tok::comma)) {
            match(tok::comma);
        } else if (lookahead(tok::id)) { // first set of identifier
            die(prevspan, "expected \",\" between identifiers");
        } else {
            break;
        }
    }
    update_location(node);
    return node;
}

tuple_expr* parse::multi_scalar() {
    // if check_call_memory is true,we will check if value called here can reach a memory space
    const tok panic[]={
        tok::id,tok::str,tok::num,tok::tktrue,
        tok::tkfalse,tok::opnot,tok::sub,tok::tknil,
        tok::func,tok::var,tok::lcurve,tok::floater,
        tok::lbrace,tok::lbracket,tok::null
    };
    auto node = new tuple_expr(toks[ptr].loc);
    match(tok::lcurve);
    while(!lookahead(tok::rcurve)) {
        node->add_element(calc());
        if (lookahead(tok::comma)) {
            match(tok::comma);
        } else if (lookahead(tok::eof)) {
            break;
        } else if (!lookahead(tok::rcurve) && !check_comma(panic)) {
            break;
        }
    }
    update_location(node);
    match(tok::rcurve, "expected \")\" after multi-scalar");
    return node;
}

multi_assign* parse::multi_assignment() {
    auto node = new multi_assign(toks[ptr].loc);
    node->set_tuple(multi_scalar());
    match(tok::eq);
    if (lookahead(tok::eof)) {
        die(thisspan, "expected value list");
        return node;
    }
    if (lookahead(tok::lcurve)) {
        node->set_value(check_tuple()?multi_scalar():calc());
    } else {
        node->set_value(calc());
    }
    update_location(node);
    return node;
}

expr* parse::loop() {
    ++in_loop;
    expr* node = nullptr;
    switch(toks[ptr].type) {
        case tok::rwhile:  node = while_loop(); break;
        case tok::rfor:    node = for_loop();   break;
        case tok::forindex:
        case tok::foreach: node = forei_loop(); break;
        default:
            die(thisspan, "unreachable");
            node = null();
            break;
    }
    --in_loop;
    return node;
}

while_expr* parse::while_loop() {
    auto node = new while_expr(toks[ptr].loc);
    match(tok::rwhile);
    match(tok::lcurve);
    node->set_condition(calc());
    match(tok::rcurve);
    node->set_code_block(expression_block());
    update_location(node);
    return node;
}

for_expr* parse::for_loop() {
    auto node = new for_expr(toks[ptr].loc);
    match(tok::rfor);
    match(tok::lcurve);

    // first expression
    if (lookahead(tok::eof)) {
        die(thisspan, "expected definition");
    }
    if (lookahead(tok::semi)) {
        node->set_initial(null());
    } else if (lookahead(tok::var)) {
        node->set_initial(definition());
    } else if (lookahead(tok::lcurve)) {
        node->set_initial(lcurve_expr());
    } else {
        node->set_initial(calc());
    }
    match(tok::semi, "expected \";\" in for(;;)");

    // conditional expression
    if (lookahead(tok::eof)) {
        die(thisspan, "expected conditional expr");
    }
    if (lookahead(tok::semi)) {
        node->set_condition(null());
    } else {
        node->set_condition(calc());
    }
    match(tok::semi, "expected \";\" in for(;;)");

    //after loop expression
    if (lookahead(tok::eof)) {
        die(thisspan, "expected calculation");
    }
    if (lookahead(tok::rcurve)) {
        node->set_step(null());
    } else {
        node->set_step(calc());
    }
    match(tok::rcurve);
    node->set_code_block(expression_block());
    update_location(node);
    return node;
}

forei_expr* parse::forei_loop() {
    auto node = new forei_expr(toks[ptr].loc);
    switch(toks[ptr].type) {
        case tok::forindex:
            node->set_loop_type(forei_expr::forei_loop_type::forindex);
            match(tok::forindex);
            break;
        case tok::foreach: 
            node->set_loop_type(forei_expr::forei_loop_type::foreach);
            match(tok::foreach);
            break;
        default: break;
    }
    match(tok::lcurve);
    // first expression
    // foreach/forindex must have an iterator to loop through
    if (!lookahead(tok::var) && !lookahead(tok::id)) {
        die(thisspan, "expected iterator");
    }
    node->set_iterator(iter_gen());
    match(tok::semi, "expected \";\" in foreach/forindex(iter;vector)");
    if (lookahead(tok::eof)) {
        die(thisspan, "expected vector");
    }
    node->set_value(calc());
    match(tok::rcurve);
    node->set_code_block(expression_block());
    update_location(node);
    return node;
}

iter_expr* parse::iter_gen() {
    auto node = new iter_expr(toks[ptr].loc);
    // definition
    if (lookahead(tok::var)) {
        match(tok::var);
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
    match(tok::rif);
    match(tok::lcurve);
    ifnode->set_condition(calc());
    match(tok::rcurve);
    ifnode->set_code_block(expression_block());
    update_location(ifnode);
    node->set_if_statement(ifnode);

    // generate elsif
    while(lookahead(tok::elsif)) {
        auto elsifnode = new if_expr(toks[ptr].loc);
        match(tok::elsif);
        match(tok::lcurve);
        elsifnode->set_condition(calc());
        match(tok::rcurve);
        elsifnode->set_code_block(expression_block());
        update_location(elsifnode);
        node->add_elsif_statement(elsifnode);
    }

    // generate else
    if (lookahead(tok::relse)) {
        auto elsenode = new if_expr(toks[ptr].loc);
        match(tok::relse);
        elsenode->set_code_block(expression_block());
        update_location(elsenode);
        node->set_else_statement(elsenode);
    }
    update_location(node);
    return node;
}

continue_expr* parse::continue_expression() {
    auto node = new continue_expr(toks[ptr].loc);
    match(tok::cont);
    return node;
}

break_expr* parse::break_expression() {
    auto node = new break_expr(toks[ptr].loc);
    match(tok::brk);
    return node;
}

return_expr* parse::return_expression() {
    auto node = new return_expr(toks[ptr].loc);
    match(tok::ret);
    tok type = toks[ptr].type;
    if (type==tok::tknil || type==tok::num ||
        type==tok::str || type==tok::id ||
        type==tok::func || type==tok::sub ||
        type==tok::opnot || type==tok::lcurve ||
        type==tok::lbracket || type==tok::lbrace ||
        type==tok::tktrue || type==tok::tkfalse) {
        node->set_value(calc());
    } else {
        node->set_value(nil());
    }
    update_location(node);
    return node;
}

}
