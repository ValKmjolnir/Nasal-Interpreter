#include "nasal_new_codegen.h"

bool codegen::check_memory_reachable(call_expr* node) {
    if (node->get_first()->get_type()==expr_type::ast_call) {
        const auto tmp=node->get_calls().back();
        if (tmp->get_type()==expr_type::ast_callf) {
            die("bad left-value with function call", node->get_location());
            return false;
        }
        if (tmp->get_type()==expr_type::ast_callv &&
            (((call_vector*)tmp)->get_slices().size()!=1 ||
            ((call_vector*)tmp)->get_slices()[0]->get_end())) {
            die("bad left-value with subvec", node->get_location());
            return false;
        }
    } else if (node->get_first()->get_type()!=expr_type::ast_id) {
        die("bad left-value", node->get_location());
        return false;
    }
    return true;
}

void codegen::check_id_exist(identifier* node) {
    const auto& name = node->get_name();
    for(u32 i=0;builtin[i].name;++i) {
        if (builtin[i].name==name) {
            if (local.empty()) {
                die("useless native function used in global scope", node->get_location());
            }
            return;
        }
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
    die("undefined symbol \"" + name + "\", and this symbol is useless here", node->get_location());
}

void codegen::regist_num(const f64 num) {
    if (!num_table.count(num)) {
        u32 size=num_table.size();
        num_table[num]=size;
        num_res.push_back(num);
    }
}

void codegen::regist_str(const std::string& str) {
    if (!str_table.count(str)) {
        u32 size=str_table.size();
        str_table[str]=size;
        str_res.push_back(str);
    }
}

void codegen::find_symbol(code_block* node) {
    auto finder = new symbol_finder;
    for(const auto& i : finder->do_find(node)) {
        add_sym(i);
    }
}

void codegen::add_sym(const std::string& name) {
    if (local.empty()) {
        if (global.count(name)) {
            return;
        }
        i32 index=global.size();
        global[name]=index;
        return;
    }
    if (local.back().count(name)) {
        return;
    }
    i32 index=local.back().size();
    local.back()[name]=index;
}

i32 codegen::local_find(const std::string& name) {
    if (local.empty()) {
        return -1;
    }
    return local.back().count(name)?local.back()[name]:-1;
}

i32 codegen::global_find(const std::string& name) {
    return global.count(name)?global[name]:-1;
}

i32 codegen::upvalue_find(const std::string& name) {
    // 32768 level 65536 upvalues
    i32 index=-1;
    usize size=local.size();
    if (size<=1) {
        return -1;
    }
    auto iter=local.begin();
    for(u32 i=0;i<size-1;++i,++iter) {
        if (iter->count(name)) {
            index=((i<<16)|(*iter)[name]);
        }
    }
    return index;
}

void codegen::gen(u8 op, u32 num, u32 line) {
    code.push_back({op, fileindex, num, line});
}

void codegen::num_gen(number_literal* node) {
    f64 num = node->get_number();
    regist_num(num);
    gen(op_pnum,num_table[num], node->get_location().begin_line);
}

void codegen::str_gen(string_literal* node) {
    regist_str(node->get_content());
    gen(op_pstr, str_table[node->get_content()], node->get_location().begin_line);
}

void codegen::bool_gen(bool_literal* node) {
    f64 num = node->get_flag()?1:0;
    regist_num(num);
    gen(op_pnum, num_table[num], node->get_location().begin_line);
}

void codegen::vec_gen(vector_expr* node) {
    for(auto child : node->get_elements()) {
        calc_gen(child);
    }
    gen(op_newv, node->get_elements().size(), node->get_location().begin_line);
}

void codegen::hash_gen(hash_expr* node) {
    gen(op_newh, 0, node->get_location().begin_line);
    for(auto child : node->get_members()) {
        calc_gen(child->get_value());
        const std::string& str=child->get_name();
        regist_str(str);
        gen(op_happ, str_table[str], child->get_location().begin_line);
    }
}

void codegen::func_gen(function* node) {
    // parameter list format check
    bool checked_default = false;
    bool checked_dynamic = false;
    std::unordered_map<std::string,bool> argname;
    for(auto tmp : node->get_parameter_list()) {
        if (tmp->get_parameter_type()==parameter::param_type::default_parameter) {
            checked_default=true;
        } else if (tmp->get_parameter_type()==parameter::param_type::dynamic_parameter) {
            checked_dynamic=true;
        }
        // check default parameter and dynamic parameter
        if (checked_default && tmp->get_parameter_type()!=parameter::param_type::default_parameter) {
            die("must use default parameters here", tmp->get_location());
        }
        if (checked_dynamic && tmp!=node->get_parameter_list().back()) {
            die("dynamic parameter must be the last one", tmp->get_location());
        }
        // check redefinition
        const auto& name = tmp->get_parameter_name();
        if (argname.count(name)) {
            die("redefinition of parameter: "+name, tmp->get_location());
        } else {
            argname[name]=true;
        }
    }

    usize newf=code.size();
    gen(op_newf, 0, node->get_location().begin_line);
    usize lsize=code.size();
    gen(op_intl, 0, node->get_location().begin_line);
    
    // add special keyword 'me' into symbol table
    // this symbol is only used in local scope(function's scope)
    // this keyword is set to nil as default value
    // after calling a hash, this keyword is set to this hash
    // this symbol's index will be 0
    local.push_back({{"me", 0}});

    // generate parameter list
    for(auto& tmp : node->get_parameter_list()) {
        const auto& str = tmp->get_parameter_name();
        if (str=="me") {
            die("\"me\" should not be a parameter", tmp->get_location());
        }
        regist_str(str);
        switch(tmp->get_parameter_type()) {
            case parameter::param_type::normal_parameter:
                gen(op_para, str_table[str], tmp->get_location().begin_line);
                break;
            case parameter::param_type::default_parameter:
                calc_gen(tmp->get_default_value());
                gen(op_deft, str_table[str], tmp->get_location().begin_line);
                break;
            case parameter::param_type::dynamic_parameter:
                gen(op_dyn, str_table[str], tmp->get_location().begin_line);
                break;
        }
        add_sym(str);
    }

    code[newf].num=code.size()+1; // entry
    usize jmp_ptr=code.size();
    gen(op_jmp, 0, node->get_location().begin_line);

    auto block = node->get_code_block();
    // search symbols first, must use after loading parameters
    // or the location of symbols will change and cause fatal error
    find_symbol(block);
    in_iterloop.push(0);
    block_gen(block);
    in_iterloop.pop();
    code[lsize].num=local.back().size();
    if (local.back().size()>=STACK_DEPTH) {
        die("too many local variants: "+std::to_string(local.back().size()), block->get_location());
    }
    local.pop_back();

    if (!block->get_expressions().size() ||
        block->get_expressions().back()->get_type()!=expr_type::ast_ret) {
        gen(op_pnil, 0, block->get_location().begin_line);
        gen(op_ret, 0, block->get_location().begin_line);
    }
    code[jmp_ptr].num=code.size();
}

void codegen::call_gen(call_expr* node) {
    calc_gen(node->get_first());
    if (code.back().op==op_callb) {
        return;
    }
    for(auto i : node->get_calls()) {
        switch(i->get_type()) {
            case expr_type::ast_callh: call_hash_gen((call_hash*)i); break;
            case expr_type::ast_callv: call_vec((call_vector*)i); break;
            case expr_type::ast_callf: call_func((call_function*)i); break;
        }
    }
}

void codegen::call_id(identifier* node) {
    const auto& name = node->get_name();
    for(u32 i=0; builtin[i].name; ++i) {
        if (builtin[i].name==name) {
            gen(op_callb, i, node->get_location().begin_line);
            if (local.empty()) {
                die("should warp native function in local scope", node->get_location());
            }
            return;
        }
    }
    i32 index;
    if ((index=local_find(name))>=0) {
        gen(op_calll, index, node->get_location().begin_line);
        return;
    }
    if ((index=upvalue_find(name))>=0) {
        gen(op_upval, index, node->get_location().begin_line);
        return;
    }
    if ((index=global_find(name))>=0) {
        gen(op_callg, index, node->get_location().begin_line);
        return;
    }
    die("undefined symbol \"" + name + "\"", node->get_location());
}

void codegen::call_hash_gen(call_hash* node) {
    regist_str(node->get_field());
    gen(op_callh, str_table[node->get_field()], node->get_location().begin_line);
}

void codegen::call_vec(call_vector* node) {
    // maybe this place can use callv-const if ast's first child is ast_num
    if (node->get_slices().size()==1 &&
        !node->get_slices()[0]->get_end()) {
        calc_gen(node->get_slices()[0]->get_begin());
        gen(op_callv, 0, node->get_slices()[0]->get_location().begin_line);
        return;
    }
    gen(op_slcbeg,0,node->get_location().begin_line);
    for(auto tmp : node->get_slices()) {
        if (!tmp->get_end()) {
            calc_gen(tmp->get_begin());
            gen(op_slc, 0, tmp->get_location().begin_line);
        } else {
            calc_gen(tmp->get_begin());
            calc_gen(tmp->get_end());
            gen(op_slc2, 0, tmp->get_location().begin_line);
        }
    }
    gen(op_slcend, 0, node->get_location().begin_line);
}

void codegen::call_func(call_function* node) {
    if (node->get_argument().size() &&
        node->get_argument()[0]->get_type()==expr_type::ast_pair) {
        gen(op_newh, 0, node->get_location().begin_line);
        for(auto child : node->get_argument()) {
            calc_gen(((hash_pair*)child)->get_value());
            const auto& str = ((hash_pair*)child)->get_name();
            regist_str(str);
            gen(op_happ, str_table[str], child->get_location().begin_line);
        }
        gen(op_callfh, 0, node->get_location().begin_line);
    } else {
        for(auto child : node->get_argument()) {
            calc_gen(child);
        }
        gen(op_callfv, node->get_argument().size(), node->get_location().begin_line);
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
void codegen::mcall(call_expr* node) {
    if (!check_memory_reachable(node)) {
        return;
    }
    if (node->get_first()->get_type()==expr_type::ast_id &&
        !node->get_calls().size()) {
        mcall_id((identifier*)node->get_first());
    }
    calc_gen(node->get_first());
    for(usize i = 0; i<node->get_calls().size()-1; ++i) {
        auto tmp = node->get_calls()[i];
        switch(tmp->get_type()) {
            case expr_type::ast_callh: call_hash_gen((call_hash*)tmp); break;
            case expr_type::ast_callv: call_vec((call_vector*)tmp); break;
            case expr_type::ast_callf: call_func((call_function*)tmp); break;
        }
    }
    auto tmp = node->get_calls().back();
    if (tmp->get_type()==expr_type::ast_callh) {
        mcall_hash((call_hash*)tmp);
    } else if (tmp->get_type()==expr_type::ast_callv) {
        mcall_vec((call_vector*)tmp);
    }
}

void codegen::mcall_id(identifier* node) {
    const auto& name = node->get_name();
    for(u32 i=0;builtin[i].name;++i) {
        if (builtin[i].name==name) {
            die("cannot modify native function", node->get_location());
            return;
        }
    }
    i32 index;
    if ((index=local_find(name))>=0) {
        gen(op_mcalll, index, node->get_location().begin_line);
        return;
    }
    if ((index=upvalue_find(name))>=0) {
        gen(op_mupval, index, node->get_location().begin_line);
        return;
    }
    if ((index=global_find(name))>=0) {
        gen(op_mcallg, index, node->get_location().begin_line);
        return;
    }
    die("undefined symbol \"" + name + "\"", node->get_location());
}

void codegen::mcall_vec(call_vector* node) {
    calc_gen(node->get_slices()[0]);
    gen(op_mcallv, 0, node->get_location().begin_line);
}

void codegen::mcall_hash(call_hash* node) {
    regist_str(node->get_field());
    gen(op_mcallh, str_table[node->get_field()], node->get_location().begin_line);
}

void codegen::single_def(definition_expr* node) {
    const auto& str = node->get_variable_name()->get_name();
    calc_gen(node->get_value());
    local.empty()?
        gen(op_loadg, global_find(str), node->get_location().begin_line):
        gen(op_loadl, local_find(str), node->get_location().begin_line);
}

void codegen::multi_def(definition_expr* node) {
    auto& ids = node->get_variables()->get_variables();
    usize size = ids.size();
    if (node->get_value()->get_type()==expr_type::ast_tuple) { // (var a,b,c)=(c,b,a);
        auto& vals = ((tuple_expr*)node->get_value())->get_elements();
        if (ids.size()<vals.size()) {
            die("lack values in multi-definition", node->get_value()->get_location());
        } else if (ids.size()>vals.size()) {
            die("too many values in multi-definition", node->get_value()->get_location());
        }
        for(usize i = 0; i<size; ++i) {
            calc_gen(vals[i]);
            const auto& str = ids[i]->get_name();
            local.empty()?
                gen(op_loadg, global_find(str), ids[i]->get_location().begin_line):
                gen(op_loadl, local_find(str), ids[i]->get_location().begin_line);
        }
    } else { // (var a,b,c)=[0,1,2];
        calc_gen(node->get_value());
        for(usize i = 0; i<size; ++i) {
            gen(op_callvi, i, node->get_value()->get_location().begin_line);
            const auto& str = ids[i]->get_name();
            local.empty()?
                gen(op_loadg, global_find(str), ids[i]->get_location().begin_line):
                gen(op_loadl, local_find(str), ids[i]->get_location().begin_line);
        }
        gen(op_pop, 0, node->get_location().begin_line);
    }
}

void codegen::def_gen(definition_expr* node) {
    if (node->get_variable_name() &&
        node->get_value()->get_type()==expr_type::ast_tuple) {
        die("cannot accept too many values", node->get_value()->get_location());
    }
    node->get_variable_name()? single_def(node):multi_def(node);
}

void codegen::multi_assign_gen(multi_assign* node) {
    if (node[1]->get_type()==expr_type::ast_tuple && node[0].size()<node[1].size()) {
        die("lack values in multi-assignment", node[1]->get_location());
    } else if (node[1]->get_type()==expr_type::ast_tuple && node[0].size()>node[1].size()) {
        die("too many values in multi-assignment", node[1]->get_location());
    }
    i32 size=node[0].size();
    if (node[1]->get_type()==expr_type::ast_tuple) {
        for(i32 i=size-1;i>=0;--i) {
            calc_gen(node[1][i]);
        }
        for(i32 i=0;i<size;++i) {
            mcall(node[0][i]);
            // multi assign user loadl and loadg to avoid meq's stack--
            // and this operation changes local and global value directly
            if (code.back().op==op_mcalll) {
                code.back().op=op_loadl;
            } else if (code.back().op==op_mupval) {
                code.back().op=op_loadu;
            } else if (code.back().op==op_mcallg) {
                code.back().op=op_loadg;
            } else {
                gen(op_meq, 1, node[0][i]->get_location().begin_line);
            }
        }
    } else {
        calc_gen(node[1]);
        for(i32 i=0;i<size;++i) {
            gen(op_callvi, i, node[1]->get_location().begin_line);
            // multi assign user loadl and loadg to avoid meq's stack--
            // and this operation changes local and global value directly
            mcall(node[0][i]);
            if (code.back().op==op_mcalll) {
                code.back().op=op_loadl;
            } else if (code.back().op==op_mupval) {
                code.back().op=op_loadu;
            } else if (code.back().op==op_mcallg) {
                code.back().op=op_loadg;
            } else {
                gen(op_meq, 1, node[0][i]->get_location().begin_line);
            }
        }
        gen(op_pop, 0, node->get_location().begin_line);
    }
}

void codegen::cond_gen(condition_expr* node) {
    std::vector<usize> jmp_label;
    for(auto& tmp:node.child()) {
        if (tmp->get_type()==expr_type::ast_if || tmp->get_type()==expr_type::ast_elsif) {
            calc_gen(tmp[0]);
            usize ptr=code.size();
            gen(op_jf, 0, tmp->get_location().begin_line);
            block_gen(tmp[1]);
            // without 'else' the last condition doesn't need to jmp
            if (&tmp!=&node.child().back()) {
                jmp_label.push_back(code.size());
                gen(op_jmp, 0, tmp->get_location().begin_line);
            }
            code[ptr].num=code.size();
        } else {
            block_gen(tmp[0]);
            break;
        }
    }
    for(auto i:jmp_label) {
        code[i].num=code.size();
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
            if (((forei_expr*)node)->get_loop_type()==forei_expr::forei_loop_type::forindex) {
                forindex_gen((forei_expr*)node);
            } else {
                foreach_gen((forei_expr*)node);
            }
            break;
    }
}

void codegen::load_continue_break(i32 continue_place,i32 break_place) {
    for(auto i:continue_ptr.front()) {
        code[i].num=continue_place;
    }
    for(auto i:break_ptr.front()) {
        code[i].num=break_place;
    }
    continue_ptr.pop_front();
    break_ptr.pop_front();
}

void codegen::while_gen(while_expr* node) {
    usize loop_ptr=code.size();
    calc_gen(node->get_condition());
    usize condition_ptr=code.size();
    gen(op_jf, 0, node->get_condition()->get_location().begin_line);

    block_gen(node->get_code_block());
    gen(op_jmp, loop_ptr, node->get_code_block()->get_location().begin_line);
    code[condition_ptr].num=code.size();
    load_continue_break(code.size()-1, code.size());
}

void codegen::for_gen(for_expr* node) {
    expr_gen(node->get_initial());
    usize jmp_place=code.size();
    if (node->get_condition()->get_type()==expr_type::ast_null) {
        gen(op_pnum, num_table[1], node->get_condition()->get_location().begin_line);
    } else {
        calc_gen(node->get_condition());
    }
    usize label_exit=code.size();
    gen(op_jf, 0, node->get_condition()->get_location().begin_line);

    block_gen(node->get_code_block());
    usize continue_place=code.size();
    expr_gen(node->get_step());
    gen(op_jmp, jmp_place, node->get_step()->get_location().begin_line);
    code[label_exit].num=code.size();

    load_continue_break(continue_place, code.size());
}

void codegen::expr_gen(expr* node) {
    switch(node->get_type()) {
        case expr_type::ast_null:break;
        case expr_type::ast_def:
            def_gen((definition_expr*)node);break;
        case expr_type::ast_multi_assign:
            multi_assign_gen((multi_assign*)node);break;
        case expr_type::ast_nil:case expr_type::ast_num:case expr_type::ast_str:case expr_type::ast_bool:break;
        case expr_type::ast_vec:case expr_type::ast_hash:case expr_type::ast_func:case expr_type::ast_call:
        case expr_type::ast_unary:
        case expr_type::ast_binary:
        case expr_type::ast_ternary:
            calc_gen(node);
            gen(op_pop, 0, node->get_location().begin_line);
            break;
        case expr_type::ast_equal:
            if (node[0]->get_type()==expr_type::ast_id) {
                calc_gen(node[1]);
                mcall_id(node[0]);
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
                    gen(op_pop, 0, node->get_location().begin_line);
                }
            }
            break;
        case expr_type::ast_addeq:case expr_type::ast_subeq:
        case expr_type::ast_multeq:case expr_type::ast_diveq:case expr_type::ast_lnkeq:
        case expr_type::ast_btandeq:case expr_type::ast_btoreq:case expr_type::ast_btxoreq:
            calc_gen(node);
            if (op_addeq<=code.back().op && code.back().op<=op_btxoreq) {
                code.back().num=1;
            } else if (op_addeqc<=code.back().op && code.back().op<=op_lnkeqc) {
                code.back().op=code.back().op-op_addeqc+op_addecp;
            } else {
                gen(op_pop, 0, node->get_location().begin_line);
            }
            break;
    }
}

void codegen::forindex_gen(forei_expr* node) {
    calc_gen(node->get_value());
    gen(op_cnt, 0, node->get_value()->get_location().begin_line);
    usize ptr = code.size();
    gen(op_findex, 0, node->get_location().begin_line);
    if (node->get_iterator()->get_name()) { // define a new iterator
        const auto& str = node->get_iterator()->get_name()->get_name();
        local.empty()?
            gen(op_loadg, global_find(str), node->get_iterator()->get_name()->get_location().begin_line):
            gen(op_loadl, local_find(str), node->get_iterator()->get_name()->get_location().begin_line);
    } else { // use exist variable as the iterator
        mcall((call_expr*)node->get_iterator()->get_call());
        if (code.back().op==op_mcallg) {
            code.back().op=op_loadg;
        } else if (code.back().op==op_mcalll) {
            code.back().op=op_loadl;
        } else if (code.back().op==op_mupval) {
            code.back().op=op_loadu;
        } else {
            gen(op_meq, 1, node->get_iterator()->get_location().begin_line);
        }
    }
    ++in_iterloop.top();
    block_gen(node->get_code_block());
    --in_iterloop.top();
    gen(op_jmp, ptr, node->get_location().begin_line);
    code[ptr].num=code.size();
    load_continue_break(code.size()-1, code.size());
    gen(op_pop, 0, node->get_value()->get_location().begin_line);// pop vector
    gen(op_pop, 0, node->get_location().begin_line);// pop iterator
}

void codegen::foreach_gen(forei_expr* node) {
    calc_gen(node->get_value());
    gen(op_cnt, 0, node->get_location().begin_line);
    usize ptr = code.size();
    gen(op_feach, 0, node->get_location().begin_line);
    if (node->get_iterator()->get_name()) { // define a new iterator
        const auto& str = node->get_iterator()->get_name()->get_name();
        local.empty()?
            gen(op_loadg, global_find(str), node->get_iterator()->get_name()->get_location().begin_line):
            gen(op_loadl, local_find(str), node->get_iterator()->get_name()->get_location().begin_line);
    } else { // use exist variable as the iterator
        mcall((call_expr*)node->get_iterator()->get_call());
        if (code.back().op==op_mcallg) {
            code.back().op=op_loadg;
        } else if (code.back().op==op_mcalll) {
            code.back().op=op_loadl;
        } else if (code.back().op==op_mupval) {
            code.back().op=op_loadu;
        } else {
            gen(op_meq, 1, node->get_iterator()->get_location().begin_line);
        }
    }
    ++in_iterloop.top();
    block_gen(node->get_code_block());
    --in_iterloop.top();
    gen(op_jmp, ptr, node->get_location().begin_line);
    code[ptr].num=code.size();
    load_continue_break(code.size()-1, code.size());
    gen(op_pop, 0, node->get_value()->get_location().begin_line); // pop vector
    gen(op_pop, 0, node->get_location().begin_line); // pop iterator
}

void codegen::or_gen(binary_operator* node) {
    calc_gen(node->get_left());
    usize l1 = code.size();
    gen(op_jt, 0, node->get_left()->get_location().begin_line);

    gen(op_pop, 0, node->get_left()->get_location().begin_line);
    calc_gen(node->get_right());
    usize l2=code.size();
    gen(op_jt, 0, node->get_right()->get_location().begin_line);

    gen(op_pop, 0, node->get_right()->get_location().begin_line);
    gen(op_pnil, 0, node->get_right()->get_location().begin_line);

    code[l1].num = code[l2].num = code.size();
}

void codegen::and_gen(binary_operator* node) {
    calc_gen(node->get_left());
    gen(op_jt, code.size()+2, node->get_left()->get_location().begin_line);

    usize lfalse=code.size();
    gen(op_jmp, 0, node->get_left()->get_location().begin_line);
    gen(op_pop, 0, node->get_right()->get_location().begin_line);// jt jumps here

    calc_gen(node->get_right());
    gen(op_jt, code.size()+3, node->get_right()->get_location().begin_line);

    code[lfalse].num=code.size();
    gen(op_pop, 0, node->get_right()->get_location().begin_line);
    gen(op_pnil, 0, node->get_right()->get_location().begin_line);
    // jt jumps here
}

void codegen::trino_gen(ternary_operator* node) {
    calc_gen(node->get_condition());
    usize lfalse=code.size();
    gen(op_jf, 0, node->get_condition()->get_location().begin_line);
    calc_gen(node->get_left());
    usize lexit=code.size();
    gen(op_jmp, 0, node->get_left()->get_location().begin_line);
    code[lfalse].num=code.size();
    calc_gen(node->get_right());
    code[lexit].num=code.size();
}

void codegen::calc_gen(expr* node) {
    switch(node->get_type()) {
        case expr_type::ast_nil:
            gen(op_pnil,0,node->get_location().begin_line); break;
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
        case expr_type::ast_equal:
            calc_gen(node[1]);
            mcall(node[0]);
            gen(op_meq, 0, node->get_location().begin_line);
            break;
        // ast_addeq(22)~ast_lnkeq(26) op_addeq(23)~op_lnkeq(27)
        case expr_type::ast_addeq:case expr_type::ast_subeq:case expr_type::ast_multeq:case expr_type::ast_diveq:
            if (node[1]->get_type()!=ast_num) {
                calc_gen(node[1]);
            }
            mcall(node[0]);
            if (node[1]->get_type()!=ast_num) {
                gen(node->get_type()-ast_addeq+op_addeq, 0, node->get_location().begin_line);
            } else {
                regist_num(node[1].num());
                gen(node->get_type()-ast_addeq+op_addeqc, num_table[node[1].num()], node->get_location().begin_line);
            }
            break;
        case expr_type::ast_lnkeq:
            if (node[1]->get_type()!=ast_str) {
                calc_gen(node[1]);
            } else {
                regist_str(node[1].str());
            }
            mcall(node[0]);
            if (node[1]->get_type()!=ast_str) {
                gen(op_lnkeq, 0, node->get_location().begin_line);
            } else {
                gen(op_lnkeqc, str_table[node[1].str()], node->get_location().begin_line);
            }
            break;
        case expr_type::ast_btandeq:case expr_type::ast_btoreq:case expr_type::ast_btxoreq:
            calc_gen(node[1]);
            mcall(node[0]);
            gen(node->get_type()-ast_btandeq+op_btandeq, 0, node->get_location().begin_line);
            break;
        case expr_type::ast_or:or_gen(node);break;
        case expr_type::ast_and:and_gen(node);break;
        // ast_add(33)~ast_link(37) op_add(18)~op_lnk(22)
        case expr_type::ast_add:case expr_type::ast_sub:case expr_type::ast_mult:case expr_type::ast_div:
            calc_gen(node[0]);
            if (node[1]->get_type()!=ast_num) {
                calc_gen(node[1]);
                gen(node->get_type()-ast_add+op_add, 0, node->get_location().begin_line);
            } else {
                regist_num(node[1].num());
                gen(node->get_type()-ast_add+op_addc, num_table[node[1].num()], node->get_location().begin_line);
            }
            break;
        case expr_type::ast_link:
            calc_gen(node[0]);
            if (node[1]->get_type()!=ast_str) {
                calc_gen(node[1]);
                gen(op_lnk, 0, node->get_location().begin_line);
            } else {
                regist_str(node[1].str());
                gen(op_lnkc, str_table[node[1].str()], node->get_location().begin_line);
            }
            break;
        // ast_cmpeq(27)~ast_geq(32) op_eq(29)~op_geq(34)
        case expr_type::ast_cmpeq:case expr_type::ast_neq:
            calc_gen(node[0]);
            calc_gen(node[1]);
            gen(node->get_type()-ast_cmpeq+op_eq, 0, node->get_location().begin_line);
            break;
        case expr_type::ast_less:case expr_type::ast_leq:case expr_type::ast_grt:case expr_type::ast_geq:
            calc_gen(node[0]);
            if (node[1]->get_type()!=ast_num) {
                calc_gen(node[1]);
                gen(node->get_type()-ast_less+op_less, 0, node->get_location().begin_line);
            } else {
                regist_num(node[1].num());
                gen(node->get_type()-ast_less+op_lessc, num_table[node[1].num()], node->get_location().begin_line);
            }
            break;
        case expr_type::ast_ternary:
            trino_gen((ternary_operator*)node);break;
        case expr_type::ast_neg:
            calc_gen(node[0]);
            gen(op_usub, 0, node->get_location().begin_line);
            break;
        case expr_type::ast_lnot:
            calc_gen(node[0]);
            gen(op_lnot, 0, node->get_location().begin_line);
            break;
        case expr_type::ast_bnot:
            calc_gen(node[0]);
            gen(op_bnot, 0, node->get_location().begin_line);
            break;
        case expr_type::ast_bitor:
            calc_gen(node[0]);
            calc_gen(node[1]);
            gen(op_btor, 0, node->get_location().begin_line);
            break;
        case expr_type::ast_bitxor:
            calc_gen(node[0]);
            calc_gen(node[1]);
            gen(op_btxor, 0, node->get_location().begin_line);
            break;
        case expr_type::ast_bitand:
            calc_gen(node[0]);
            calc_gen(node[1]);
            gen(op_btand, 0, node->get_location().begin_line);
            break;
        case expr_type::ast_def:
            single_def((definition_expr*)node);
            call_id(((definition_expr*)node)->get_variable_name());
            break;
    }
}

void codegen::block_gen(code_block* node) {
    for(auto tmp : node->get_expressions()) {
        switch(tmp->get_type()) {
            case expr_type::ast_null:break;
            case expr_type::ast_id:
                check_id_exist((identifier*)tmp); break;
            case expr_type::ast_nil:case expr_type::ast_num:
            case expr_type::ast_str:case expr_type::ast_bool:break;
            case expr_type::ast_file_info:
                fileindex = ((file_info*)tmp)->get_index();break; // special node type in main block
            case expr_type::ast_cond:
                cond_gen((condition_expr*)tmp); break;
            case expr_type::ast_continue:
                continue_ptr.front().push_back(code.size());
                gen(op_jmp, 0, tmp->get_location().begin_line);
                break;
            case expr_type::ast_break:
                break_ptr.front().push_back(code.size());
                gen(op_jmp, 0, tmp->get_location().begin_line);
                break;
            case expr_type::ast_while:
            case expr_type::ast_for:
            case expr_type::ast_forei:
                loop_gen(tmp); break;
            case expr_type::ast_binary:
            case expr_type::ast_vec:case expr_type::ast_hash:
            case expr_type::ast_func:case expr_type::ast_call:
            case expr_type::ast_unary:
            case expr_type::ast_ternary:
            case expr_type::ast_def:
            case expr_type::ast_multi_assign:
                expr_gen(tmp); break;
            case expr_type::ast_ret:
                ret_gen((return_expr*)tmp); break;
        }
    }
}

void codegen::ret_gen(return_expr* node) {
    for(u32 i=0;i<in_iterloop.top();++i) {
        gen(op_pop, 0, node->get_location().begin_line);
        gen(op_pop, 0, node->get_location().begin_line);
    }
    calc_gen(node->get_value());
    gen(op_ret, 0, node->get_location().begin_line);
}

const error& codegen::compile(parse& parse, linker& import) {
    fileindex=0;
    file=import.filelist().data();
    in_iterloop.push(0);
    find_symbol(parse.tree()); // search symbols first
    gen(op_intg, global.size(), 0);
    block_gen(parse.tree()); // generate main block
    gen(op_exit, 0, 0);

    // size out of bound check
    if (num_res.size()>0xffffff) {
        err.load(file[0]); // load main execute file
        err.err("code", "too many constant numbers: "+std::to_string(num_res.size()));
    }
    if (str_res.size()>0xffffff) {
        err.load(file[0]); // load main execute file
        err.err("code", "too many constant strings: "+std::to_string(str_res.size()));
    }
    if (global.size()>=STACK_DEPTH) {
        err.load(file[0]); // load main execute file
        err.err("code", "too many global variants: "+std::to_string(global.size()));
    }
    if (code.size()>0xffffff) {
        err.load(file[0]); // load main execute file
        err.err("code", "bytecode size overflow: "+std::to_string(code.size()));
    }
    return err;
}

void codegen::print() {
    // func end stack, reserved for code print
    std::stack<u32> fbstk;
    std::stack<u32> festk;

    // print const numbers
    for(auto& num:num_res) {
        std::cout<<"  .number "<<num<<"\n";
    }

    // print const strings
    for(auto& str:str_res) {
        std::cout<<"  .symbol \""<<rawstr(str)<<"\"\n";
    }

    // print code
    std::cout<<"\n";
    codestream::set(num_res.data(), str_res.data());
    for(u32 i=0;i<code.size();++i) {
        // print opcode index, opcode name, opcode immediate number
        const opcode& c=code[i];
        if (!festk.empty() && i==festk.top()) {
            std::cout<<std::hex<<"<0x"<<fbstk.top()<<std::dec<<">;\n";
            // avoid two empty lines
            if (c.op!=op_newf) {
                std::cout<<"\n";
            }
            fbstk.pop();
            festk.pop();
        }

        // get function begin index and end index
        if (c.op==op_newf) {
            std::cout<<std::hex<<"\nfunc <0x"<<i<<std::dec<<">:\n";
            for(u32 j=i;j<code.size();++j) {
                if (code[j].op==op_jmp) {
                    fbstk.push(i);
                    festk.push(code[j].num);
                    break;
                }
            }
        }

        // output bytecode
        std::cout<<"  "<<codestream(c,i)<<"\n";
    }
}
