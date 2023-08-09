#include "nasal_codegen.h"

void codegen::init_file_map(const std::vector<std::string>& file_list) {
    file_map = {};
    for(usize i = 0; i<file_list.size(); ++i) {
        file_map.insert({file_list[i], i});
    }
}

void codegen::load_native_function_table(nasal_builtin_table* table) {
    for(usize i = 0; table[i].func; ++i) {
        if (native_function_mapper.count(table[i].name)) {
            err.err("code", "\"" + std::string(table[i].name) + "\" conflicts.");
            continue;
        }
        native_function.push_back(table[i]);
        auto index = native_function_mapper.size();
        native_function_mapper[table[i].name] = index;
    }
}

void codegen::init_native_function() {
    load_native_function_table(builtin);
    load_native_function_table(io_lib_native);
    load_native_function_table(math_lib_native);
    load_native_function_table(bits_native);
    load_native_function_table(coroutine_native);
    load_native_function_table(flight_gear_native);
    load_native_function_table(dylib_lib_native);
    load_native_function_table(unix_lib_native);
}

void codegen::check_id_exist(identifier* node) {
    const auto& name = node->get_name();
    if (native_function_mapper.count(name)) {
        if (local.empty()) {
            die("native function should not be used in global scope",
                node->get_location());
        }
        return;
    }

    if (local_find(name)>=0) {
        return;
    }
    if (upvalue_find(name)>=0) {
        return;
    }
    if (global_find(name)>=0) {
        return;
    }
    die("undefined symbol \"" + name +
        "\", and this symbol is useless here",
        node->get_location());
}

void codegen::regist_num(const f64 num) {
    if (const_number_map.count(num)) {
        return;
    }
    u32 size = const_number_map.size();
    const_number_map[num] = size;
    const_number_table.push_back(num);
}

void codegen::regist_str(const std::string& str) {
    if (const_string_map.count(str)) {
        return;
    }
    u32 size = const_string_map.size();
    const_string_map[str] = size;
    const_string_table.push_back(str);
}

void codegen::find_symbol(code_block* node) {
    auto finder = std::unique_ptr<symbol_finder>(new symbol_finder);
    for(const auto& i : finder->do_find(node)) {
        if (!experimental_namespace.count(i.file)) {
            experimental_namespace[i.file] = {};
        }
        if (local.empty() && !experimental_namespace.at(i.file).count(i.name)) {
            experimental_namespace.at(i.file).insert(i.name);
        }
        add_symbol(i.name);
    }
}

void codegen::add_symbol(const std::string& name) {
    if (local.empty()) {
        if (global.count(name)) {
            return;
        }
        i32 index = global.size();
        global[name] = index;
        return;
    }
    if (local.back().count(name)) {
        return;
    }
    i32 index = local.back().size();
    local.back()[name] = index;
}

i32 codegen::local_find(const std::string& name) {
    if (local.empty()) {
        return -1;
    }
    return local.back().count(name)? local.back().at(name):-1;
}

i32 codegen::global_find(const std::string& name) {
    return global.count(name)? global.at(name):-1;
}

i32 codegen::upvalue_find(const std::string& name) {
    // 32768 level 65536 upvalues
    i32 index = -1;
    usize size = local.size();
    if (size<=1) {
        return -1;
    }
    auto iter = local.begin();
    for(u32 i = 0; i<size-1; ++i, ++iter) {
        if (iter->count(name)) {
            index = ((i<<16)|(*iter).at(name));
        }
    }
    return index;
}

void codegen::gen(u8 operation_code, u32 num, const span& loc) {
    code.push_back({operation_code,
        static_cast<u16>(file_map.at(loc.file)),
        num, loc.begin_line});
}

void codegen::num_gen(number_literal* node) {
    f64 num = node->get_number();
    regist_num(num);
    gen(op_pnum, const_number_map.at(num), node->get_location());
}

void codegen::str_gen(string_literal* node) {
    const auto& str = node->get_content();
    regist_str(str);
    gen(op_pstr, const_string_map.at(str), node->get_location());
}

void codegen::bool_gen(bool_literal* node) {
    f64 num = node->get_flag()? 1:0;
    regist_num(num);
    gen(op_pnum, const_number_map.at(num), node->get_location());
}

void codegen::vec_gen(vector_expr* node) {
    for(auto child : node->get_elements()) {
        calc_gen(child);
    }
    gen(op_newv, node->get_elements().size(), node->get_location());
}

void codegen::hash_gen(hash_expr* node) {
    gen(op_newh, 0, node->get_location());
    for(auto child : node->get_members()) {
        calc_gen(child->get_value());
        const auto& field_name = child->get_name();
        regist_str(field_name);
        gen(op_happ, const_string_map.at(field_name), child->get_location());
    }
}

