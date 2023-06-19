#include "nasal_new_ast.h"
#include "nasal_new_parse.h"

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
            die(prevspan, "expected \";\"");
        }
    }
    update_location(root);
    return err;
}

void parse::easter_egg() const {
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

void parse::die(const span& loc, std::string info) {
    err.err("parse", loc, info);
}

void parse::match(tok type,const char* info) {
    if (!lookahead(type)) {
        if (info) {
            die(thisspan, info);
            return;
        }
        switch(type) {
            case tok::num:die(thisspan, "expected number");    break;
            case tok::str:die(thisspan, "expected string");    break;
            case tok::id: die(thisspan, "expected identifier");break;
            default:      die(thisspan, "expected '"+tokname.at(type)+"'"); break;
        }
        return;
    }
    if (lookahead(tok::eof)) {
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
            die(prevspan, "expected ',' between scalars");
            return true;
        }
    }
    return false;
}

bool parse::check_tuple() {
    u32 check_ptr=ptr, curve=1, bracket=0, brace=0;
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

bool parse::check_func_end(const ast& node) {
    u32 type=node.type();
    if (type==ast_func) {
        return true;
    } else if (type==ast_num || type==ast_id || type==ast_str || type==ast_nil || type==ast_vec || type==ast_hash) {
        return false;
    }
    if (node.child().empty() || (
        type!=ast_def &&
        type!=ast_equal &&
        type!=ast_addeq &&
        type!=ast_subeq &&
        type!=ast_multeq &&
        type!=ast_diveq &&
        type!=ast_lnkeq)) {
        return false;
    } else {
        return check_func_end(node.child().back());
    }
    return false;
}

bool parse::check_special_call() {
    // special call means like this: function_name(a:1,b:2,c:3);
    u32 check_ptr=ptr, curve=1, bracket=0, brace=0;
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
    auto type=node->get_type();
    if (type==expr_type::ast_for ||
        type==expr_type::ast_forei ||
        type==expr_type::ast_while ||
        type==expr_type::ast_cond) {
        return false;
    }
    return !check_func_end(node);
}

void parse::update_location(expr* node) {
    node->update_location(toks[ptr].loc);
}

null_expr* parse::null() {
    return new null_expr(toks[ptr].loc);
}

nil_expr* parse::nil() {
    return new nil_expr(toks[ptr].loc);
}

number_literal* parse::num() {
    auto node = new number_literal(toks[ptr].loc,
        str2num(toks[ptr].str.c_str()));
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
    match(tok::rbracket, "expected ']' when generating vector");
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
            die(prevspan, "expected ',' between hash members");
        } else {
            break;
        }
    }
    update_location(node);
    match(tok::rbrace, "expected '}' when generating hash");
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
    node->set_element(calc());
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
        param->set_parameter_name(id());
        if (lookahead(tok::eq) || lookahead(tok::ellipsis)) {
            ast special_arg(toks[ptr].loc, ast_null);
            if (lookahead(tok::eq)) {
                match(tok::eq);
                param->set_parameter_type(parameter::param_type::default_parameter);
                param->set_default_value(calc());
            } else {
                match(tok::ellipsis);
                param->set_parameter_type(parameter::param_type::dynamic_parameter);
            }
        } else {
            param->set_parameter_type(parameter::param_type::normal_parameter);
        }
        update_location(param);
        func_node->add_parameter(param);
        if (lookahead(tok::comma)) {
            match(tok::comma);
        } else if (lookahead(tok::id)) { // first set of identifier
            die(prevspan, "expected ',' between identifiers");
        } else {
            break;
        }
    }
    update_location(func_node);
    match(tok::rcurve, "expected ')' after parameter list");
    return;
}

expr* parse::lcurve_expr() {
    if (toks[ptr+1].type==tok::var)
        return definition();
    return check_tuple()?multi_assgin():calc();
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
        case tok::semi: break;
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
                die(prevspan, "expected ';'");
            }
        }
        match(tok::rbrace, "expected '}' when generating expressions");
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
        ast tmp(toks[ptr].loc, ast_trino);
        match(tok::quesmark);
        tmp.add(std::move(node));
        tmp.add(calc());
        match(tok::colon);
        tmp.add(calc());
        node=std::move(tmp);
    } else if (tok::eq<=toks[ptr].type && toks[ptr].type<=tok::lnkeq) {
        // tok::eq~tok::lnkeq is 37 to 42,ast_equal~ast_lnkeq is 21~26
        ast tmp(toks[ptr].loc, (u32)toks[ptr].type-(u32)tok::eq+ast_equal);
        tmp.add(std::move(node));
        match(toks[ptr].type);
        tmp.add(calc());
        node=std::move(tmp);
    } else if (toks[ptr].type==tok::btandeq || toks[ptr].type==tok::btoreq || toks[ptr].type==tok::btxoreq) {
        ast tmp(toks[ptr].loc, (u32)toks[ptr].type-(u32)tok::btandeq+ast_btandeq);
        tmp.add(std::move(node));
        match(toks[ptr].type);
        tmp.add(calc());
        node=std::move(tmp);
    }
    update_location(node);
    return node;
}

