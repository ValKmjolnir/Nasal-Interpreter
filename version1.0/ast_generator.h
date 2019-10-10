#ifndef __AST_GENERATOR_H__
#define __AST_GENERATOR_H__

class generator
{
	private:
		abstract_syntax_tree root;
		std::stack<token> parse;
		std::stack<abstract_syntax_tree> node_cache;
		token this_token;
	public:
		generator()
		{
			this_token.type=0;
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
		void in_curve_calc_expr();
		void number_begin_expr();
		void string_begin_expr();
		void identifier_begin_expr();
		void call_list_expr();
		void call_function_expr();
		void call_hash_expr();
		void list_generate_expr();
		void hash_generate_expr();
		void definition_expr();
		void assignment_expr();
		void loop_expr();
		bool else_if_check();
		void if_else_expr();
		void add_sub_operator_expr();
		void mul_div_operator_expr();
		void link_operator_expr();
		void compare_operator_expr();
		void one_operator_expr();
		void check_semi_at_end();
		void statements_block();
		void function_generate_expr();
		void return_expr();
};
void generator::check_semi_at_end()
{
	get_token();
	return;
}
void generator::return_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __id:identifier_begin_expr();break;
		case __func:function_generate_expr();break;
		case __left_bracket:list_generate_expr();break;
		case __left_brace:hash_generate_expr();break;
		case __semi:parse.push(this_token);break;
		default:break;
	}
	return;
}
void generator::statements_block()
{
	get_token();
	get_token();
	while(this_token.type!=__right_brace)
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
		get_token();
	}
	return;
}
void generator::function_generate_expr()
{
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
				get_token();
				if(this_token.type==__right_curve)
					parse.push(this_token);
			}
			else if(this_token.type==__dynamic_id)
			{
				get_token();
				parse.push(this_token);
			}
			get_token();
		}
	}
	statements_block();
	return;
}
void generator::list_generate_expr()
{
	get_token();
	while(this_token.type!=__right_bracket)
	{
		switch(this_token.type)
		{
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __id:identifier_begin_expr();break;
			case __left_bracket:list_generate_expr();break;
			case __left_brace:hash_generate_expr();break;
			case __left_curve:in_curve_calc_expr();break;
			default:break;
		}
		get_token();
		if(this_token.type==__comma)
			get_token();
	}
	return;
}
void generator::hash_generate_expr()
{
	get_token();
	while(this_token.type!=__right_brace)
	{
		switch(this_token.type)
		{
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __id:identifier_begin_expr();break;
			default:break;
		}
		get_token();
		get_token();
		switch(this_token.type)
		{
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __id:identifier_begin_expr();break;
			case __func:function_generate_expr();break;
			case __left_bracket:list_generate_expr();break;
			case __left_brace:hash_generate_expr();break;
			case __left_curve:in_curve_calc_expr();break;
			default:break;
		}
		get_token();
		if(this_token.type==__comma)
			get_token();
	}
	return;
}
void generator::definition_expr()
{
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
	get_token();
	if(this_token.type==__semi)
	{
		parse.push(this_token);
		return;
	}
	get_token();
	token t;
	t.line=this_token.line;
	t.type=__semi;
	switch(this_token.type)
	{
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __id:identifier_begin_expr();break;
		case __func:function_generate_expr();parse.push(t);break;
		case __add_operator:
		case __sub_operator:
		case __nor_operator:one_operator_expr();break;
		case __left_bracket:list_generate_expr();break;
		case __left_brace:hash_generate_expr();break;
		case __left_curve:in_curve_calc_expr();break;
		default:break;
	}
	return;
}
void generator::assignment_expr()
{
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
	return;
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
void generator::if_else_expr()
{
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
		return;
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
			return;
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
	return;
}
void generator::loop_expr()
{
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
	return;
}
void generator::add_sub_operator_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __id:identifier_begin_expr();break;
		case __left_curve:in_curve_calc_expr();break;
		default:break;
	}
	return;
}
void generator::mul_div_operator_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __id:identifier_begin_expr();break;
		case __left_curve:in_curve_calc_expr();break;
		default:break;
	}
	return;
}
void generator::link_operator_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __id:identifier_begin_expr();break;
		case __left_curve:in_curve_calc_expr();break;
		default:break;
	}
	return;
}
void generator::compare_operator_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __id:identifier_begin_expr();break;
		case __left_curve:in_curve_calc_expr();break;
		case __add_operator:
		case __sub_operator:
		case __nor_operator:one_operator_expr();break;
		default:break;
	}
	return;
}
void generator::one_operator_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __id:identifier_begin_expr();break;
		case __left_curve:in_curve_calc_expr();break;
		default:break;
	}
	return;
}
void generator::in_curve_calc_expr()
{
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
		default:parse.push(this_token);break;
	}
	get_token();
	get_token();
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
	return;
}
void generator::number_begin_expr()
{
	get_token();
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
	return;
}
void generator::string_begin_expr()
{
	get_token();
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
	return;
}
void generator::call_list_expr()
{
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
			case __right_bracket:return;break;// this is [number:]
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
	return;
}
void generator::call_function_expr()
{
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
	return;
}
void generator::call_hash_expr()
{
	get_token();
	identifier_begin_expr();
	return;
}
void generator::identifier_begin_expr()
{
	// __id has been checked
	get_token();
	switch(this_token.type)
	{
		case __left_bracket:call_list_expr();break;
		case __left_curve:call_function_expr();break;
		case __dot:call_hash_expr();break;
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
	get_token();
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
		case __equal:
		case __add_equal:
		case __sub_equal:
		case __mul_equal:
		case __div_equal:
		case __link_equal:assignment_expr();break;
		default:parse.push(this_token);break;
	}
	return;
}
void generator::gen_main_work()
{
	while(!parse.empty())
	{
		get_token();
		token t;
		switch(this_token.type)
		{
			case __var:definition_expr();check_semi_at_end();break;
			case __id:identifier_begin_expr();check_semi_at_end();break;
			case __number:number_begin_expr();check_semi_at_end();break;
			case __string:string_begin_expr();check_semi_at_end();break;
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
			case __if:parse.push(this_token);if_else_expr();break;
			case __add_operator:
			case __sub_operator:
			case __nor_operator:one_operator_expr();break;
			case __while:
			case __for:
			case __foreach:
			case __forindex:parse.push(this_token);loop_expr();break;
			case __semi:break;
			default:break;
		}
	}
	std::cout<<">>[Abstract-syntax-tree] complete generation."<<std::endl;
	return;
}

#endif
