#include "nasal_codegen.h"
#include "util/util.h"

namespace nasal {

void codegen::init_file_map(const std::vector<std::string>& file_list) {
    file_map = {};
    for (usize i = 0; i<file_list.size(); ++i) {
        file_map.insert({file_list[i], i});
    }
}

void codegen::load_native_function_table(nasal_builtin_table* table) {
    for (usize i = 0; table[i].func; ++i) {
        // check confliction
        if (native_function_mapper.count(table[i].name)) {
            err.err("code", "\"" + std::string(table[i].name) + "\" conflicts.");
            continue;
        }

        // replace unsafe native functions with redirect function in limit mode
        if (flag_limited_mode && unsafe_system_api.count(table[i].name)) {
            native_function.push_back({"__unsafe_redirect", builtin_unsafe});
        } else {
            native_function.push_back(table[i]);
        }

        // insert into mapper
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
    load_native_function_table(json_lib_native);
    load_native_function_table(regex_lib_native);
    load_native_function_table(subprocess_native);
}

void codegen::check_id_exist(identifier* node) {
    const auto& name = node->get_name();
    if (native_function_mapper.count(name)) {
        if (local.empty()) {
            die("native function should not be used in global scope", node);
        }
        return;
    }

    if (local_symbol_find(name)>=0) {
        return;
    }
    if (upvalue_symbol_find(name)>=0) {
        return;
    }
    if (global_symbol_find(name)>=0) {
        return;
    }
    die("undefined symbol \""
        + name + "\", and this symbol is useless here",
        node
    );
}

void codegen::regist_number(const f64 num) {
    if (const_number_map.count(num)) {
        return;
    }

    auto size = const_number_map.size();
    const_number_map[num] = size;
    const_number_table.push_back(num);
}

void codegen::regist_string(const std::string& str) {
    if (const_string_map.count(str)) {
        return;
    }

    auto size = const_string_map.size();
    const_string_map[str] = size;
    const_string_table.push_back(str);
}

void codegen::find_symbol(code_block* node) {
    auto finder = std::make_unique<symbol_finder>();
    for (const auto& i : finder->do_find(node)) {
        const auto& file = i.pos_node->get_location().file;
        // check if symbol conflicts with native function name
        if (native_function_mapper.count(i.name)) {
            die("symbol conflicts with native function", i.pos_node);
            continue;
        }
        // create new namespace with checking existence of location file
        if (!nasal_namespace.count(file)) {
            nasal_namespace[file] = {};
        }
        // if in global scope, load global symbol into this namespace
        auto& scope = nasal_namespace.at(file);
        if (local.empty() && !scope.count(i.name)) {
            scope.insert(i.name);
        }
        // add symbol for codegen symbol check
        regist_symbol(i.name);
    }
}

void codegen::regist_symbol(const std::string& name) {
    // regist global if local scope list is empty
    if (local.empty()) {
        if (global.count(name)) {
            return;
        }
        auto index = global.size();
        global[name] = index;
        return;
    }

    if (local.back().count(name)) {
        return;
    }
    auto index = local.back().size();
    local.back()[name] = index;
}

i64 codegen::local_symbol_find(const std::string& name) {
    if (local.empty()) {
        return -1;
    }
    return local.back().count(name)? local.back().at(name):-1;
}

i64 codegen::global_symbol_find(const std::string& name) {
    return global.count(name)? global.at(name):-1;
}

i64 codegen::upvalue_symbol_find(const std::string& name) {
    // 32768 level 65536 upvalues
    // may cause some errors if local scope depth is too deep or
    // local scope's symbol list size is greater than 65536,
    // but we check the local size in codegen::func_gen
    i64 index = -1;
    usize size = local.size();
    if (size<=1) {
        return -1;
    }

    auto iter = local.begin();
    for (u64 i = 0; i<size-1; ++i, ++iter) {
        if (iter->count(name)) {
            index = ((i<<16)|(*iter).at(name));
        }
    }
    return index;
}

void codegen::emit(u8 operation_code, u64 immediate_num, const span& location) {
    code.push_back({
        operation_code,
        static_cast<u16>(file_map.at(location.file)),
        immediate_num,
        location.begin_line
    });
}

void codegen::number_gen(number_literal* node) {
    f64 num = node->get_number();
    regist_number(num);
    emit(op_pnum, const_number_map.at(num), node->get_location());
}

void codegen::string_gen(string_literal* node) {
    const auto& str = node->get_content();
    regist_string(str);
    emit(op_pstr, const_string_map.at(str), node->get_location());
}

void codegen::bool_gen(bool_literal* node) {
    f64 num = node->get_flag()? 1:0;
    regist_number(num);
    emit(op_pnum, const_number_map.at(num), node->get_location());
}

void codegen::vector_gen(vector_expr* node) {
    for (auto child : node->get_elements()) {
        calc_gen(child);
    }
    emit(op_newv, node->get_elements().size(), node->get_location());
}

void codegen::hash_gen(hash_expr* node) {
    emit(op_newh, 0, node->get_location());
    for (auto child : node->get_members()) {
        calc_gen(child->get_value());
        const auto& field_name = child->get_name();
        regist_string(field_name);
        emit(op_happ, const_string_map.at(field_name), child->get_location());
    }
}

void codegen::func_gen(function* node) {
    // parameter list format check
    bool checked_default = false;
    bool checked_dynamic = false;
    std::unordered_map<std::string, bool> argname;
    for (auto tmp : node->get_parameter_list()) {
        if (tmp->get_parameter_type()==
            parameter::kind::default_parameter) {
            checked_default = true;
        } else if (tmp->get_parameter_type()==
            parameter::kind::dynamic_parameter) {
            checked_dynamic = true;
        }
        // check default parameter and dynamic parameter
        if (checked_default &&
            tmp->get_parameter_type()!=
            parameter::kind::default_parameter) {
            die("must use default parameter here", tmp);
        }
        if (checked_dynamic &&
            tmp!=node->get_parameter_list().back()) {
            die("dynamic parameter must be the last one", tmp);
        }
        // check redefinition
        const auto& name = tmp->get_parameter_name();
        if (argname.count(name)) {
            die("redefinition of parameter: " + name, tmp);
        } else {
            argname[name] = true;
        }
    }

    const auto newf = code.size();
    emit(op_newf, 0, node->get_location());
    const auto lsize = code.size();
    emit(op_intl, 0, node->get_location());
    
    // add special keyword 'me' into symbol table
    // this symbol is only used in local scope(function's scope)
    // this keyword is set to nil as default value
    // after calling a hash, this keyword is set to this hash
    // this symbol's index will be 0
    local.push_back({{"me", 0}});

    // generate parameter list
    for (auto tmp : node->get_parameter_list()) {
        const auto& name = tmp->get_parameter_name();
        if (name=="me") {
            die("\"me\" should not be parameter", tmp);
        }
        regist_string(name);
        switch(tmp->get_parameter_type()) {
            case parameter::kind::normal_parameter:
                emit(op_para, const_string_map.at(name), tmp->get_location());
                break;
            case parameter::kind::default_parameter:
                calc_gen(tmp->get_default_value());
                emit(op_deft, const_string_map.at(name), tmp->get_location());
                break;
            case parameter::kind::dynamic_parameter:
                emit(op_dyn, const_string_map.at(name), tmp->get_location());
                break;
        }
        regist_symbol(name);
    }

    code[newf].num = code.size()+1; // entry
    usize jmp_ptr = code.size();
    emit(op_jmp, 0, node->get_location());

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
    while(local_symbol_find(arg)>=0) {
        arg = "0" + arg;
    }
    regist_symbol(arg);

    // generate code block
    in_foreach_loop_level.push_back(0);
    block_gen(block);
    in_foreach_loop_level.pop_back();

    // we must check the local scope symbol list size
    // the local scope should not cause stack overflow
    // and should not greater than upvalue's max size(65536)
    code[lsize].num = local.back().size();
    if (local.back().size()>=VM_STACK_DEPTH || local.back().size()>=UINT16_MAX) {
        die("too many local variants: " +
            std::to_string(local.back().size()),
            block
        );
    }
    local.pop_back();

    if (!block->get_expressions().size() ||
        block->get_expressions().back()->get_type()!=expr_type::ast_ret) {
        emit(op_pnil, 0, block->get_location());
        emit(op_ret, 0, block->get_location());
    }
    code[jmp_ptr].num = code.size();
}

void codegen::call_gen(call_expr* node) {
    calc_gen(node->get_first());
    if (code.size() && code.back().op==op_callb) {
        return;
    }
    for (auto i : node->get_calls()) {
        switch(i->get_type()) {
            case expr_type::ast_callh:
                call_hash_gen(reinterpret_cast<call_hash*>(i)); break;
            case expr_type::ast_callv:
                call_vector_gen(reinterpret_cast<call_vector*>(i)); break;
            case expr_type::ast_callf:
                call_func_gen(reinterpret_cast<call_function*>(i)); break;
            case expr_type::ast_null_access:
                null_access_gen(reinterpret_cast<null_access*>(i)); break;
            default: break;
        }
    }
}

void codegen::call_identifier(identifier* node) {
    const auto& name = node->get_name();
    if (native_function_mapper.count(name)) {
        emit(op_callb,
            static_cast<u32>(native_function_mapper.at(name)),
            node->get_location()
        );
        if (local.empty()) {
            die("should wrap up native function in local scope", node);
        }
        return;
    }

    i64 index;
    if ((index = local_symbol_find(name))>=0) {
        emit(op_calll, index, node->get_location());
        return;
    }
    if ((index = upvalue_symbol_find(name))>=0) {
        emit(op_upval, index, node->get_location());
        return;
    }
    if ((index = global_symbol_find(name))>=0) {
        emit(op_callg, index, node->get_location());
        return;
    }
    die("undefined symbol \"" + name + "\"", node);
    // generation failed, put a push nil operand here to fill the space
    emit(op_pnil, index, node->get_location());
}

void codegen::call_hash_gen(call_hash* node) {
    regist_string(node->get_field());
    emit(op_callh, const_string_map.at(node->get_field()), node->get_location());
}

void codegen::null_access_gen(null_access* node) {
    regist_string(node->get_field());

    emit(op_dup, 0, node->get_location());
    emit(op_pnil, 0, node->get_location());
    emit(op_eq, 0, node->get_location());

    const auto jmp_false_point = code.size();
    emit(op_jf, 0, node->get_location());

    const auto jmp_direct_point = code.size();
    emit(op_jmp, 0, node->get_location());

    code[jmp_false_point].num = code.size();
    emit(op_callh, const_string_map.at(node->get_field()), node->get_location());
    code[jmp_direct_point].num = code.size();
}

void codegen::call_vector_gen(call_vector* node) {
    // maybe this place can use callv-const if ast's first child is ast_num
    if (node->get_slices().size()==1 &&
        !node->get_slices()[0]->get_end()) {
        calc_gen(node->get_slices()[0]->get_begin());
        emit(op_callv, 0, node->get_slices()[0]->get_location());
        return;
    }
    emit(op_slcbeg, 0, node->get_location());
    for (auto tmp : node->get_slices()) {
        if (!tmp->get_end()) {
            calc_gen(tmp->get_begin());
            emit(op_slc, 0, tmp->get_location());
        } else {
            calc_gen(tmp->get_begin());
            calc_gen(tmp->get_end());
            emit(op_slc2, 0, tmp->get_location());
        }
    }
    emit(op_slcend, 0, node->get_location());
}

void codegen::call_func_gen(call_function* node) {
    if (node->get_argument().size() &&
        node->get_argument()[0]->get_type()==expr_type::ast_pair) {
        emit(op_newh, 0, node->get_location());
        for (auto child : node->get_argument()) {
            auto pair_node = reinterpret_cast<hash_pair*>(child);
            calc_gen(pair_node->get_value());
            const auto& field_name = pair_node->get_name();
            regist_string(field_name);
            emit(op_happ, const_string_map.at(field_name), child->get_location());
        }
        emit(op_callfh, 0, node->get_location());
    } else {
        for (auto child : node->get_argument()) {
            calc_gen(child);
        }
        emit(op_callfv, node->get_argument().size(), node->get_location());
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
        die("bad left-value: cannot get memory space", node);
        return;
    }
    // generate symbol call if node is just ast_id and return
    if (node->get_type()==expr_type::ast_id) {
        mcall_identifier(reinterpret_cast<identifier*>(node));
        return;
    }
    // generate call expression until the last sub-node
    auto call_node = static_cast<call_expr*>(node);
    calc_gen(call_node->get_first());
    for (usize i = 0; i<call_node->get_calls().size()-1; ++i) {
        auto tmp = call_node->get_calls()[i];
        switch(tmp->get_type()) {
            case expr_type::ast_callh:
                call_hash_gen(reinterpret_cast<call_hash*>(tmp)); break;
            case expr_type::ast_callv:
                call_vector_gen(reinterpret_cast<call_vector*>(tmp)); break;
            case expr_type::ast_callf:
                call_func_gen(reinterpret_cast<call_function*>(tmp)); break;
            case expr_type::ast_null_access:
                null_access_gen(reinterpret_cast<null_access*>(tmp)); break;
            default: break;
        }
    }
    // the last sub-node will be used to generate memory call expression
    auto tmp = call_node->get_calls().back();
    switch(tmp->get_type()) {
        case expr_type::ast_callh:
            mcall_hash(reinterpret_cast<call_hash*>(tmp)); break;
        case expr_type::ast_callv:
            mcall_vec(reinterpret_cast<call_vector*>(tmp)); break;
        case expr_type::ast_callf:
            die("bad left-value: function call", tmp); break;
        case expr_type::ast_null_access:
            die("bad left-value: null access test", tmp); break;
        default:
            die("bad left-value: unknown call", tmp); break;
    }
}

void codegen::mcall_identifier(identifier* node) {
    const auto& name = node->get_name();
    if (native_function_mapper.count(name)) {
        die("cannot modify native function", node);
        return;
    }

    i64 index;
    if ((index = local_symbol_find(name))>=0) {
        emit(op_mcalll, index, node->get_location());
        return;
    }
    if ((index = upvalue_symbol_find(name))>=0) {
        emit(op_mupval, index, node->get_location());
        return;
    }
    if ((index = global_symbol_find(name))>=0) {
        emit(op_mcallg, index, node->get_location());
        return;
    }
    die("undefined symbol \"" + name + "\"", node);
}

void codegen::mcall_vec(call_vector* node) {
    if (node->get_slices().size()>1) {
        die("bad left-value: subvec call", node);
        return;
    }
    auto call = node->get_slices()[0];
    if (call->get_end()) {
        die("bad left-value: subvec call", node);
        return;
    }
    calc_gen(call->get_begin());
    emit(op_mcallv, 0, node->get_location());
}

void codegen::mcall_hash(call_hash* node) {
    regist_string(node->get_field());
    emit(op_mcallh, const_string_map.at(node->get_field()), node->get_location());
}

void codegen::single_def(definition_expr* node) {
    const auto& str = node->get_variable_name()->get_name();
    calc_gen(node->get_value());
    // only generate in repl mode and in global scope
    if (flag_need_repl_output && local.empty()) {
        emit(op_repl, 0, node->get_location());
    }
    if (local.empty()) {
        emit(op_loadg, global_symbol_find(str), node->get_location());
    } else {
        emit(op_loadl, local_symbol_find(str), node->get_location());
    }
}

void codegen::multi_def(definition_expr* node) {
    auto& identifiers = node->get_variables()->get_variables();
    usize size = identifiers.size();
    // (var a, b, c) = (c, b, a);
    if (node->get_tuple()) {
        auto& vals = node->get_tuple()->get_elements();
        if (identifiers.size()>vals.size()) {
            die("lack values in multi-definition, expect " +
                std::to_string(identifiers.size()) + " but get " +
                std::to_string(vals.size()),
                node->get_tuple()
            );
            return;
        } else if (identifiers.size()<vals.size()) {
            die("too many values in multi-definition, expect " +
                std::to_string(identifiers.size()) + " but get " +
                std::to_string(vals.size()),
                node->get_tuple()
            );
            return;
        }
        for (usize i = 0; i<size; ++i) {
            calc_gen(vals[i]);
            const auto& name = identifiers[i]->get_name();
            local.empty()?
                emit(op_loadg, global_symbol_find(name), identifiers[i]->get_location()):
                emit(op_loadl, local_symbol_find(name), identifiers[i]->get_location());
        }
        return;
    }
    // (var a, b, c) = [0, 1, 2];
    calc_gen(node->get_value());
    for (usize i = 0; i<size; ++i) {
        emit(op_callvi, i, node->get_value()->get_location());
        const auto& name = identifiers[i]->get_name();
        local.empty()?
            emit(op_loadg, global_symbol_find(name), identifiers[i]->get_location()):
            emit(op_loadl, local_symbol_find(name), identifiers[i]->get_location());
    }
    emit(op_pop, 0, node->get_location());
}

void codegen::definition_gen(definition_expr* node) {
    if (node->get_variable_name() && node->get_tuple()) {
        die("cannot accept too many values", node->get_value());
    }
    node->get_variable_name()? single_def(node):multi_def(node);
}

void codegen::assignment_expression(assignment_expr* node) {
    switch(node->get_assignment_type()) {
        case assignment_expr::kind::equal:
            calc_gen(node->get_right());
            mcall(node->get_left());
            emit(op_meq, 0, node->get_location());
            break;
        case assignment_expr::kind::add_equal:
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
            }
            mcall(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                emit(op_addeq, 0, node->get_location());
            } else {
                auto num = reinterpret_cast<number_literal*>(node->get_right())
                           ->get_number();
                regist_number(num);
                emit(op_addeqc, const_number_map[num], node->get_location());
            }
            break;
        case assignment_expr::kind::sub_equal:
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
            }
            mcall(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                emit(op_subeq, 0, node->get_location());
            } else {
                auto num = reinterpret_cast<number_literal*>(node->get_right())
                           ->get_number();
                regist_number(num);
                emit(op_subeqc, const_number_map[num], node->get_location());
            }
            break;
        case assignment_expr::kind::mult_equal:
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
            }
            mcall(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                emit(op_muleq, 0, node->get_location());
            } else {
                auto num = reinterpret_cast<number_literal*>(node->get_right())
                           ->get_number();
                regist_number(num);
                emit(op_muleqc, const_number_map[num], node->get_location());
            }
            break;
        case assignment_expr::kind::div_equal:
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
            }
            mcall(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                emit(op_diveq, 0, node->get_location());
            } else {
                auto num = reinterpret_cast<number_literal*>(node->get_right())
                           ->get_number();
                regist_number(num);
                emit(op_diveqc, const_number_map[num], node->get_location());
            }
            break;
        case assignment_expr::kind::concat_equal:
            if (node->get_right()->get_type()!=expr_type::ast_str) {
                calc_gen(node->get_right());
            }
            mcall(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_str) {
                emit(op_lnkeq, 0, node->get_location());
            } else {
                const auto& str = reinterpret_cast<string_literal*>(
                                  node->get_right())->get_content();
                regist_string(str);
                emit(op_lnkeqc, const_string_map[str], node->get_location());
            }
            break;
        case assignment_expr::kind::bitwise_and_equal:
            calc_gen(node->get_right());
            mcall(node->get_left());
            emit(op_btandeq, 0, node->get_location());
            break;
        case assignment_expr::kind::bitwise_or_equal:
            calc_gen(node->get_right());
            mcall(node->get_left());
            emit(op_btoreq, 0, node->get_location());
            break;
        case assignment_expr::kind::bitwise_xor_equal:
            calc_gen(node->get_right());
            mcall(node->get_left());
            emit(op_btxoreq, 0, node->get_location());
            break;
    }
}