void codegen::func_gen(function* node) {
    // parameter list format check
    bool checked_default = false;
    bool checked_dynamic = false;
    std::unordered_map<std::string, bool> argname;
    for(auto tmp : node->get_parameter_list()) {
        if (tmp->get_parameter_type()==
            parameter::param_type::default_parameter) {
            checked_default=true;
        } else if (tmp->get_parameter_type()==
            parameter::param_type::dynamic_parameter) {
            checked_dynamic=true;
        }
        // check default parameter and dynamic parameter
        if (checked_default &&
            tmp->get_parameter_type()!=
            parameter::param_type::default_parameter) {
            die("must use default parameter here",
                tmp->get_location());
        }
        if (checked_dynamic &&
            tmp!=node->get_parameter_list().back()) {
            die("dynamic parameter must be the last one",
                tmp->get_location());
        }
        // check redefinition
        const auto& name = tmp->get_parameter_name();
        if (argname.count(name)) {
            die("redefinition of parameter: " + name,
                tmp->get_location());
        } else {
            argname[name] = true;
        }
    }

    usize newf=code.size();
    gen(op_newf, 0, node->get_location());
    usize lsize=code.size();
    gen(op_intl, 0, node->get_location());
    
    // add special keyword 'me' into symbol table
    // this symbol is only used in local scope(function's scope)
    // this keyword is set to nil as default value
    // after calling a hash, this keyword is set to this hash
    // this symbol's index will be 0
    local.push_back({{"me", 0}});

    // generate parameter list
    for(auto tmp : node->get_parameter_list()) {
        const auto& name = tmp->get_parameter_name();
        if (name=="me") {
            die("\"me\" should not be a parameter",
                tmp->get_location());
        }
        regist_str(name);
        switch(tmp->get_parameter_type()) {
            case parameter::param_type::normal_parameter:
                gen(op_para, const_string_map.at(name), tmp->get_location());
                break;
            case parameter::param_type::default_parameter:
                calc_gen(tmp->get_default_value());
                gen(op_deft, const_string_map.at(name), tmp->get_location());
                break;
            case parameter::param_type::dynamic_parameter:
                gen(op_dyn, const_string_map.at(name), tmp->get_location());
                break;
        }
        add_symbol(name);
    }

    code[newf].num = code.size()+1; // entry
    usize jmp_ptr = code.size();
    gen(op_jmp, 0, node->get_location());

    auto block = node->get_code_block();
    // search symbols first, must use after loading parameters
    // or the location of symbols will change and cause fatal error
    find_symbol(block);
    // add special varibale "arg", which is used to store overflowed args
    // but if dynamic parameter is declared, this variable will be useless
    // for example:
    //     var f = func(a) {print(arg)}
    //     f(1, 2, 3);
    // then the arg is [2, 3], because 1 is accepted by "a"
    // so in fact "f" is the same as:
    //     var f = func(a, arg...) {return(arg)}
    auto arg = std::string("arg");
    // this is used to avoid confliction with defined parameter
    while(local_find(arg)>=0) {
        arg = "0" + arg;
    }
    add_symbol(arg);

    in_loop_level.push_back(0);
    block_gen(block);
    in_loop_level.pop_back();
    code[lsize].num = local.back().size();
    if (local.back().size()>=STACK_DEPTH) {
        die("too many local variants: " +
            std::to_string(local.back().size()), block->get_location());
    }
    local.pop_back();

    if (!block->get_expressions().size() ||
        block->get_expressions().back()->get_type()!=expr_type::ast_ret) {
        gen(op_pnil, 0, block->get_location());
        gen(op_ret, 0, block->get_location());
    }
    code[jmp_ptr].num = code.size();
}

void codegen::call_gen(call_expr* node) {
    calc_gen(node->get_first());
    if (code.back().op==op_callb) {
        return;
    }
    for(auto i : node->get_calls()) {
        switch(i->get_type()) {
            case expr_type::ast_callh: call_hash_gen((call_hash*)i); break;
            case expr_type::ast_callv: call_vector_gen((call_vector*)i); break;
            case expr_type::ast_callf: call_func_gen((call_function*)i); break;
            default: break;
        }
    }
}

void codegen::call_id(identifier* node) {
    const auto& name = node->get_name();
    if (native_function_mapper.count(name)) {
        gen(op_callb,
            static_cast<u32>(native_function_mapper.at(name)),
            node->get_location());
        if (local.empty()) {
            die("should warp native function in local scope",
                node->get_location());
        }
        return;
    }

    i32 index;
    if ((index=local_find(name))>=0) {
        gen(op_calll, index, node->get_location());
        return;
    }
    if ((index=upvalue_find(name))>=0) {
        gen(op_upval, index, node->get_location());
        return;
    }
    if ((index=global_find(name))>=0) {
        gen(op_callg, index, node->get_location());
        return;
    }
    die("undefined symbol \"" + name + "\"", node->get_location());
}

void codegen::call_hash_gen(call_hash* node) {
    regist_str(node->get_field());
    gen(op_callh, const_string_map.at(node->get_field()), node->get_location());
}

