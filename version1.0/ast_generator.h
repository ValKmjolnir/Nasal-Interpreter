#ifndef __AST_GENERATOR_H__
#define __AST_GENERATOR_H__

class generator
{
	private:
		abstract_syntax_tree root;
		std::stack<token> parse;
		token this_token;
	public:
		generator()
		{
			this_token.type=0;
			root.set_root();
		}
		void get_token()
		{
			if(parse.empty())
			{
				this_token.type=0;
				return;
			}
			this_token=parse.top();
			parse.pop();
			return;
		}
		void set_ast_empty()
		{
			root.tree_set_clear();
			return;
		}
		void gen_process(std::list<token>& lexer)
		{
			while(!parse.empty())
				parse.pop();
			std::stack<token> temp;
			for(std::list<token>::iterator i=lexer.begin();i!=lexer.end();++i)
			{
				token temp_parse;
				temp_parse=*i;
				temp.push(temp_parse);//push this into stack
			}
			if(temp.empty())
			{
				std::cout<<">>[Abstract-syntax-tree] warning: empty lexer list."<<std::endl;
				return;
			}
			while(!temp.empty())
			{
				parse.push(temp.top());
				temp.pop();
			}
			return;
		}
		void print_ast()
		{
			std::cout<<">>\n[Abstract-syntax-tree]"<<std::endl;
			root.print_ast_node(0);
			return;
		}
		void run()
		{
			
			return;
		}
		void gen_main_work();
		abstract_syntax_tree in_curve_calc_expr();
		abstract_syntax_tree number_begin_expr();
		abstract_syntax_tree string_begin_expr();
		abstract_syntax_tree identifier_begin_expr();
		abstract_syntax_tree call_list_expr();
		abstract_syntax_tree call_function_expr();
		abstract_syntax_tree call_hash_expr();
		abstract_syntax_tree list_generate_expr();
		abstract_syntax_tree hash_generate_expr();
		abstract_syntax_tree definition_expr();
		abstract_syntax_tree assignment_expr();
		abstract_syntax_tree loop_expr();
		abstract_syntax_tree continue_break_expr();
		bool else_if_check();
		abstract_syntax_tree if_else_expr();
		abstract_syntax_tree add_sub_operator_expr();
		abstract_syntax_tree mul_div_operator_expr();
		abstract_syntax_tree link_operator_expr();
		abstract_syntax_tree compare_operator_expr();
		abstract_syntax_tree one_operator_expr();
		void check_semi_at_end();
		abstract_syntax_tree statements_block();
		abstract_syntax_tree function_generate_expr();
		abstract_syntax_tree return_expr();
};
void generator::check_semi_at_end()
{
	get_token();
	return;
}
abstract_syntax_tree generator::return_expr()
{
	abstract_syntax_tree node;
	
	get_token();
	switch(this_token.type)
	{
		case __number:node.set_node_to_ret(number_begin_expr());;break;
		case __string:node.set_node_to_ret(string_begin_expr());;break;
		case __id:node.set_node_to_ret(identifier_begin_expr());;break;
		case __func:node.set_node_to_ret(function_generate_expr());;break;
		case __left_bracket:node.set_node_to_ret(list_generate_expr());;break;
		case __left_brace:node.set_node_to_ret(hash_generate_expr());;break;
		case __semi:parse.push(this_token);node.set_node_to_ret(node);break;
		default:break;
	}
	return node;
}
abstract_syntax_tree generator::statements_block()
{
	abstract_syntax_tree node;
	node.set_block();
	get_token();
	get_token();
	while(this_token.type!=__right_brace)
	{
		token t;
		switch(this_token.type)
		{
			case __var:node.add_statement(definition_expr());check_semi_at_end();break;
			case __id:node.add_statement(identifier_begin_expr());check_semi_at_end();break;
			case __number:node.add_statement(number_begin_expr());check_semi_at_end();break;
			case __string:node.add_statement(string_begin_expr());check_semi_at_end();break;
			case __if:parse.push(this_token);node.add_statement(if_else_expr());break;
			case __left_curve:
				t=this_token;
				get_token();
				if(this_token.type==__var)
				{
					parse.push(t);
					node.add_statement(definition_expr());
				}
				else
				{
					node.add_statement(in_curve_calc_expr());
				}
				check_semi_at_end();
				break;
			case __add_operator:
			case __sub_operator:
			case __nor_operator:node.add_statement(one_operator_expr());break;
			case __while:
			case __for:
			case __foreach:
			case __forindex:parse.push(this_token);node.add_statement(loop_expr());break;
			case __continue:
			case __break:node.add_statement(continue_break_expr());check_semi_at_end();break;
			case __semi:break;
			case __return:node.add_statement(return_expr());check_semi_at_end();break;
			default:break;
		}
		get_token();
	}
	return node;
}
abstract_syntax_tree generator::function_generate_expr()
{
	abstract_syntax_tree node;
	std::list<abstract_syntax_tree> parameters;
	get_token();
	if(this_token.type==__left_brace)
		parse.push(this_token);
	else if(this_token.type==__left_curve)
	{
		get_token();
		while(this_token.type!=__right_curve)
		{
			if(this_token.type==__id)
			{
				abstract_syntax_tree param;
				param.set_node_to_id(this_token.content);
				parameters.push_back(param);
				get_token();
				if(this_token.type==__right_curve)
					parse.push(this_token);
			}
			else if(this_token.type==__dynamic_id)
			{
				abstract_syntax_tree param;
				param.set_node_to_dynid(this_token.content);
				parameters.push_back(param);
				get_token();
				parse.push(this_token);
			}
			get_token();
		}
	}
	node.set_node_to_function(parameters,statements_block());
	return node;
}
abstract_syntax_tree generator::list_generate_expr()
{
	abstract_syntax_tree node;
	std::list<abstract_syntax_tree> list_members;
	get_token();
	while(this_token.type!=__right_bracket)
	{
		switch(this_token.type)
		{
			case __number:list_members.push_back(number_begin_expr());break;
			case __string:list_members.push_back(string_begin_expr());break;
			case __id:list_members.push_back(identifier_begin_expr());break;
			case __left_bracket:list_members.push_back(list_generate_expr());break;
			case __left_brace:list_members.push_back(hash_generate_expr());break;
			case __left_curve:list_members.push_back(in_curve_calc_expr());break;
			default:break;
		}
		get_token();
		if(this_token.type==__comma)
			get_token();
	}
	node.set_node_to_list(list_members);
	return node;
}
abstract_syntax_tree generator::hash_generate_expr()
{
	abstract_syntax_tree node;
	std::list<abstract_syntax_tree> hashmember;
	abstract_syntax_tree elem;
	std::string id_name;
	get_token();
	while(this_token.type!=__right_brace)
	{
		switch(this_token.type)
		{
			// case __number:number_begin_expr();break;
			// case __string:string_begin_expr();break;
			case __id:id_name=this_token.content;break;
			default:break;
		}
		get_token();
		get_token();
		switch(this_token.type)
		{
			case __number:elem.set_node_to_hashmember(id_name,number_begin_expr());break;
			case __string:elem.set_node_to_hashmember(id_name,string_begin_expr());break;
			case __id:elem.set_node_to_hashmember(id_name,identifier_begin_expr());break;
			case __func:elem.set_node_to_hashmember(id_name,function_generate_expr());break;
			case __left_bracket:elem.set_node_to_hashmember(id_name,list_generate_expr());break;
			case __left_brace:elem.set_node_to_hashmember(id_name,hash_generate_expr());break;
			case __left_curve:elem.set_node_to_hashmember(id_name,in_curve_calc_expr());break;
			default:break;
		}
		hashmember.push_back(elem);
		get_token();
		if(this_token.type==__comma)
			get_token();
	}
	node.set_node_to_hash(hashmember);
	return node;
}
abstract_syntax_tree generator::definition_expr()
{
	abstract_syntax_tree node;
	std::string id_name;
	get_token();
	if(this_token.type==__left_curve)
	{
		while(this_token.type!=__right_curve)
		{
			get_token();
			get_token();
			if(this_token.type==__right_curve)
				break;
		}
	}
	else if(this_token.type==__id)
	{
		id_name=this_token.content;
	}
	get_token();
	if(this_token.type==__semi)
	{
		parse.push(this_token);
		return node;
	}
	get_token();
	token t;
	t.line=this_token.line;
	t.type=__semi;
	switch(this_token.type)
	{
		case __number:node.set_definition_expr(id_name,number_begin_expr());break;
		case __string:node.set_definition_expr(id_name,string_begin_expr());break;
		case __id:node.set_definition_expr(id_name,identifier_begin_expr());break;
		case __func:node.set_definition_expr(id_name,function_generate_expr());parse.push(t);break;
		case __add_operator:
		case __sub_operator:
		case __nor_operator:node.set_definition_expr(id_name,one_operator_expr());break;
		case __left_bracket:node.set_definition_expr(id_name,list_generate_expr());break;
		case __left_brace:node.set_definition_expr(id_name,hash_generate_expr());break;
		case __left_curve:node.set_definition_expr(id_name,in_curve_calc_expr());break;
		default:break;
	}
	return node;
}
abstract_syntax_tree generator::assignment_expr()
{
	abstract_syntax_tree node;
	get_token();
	switch(this_token.type)
	{
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __id:identifier_begin_expr();break;
		case __func:function_generate_expr();break;
		case __add_operator:
		case __sub_operator:
		case __nor_operator:one_operator_expr();break;
		case __left_bracket:list_generate_expr();break;
		case __left_brace:hash_generate_expr();break;
		case __left_curve:in_curve_calc_expr();break;
		default:break;
	}
	node.set_assignment_expr();
	return node;
}
bool generator::else_if_check()
{
	token temp=this_token;
	if(this_token.type!=__else)
		return false;
	else
	{
		get_token();
		if(this_token.type!=__if)
		{
			parse.push(this_token);
			this_token=temp;// to avoid when recognizing 'else' without 'if' 
			
			return false;
		}
	}
	return true;
}
abstract_syntax_tree generator::if_else_expr()
{
	abstract_syntax_tree node;
	get_token();
	get_token();
	get_token();
	switch(this_token.type)
	{
		case __add_operator:
		case __sub_operator:
		case __nor_operator:one_operator_expr();break;
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __id:identifier_begin_expr();break;
		case __left_curve:in_curve_calc_expr();break;
		default:break;
	}
	get_token();
	get_token();
	if(this_token.type==__left_brace)
	{
		// if without { then only one statement is behind it
		parse.push(this_token);
		statements_block();
	}
	else
	{
		token t;
		switch(this_token.type)
		{
			case __var:definition_expr();check_semi_at_end();break;
			case __id:identifier_begin_expr();check_semi_at_end();break;
			case __number:number_begin_expr();check_semi_at_end();break;
			case __string:string_begin_expr();check_semi_at_end();break;
			case __if:parse.push(this_token);if_else_expr();break;
			case __left_curve:
				t=this_token;
				get_token();
				if(this_token.type==__var)
				{
					parse.push(t);
					definition_expr();
				}
				else
				{
					in_curve_calc_expr();
				}
				check_semi_at_end();
				break;
			case __add_operator:
			case __sub_operator:
			case __nor_operator:one_operator_expr();break;
			case __while:
			case __for:
			case __foreach:
			case __forindex:parse.push(this_token);loop_expr();break;
			case __continue:
			case __break:check_semi_at_end();break;
			case __semi:break;
			case __return:return_expr();check_semi_at_end();break;
			default:break;
		}
	}
	if(parse.empty())
		return node;
	get_token();
	while(this_token.type==__elsif || else_if_check())
	{
		get_token();
		get_token();
		switch(this_token.type)
		{
			case __add_operator:
			case __sub_operator:
			case __nor_operator:one_operator_expr();break;
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __id:identifier_begin_expr();break;
			default:break;
		}
		get_token();
		get_token();
		if(this_token.type==__left_brace)
		{
			// if without { then only one statement is behind it
			parse.push(this_token);
			statements_block();
		}
		else
		{
			token t;
			switch(this_token.type)
			{
				case __var:definition_expr();check_semi_at_end();break;
				case __id:identifier_begin_expr();check_semi_at_end();break;
				case __number:number_begin_expr();check_semi_at_end();break;
				case __string:string_begin_expr();check_semi_at_end();break;
				case __if:parse.push(this_token);if_else_expr();break;
				case __left_curve:
					t=this_token;
					get_token();
					if(this_token.type==__var)
					{
						parse.push(t);
						definition_expr();
					}
					else
					{
						in_curve_calc_expr();
					}
					check_semi_at_end();
					break;
				case __add_operator:
				case __sub_operator:
				case __nor_operator:one_operator_expr();break;
				case __while:
				case __for:
				case __foreach:
				case __forindex:parse.push(this_token);loop_expr();break;
				case __continue:
				case __break:check_semi_at_end();break;
				case __semi:break;
				case __return:return_expr();check_semi_at_end();break;
				default:break;
			}
		}
		if(parse.empty())
			return node;
		get_token();
	}
	if(this_token.type==__else)
	{
		get_token();
		if(this_token.type==__left_brace)
		{
			// if without { then only one statement is behind it
			parse.push(this_token);
			statements_block();
		}
		else
		{
			token t;
			switch(this_token.type)
			{
				case __var:definition_expr();check_semi_at_end();break;
				case __id:identifier_begin_expr();check_semi_at_end();break;
				case __number:number_begin_expr();check_semi_at_end();break;
				case __string:string_begin_expr();check_semi_at_end();break;
				case __if:parse.push(this_token);if_else_expr();break;
				case __left_curve:
					t=this_token;
					get_token();
					if(this_token.type==__var)
					{
						parse.push(t);
						definition_expr();
					}
					else
					{
						in_curve_calc_expr();
					}
					check_semi_at_end();
					break;
				case __add_operator:
				case __sub_operator:
				case __nor_operator:one_operator_expr();break;
				case __while:
				case __for:
				case __foreach:
				case __forindex:parse.push(this_token);loop_expr();break;
				case __continue:
				case __break:check_semi_at_end();break;
				case __semi:break;
				case __return:return_expr();check_semi_at_end();break;
				default:break;
			}
		}
	}
	else
		parse.push(this_token);
	return node;
}
abstract_syntax_tree generator::loop_expr()
{
	abstract_syntax_tree node;
	get_token();
	if(this_token.type==__while)
	{
		get_token();
		get_token();
		switch(this_token.type)
		{
			case __add_operator:
			case __sub_operator:
			case __nor_operator:one_operator_expr();break;
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __id:identifier_begin_expr();break;
			case __left_curve:in_curve_calc_expr();break;
			default:break;
		}
		get_token();
		get_token();
		if(this_token.type==__left_brace)
		{
			// if without { then only one statement is behind it
			parse.push(this_token);
			statements_block();
		}
		else
		{
			token t;
			switch(this_token.type)
			{
				case __var:definition_expr();check_semi_at_end();break;
				case __id:identifier_begin_expr();check_semi_at_end();break;
				case __number:number_begin_expr();check_semi_at_end();break;
				case __string:string_begin_expr();check_semi_at_end();break;
				case __if:parse.push(this_token);if_else_expr();break;
				case __left_curve:
					t=this_token;
					get_token();
					if(this_token.type==__var)
					{
						parse.push(t);
						definition_expr();
					}
					else
					{
						in_curve_calc_expr();
					}
					check_semi_at_end();
					break;
				case __add_operator:
				case __sub_operator:
				case __nor_operator:one_operator_expr();break;
				case __while:
				case __for:
				case __foreach:
				case __forindex:parse.push(this_token);loop_expr();break;
				case __continue:
				case __break:check_semi_at_end();break;
				case __semi:break;
				case __return:return_expr();check_semi_at_end();break;
				default:break;
			}
		}
	}
	else if(this_token.type==__for)
	{
		get_token();
		get_token();
		switch(this_token.type)
		{
			case __var:definition_expr();break;
			case __id:identifier_begin_expr();break;
			case __semi:parse.push(this_token);break;
			default:break;
		}
		check_semi_at_end();
		get_token();
		switch(this_token.type)
		{
			case __add_operator:
			case __sub_operator:
			case __nor_operator:one_operator_expr();break;
			case __id:identifier_begin_expr();break;
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __left_curve:in_curve_calc_expr();break;
			case __semi:parse.push(this_token);break;
			default:break;
		}
		check_semi_at_end();
		get_token();
		switch(this_token.type)
		{
			case __id:identifier_begin_expr();break;
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __left_curve:in_curve_calc_expr();break;
			case __right_curve:parse.push(this_token);break;
			default:break;
		}
		get_token();
		get_token();
		if(this_token.type==__left_brace)
		{
			// if without { then only one statement is behind it
			parse.push(this_token);
			statements_block();
		}
		else
		{
			token t;
			switch(this_token.type)
			{
				case __var:definition_expr();check_semi_at_end();break;
				case __id:identifier_begin_expr();check_semi_at_end();break;
				case __number:number_begin_expr();check_semi_at_end();break;
				case __string:string_begin_expr();check_semi_at_end();break;
				case __if:parse.push(this_token);if_else_expr();break;
				case __left_curve:
					t=this_token;
					get_token();
					if(this_token.type==__var)
					{
						parse.push(t);
						definition_expr();
					}
					else
					{
						in_curve_calc_expr();
					}
					check_semi_at_end();
					break;
				case __add_operator:
				case __sub_operator:
				case __nor_operator:one_operator_expr();break;
				case __while:
				case __for:
				case __foreach:
				case __forindex:parse.push(this_token);loop_expr();break;
				case __continue:
				case __break:check_semi_at_end();break;
				case __semi:break;
				case __return:return_expr();check_semi_at_end();break;
				default:break;
			}
		}
	}
	else if(this_token.type==__forindex || this_token.type==__foreach)
	{
		get_token();
		get_token();
		switch(this_token.type)
		{
			case __var:definition_expr();break;
			case __id:identifier_begin_expr();break;
			case __semi:parse.push(this_token);break;
			default:break;
		}
		check_semi_at_end();
		get_token();
		switch(this_token.type)
		{
			case __id:identifier_begin_expr();break;
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __left_curve:in_curve_calc_expr();break;
			case __left_bracket:list_generate_expr();break;
			case __right_curve:parse.push(this_token);break;
			default:break;
		}
		get_token();
		get_token();
		if(this_token.type==__left_brace)
		{
			// if without { then only one statement is behind it
			parse.push(this_token);
			statements_block();
		}
		else
		{
			token t;
			switch(this_token.type)
			{
				case __var:definition_expr();check_semi_at_end();break;
				case __id:identifier_begin_expr();check_semi_at_end();break;
				case __number:number_begin_expr();check_semi_at_end();break;
				case __string:string_begin_expr();check_semi_at_end();break;
				case __if:parse.push(this_token);if_else_expr();break;
				case __left_curve:
					t=this_token;
					get_token();
					if(this_token.type==__var)
					{
						parse.push(t);
						definition_expr();
					}
					else
					{
						in_curve_calc_expr();
					}
					check_semi_at_end();
					break;
				case __add_operator:
				case __sub_operator:
				case __nor_operator:one_operator_expr();break;
				case __while:
				case __for:
				case __foreach:
				case __forindex:parse.push(this_token);loop_expr();break;
				case __continue:
				case __break:check_semi_at_end();break;
				case __semi:break;
				case __return:return_expr();check_semi_at_end();break;
				default:break;
			}
		}
	}
	node.set_node_to_loop();
	return node;
}
abstract_syntax_tree generator::continue_break_expr()
{
	abstract_syntax_tree node;
	node.set_node_to_continue_break(this_token.type);
	return node;
}
abstract_syntax_tree generator::add_sub_operator_expr()
{
	abstract_syntax_tree node;
	get_token();
	switch(this_token.type)
	{
		case __number:node=number_begin_expr();break;
		case __string:node=string_begin_expr();break;
		case __id:node=identifier_begin_expr();break;
		case __left_curve:node=in_curve_calc_expr();break;
		default:break;
	}
	return node;
}
abstract_syntax_tree generator::mul_div_operator_expr()
{
	abstract_syntax_tree node;
	get_token();
	switch(this_token.type)
	{
		case __number:node=number_begin_expr();break;
		case __string:node=string_begin_expr();break;
		case __id:node=identifier_begin_expr();break;
		case __left_curve:node=in_curve_calc_expr();break;
		default:break;
	}
	return node;
}
abstract_syntax_tree generator::link_operator_expr()
{
	abstract_syntax_tree node;
	get_token();
	switch(this_token.type)
	{
		case __number:node=number_begin_expr();break;
		case __string:node=string_begin_expr();break;
		case __id:node=identifier_begin_expr();break;
		case __left_curve:node=in_curve_calc_expr();break;
		default:break;
	}
	return node;
}
abstract_syntax_tree generator::compare_operator_expr()
{
	abstract_syntax_tree node;
	get_token();
	switch(this_token.type)
	{
		case __number:node=number_begin_expr();break;
		case __string:node=string_begin_expr();break;
		case __id:node=identifier_begin_expr();break;
		case __left_curve:node=in_curve_calc_expr();break;
		case __add_operator:
		case __sub_operator:
		case __nor_operator:node=one_operator_expr();break;
		default:break;
	}
	return node;
}
abstract_syntax_tree generator::one_operator_expr()
{
	abstract_syntax_tree node;
	int type=this_token.type;
	get_token();
	switch(this_token.type)
	{
		case __number:node.set_one_operator(type,number_begin_expr());;break;
		case __string:node.set_one_operator(type,string_begin_expr());break;
		case __id:node.set_one_operator(type,identifier_begin_expr());break;
		case __left_curve:node.set_one_operator(type,in_curve_calc_expr());break;
		default:break;
	}
	return node;
}
abstract_syntax_tree generator::in_curve_calc_expr()
{
	abstract_syntax_tree node;
	get_token();
	switch(this_token.type)
	{
		case __add_operator:
		case __sub_operator:
		case __nor_operator:node=one_operator_expr();break;
		case __number:node=number_begin_expr();break;
		case __string:node=string_begin_expr();break;
		case __id:node=identifier_begin_expr();break;
		case __left_curve:node=in_curve_calc_expr();break;
		default:parse.push(this_token);break;
	}
	get_token();
	get_token();
	// unfinished




	switch(this_token.type)
	{
		case __add_operator:
		case __sub_operator:add_sub_operator_expr();break;
		case __mul_operator:
		case __div_operator:mul_div_operator_expr();break;
		case __link_operator:link_operator_expr();break;
		case __and_operator:
		case __or_operator:
		case __cmp_equal:
		case __cmp_not_equal:
		case __cmp_less:
		case __cmp_more:
		case __cmp_less_or_equal:
		case __cmp_more_or_equal:compare_operator_expr();break;
		default:parse.push(this_token);break;
	}
	return node;
}
abstract_syntax_tree generator::number_begin_expr()
{
	abstract_syntax_tree node;
	node.set_node_to_number(this_token.content);
	get_token();
	switch(this_token.type)
	{
		case __add_operator:
		case __sub_operator:node.set_two_operator(this_token.type,node,add_sub_operator_expr());break;
		case __mul_operator:
		case __div_operator:node.set_two_operator(this_token.type,node,mul_div_operator_expr());break;
		case __link_operator:node.set_two_operator(this_token.type,node,link_operator_expr());break;
		case __and_operator:
		case __or_operator:
		case __cmp_equal:
		case __cmp_not_equal:
		case __cmp_less:
		case __cmp_more:
		case __cmp_less_or_equal:
		case __cmp_more_or_equal:node.set_two_operator(this_token.type,node,compare_operator_expr());break;
		default:parse.push(this_token);break;
	}
	return node;
}
abstract_syntax_tree generator::string_begin_expr()
{
	abstract_syntax_tree node;
	node.set_node_to_string(this_token.content);
	get_token();
	switch(this_token.type)
	{
		case __add_operator:
		case __sub_operator:node.set_two_operator(this_token.type,node,add_sub_operator_expr());break;
		case __mul_operator:
		case __div_operator:node.set_two_operator(this_token.type,node,mul_div_operator_expr());break;
		case __link_operator:node.set_two_operator(this_token.type,node,link_operator_expr());break;
		case __and_operator:
		case __or_operator:
		case __cmp_equal:
		case __cmp_not_equal:
		case __cmp_less:
		case __cmp_more:
		case __cmp_less_or_equal:
		case __cmp_more_or_equal:node.set_two_operator(this_token.type,node,compare_operator_expr());break;
		default:parse.push(this_token);break;
	}
	return node;
}
abstract_syntax_tree generator::call_list_expr()
{
	abstract_syntax_tree node;
	get_token();
	switch(this_token.type)
	{
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __id:identifier_begin_expr();break;
		case __left_curve:in_curve_calc_expr();break;
		default:break;
	}
	get_token();
	if(this_token.type==__colon)// to check if there is [number:number] or [number:]
	{
		get_token();
		switch(this_token.type)
		{
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __id:identifier_begin_expr();break;
			case __left_curve:in_curve_calc_expr();break;
			case __right_bracket:break;// this is [number:]
		}
		get_token();
		get_token();
		switch(this_token.type)
		{
			case __left_bracket:call_list_expr();break;
			case __left_curve:call_function_expr();break;
			case __dot:call_hash_expr();break;
			default:parse.push(this_token);break;
		}
	}
	else if(this_token.type==__right_bracket)
	{
		get_token();
		switch(this_token.type)
		{
			case __left_bracket:call_list_expr();break;
			case __left_curve:call_function_expr();break;
			case __dot:call_hash_expr();break;
			default:parse.push(this_token);break;
		}
	}
	node.set_node_to_list_search();
	return node;
}
abstract_syntax_tree generator::call_function_expr()
{
	abstract_syntax_tree node;
	get_token();
	while(this_token.type!=__right_curve)
	{
		switch(this_token.type)
		{
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __id:identifier_begin_expr();break;
			case __left_bracket:list_generate_expr();break;
			case __left_brace:hash_generate_expr();break;
			case __left_curve:in_curve_calc_expr();break;
			case __func:function_generate_expr();break;
			default:break;
		}
		get_token();
		if(this_token.type==__comma)
			get_token();
	}
	get_token();
	switch(this_token.type)
	{
		case __left_bracket:call_list_expr();break;
		case __left_curve:call_function_expr();break;
		case __dot:call_hash_expr();break;
		default:parse.push(this_token);break;
	}
	
	node.set_node_to_call_function();
	return node;
}
abstract_syntax_tree generator::call_hash_expr()
{
	get_token();
	return identifier_begin_expr();
}
abstract_syntax_tree generator::identifier_begin_expr()
{
	// __id has been checked
	abstract_syntax_tree node;
	
	node.set_node_to_id(this_token.content);
	
	get_token();
	switch(this_token.type)
	{
		case __left_bracket:node=call_list_expr();break;
		case __left_curve:node=call_function_expr();break;
		case __dot:node=call_hash_expr();break;
		case __add_operator:
		case __sub_operator:node.set_two_operator(this_token.type,node,add_sub_operator_expr());break;
		case __mul_operator:
		case __div_operator:node.set_two_operator(this_token.type,node,mul_div_operator_expr());break;
		case __link_operator:node.set_two_operator(this_token.type,node,link_operator_expr());break;
		case __and_operator:
		case __or_operator:
		case __cmp_equal:
		case __cmp_not_equal:
		case __cmp_less:
		case __cmp_more:
		case __cmp_less_or_equal:
		case __cmp_more_or_equal:node.set_two_operator(this_token.type,node,compare_operator_expr());break;
		default:parse.push(this_token);break;
	}
	get_token();
	switch(this_token.type)
	{
		case __add_operator:
		case __sub_operator:node.set_two_operator(this_token.type,node,add_sub_operator_expr());break;
		case __mul_operator:
		case __div_operator:node.set_two_operator(this_token.type,node,mul_div_operator_expr());break;
		case __link_operator:node.set_two_operator(this_token.type,node,link_operator_expr());break;
		case __and_operator:
		case __or_operator:
		case __cmp_equal:
		case __cmp_not_equal:
		case __cmp_less:
		case __cmp_more:
		case __cmp_less_or_equal:
		case __cmp_more_or_equal:node.set_two_operator(this_token.type,node,compare_operator_expr());break;
		case __equal:
		case __add_equal:
		case __sub_equal:
		case __mul_equal:
		case __div_equal:
		case __link_equal:node=assignment_expr();break;
		default:parse.push(this_token);break;
	}
	return node;
}
void generator::gen_main_work()
{
	root.tree_set_clear();
	
	while(!parse.empty())
	{
		get_token();
		token t;
		switch(this_token.type)
		{
			case __var:root.add_statement(definition_expr());check_semi_at_end();break;
			case __id:root.add_statement(identifier_begin_expr());check_semi_at_end();break;
			case __number:root.add_statement(number_begin_expr());check_semi_at_end();break;
			case __string:root.add_statement(string_begin_expr());check_semi_at_end();break;
			case __left_curve:
				t=this_token;
				get_token();
				if(this_token.type==__var)
				{
					parse.push(t);
					root.add_statement(definition_expr());
				}
				else
				{
					root.add_statement(in_curve_calc_expr());
				}
				check_semi_at_end();
				break;
			case __if:parse.push(this_token);root.add_statement(if_else_expr());break;
			case __add_operator:
			case __sub_operator:
			case __nor_operator:root.add_statement(one_operator_expr());break;
			case __while:
			case __for:
			case __foreach:
			case __forindex:parse.push(this_token);root.add_statement(loop_expr());break;
			case __semi:break;
			default:break;
		}
	}
	std::cout<<">>[Abstract-syntax-tree] complete generation."<<std::endl;
	return;
}

#endif