binary_operator* parse::bitwise_or() {
    ast node=bitwise_xor();
    while(lookahead(tok::btor)) {
        ast tmp(toks[ptr].loc, ast_bitor);
        tmp.add(std::move(node));
        match(tok::btor);
        tmp.add(bitwise_xor());
        tmp.update_span();
        node=std::move(tmp);
    }
    update_location(node);
    return node;
}

binary_operator* parse::bitwise_xor() {
    ast node=bitwise_and();
    while(lookahead(tok::btxor)) {
        ast tmp(toks[ptr].loc, ast_bitxor);
        tmp.add(std::move(node));
        match(tok::btxor);
        tmp.add(bitwise_and());
        tmp.update_span();
        node=std::move(tmp);
    }
    update_location(node);
    return node;
}

binary_operator* parse::bitwise_and() {
    ast node=or_expr();
    while(lookahead(tok::btand)) {
        ast tmp(toks[ptr].loc, ast_bitand);
        tmp.add(std::move(node));
        match(tok::btand);
        tmp.add(or_expr());
        tmp.update_span();
        node=std::move(tmp);
    }
    update_location(node);
    return node;
}

binary_operator* parse::or_expr() {
    ast node=and_expr();
    while(lookahead(tok::opor)) {
        ast tmp(toks[ptr].loc, ast_or);
        tmp.add(std::move(node));
        match(tok::opor);
        tmp.add(and_expr());
        tmp.update_span();
        node=std::move(tmp);
    }
    update_location(node);
    return node;
}

binary_operator* parse::and_expr() {
    ast node=cmp_expr();
    while(lookahead(tok::opand)) {
        ast tmp(toks[ptr].loc, ast_and);
        tmp.add(std::move(node));
        match(tok::opand);
        tmp.add(cmp_expr());
        tmp.update_span();
        node=std::move(tmp);
    }
    update_location(node);
    return node;
}

binary_operator* parse::cmp_expr() {
    ast node=additive_expr();
    while(tok::cmpeq<=toks[ptr].type && toks[ptr].type<=tok::geq) {
        // tok::cmpeq~tok::geq is 43~48,ast_cmpeq~ast_geq is 27~32
        ast tmp(toks[ptr].loc, (u32)toks[ptr].type-(u32)tok::cmpeq+ast_cmpeq);
        tmp.add(std::move(node));
        match(toks[ptr].type);
        tmp.add(additive_expr());
        tmp.update_span();
        node=std::move(tmp);
    }
    update_location(node);
    return node;
}

binary_operator* parse::additive_expr() {
    ast node=multive_expr();
    while(lookahead(tok::add) || lookahead(tok::sub) || lookahead(tok::floater)) {
        ast tmp(toks[ptr].loc, ast_null);
        switch(toks[ptr].type) {
            case tok::add: tmp.set_type(ast_add);  break;
            case tok::sub: tmp.set_type(ast_sub);  break;
            case tok::floater: tmp.set_type(ast_link); break;
            default: break;
        }
        tmp.add(std::move(node));
        match(toks[ptr].type);
        tmp.add(multive_expr());
        tmp.update_span();
        node=std::move(tmp);
    }
    update_location(node);
    return node;
}