void codegen::call_vector_gen(call_vector* node) {
    // maybe this place can use callv-const if ast's first child is ast_num
    if (node->get_slices().size()==1 &&
        !node->get_slices()[0]->get_end()) {
        calc_gen(node->get_slices()[0]->get_begin());
        gen(op_callv, 0, node->get_slices()[0]->get_location());
        return;
    }
    gen(op_slcbeg, 0, node->get_location());
    for(auto tmp : node->get_slices()) {
        if (!tmp->get_end()) {
            calc_gen(tmp->get_begin());
            gen(op_slc, 0, tmp->get_location());
        } else {
            calc_gen(tmp->get_begin());
            calc_gen(tmp->get_end());
            gen(op_slc2, 0, tmp->get_location());
        }
    }
    gen(op_slcend, 0, node->get_location());
}

void codegen::call_func_gen(call_function* node) {
    if (node->get_argument().size() &&
        node->get_argument()[0]->get_type()==expr_type::ast_pair) {
        gen(op_newh, 0, node->get_location());
        for(auto child : node->get_argument()) {
            calc_gen(((hash_pair*)child)->get_value());
            const auto& field_name = ((hash_pair*)child)->get_name();
            regist_str(field_name);
            gen(op_happ, const_string_map.at(field_name), child->get_location());
        }
        gen(op_callfh, 0, node->get_location());
    } else {
        for(auto child : node->get_argument()) {
            calc_gen(child);
        }
        gen(op_callfv, node->get_argument().size(), node->get_location());
    }
}

/* mcall should run after calc_gen because this operation may trigger gc.
*  to avoid gc incorrectly collecting values that include the memory space(which will cause SIGSEGV),
*  we must run ast[1] then we run ast[0] to get the memory space.
*  at this time the value including the memory space can must be found alive.
*  BUT in fact this method does not make much safety.
*  so we use another way to avoid gc-caused SIGSEGV: reserve m-called value on stack.
*  you could see the notes in `vm::opr_mcallv()`.
*/
void codegen::mcall(expr* node) {
    if (node->get_type()!=expr_type::ast_id &&
        node->get_type()!=expr_type::ast_call) {
        die("bad left-value", node->get_location());
        return;
    }
    if (node->get_type()==expr_type::ast_id) {
        mcall_id((identifier*)node);
        return;
    }
    auto call_node = (call_expr*)node;
    calc_gen(call_node->get_first());
    for(usize i = 0; i<call_node->get_calls().size()-1; ++i) {
        auto tmp = call_node->get_calls()[i];
        switch(tmp->get_type()) {
            case expr_type::ast_callh: call_hash_gen((call_hash*)tmp); break;
            case expr_type::ast_callv: call_vector_gen((call_vector*)tmp); break;
            case expr_type::ast_callf: call_func_gen((call_function*)tmp); break;
            default: break;
        }
    }
    auto tmp = call_node->get_calls().back();
    if (tmp->get_type()==expr_type::ast_callh) {
        mcall_hash((call_hash*)tmp);
    } else if (tmp->get_type()==expr_type::ast_callv) {
        mcall_vec((call_vector*)tmp);
    } else if (tmp->get_type()==expr_type::ast_callf) {
        die("bad left-value: function call", tmp->get_location());
    } else {
        die("bad left-value: unknown call", tmp->get_location());
    }
}

void codegen::mcall_id(identifier* node) {
    const auto& name = node->get_name();
    if (native_function_mapper.count(name)) {
        die("cannot modify native function", node->get_location());
        return;
    }

    i32 index;
    if ((index=local_find(name))>=0) {
        gen(op_mcalll, index, node->get_location());
        return;
    }
    if ((index=upvalue_find(name))>=0) {
        gen(op_mupval, index, node->get_location());
        return;
    }
    if ((index=global_find(name))>=0) {
        gen(op_mcallg, index, node->get_location());
        return;
    }
    die("undefined symbol \"" + name + "\"", node->get_location());
}

void codegen::mcall_vec(call_vector* node) {
    if (node->get_slices().size()>1) {
        die("bad left-value: subvec call", node->get_location());
        return;
    }
    auto call = node->get_slices()[0];
    if (call->get_end()) {
        die("bad left-value: subvec call", node->get_location());
        return;
    }
    calc_gen(call->get_begin());
    gen(op_mcallv, 0, node->get_location());
}

void codegen::mcall_hash(call_hash* node) {
    regist_str(node->get_field());
    gen(op_mcallh, const_string_map.at(node->get_field()), node->get_location());
}

void codegen::single_def(definition_expr* node) {
    const auto& str = node->get_variable_name()->get_name();
    calc_gen(node->get_value());
    local.empty()?
        gen(op_loadg, global_find(str), node->get_location()):
        gen(op_loadl, local_find(str), node->get_location());
}