void codegen::gen_assignment_equal_statement(assignment_expr* node) {
    // if is not symbol load type assignment, use calc_gen directly
    // what is symbol load assignment? example:
    //     a = 1;         # this is symbol load
    //     a.foo = "bar"; # this is not symbol load
    if (node->get_left()->get_type()!=expr_type::ast_id) {
        calc_gen(node);
        if (code.back().op==op_meq) {
            code.back().num = 1; // 1 means need pop after op_meq
        } else {
            emit(op_pop, 0, node->get_location());
        }
        return;
    }
    
    // generate symbol load
    calc_gen(node->get_right());
    // get memory space of left identifier
    mcall_identifier(reinterpret_cast<identifier*>(node->get_left()));
    // check memory get operand type and replace it with load operand
    switch(code.back().op) {
        case op_mcallg: code.back().op = op_loadg; break;
        case op_mcalll: code.back().op = op_loadl; break;
        case op_mupval: code.back().op = op_loadu; break;
        default: die("unexpected operand to replace", node);
    }
}

void codegen::replace_left_assignment_with_load(const span& location) {
    // replace mcall with load,
    // because mcall needs meq(1) (meq-pop) after it to load,
    // but load to mcall-meq-pop in one operand.
    // if is not mcall operand, emit meq(1) (meq-pop).
    switch(code.back().op) {
        case op_mcallg: code.back().op = op_loadg; break;
        case op_mcalll: code.back().op = op_loadl; break;
        case op_mupval: code.back().op = op_loadu; break;
        default: emit(op_meq, 1, location); break;
    }
    return;
}