expr* parse::multive_expr() {
    expr* node=(lookahead(tok::sub) || lookahead(tok::opnot) || lookahead(tok::floater))?unary():scalar();
    while(lookahead(tok::mult) || lookahead(tok::div)) {
        auto tmp = new binary_operator(toks[ptr].loc);
        if (lookahead(tok::mult)) {
            tmp->set_type(binary_operator::binary_type::mult);
        } else {
            tmp->set_type(binary_operator::binary_type::div);
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
            node->set_type(unary_operator::unary_type::negative);
            match(tok::sub);
            break;
        case tok::opnot:
            node->set_type(unary_operator::unary_type::logical_not);
            match(tok::opnot);
            break;
        case tok::floater:
            node->set_type(unary_operator::unary_type::bitwise_not);
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
        node=nil();
        match(tok::tknil);
    } else if (lookahead(tok::num)) {
        node=num();
    } else if (lookahead(tok::str)) {
        node=str();
    } else if (lookahead(tok::id)) {
        node=id();
    } else if (lookahead(tok::tktrue) || lookahead(tok::tkfalse)) {
        node=bools();
    } else if (lookahead(tok::func)) {
        node=func();
    } else if (lookahead(tok::lbracket)) {
        node=vec();
    } else if (lookahead(tok::lbrace)) {
        node=hash();
    } else if (lookahead(tok::lcurve)) {
        const auto& loc=toks[ptr].loc;
        match(tok::lcurve);
        node=calc();
        node->set_begin(loc.begin_line, loc.begin_column);
        update_location(node);
        match(tok::rcurve);
    } else if (lookahead(tok::var)) {
        match(tok::var);
        node.set_type(ast_def);
        node.add(id());
        match(tok::eq);
        node.add(calc());
    } else {
        die(thisspan, "expected scalar");
        return node;
    }
    // check call and avoid ambiguous syntax
    if (is_call(toks[ptr].type) && !(lookahead(tok::lcurve) && toks[ptr+1].type==tok::var)) {
        ast tmp=std::move(node);
        node={toks[ptr].loc, ast_call};
        node.add(std::move(tmp));
        while(is_call(toks[ptr].type)) {
            node.add(call_scalar());
        }
    }
    update_location(node);
    return node;
}

expr* parse::call_scalar() {
    switch(toks[ptr].type) {
        case tok::lcurve:   return callf(); break;
        case tok::lbracket: return callv(); break;
        case tok::dot:      return callh(); break;
        default: break;
    }
    // unreachable
    return null();
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
    match(tok::rbracket, "expected ']' when calling vector");
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
    match(tok::rcurve, "expected ')' when calling function");
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
    ast node(toks[ptr].loc, ast_def);
    if (lookahead(tok::var)) {
        match(tok::var);
        switch(toks[ptr].type) {
            case tok::id: node.add(id());break;
            case tok::lcurve: node.add(outcurve_def());break;
            default: die(thisspan, "expected identifier");break;
        }
    } else if (lookahead(tok::lcurve)) {
        node.add(incurve_def());
    }
    match(tok::eq);
    if (lookahead(tok::lcurve)) {
        node.add(check_tuple()?multi_scalar():calc());
    } else {
        node.add(calc());
    }
    update_location(node);
    return node;
}

expr* parse::incurve_def() {
    const auto& loc=toks[ptr].loc;
    match(tok::lcurve);
    match(tok::var);
    ast node=multi_id();
    update_location(node);
    match(tok::rcurve);
    node.set_begin(loc.begin_line, loc.begin_column);
    return node;
}

expr* parse::outcurve_def() {
    const auto& loc=toks[ptr].loc;
    match(tok::lcurve);
    ast node=multi_id();
    update_location(node);
    match(tok::rcurve);
    node.set_begin(loc.begin_line, loc.begin_column);
    return node;
}

expr* parse::multi_id() {
    ast node(toks[ptr].loc, ast_multi_id);
    while(!lookahead(tok::eof)) {
        // only identifier is allowed here
        // but we check it at codegen stage
        node.add(calc());
        if (lookahead(tok::comma)) {
            match(tok::comma);
        } else if (lookahead(tok::id)) { // first set of identifier
            die(prevspan, "expected ',' between identifiers");
        } else {
            break;
        }
    }
    update_location(node);
    return node;
}

expr* parse::multi_scalar() {
    // if check_call_memory is true,we will check if value called here can reach a memory space
    const tok panic[]={
        tok::id,tok::str,tok::num,tok::tktrue,
        tok::tkfalse,tok::opnot,tok::sub,tok::tknil,
        tok::func,tok::var,tok::lcurve,tok::floater,
        tok::lbrace,tok::lbracket,tok::null
    };
    ast node(toks[ptr].loc, ast_tuple);
    match(tok::lcurve);
    while(!lookahead(tok::rcurve)) {
        node.add(calc());
        if (lookahead(tok::comma)) {
            match(tok::comma);
        } else if (lookahead(tok::eof)) {
            break;
        } else if (!lookahead(tok::rcurve) && !check_comma(panic)) {
            break;
        }
    }
    update_location(node);
    match(tok::rcurve, "expected ')' after multi-scalar");
    return node;
}

expr* parse::multi_assgin() {
    ast node(toks[ptr].loc, ast_multi_assign);
    node.add(multi_scalar());
    match(tok::eq);
    if (lookahead(tok::eof)) {
        die(thisspan, "expected value list");
        return node;
    }
    if (lookahead(tok::lcurve)) {
        node.add(check_tuple()?multi_scalar():calc());
    } else {
        node.add(calc());
    }
    update_location(node);
    return node;
}

expr* parse::loop() {
    ++in_loop;
    expr* node = nullptr;
    switch(toks[ptr].type) {
        case tok::rwhile:  node=while_loop(); break;
        case tok::rfor:    node=for_loop();   break;
        case tok::forindex:
        case tok::foreach: node=forei_loop(); break;
        default: break;
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
    match(tok::semi, "expected ';' in for(;;)");
    // conditional expression
    if (lookahead(tok::eof)) {
        die(thisspan, "expected conditional expr");
    }
    if (lookahead(tok::semi)) {
        node->set_condition(null());
    } else {
        node->set_condition(calc());
    }
    match(tok::semi, "expected ';' in for(;;)");
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
            node->set_type(forei_expr::forei_loop_type::forindex);
            match(tok::forindex);
            break;
        case tok::foreach: 
            node->set_type(forei_expr::forei_loop_type::foreach);
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
    match(tok::semi, "expected ';' in foreach/forindex(iter;vector)");
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
    if (lookahead(tok::var)) {
        match(tok::var);
        node.set_type(ast_iter);
        node->set_name(id());
    } else {
        node.set_type(ast_call);
        node.add(id());
        while(is_call(toks[ptr].type)) {
            node.add(call_scalar());
        }
    }
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
    tok type=toks[ptr].type;
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