void codegen::multi_def(definition_expr* node) {
    auto& identifiers = node->get_variables()->get_variables();
    usize size = identifiers.size();
    // (var a,b,c) = (c,b,a);
    if (node->get_tuple()) {
        auto& vals = node->get_tuple()->get_elements();
        if (identifiers.size()<vals.size()) {
            die("lack values in multi-definition",
                node->get_tuple()->get_location());
        } else if (identifiers.size()>vals.size()) {
            die("too many values in multi-definition",
                node->get_tuple()->get_location());
        }
        for(usize i = 0; i<size; ++i) {
            calc_gen(vals[i]);
            const auto& name = identifiers[i]->get_name();
            local.empty()?
                gen(op_loadg, global_find(name), identifiers[i]->get_location()):
                gen(op_loadl, local_find(name), identifiers[i]->get_location());
        }
        return;
    }
    // (var a,b,c) = [0,1,2];
    calc_gen(node->get_value());
    for(usize i = 0; i<size; ++i) {
        gen(op_callvi, i, node->get_value()->get_location());
        const auto& name = identifiers[i]->get_name();
        local.empty()?
            gen(op_loadg, global_find(name), identifiers[i]->get_location()):
            gen(op_loadl, local_find(name), identifiers[i]->get_location());
    }
    gen(op_pop, 0, node->get_location());
}

void codegen::def_gen(definition_expr* node) {
    if (node->get_variable_name() && node->get_tuple()) {
        die("cannot accept too many values", node->get_value()->get_location());
    }
    node->get_variable_name()? single_def(node):multi_def(node);
}

void codegen::assignment_expression(assignment_expr* node) {
    switch(node->get_assignment_type()) {
        case assignment_expr::assign_type::equal:
            calc_gen(node->get_right());
            mcall(node->get_left());
            gen(op_meq, 0, node->get_location());
            break;
        case assignment_expr::assign_type::add_equal:
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
            }
            mcall(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                gen(op_addeq, 0, node->get_location());
            } else {
                auto num = ((number_literal*)node->get_right())->get_number();
                regist_num(num);
                gen(op_addeqc, const_number_map[num], node->get_location());
            }
            break;
        case assignment_expr::assign_type::sub_equal:
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
            }
            mcall(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                gen(op_subeq, 0, node->get_location());
            } else {
                auto num = ((number_literal*)node->get_right())->get_number();
                regist_num(num);
                gen(op_subeqc, const_number_map[num], node->get_location());
            }
            break;
        case assignment_expr::assign_type::mult_equal:
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
            }
            mcall(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                gen(op_muleq, 0, node->get_location());
            } else {
                auto num = ((number_literal*)node->get_right())->get_number();
                regist_num(num);
                gen(op_muleqc, const_number_map[num], node->get_location());
            }
            break;
        case assignment_expr::assign_type::div_equal:
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
            }
            mcall(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                gen(op_diveq, 0, node->get_location());
            } else {
                auto num = ((number_literal*)node->get_right())->get_number();
                regist_num(num);
                gen(op_diveqc, const_number_map[num], node->get_location());
            }
            break;
        case assignment_expr::assign_type::concat_equal:
            if (node->get_right()->get_type()!=expr_type::ast_str) {
                calc_gen(node->get_right());
            }
            mcall(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_str) {
                gen(op_lnkeq, 0, node->get_location());
            } else {
                const auto& str = ((string_literal*)node->get_right())->get_content();
                regist_str(str);
                gen(op_lnkeqc, const_string_map[str], node->get_location());
            }
            break;
        case assignment_expr::assign_type::bitwise_and_equal:
            calc_gen(node->get_right());
            mcall(node->get_left());
            gen(op_btandeq, 0, node->get_location());
            break;
        case assignment_expr::assign_type::bitwise_or_equal:
            calc_gen(node->get_right());
            mcall(node->get_left());
            gen(op_btoreq, 0, node->get_location());
            break;
        case assignment_expr::assign_type::bitwise_xor_equal:
            calc_gen(node->get_right());
            mcall(node->get_left());
            gen(op_btxoreq, 0, node->get_location());
            break;
    }
}

void codegen::assignment_statement(assignment_expr* node) {
    switch(node->get_assignment_type()) {
        case assignment_expr::assign_type::equal:
            if (node->get_left()->get_type()==expr_type::ast_id) {
                calc_gen(node->get_right());
                mcall_id((identifier*)node->get_left());
                // only the first mcall_id can use load
                if (code.back().op==op_mcalll) {
                    code.back().op=op_loadl;
                } else if (code.back().op==op_mupval) {
                    code.back().op=op_loadu;
                } else {
                    code.back().op=op_loadg;
                }
            } else {
                calc_gen(node);
                if (code.back().op==op_meq) {
                    code.back().num=1;
                } else {
                    gen(op_pop, 0, node->get_location());
                }
            }
            break;
        case assignment_expr::assign_type::add_equal:
        case assignment_expr::assign_type::sub_equal:
        case assignment_expr::assign_type::mult_equal:
        case assignment_expr::assign_type::div_equal:
        case assignment_expr::assign_type::concat_equal:
        case assignment_expr::assign_type::bitwise_and_equal:
        case assignment_expr::assign_type::bitwise_or_equal:
        case assignment_expr::assign_type::bitwise_xor_equal:
            calc_gen(node);
            if (op_addeq<=code.back().op && code.back().op<=op_btxoreq) {
                code.back().num=1;
            } else if (op_addeqc<=code.back().op && code.back().op<=op_lnkeqc) {
                code.back().op=code.back().op-op_addeqc+op_addecp;
            } else {
                gen(op_pop, 0, node->get_location());
            }
            break;
    }
}