void codegen::assignment_statement(assignment_expr* node) {
    switch(node->get_assignment_type()) {
        case assignment_expr::kind::equal:
            gen_assignment_equal_statement(node);
            break;
        case assignment_expr::kind::add_equal:
        case assignment_expr::kind::sub_equal:
        case assignment_expr::kind::mult_equal:
        case assignment_expr::kind::div_equal:
        case assignment_expr::kind::concat_equal:
        case assignment_expr::kind::bitwise_and_equal:
        case assignment_expr::kind::bitwise_or_equal:
        case assignment_expr::kind::bitwise_xor_equal:
            calc_gen(node);
            if (op_addeq<=code.back().op && code.back().op<=op_btxoreq) {
                code.back().num = 1;
            } else if (op_addeqc<=code.back().op && code.back().op<=op_lnkeqc) {
                code.back().op = code.back().op-op_addeqc+op_addecp;
            } else {
                emit(op_pop, 0, node->get_location());
            }
            break;
    }
}

void codegen::multi_assign_gen(multi_assign* node) {
    auto tuple_node = node->get_tuple();
    auto value_node = node->get_value();
    if (value_node->get_type()==expr_type::ast_tuple) {
        auto tuple_size = tuple_node->get_elements().size();
        auto value_size = reinterpret_cast<tuple_expr*>(value_node)
                          ->get_elements().size();
        if (tuple_size>value_size) {
            die(
                "lack value(s) in multi-assignment, expect " +
                std::to_string(tuple_size) + " but get " +
                std::to_string(value_size),
                value_node
            );
            return;
        } else if (tuple_size<value_size) {
            die(
                "too many values in multi-assignment, expect " +
                std::to_string(tuple_size) + " but get " +
                std::to_string(value_size),
                value_node
            );
            return;
        }
    }

    i64 size = static_cast<i64>(tuple_node->get_elements().size());
    // generate multiple assignment: (a, b, c) = (1, 2, 3);
    if (value_node->get_type()==expr_type::ast_tuple) {
        const auto& value_tuple = reinterpret_cast<tuple_expr*>(value_node)
                                  ->get_elements();
        for (i64 i = size-1; i>=0; --i) {
            calc_gen(value_tuple[i]);
        }
        auto& tuple = tuple_node->get_elements();
        for (i64 i = 0; i<size; ++i) {
            mcall(tuple[i]);
            // use load operands to avoid meq's pop operand
            // and this operation changes local and global value directly
            replace_left_assignment_with_load(tuple[i]->get_location());
        }
        return;
    }

    // generate multiple assignment: (a, b, c) = [1, 2, 3];
    calc_gen(value_node);
    auto& tuple = tuple_node->get_elements();
    for (i64 i = 0; i<size; ++i) {
        emit(op_callvi, i, value_node->get_location());
        mcall(tuple[i]);
        // use load operands to avoid meq's pop operand
        // and this operation changes local and global value directly
        replace_left_assignment_with_load(tuple[i]->get_location());
    }

    // pop source vector
    emit(op_pop, 0, node->get_location());
}