void codegen::multi_assign_gen(multi_assign* node) {
    if (node->get_value()->get_type()==expr_type::ast_tuple &&
        node->get_tuple()->get_elements().size()<((tuple_expr*)node->get_value())->get_elements().size()) {
        die("lack values in multi-assignment", node->get_value()->get_location());
    } else if (node->get_value()->get_type()==expr_type::ast_tuple &&
        node->get_tuple()->get_elements().size()>((tuple_expr*)node->get_value())->get_elements().size()) {
        die("too many values in multi-assignment", node->get_value()->get_location());
    }
    i32 size = node->get_tuple()->get_elements().size();
    if (node->get_value()->get_type()==expr_type::ast_tuple) {
        for(i32 i = size-1; i>=0; --i) {
            calc_gen(((tuple_expr*)node->get_value())->get_elements()[i]);
        }
        auto& tuple = node->get_tuple()->get_elements();
        for(i32 i = 0; i<size; ++i) {
            mcall(tuple[i]);
            // multi assign user loadl and loadg to avoid meq's stack--
            // and this operation changes local and global value directly
            if (code.back().op==op_mcalll) {
                code.back().op=op_loadl;
            } else if (code.back().op==op_mupval) {
                code.back().op=op_loadu;
            } else if (code.back().op==op_mcallg) {
                code.back().op=op_loadg;
            } else {
                gen(op_meq, 1, tuple[i]->get_location());
            }
        }
        return;
    }
    calc_gen(node->get_value());
    auto& tuple = node->get_tuple()->get_elements();
    for(i32 i = 0; i<size; ++i) {
        gen(op_callvi, i, node->get_value()->get_location());
        // multi assign user loadl and loadg to avoid meq's stack--
        // and this operation changes local and global value directly
        mcall(tuple[i]);
        if (code.back().op==op_mcalll) {
            code.back().op=op_loadl;
        } else if (code.back().op==op_mupval) {
            code.back().op=op_loadu;
        } else if (code.back().op==op_mcallg) {
            code.back().op=op_loadg;
        } else {
            gen(op_meq, 1, tuple[i]->get_location());
        }
    }
    gen(op_pop, 0, node->get_location());
}

void codegen::cond_gen(condition_expr* node) {
    std::vector<usize> jmp_label;
    calc_gen(node->get_if_statement()->get_condition());
    auto ptr = code.size();
    gen(op_jf, 0, node->get_if_statement()->get_location());
    block_gen(node->get_if_statement()->get_code_block());
    if (node->get_elsif_stataments().size() ||
        node->get_else_statement()) {
        jmp_label.push_back(code.size());
        gen(op_jmp, 0, node->get_if_statement()->get_location());
    }
    code[ptr].num = code.size();

    for(auto tmp : node->get_elsif_stataments()) {
        calc_gen(tmp->get_condition());
        ptr = code.size();
        gen(op_jf, 0, tmp->get_location());
        block_gen(tmp->get_code_block());
        // the last condition doesn't need to jmp
        if (tmp!=node->get_elsif_stataments().back() ||
            node->get_else_statement()) {
            jmp_label.push_back(code.size());
            gen(op_jmp, 0, tmp->get_location());
        }
        code[ptr].num=code.size();
    }

    if (node->get_else_statement()) {
        block_gen(node->get_else_statement()->get_code_block());
    }
    for(auto i:jmp_label) {
        code[i].num = code.size();
    }
}

void codegen::loop_gen(expr* node) {
    continue_ptr.push_front({});
    break_ptr.push_front({});
    switch(node->get_type()) {
        case expr_type::ast_while:
            while_gen((while_expr*)node); break;
        case expr_type::ast_for:
            for_gen((for_expr*)node); break;
        case expr_type::ast_forei:
            forei_gen((forei_expr*)node); break;
        default: break;
    }
}

void codegen::load_continue_break(i32 continue_place, i32 break_place) {
    for(auto i : continue_ptr.front()) {
        code[i].num = continue_place;
    }
    for(auto i : break_ptr.front()) {
        code[i].num = break_place;
    }
    continue_ptr.pop_front();
    break_ptr.pop_front();
}

void codegen::while_gen(while_expr* node) {
    usize loop_ptr = code.size();
    calc_gen(node->get_condition());
    usize condition_ptr = code.size();
    gen(op_jf, 0, node->get_condition()->get_location());

    block_gen(node->get_code_block());
    gen(op_jmp, loop_ptr, node->get_code_block()->get_location());
    code[condition_ptr].num = code.size();
    load_continue_break(code.size()-1, code.size());
}

void codegen::for_gen(for_expr* node) {
    statement_generation(node->get_initial());
    usize jmp_place = code.size();
    if (node->get_condition()->get_type()==expr_type::ast_null) {
        regist_num(1);
        gen(op_pnum, const_number_map.at(1), node->get_condition()->get_location());
    } else {
        calc_gen(node->get_condition());
    }
    usize label_exit = code.size();
    gen(op_jf, 0, node->get_condition()->get_location());

    block_gen(node->get_code_block());
    usize continue_place = code.size();
    statement_generation(node->get_step());
    gen(op_jmp, jmp_place, node->get_step()->get_location());
    code[label_exit].num = code.size();

    load_continue_break(continue_place, code.size());
}

void codegen::forei_gen(forei_expr* node) {
    calc_gen(node->get_value());
    gen(op_cnt, 0, node->get_value()->get_location());
    usize ptr = code.size();
    if (node->get_loop_type()==forei_expr::forei_loop_type::forindex) {
        gen(op_findex, 0, node->get_location());
    } else {
        gen(op_feach, 0, node->get_location());
    }
    if (node->get_iterator()->get_name()) { // define a new iterator
        auto name_node = node->get_iterator()->get_name();
        const auto& str = name_node->get_name();
        local.empty()?
            gen(op_loadg, global_find(str), name_node->get_location()):
            gen(op_loadl, local_find(str), name_node->get_location());
    } else { // use exist variable as the iterator
        mcall(node->get_iterator()->get_call());
        if (code.back().op==op_mcallg) {
            code.back().op=op_loadg;
        } else if (code.back().op==op_mcalll) {
            code.back().op=op_loadl;
        } else if (code.back().op==op_mupval) {
            code.back().op=op_loadu;
        } else {
            gen(op_meq, 1, node->get_iterator()->get_location());
        }
    }
    ++in_loop_level.back();
    block_gen(node->get_code_block());
    --in_loop_level.back();
    gen(op_jmp, ptr, node->get_location());
    code[ptr].num=code.size();
    load_continue_break(code.size()-1, code.size());
    gen(op_pop, 0, node->get_value()->get_location());// pop vector
    gen(op_pop, 0, node->get_location());// pop iterator
}

void codegen::statement_generation(expr* node) {
    switch(node->get_type()) {
        case expr_type::ast_null: break;
        case expr_type::ast_def:
            def_gen((definition_expr*)node); break;
        case expr_type::ast_multi_assign:
            multi_assign_gen((multi_assign*)node); break;
        case expr_type::ast_assign:
            assignment_statement((assignment_expr*)node); break;
        case expr_type::ast_nil:
        case expr_type::ast_num:
        case expr_type::ast_str:
        case expr_type::ast_bool: break;
        case expr_type::ast_vec:
        case expr_type::ast_hash:
        case expr_type::ast_func:
        case expr_type::ast_call:
        case expr_type::ast_unary:
        case expr_type::ast_binary:
        case expr_type::ast_ternary:
            calc_gen(node);
            gen(op_pop, 0, node->get_location());
            break;
        default: break;
    }
}

void codegen::or_gen(binary_operator* node) {
    calc_gen(node->get_left());
    usize label_jump_true_1 = code.size();
    gen(op_jt, 0, node->get_left()->get_location());

    gen(op_pop, 0, node->get_left()->get_location());
    calc_gen(node->get_right());
    usize label_jump_true_2 = code.size();
    gen(op_jt, 0, node->get_right()->get_location());

    gen(op_pop, 0, node->get_right()->get_location());
    gen(op_pnil, 0, node->get_right()->get_location());

    code[label_jump_true_1].num = code[label_jump_true_2].num = code.size();
}

void codegen::and_gen(binary_operator* node) {
    calc_gen(node->get_left());
    gen(op_jt, code.size()+2, node->get_left()->get_location());

    usize lable_jump_false = code.size();
    gen(op_jmp, 0, node->get_left()->get_location());
    gen(op_pop, 0, node->get_right()->get_location()); // jt jumps here

    calc_gen(node->get_right());
    gen(op_jt, code.size()+3, node->get_right()->get_location());

    code[lable_jump_false].num = code.size();
    gen(op_pop, 0, node->get_right()->get_location());
    gen(op_pnil, 0, node->get_right()->get_location());
    // jt jumps here, avoid pop and pnil
}

void codegen::unary_gen(unary_operator* node) {
    // generate optimized result
    if (node->get_optimized_number()) {
        num_gen(node->get_optimized_number());
        return;
    }

    calc_gen(node->get_value());
    switch(node->get_operator_type()) {
        case unary_operator::unary_type::negative:
            gen(op_usub, 0, node->get_location()); break;
        case unary_operator::unary_type::logical_not:
            gen(op_lnot, 0, node->get_location()); break;
        case unary_operator::unary_type::bitwise_not:
            gen(op_bnot, 0, node->get_location()); break;
    }
}