void codegen::cond_gen(condition_expr* node) {
    std::vector<usize> jmp_label;
    calc_gen(node->get_if_statement()->get_condition());
    auto ptr = code.size();
    emit(op_jf, 0, node->get_if_statement()->get_location());
    block_gen(node->get_if_statement()->get_code_block());
    if (node->get_elsif_stataments().size() ||
        node->get_else_statement()) {
        jmp_label.push_back(code.size());
        emit(op_jmp, 0, node->get_if_statement()->get_location());
    }
    code[ptr].num = code.size();

    for (auto tmp : node->get_elsif_stataments()) {
        calc_gen(tmp->get_condition());
        ptr = code.size();
        emit(op_jf, 0, tmp->get_location());
        block_gen(tmp->get_code_block());
        // the last condition doesn't need to jmp
        if (tmp!=node->get_elsif_stataments().back() ||
            node->get_else_statement()) {
            jmp_label.push_back(code.size());
            emit(op_jmp, 0, tmp->get_location());
        }
        code[ptr].num=code.size();
    }

    if (node->get_else_statement()) {
        block_gen(node->get_else_statement()->get_code_block());
    }
    for (auto i:jmp_label) {
        code[i].num = code.size();
    }
}

void codegen::loop_gen(expr* node) {
    continue_ptr.push_front({});
    break_ptr.push_front({});

    switch(node->get_type()) {
        case expr_type::ast_while:
            while_gen(reinterpret_cast<while_expr*>(node)); break;
        case expr_type::ast_for:
            for_gen(reinterpret_cast<for_expr*>(node)); break;
        case expr_type::ast_forei:
            forei_gen(reinterpret_cast<forei_expr*>(node)); break;
        default: break;
    }
}