void codegen::binary_gen(binary_operator* node) {
    // generate optimized result
    if (node->get_optimized_number()) {
        num_gen(node->get_optimized_number());
        return;
    }
    if (node->get_optimized_string()) {
        str_gen(node->get_optimized_string());
        return;
    }

    switch(node->get_operator_type()) {
        case binary_operator::binary_type::condition_or: or_gen(node); return;
        case binary_operator::binary_type::condition_and: and_gen(node); return;
        default: break;
    }
    switch(node->get_operator_type()) {
        case binary_operator::binary_type::cmpeq:
            calc_gen(node->get_left());
            calc_gen(node->get_right());
            gen(op_eq, 0, node->get_location());
            return;
        case binary_operator::binary_type::cmpneq:
            calc_gen(node->get_left());
            calc_gen(node->get_right());
            gen(op_neq, 0, node->get_location());
            return;
        case binary_operator::binary_type::bitwise_or:
            calc_gen(node->get_left());
            calc_gen(node->get_right());
            gen(op_btor, 0, node->get_location());
            return;
        case binary_operator::binary_type::bitwise_xor:
            calc_gen(node->get_left());
            calc_gen(node->get_right());
            gen(op_btxor, 0, node->get_location());
            return;
        case binary_operator::binary_type::bitwise_and:
            calc_gen(node->get_left());
            calc_gen(node->get_right());
            gen(op_btand, 0, node->get_location());
            return;
        default: break;
    }
    switch(node->get_operator_type()) {
        case binary_operator::binary_type::add:
            calc_gen(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
                gen(op_add, 0, node->get_location());
            } else {
                auto num = ((number_literal*)node->get_right())->get_number();
                regist_num(num);
                gen(op_addc, const_number_map.at(num), node->get_location());
            }
            return;
        case binary_operator::binary_type::sub:
            calc_gen(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
                gen(op_sub, 0, node->get_location());
            } else {
                auto num = ((number_literal*)node->get_right())->get_number();
                regist_num(num);
                gen(op_subc, const_number_map.at(num), node->get_location());
            }
            return;
        case binary_operator::binary_type::mult:
            calc_gen(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
                gen(op_mul, 0, node->get_location());
            } else {
                auto num = ((number_literal*)node->get_right())->get_number();
                regist_num(num);
                gen(op_mulc, const_number_map.at(num), node->get_location());
            }
            return;
        case binary_operator::binary_type::div:
            calc_gen(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
                gen(op_div, 0, node->get_location());
            } else {
                auto num = ((number_literal*)node->get_right())->get_number();
                regist_num(num);
                gen(op_divc, const_number_map.at(num), node->get_location());
            }
            return;
        case binary_operator::binary_type::concat:
            calc_gen(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_str) {
                calc_gen(node->get_right());
                gen(op_lnk, 0, node->get_location());
            } else {
                const auto& str = ((string_literal*)node->get_right())->get_content();
                regist_str(str);
                gen(op_lnkc, const_string_map.at(str), node->get_location());
            }
            break;
        case binary_operator::binary_type::less:
            calc_gen(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
                gen(op_less, 0, node->get_location());
            } else {
                auto num = ((number_literal*)node->get_right())->get_number();
                regist_num(num);
                gen(op_lessc, const_number_map.at(num), node->get_location());
            }
            return;
        case binary_operator::binary_type::leq:
            calc_gen(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
                gen(op_leq, 0, node->get_location());
            } else {
                auto num = ((number_literal*)node->get_right())->get_number();
                regist_num(num);
                gen(op_leqc, const_number_map.at(num), node->get_location());
            }
            return;
        case binary_operator::binary_type::grt:
            calc_gen(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
                gen(op_grt, 0, node->get_location());
            } else {
                auto num = ((number_literal*)node->get_right())->get_number();
                regist_num(num);
                gen(op_grtc, const_number_map.at(num), node->get_location());
            }
            return;
        case binary_operator::binary_type::geq:
            calc_gen(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
                gen(op_geq, 0, node->get_location());
            } else {
                auto num = ((number_literal*)node->get_right())->get_number();
                regist_num(num);
                gen(op_geqc, const_number_map.at(num), node->get_location());
            }
            return;
        default: break;
    }
}

void codegen::trino_gen(ternary_operator* node) {
    calc_gen(node->get_condition());
    usize label_jump_false = code.size();
    gen(op_jf, 0, node->get_condition()->get_location());
    calc_gen(node->get_left());
    usize label_jump_to_exit = code.size();
    gen(op_jmp, 0, node->get_left()->get_location());
    code[label_jump_false].num = code.size();
    calc_gen(node->get_right());
    code[label_jump_to_exit].num = code.size();
}

void codegen::calc_gen(expr* node) {
    switch(node->get_type()) {
        case expr_type::ast_nil:
            gen(op_pnil, 0, node->get_location()); break;
        case expr_type::ast_num:
            num_gen((number_literal*)node); break;
        case expr_type::ast_str:
            str_gen((string_literal*)node); break;
        case expr_type::ast_id:
            call_id((identifier*)node); break;
        case expr_type::ast_bool:
            bool_gen((bool_literal*)node); break;
        case expr_type::ast_vec:
            vec_gen((vector_expr*)node); break;
        case expr_type::ast_hash:
            hash_gen((hash_expr*)node); break;
        case expr_type::ast_func:
            func_gen((function*)node); break;
        case expr_type::ast_call:
            call_gen((call_expr*)node); break;
        case expr_type::ast_assign:
            assignment_expression((assignment_expr*)node); break;
        case expr_type::ast_ternary:
            trino_gen((ternary_operator*)node); break;
        case expr_type::ast_unary:
            unary_gen((unary_operator*)node); break;
        case expr_type::ast_binary:
            binary_gen((binary_operator*)node); break;
        case expr_type::ast_def:
            // definition in calculation only should be single def
            single_def((definition_expr*)node);
            call_id(((definition_expr*)node)->get_variable_name());
            break;
        default: break;
    }
}

void codegen::block_gen(code_block* node) {
    for(auto tmp : node->get_expressions()) {
        switch(tmp->get_type()) {
            case expr_type::ast_null: break;
            case expr_type::ast_id:
                check_id_exist((identifier*)tmp); break;
            case expr_type::ast_nil:
            case expr_type::ast_num:
            case expr_type::ast_str:
            case expr_type::ast_bool: break;
            case expr_type::ast_cond:
                cond_gen((condition_expr*)tmp); break;
            case expr_type::ast_continue:
                continue_ptr.front().push_back(code.size());
                gen(op_jmp, 0, tmp->get_location());
                break;
            case expr_type::ast_break:
                break_ptr.front().push_back(code.size());
                gen(op_jmp, 0, tmp->get_location());
                break;
            case expr_type::ast_while:
            case expr_type::ast_for:
            case expr_type::ast_forei:
                loop_gen(tmp); break;
            case expr_type::ast_vec:
            case expr_type::ast_hash:
            case expr_type::ast_func:
            case expr_type::ast_call:
            case expr_type::ast_unary:
            case expr_type::ast_binary:
            case expr_type::ast_ternary:
            case expr_type::ast_def:
            case expr_type::ast_assign:
            case expr_type::ast_multi_assign:
                statement_generation(tmp); break;
            case expr_type::ast_ret:
                ret_gen((return_expr*)tmp); break;
            default: break;
        }
    }
}

void codegen::ret_gen(return_expr* node) {
    for(u32 i = 0; i<in_loop_level.back(); ++i) {
        gen(op_pop, 0, node->get_location());
        gen(op_pop, 0, node->get_location());
    }
    calc_gen(node->get_value());
    gen(op_ret, 0, node->get_location());
}

const error& codegen::compile(parse& parse, linker& import, bool repl) {
    init_native_function();
    init_file_map(import.get_file_list());

    in_loop_level.push_back(0);

    // add special symbol globals, which is a hash stores all global variables
    add_symbol("globals");
    // add special symbol arg here, which is used to store command line args
    add_symbol("arg");

    // search global symbols first
    find_symbol(parse.tree());
    gen(op_intg, repl? STACK_DEPTH/2:global.size(), parse.tree()->get_location());

    // generate main block
    block_gen(parse.tree());
    // generate exit operand, vm stops here
    gen(op_exit, 0, parse.tree()->get_location());

    // size out of bound check
    if (const_number_table.size()>0xffffff) {
        err.err("code",
            "too many constant numbers: " +
            std::to_string(const_number_table.size()));
    }
    if (const_string_table.size()>0xffffff) {
        err.err("code",
            "too many constant strings: " +
            std::to_string(const_string_table.size()));
    }

    // check global variables size
    if (global.size()>=STACK_DEPTH/2) {
        err.err("code",
            "too many global variables: " + std::to_string(global.size()));
    }

    // check generated code size
    if (code.size()>0xffffff) {
        err.err("code",
            "bytecode size overflow: " + std::to_string(code.size()));
    }
    return err;
}

void codegen::print(std::ostream& out) {
    // func end stack, reserved for code print
    std::stack<u32> func_begin_stack;
    std::stack<u32> func_end_stack;

    // print const numbers
    for(auto num : const_number_table) {
        out << "  .number " << num << "\n";
    }

    // print const strings
    for(const auto& str : const_string_table) {
        out << "  .symbol \"" << rawstr(str) << "\"\n";
    }
    
    // print blank line
    if (const_number_table.size() || const_string_table.size()) {
        out << "\n";
    }

    // print code
    codestream::set(
        const_number_table.data(),
        const_string_table.data(),
        native_function.data());
    for(u32 i = 0; i<code.size(); ++i) {
        // print opcode index, opcode name, opcode immediate number
        const auto& c = code[i];
        if (!func_end_stack.empty() && i==func_end_stack.top()) {
            out << std::hex << "<0x" << func_begin_stack.top() << std::dec << ">;\n";
            // avoid two empty lines
            if (c.op!=op_newf) {
                out<<"\n";
            }
            func_begin_stack.pop();
            func_end_stack.pop();
        }

        // get function begin index and end index
        if (c.op==op_newf) {
            out << std::hex << "\nfunc <0x" << i << std::dec << ">:\n";
            for(u32 j = i; j<code.size(); ++j) {
                if (code[j].op==op_jmp) {
                    func_begin_stack.push(i);
                    func_end_stack.push(code[j].num);
                    break;
                }
            }
        }

        // output bytecode
        out << "  " << codestream(c,i) << "\n";
    }
}

void codegen::symbol_dump(std::ostream& out) const {
    for(const auto& domain : experimental_namespace) {
        out << "<" << domain.first << ">\n";
        for(const auto& i : domain.second) {
            out << "  0x" << std::setw(4) << std::setfill('0');
            out << std::hex << global.at(i) << std::dec << " ";
            out << i << std::endl;
        }
    }
}