void codegen::load_continue_break(u64 continue_place, u64 break_place) {
    for (auto i : continue_ptr.front()) {
        code[i].num = continue_place;
    }
    for (auto i : break_ptr.front()) {
        code[i].num = break_place;
    }
    continue_ptr.pop_front();
    break_ptr.pop_front();
}

void codegen::while_gen(while_expr* node) {
    usize loop_ptr = code.size();
    calc_gen(node->get_condition());
    usize condition_ptr = code.size();
    emit(op_jf, 0, node->get_condition()->get_location());

    block_gen(node->get_code_block());
    emit(op_jmp, loop_ptr, node->get_code_block()->get_location());
    code[condition_ptr].num = code.size();
    load_continue_break(code.size()-1, code.size());
}

void codegen::for_gen(for_expr* node) {
    statement_generation(node->get_initial());
    usize jmp_place = code.size();
    if (node->get_condition()->get_type()==expr_type::ast_null) {
        regist_number(1);
        emit(op_pnum, const_number_map.at(1), node->get_condition()->get_location());
    } else {
        calc_gen(node->get_condition());
    }
    usize label_exit = code.size();
    emit(op_jf, 0, node->get_condition()->get_location());

    block_gen(node->get_code_block());
    usize continue_place = code.size();
    statement_generation(node->get_step());
    emit(op_jmp, jmp_place, node->get_step()->get_location());
    code[label_exit].num = code.size();

    load_continue_break(continue_place, code.size());
}

void codegen::forei_gen(forei_expr* node) {
    calc_gen(node->get_value());
    emit(op_cnt, 0, node->get_value()->get_location());
    usize loop_begin = code.size();
    if (node->get_loop_type()==forei_expr::kind::forindex) {
        emit(op_findex, 0, node->get_location());
    } else {
        emit(op_feach, 0, node->get_location());
    }

    auto iterator_node = node->get_iterator();
    if (iterator_node->is_definition()) {
        // define a new iterator
        const auto name_node = iterator_node->get_name();
        const auto& str = name_node->get_name();
        local.empty()?
            emit(op_loadg, global_symbol_find(str), name_node->get_location()):
            emit(op_loadl, local_symbol_find(str), name_node->get_location());
    } else if (!iterator_node->is_definition() && iterator_node->get_name()) {
        mcall(node->get_iterator()->get_name());
        replace_left_assignment_with_load(node->get_iterator()->get_location());
    } else {
        // use exist variable as the iterator
        mcall(node->get_iterator()->get_call());
        replace_left_assignment_with_load(node->get_iterator()->get_location());
    }

    // generate code block
    ++in_foreach_loop_level.back();
    block_gen(node->get_code_block());
    --in_foreach_loop_level.back();

    // jump to loop begin
    emit(op_jmp, loop_begin, node->get_location());
    code[loop_begin].num=code.size();
    load_continue_break(code.size()-1, code.size());

    // pop source vector
    emit(op_pop, 0, node->get_value()->get_location());
    // pop loop iterator
    emit(op_pop, 0, node->get_location());
}

void codegen::statement_generation(expr* node) {
    switch(node->get_type()) {
        case expr_type::ast_null: break;
        case expr_type::ast_def:
            definition_gen(reinterpret_cast<definition_expr*>(node)); break;
        case expr_type::ast_multi_assign:
            multi_assign_gen(reinterpret_cast<multi_assign*>(node)); break;
        case expr_type::ast_assign:
            assignment_statement(reinterpret_cast<assignment_expr*>(node)); break;
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
            // only generate in repl mode and in global scope
            if (flag_need_repl_output && local.empty()) {
                emit(op_repl, 0, node->get_location());
            }
            emit(op_pop, 0, node->get_location());
            break;
        default: break;
    }
}

void codegen::or_gen(binary_operator* node) {
    calc_gen(node->get_left());
    usize label_jump_true_1 = code.size();
    emit(op_jt, 0, node->get_left()->get_location());

    emit(op_pop, 0, node->get_left()->get_location());
    calc_gen(node->get_right());
    usize label_jump_true_2 = code.size();
    emit(op_jt, 0, node->get_right()->get_location());

    emit(op_pop, 0, node->get_right()->get_location());
    emit(op_pnil, 0, node->get_right()->get_location());

    code[label_jump_true_1].num = code[label_jump_true_2].num = code.size();
}

void codegen::and_gen(binary_operator* node) {
    calc_gen(node->get_left());
    emit(op_jt, code.size()+2, node->get_left()->get_location());

    usize lable_jump_false = code.size();
    emit(op_jmp, 0, node->get_left()->get_location());
    emit(op_pop, 0, node->get_right()->get_location()); // jt jumps here

    calc_gen(node->get_right());
    emit(op_jt, code.size()+3, node->get_right()->get_location());

    code[lable_jump_false].num = code.size();
    emit(op_pop, 0, node->get_right()->get_location());
    emit(op_pnil, 0, node->get_right()->get_location());
    // jt jumps here, avoid pop and pnil
}

void codegen::unary_gen(unary_operator* node) {
    // generate optimized result
    if (node->get_optimized_number()) {
        number_gen(node->get_optimized_number());
        return;
    }

    calc_gen(node->get_value());
    switch(node->get_operator_type()) {
        case unary_operator::kind::negative:
            emit(op_usub, 0, node->get_location()); break;
        case unary_operator::kind::logical_not:
            emit(op_lnot, 0, node->get_location()); break;
        case unary_operator::kind::bitwise_not:
            emit(op_bnot, 0, node->get_location()); break;
    }
}

void codegen::binary_gen(binary_operator* node) {
    // generate optimized result
    if (node->get_optimized_number()) {
        number_gen(node->get_optimized_number());
        return;
    }
    if (node->get_optimized_string()) {
        string_gen(node->get_optimized_string());
        return;
    }

    switch(node->get_operator_type()) {
        case binary_operator::kind::condition_or: or_gen(node); return;
        case binary_operator::kind::condition_and: and_gen(node); return;
        default: break;
    }
    switch(node->get_operator_type()) {
        case binary_operator::kind::cmpeq:
            calc_gen(node->get_left());
            calc_gen(node->get_right());
            emit(op_eq, 0, node->get_location());
            return;
        case binary_operator::kind::cmpneq:
            calc_gen(node->get_left());
            calc_gen(node->get_right());
            emit(op_neq, 0, node->get_location());
            return;
        case binary_operator::kind::bitwise_or:
            calc_gen(node->get_left());
            calc_gen(node->get_right());
            emit(op_btor, 0, node->get_location());
            return;
        case binary_operator::kind::bitwise_xor:
            calc_gen(node->get_left());
            calc_gen(node->get_right());
            emit(op_btxor, 0, node->get_location());
            return;
        case binary_operator::kind::bitwise_and:
            calc_gen(node->get_left());
            calc_gen(node->get_right());
            emit(op_btand, 0, node->get_location());
            return;
        case binary_operator::kind::null_chain:
            null_chain_gen(node);
            return;
        default: break;
    }
    switch(node->get_operator_type()) {
        case binary_operator::kind::add:
            calc_gen(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
                emit(op_add, 0, node->get_location());
            } else {
                auto num = reinterpret_cast<number_literal*>(node->get_right())
                           ->get_number();
                regist_number(num);
                emit(op_addc, const_number_map.at(num), node->get_location());
            }
            return;
        case binary_operator::kind::sub:
            calc_gen(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
                emit(op_sub, 0, node->get_location());
            } else {
                auto num = reinterpret_cast<number_literal*>(node->get_right())
                           ->get_number();
                regist_number(num);
                emit(op_subc, const_number_map.at(num), node->get_location());
            }
            return;
        case binary_operator::kind::mult:
            calc_gen(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
                emit(op_mul, 0, node->get_location());
            } else {
                auto num = reinterpret_cast<number_literal*>(node->get_right())
                           ->get_number();
                regist_number(num);
                emit(op_mulc, const_number_map.at(num), node->get_location());
            }
            return;
        case binary_operator::kind::div:
            calc_gen(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
                emit(op_div, 0, node->get_location());
            } else {
                auto num = reinterpret_cast<number_literal*>(node->get_right())
                           ->get_number();
                regist_number(num);
                emit(op_divc, const_number_map.at(num), node->get_location());
            }
            return;
        case binary_operator::kind::concat:
            calc_gen(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_str) {
                calc_gen(node->get_right());
                emit(op_lnk, 0, node->get_location());
            } else {
                const auto& str = reinterpret_cast<string_literal*>(
                                  node->get_right())->get_content();
                regist_string(str);
                emit(op_lnkc, const_string_map.at(str), node->get_location());
            }
            break;
        case binary_operator::kind::less:
            calc_gen(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
                emit(op_less, 0, node->get_location());
            } else {
                auto num = reinterpret_cast<number_literal*>(node->get_right())
                           ->get_number();
                regist_number(num);
                emit(op_lessc, const_number_map.at(num), node->get_location());
            }
            return;
        case binary_operator::kind::leq:
            calc_gen(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
                emit(op_leq, 0, node->get_location());
            } else {
                auto num = reinterpret_cast<number_literal*>(node->get_right())
                           ->get_number();
                regist_number(num);
                emit(op_leqc, const_number_map.at(num), node->get_location());
            }
            return;
        case binary_operator::kind::grt:
            calc_gen(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
                emit(op_grt, 0, node->get_location());
            } else {
                auto num = reinterpret_cast<number_literal*>(node->get_right())
                           ->get_number();
                regist_number(num);
                emit(op_grtc, const_number_map.at(num), node->get_location());
            }
            return;
        case binary_operator::kind::geq:
            calc_gen(node->get_left());
            if (node->get_right()->get_type()!=expr_type::ast_num) {
                calc_gen(node->get_right());
                emit(op_geq, 0, node->get_location());
            } else {
                auto num = reinterpret_cast<number_literal*>(node->get_right())
                           ->get_number();
                regist_number(num);
                emit(op_geqc, const_number_map.at(num), node->get_location());
            }
            return;
        default: break;
    }
}

void codegen::null_chain_gen(binary_operator* node) {
    calc_gen(node->get_left());
    emit(op_pnil, 0, node->get_location());
    emit(op_eq, 0, node->get_location());

    const auto jmp_false_point = code.size();
    emit(op_jf, 0, node->get_location());

    calc_gen(node->get_right());
    const auto jmp_direct_point = code.size();
    emit(op_jmp, 0, node->get_location());

    code[jmp_false_point].num = code.size();
    emit(op_pop, 0, node->get_location());
    code[jmp_direct_point].num = code.size();
}

void codegen::trino_gen(ternary_operator* node) {
    calc_gen(node->get_condition());
    usize label_jump_false = code.size();
    emit(op_jf, 0, node->get_condition()->get_location());
    calc_gen(node->get_left());
    usize label_jump_to_exit = code.size();
    emit(op_jmp, 0, node->get_left()->get_location());
    code[label_jump_false].num = code.size();
    calc_gen(node->get_right());
    code[label_jump_to_exit].num = code.size();
}

void codegen::calc_gen(expr* node) {
    switch(node->get_type()) {
        case expr_type::ast_nil:
            emit(op_pnil, 0, node->get_location()); break;
        case expr_type::ast_num:
            number_gen(reinterpret_cast<number_literal*>(node)); break;
        case expr_type::ast_str:
            string_gen(reinterpret_cast<string_literal*>(node)); break;
        case expr_type::ast_id:
            call_identifier(reinterpret_cast<identifier*>(node)); break;
        case expr_type::ast_bool:
            bool_gen(reinterpret_cast<bool_literal*>(node)); break;
        case expr_type::ast_vec:
            vector_gen(reinterpret_cast<vector_expr*>(node)); break;
        case expr_type::ast_hash:
            hash_gen(reinterpret_cast<hash_expr*>(node)); break;
        case expr_type::ast_func:
            func_gen(reinterpret_cast<function*>(node)); break;
        case expr_type::ast_call:
            call_gen(reinterpret_cast<call_expr*>(node)); break;
        case expr_type::ast_assign:
            assignment_expression(
                reinterpret_cast<assignment_expr*>(node)
            );
            break;
        case expr_type::ast_ternary:
            trino_gen(reinterpret_cast<ternary_operator*>(node)); break;
        case expr_type::ast_unary:
            unary_gen(reinterpret_cast<unary_operator*>(node)); break;
        case expr_type::ast_binary:
            binary_gen(reinterpret_cast<binary_operator*>(node)); break;
        case expr_type::ast_def:
            // definition in calculation only should be single def
            single_def(reinterpret_cast<definition_expr*>(node));
            call_identifier(
                (reinterpret_cast<definition_expr*>(node))->get_variable_name()
            );
            break;
        default: break;
    }
}

void codegen::repl_mode_info_output_gen(expr* node) {
    switch(node->get_type()) {
        case expr_type::ast_id:
            call_identifier(reinterpret_cast<identifier*>(node)); break;
        case expr_type::ast_nil:
            emit(op_pnil, 0, node->get_location()); break;
        case expr_type::ast_num:
            number_gen(reinterpret_cast<number_literal*>(node)); break;
        case expr_type::ast_str:
            string_gen(reinterpret_cast<string_literal*>(node)); break;
        case expr_type::ast_bool:
            bool_gen(reinterpret_cast<bool_literal*>(node)); break;
        default: return;
    }
    // generate repl output operand
    emit(op_repl, 0, node->get_location());
    // pop stack
    emit(op_pop, 0, node->get_location());
}

void codegen::block_gen(code_block* node) {
    bool is_use_statement = true;
    for (auto tmp : node->get_expressions()) {
        if (tmp->get_type()!=expr_type::ast_use) {
            is_use_statement = false;
        }
        switch(tmp->get_type()) {
            case expr_type::ast_use:
                if (!local.empty()) {
                    die("module import is not allowed here.", tmp);
                } else if (!is_use_statement) {
                    die("module import should be used at top of file.", tmp);
                }
                break;
            case expr_type::ast_null: break;
            case expr_type::ast_id:
                if (flag_need_repl_output && local.empty()) {
                    repl_mode_info_output_gen(tmp);
                } else {
                    check_id_exist(reinterpret_cast<identifier*>(tmp));
                }
                break;
            case expr_type::ast_nil:
            case expr_type::ast_num:
            case expr_type::ast_str:
            case expr_type::ast_bool:
                if (flag_need_repl_output && local.empty()) {
                    repl_mode_info_output_gen(tmp);
                }
                break;
            case expr_type::ast_cond:
                cond_gen(reinterpret_cast<condition_expr*>(tmp)); break;
            case expr_type::ast_continue:
                continue_ptr.front().push_back(code.size());
                emit(op_jmp, 0, tmp->get_location());
                break;
            case expr_type::ast_break:
                break_ptr.front().push_back(code.size());
                emit(op_jmp, 0, tmp->get_location());
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
                ret_gen(reinterpret_cast<return_expr*>(tmp)); break;
            default: break;
        }
    }
}

void codegen::ret_gen(return_expr* node) {
    for (u32 i = 0; i<in_foreach_loop_level.back(); ++i) {
        emit(op_pop, 0, node->get_location());
        emit(op_pop, 0, node->get_location());
    }
    calc_gen(node->get_value());
    emit(op_ret, 0, node->get_location());
}

const error& codegen::compile(parse& parse,
                              linker& import,
                              bool repl_flag,
                              bool limit_mode) {
    flag_need_repl_output = repl_flag;
    flag_limited_mode = limit_mode;
    init_native_function();
    init_file_map(import.get_file_list());

    in_foreach_loop_level.push_back(0);

    // add special symbol globals, which is a hash stores all global variables
    regist_symbol("globals");
    // add special symbol arg here, which is used to store command line args
    regist_symbol("arg");

    // search global symbols first
    find_symbol(parse.tree());

    // generate main block
    block_gen(parse.tree());
    // generate exit operand, vm stops here
    emit(op_exit, 0, parse.tree()->get_location());

    // size out of bound check
    if (const_number_table.size()>INT64_MAX) {
        err.err("code",
            "too many constant numbers: " +
            std::to_string(const_number_table.size())
        );
    }
    if (const_string_table.size()>INT64_MAX) {
        err.err("code",
            "too many constant strings: " +
            std::to_string(const_string_table.size())
        );
    }

    // check global variables size
    if (global.size()>=VM_STACK_DEPTH) {
        err.err("code",
            "too many global variables: " +
            std::to_string(global.size())
        );
    }

    // check generated code size
    if (code.size()>INT64_MAX) {
        err.err("code",
            "bytecode size overflow: " +
            std::to_string(code.size())
        );
    }
    return err;
}

void codegen::print(std::ostream& out) {
    // func end stack, reserved for code print
    std::stack<u64> func_begin_stack;
    std::stack<u64> func_end_stack;

    // print const numbers
    for (auto num : const_number_table) {
        out << "  .number " << num << "\n";
    }

    // print const strings
    for (const auto& str : const_string_table) {
        out << "  .symbol \"" << util::rawstr(str) << "\"\n";
    }
    
    // print blank line
    if (const_number_table.size() || const_string_table.size()) {
        out << "\n";
    }

    // print code
    codestream::set(
        const_number_table.data(),
        const_string_table.data(),
        global,
        native_function.data()
    );

    for (u64 i = 0; i<code.size(); ++i) {
        // print opcode index, opcode name, opcode immediate number
        const auto& c = code[i];
        if (!func_end_stack.empty() && i==func_end_stack.top()) {
            out << std::hex << "<0x" << func_begin_stack.top() << std::dec << ">;\n";
            // avoid two empty lines
            if (c.op!=op_newf) {
                out << "\n";
            }
            func_begin_stack.pop();
            func_end_stack.pop();
        }

        // get function begin index and end index
        if (c.op==op_newf) {
            out << std::hex << "\nfunc <0x" << i << std::dec << ">:\n";
            for (u64 j = i; j<code.size(); ++j) {
                if (code[j].op==op_jmp) {
                    func_begin_stack.push(i);
                    func_end_stack.push(code[j].num);
                    break;
                }
            }
        }

        // output bytecode
        out << "  " << codestream(c, i) << "\n";
    }
}

void codegen::symbol_dump(std::ostream& out) const {
    for (const auto& domain : nasal_namespace) {
        out << "<" << domain.first << ">\n";
        for (const auto& i : domain.second) {
            out << "  0x" << std::setw(4) << std::setfill('0');
            out << std::hex << global.at(i) << std::dec << " ";
            out << i << std::endl;
        }
    }
}

}
