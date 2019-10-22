#ifndef __NASAL_PARSER_H__
#define __NASAL_PARSER_H__

class nasal_parser
{
	private:
		abstract_syntax_tree root;
		std::stack<token> parse;
		token this_token;
		int error;
		int warning;
	public:
		nasal_parser()
		{
			error=0;
			warning=0;
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
		int get_error_num()
		{
			return error;
		}
		void print_generated_ast()
		{
			std::cout<<">>[Abstract-syntax-tree]"<<std::endl;
			root.print_tree(1);
			return;
		}
		void print_parser_stack()
		{
			if(parse.empty())
				return;
			int line=0;
			std::stack<token> temp;
			while(!parse.empty())
			{
				temp.push(parse.top());
				if(line!=temp.top().line)
				{
					if(line+1==temp.top().line)
					{
						line=temp.top().line;
						if(line==1)
							std::cout<<"line "<<line<<"   ";
						else
							std::cout<<std::endl<<"line "<<line<<"   ";
					}
					else
					{
						while(line!=temp.top().line)
						{
							++line;
							std::cout<<std::endl<<"line "<<line<<"   ";
						}
					}
				}
				std::cout<<"<";
				print_token(temp.top().type);
				std::cout<<"> ";
				parse.pop();
			}
			while(!temp.empty())
			{
				parse.push(temp.top());
				temp.pop();
			}
			std::cout<<std::endl;
			return;
		}
		void parse_process(std::list<token>& lexer)
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
				std::cout<<">>[Parse] warning: empty lexer list."<<std::endl;
				return;
			}
			while(!temp.empty())
			{
				parse.push(temp.top());
				temp.pop();
			}
			return;
		}
		void parse_main_work();
		abstract_syntax_tree in_curve_calc_expr();
		abstract_syntax_tree calculation_expr();
		abstract_syntax_tree identifier_call_expr();
		abstract_syntax_tree call_list_expr();
		abstract_syntax_tree parameter_function_expr();
		abstract_syntax_tree call_function_expr();
		abstract_syntax_tree call_hash_expr();
		abstract_syntax_tree list_generate_expr();
		abstract_syntax_tree hash_generate_expr();
		abstract_syntax_tree definition_expr();
		abstract_syntax_tree assignment_expr();
		abstract_syntax_tree loop_expr();
		bool else_if_check();
		abstract_syntax_tree if_else_expr();
		abstract_syntax_tree mul_div_operator_expr();
		abstract_syntax_tree unary_operator_expr();
		void check_semi_at_end();
		abstract_syntax_tree statements_block();
		abstract_syntax_tree function_generate_expr();
		abstract_syntax_tree return_expr();
};
void nasal_parser::check_semi_at_end()
{
	get_token();
	if(this_token.type!=__semi)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a ';'."<<std::endl;
	}
	return;
}
abstract_syntax_tree nasal_parser::return_expr()
{
	abstract_syntax_tree node;
	node.set_node_type(__return);
	get_token();
	switch(this_token.type)
	{
		case __left_curve:
		case __number:
		case __string:
		case __id:
			parse.push(this_token);
			node.add_child(calculation_expr());
			break;
		case __func:
			node.add_child(function_generate_expr());
			break;
		case __left_bracket:
			node.add_child(list_generate_expr());
			break;
		case __left_brace:
			node.add_child(hash_generate_expr());
			break;
		case __semi:parse.push(this_token);break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a data or ';' after __return_expr."<<std::endl;
			return node;
			break;
	}
	return node;
}
abstract_syntax_tree nasal_parser::statements_block()
{
	abstract_syntax_tree node;
	abstract_syntax_tree temp;
	node.set_node_type(__normal_statement_block);
	get_token();
	if(this_token.type!=__left_brace)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a '{' when generating a __block."<<std::endl;
		return node;
	}
	get_token();
	while(this_token.type!=__right_brace)
	{
		temp.set_clear();
		switch(this_token.type)
		{
			case __var:
				node.add_child(definition_expr());
				check_semi_at_end();break;
			case __left_curve:
			case __sub_operator:
			case __nor_operator:
			case __id:
			case __number:
			case __string:
				parse.push(this_token);
				node.add_child(calculation_expr());
				check_semi_at_end();break;
			case __if:
				parse.push(this_token);
				node.add_child(if_else_expr());
				break;
			case __while:
			case __for:
			case __foreach:
			case __forindex:
				parse.push(this_token);
				node.add_child(loop_expr());
				break;
			case __continue:
			case __break:
				temp.set_node_type(this_token.type);
				node.add_child(temp);
				check_semi_at_end();break;
			case __semi:break;
			case __return:
				node.add_child(return_expr());
				check_semi_at_end();break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": '";
				print_token(this_token.type);
				std::cout<<"' incorrect token as the beginning of statement."<<std::endl;
				return node;
				break;
		}
		get_token();
	}
	return node;
}
abstract_syntax_tree nasal_parser::function_generate_expr()
{
	abstract_syntax_tree node;
	abstract_syntax_tree temp;
	node.set_node_type(__function);
	get_token();
	if(this_token.type==__left_brace)
		parse.push(this_token);
	else if(this_token.type==__left_curve)
	{
		get_token();
		while(this_token.type!=__right_curve)
		{
			temp.set_clear();
			if(this_token.type==__id)
			{
				temp.set_node_type(__id);
				temp.set_var_name(this_token.content);
				get_token();
				if(this_token.type!=__equal)
					parse.push(this_token);
				else
				{
					get_token();
					switch(this_token.type)
					{
						case __left_curve:
						case __id:
						case __number:
						case __string:
						case __nor_operator:
						case __sub_operator:
							parse.push(this_token);
							temp.add_child(calculation_expr());
							break;
						case __left_brace:
							temp.add_child(hash_generate_expr());
							break;
						case __left_bracket:
							temp.add_child(list_generate_expr());
							break;
						case __func:
							temp.add_child(function_generate_expr());
							break;
						default:
							++error;
							std::cout<<">>[Error] line "<<this_token.line<<": expect a scalar after operator '=' ."<<std::endl;
							return node;
							break;
					}
				}
				node.add_child(temp);
				get_token();
				if(this_token.type!=__right_curve && this_token.type!=__comma)
				{
					++error;
					std::cout<<">>[Error] line "<<this_token.line<<": expect a ',' or ')' when creating a __function."<<std::endl;
					return node;
				}
				else if(this_token.type==__right_curve)
					parse.push(this_token);
			}
			else if(this_token.type==__dynamic_id)
			{
				get_token();
				if(this_token.type!=__right_curve)
				{
					++error;
					std::cout<<">>[Error] line "<<this_token.line<<": must put ')' after __dynamic_id."<<std::endl;
					return node;
				}
				parse.push(this_token);
			}
			else
			{
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": expect __id and __dynamic_id only."<<std::endl;
				return node;
			}
			get_token();
		}
	}
	else
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": incorrect token '";
		print_token(this_token.type);
		std::cout<<"' when creating a __function."<<std::endl;
		return node;
	}
	get_token();
	if(this_token.type==__left_brace)
	{
		parse.push(this_token);
		node.add_child(statements_block());
	}
	else
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": function must have a statement block begin with '{' ."<<std::endl;
		return node;
	}
	return node;
}
abstract_syntax_tree nasal_parser::list_generate_expr()
{
	abstract_syntax_tree node;
	node.set_node_type(__list);
	get_token();
	while(this_token.type!=__right_bracket)
	{
		switch(this_token.type)
		{
			case __nor_operator:
			case __sub_operator:
			case __left_curve:
			case __number:
			case __string:
			case __id:
				parse.push(this_token);
				node.add_child(calculation_expr());
				break;
			case __left_bracket:
				node.add_child(list_generate_expr());
				break;
			case __left_brace:
				node.add_child(hash_generate_expr());
				break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": incorrect token '";
				print_token(this_token.type);
				std::cout<<"' when creating a __list."<<std::endl;
				return node;
				break;
		}
		get_token();
		if(this_token.type!=__comma && this_token.type!=__right_bracket)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ',' or ']' but get '";
			print_token(this_token.type);
			std::cout<<"' when creating a __list."<<std::endl;
			return node;
		}
		else if(this_token.type==__comma)
			get_token();
	}
	return node;
}
abstract_syntax_tree nasal_parser::hash_generate_expr()
{
	abstract_syntax_tree node;
	abstract_syntax_tree temp;
	node.set_node_type(__hash);
	get_token();
	while(this_token.type!=__right_brace)
	{
		if(this_token.type!=__id && this_token.type!=__string)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": incorrect token '";
			print_token(this_token.type);
			std::cout<<"' when creating a __hash_member."<<std::endl;
			return node;
		}
		else
		{
			parse.push(this_token);
			node.add_child(calculation_expr());
		}
		get_token();
		if(this_token.type!=__colon)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ':' when creating a __hash_member."<<std::endl;
			return node;
		}
		get_token();
		switch(this_token.type)
		{
			case __left_curve:
			case __number:
			case __string:
			case __id:
				parse.push(this_token);
				node.add_child(calculation_expr());
				break;
			case __func:
				get_token();
				if(this_token.type==__id)
				{
					parse.push(this_token);
					node.add_child(calculation_expr());
				}
				else
				{
					parse.push(this_token);
					node.add_child(function_generate_expr());
				}
				break;
			case __left_bracket:
				node.add_child(list_generate_expr());
				break;
			case __left_brace:
				node.add_child(hash_generate_expr());
				break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": incorrect token '";
				print_token(this_token.type);
				std::cout<<"' when creating a __hash_member."<<std::endl;
				return node;
				break;
		}
		get_token();
		if(this_token.type!=__comma && this_token.type!=__right_brace)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ',' or '}' but get '";
			print_token(this_token.type);
			std::cout<<"' when creating a __hash."<<std::endl;
			return node;
		}
		else if(this_token.type==__comma)
			get_token();
	}
	return node;
}
abstract_syntax_tree nasal_parser::definition_expr()
{
	abstract_syntax_tree node;
	node.set_node_type(__definition);
	get_token();
	if(this_token.type!=__id)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect an __id."<<std::endl;
		return node;
	}
	else
	{
		abstract_syntax_tree new_identifier;
		new_identifier.set_node_type(__id);
		new_identifier.set_var_name(this_token.content);
		node.add_child(new_identifier);
	}
	get_token();
	if(this_token.type!=__equal && this_token.type!=__semi)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a '=' after __id."<<std::endl;
		return node;
	}
	else if(this_token.type==__semi)
	{
		parse.push(this_token);// for semi check
		++warning;
		std::cout<<">>[Warning] line "<<this_token.line<<": better initializing this."<<std::endl;
		return node;
	}
	get_token();
	token semi_token;
	semi_token.line=this_token.line;
	semi_token.type=__semi;
	semi_token.content=";";
	switch(this_token.type)
	{
		case __left_curve:
		case __sub_operator:
		case __nor_operator:
		case __number:
		case __string:
		case __id:
			parse.push(this_token);
			node.add_child(calculation_expr());
			break;
		case __func:
			node.add_child(function_generate_expr());
			parse.push(semi_token);
			break;
		case __left_bracket:
			node.add_child(list_generate_expr());
			break;
		case __left_brace:
			node.add_child(hash_generate_expr());
			break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a data after this operator."<<std::endl;
			return node;
			break;
	}
	return node;
}
abstract_syntax_tree nasal_parser::assignment_expr()
{
	abstract_syntax_tree node;
	get_token();
	switch(this_token.type)
	{
		case __left_curve:
		case __sub_operator:
		case __nor_operator:
		case __number:
		case __string:
		case __id:
			parse.push(this_token);
			node=calculation_expr();
			break;
		case __func:
			node=function_generate_expr();
			break;
		case __left_bracket:
			node=list_generate_expr();
			break;
		case __left_brace:
			node=hash_generate_expr();
			break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": incorrect data type when doing assignment."<<std::endl;
			return node;
			break;
	}
	return node;
}
bool nasal_parser::else_if_check()
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
			this_token=temp;
			// to avoid when recognizing 'else' without 'if' 
			return false;
		}
	}
	return true;
}
abstract_syntax_tree nasal_parser::if_else_expr()
{
	abstract_syntax_tree node;
	abstract_syntax_tree temp;
	abstract_syntax_tree temp2;
	node.set_node_type(__ifelse);
	temp.set_clear();
	get_token();
	if(this_token.type!=__if)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a 'if' when creating new if-else statement."<<std::endl;
		return node;
	}
	temp.set_node_type(__if);
	get_token();
	if(this_token.type!=__left_curve)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a '(' when making a condition."<<std::endl;
		return node;
	}
	get_token();
	switch(this_token.type)
	{
		case __left_curve:
		case __sub_operator:
		case __nor_operator:
		case __number:
		case __string:
		case __id:
			parse.push(this_token);
			temp.add_child(calculation_expr());
			break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a correct condition."<<std::endl;
			return node;
			break;
	}
	get_token();
	if(this_token.type!=__right_curve)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a ')' when making a condition."<<std::endl;
		return node;
	}
	get_token();
	if(this_token.type==__left_brace)
	{
		// if without { then only one statement is behind it
		parse.push(this_token);
		temp.add_child(statements_block());
	}
	else
	{
		switch(this_token.type)
		{
			case __var:
				temp.add_child(definition_expr());
				check_semi_at_end();
				break;
			case __sub_operator:
			case __nor_operator:
			case __id:
			case __number:
			case __string:
			case __left_curve:
				parse.push(this_token);
				temp.add_child(calculation_expr());
				check_semi_at_end();
				break;
			case __if:
				parse.push(this_token);
				temp.add_child(if_else_expr());
				break;
			case __while:
			case __for:
			case __foreach:
			case __forindex:
				parse.push(this_token);
				temp.add_child(loop_expr());
				break;
			case __continue:
			case __break:
				temp2.set_clear();
				temp2.set_node_type(this_token.type);
				temp.add_child(temp2);
				check_semi_at_end();
				break;
			case __semi:break;
			case __return:
				temp.add_child(return_expr());
				check_semi_at_end();
				break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' when generating a statement."<<std::endl;
				return node;
				break;
		}
	}
	node.add_child(temp);
	temp.set_clear();
	if(parse.empty())
		return node;
	get_token();
	while(this_token.type==__elsif || else_if_check())
	{
		temp.set_node_type(__elsif);
		get_token();
		if(this_token.type!=__left_curve)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a '(' when making a condition."<<std::endl;
			return node;
		}
		get_token();
		switch(this_token.type)
		{
			case __left_curve:
			case __sub_operator:
			case __nor_operator:
			case __number:
			case __string:
			case __id:
				parse.push(this_token);
				temp.add_child(calculation_expr());
				break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": expect a correct condition."<<std::endl;
				return node;
				break;
		}
		get_token();
		if(this_token.type!=__right_curve)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ')' when making a condition."<<std::endl;
			return node;
		}
		get_token();
		if(this_token.type==__left_brace)
		{
			// if without { then only one statement is behind it
			parse.push(this_token);
			temp.add_child(statements_block());
		}
		else
		{
			switch(this_token.type)
			{
				case __var:
					temp.add_child(definition_expr());
					check_semi_at_end();
					break;
				case __sub_operator:
				case __nor_operator:
				case __id:
				case __number:
				case __string:
				case __left_curve:
					parse.push(this_token);
					temp.add_child(calculation_expr());
					check_semi_at_end();
					break;
				case __if:
					parse.push(this_token);
					temp.add_child(if_else_expr());
					break;
				case __while:
				case __for:
				case __foreach:
				case __forindex:
					parse.push(this_token);
					temp.add_child(loop_expr());
					break;
				case __continue:
				case __break:
					temp2.set_node_type(this_token.type);
					temp.add_child(temp2);
					check_semi_at_end();
					break;
				case __semi:break;
				case __return:
					temp.add_child(return_expr());
					check_semi_at_end();
					break;
				default:
					++error;
					std::cout<<">>[Error] line "<<this_token.line<<": \'";
					print_token(this_token.type);
					std::cout<<"\' when generating a statement."<<std::endl;
					return node;
					break;
			}
		}
		if(parse.empty())
			return node;
		get_token();
	}
	node.add_child(temp);
	temp.set_clear();
	if(this_token.type==__else)
	{
		temp.set_node_type(__else);
		get_token();
		if(this_token.type==__left_brace)
		{
			// if without { then only one statement is behind it
			parse.push(this_token);
			temp.add_child(statements_block());
		}
		else
		{
			switch(this_token.type)
			{
				case __var:
					temp.add_child(definition_expr());
					check_semi_at_end();
					break;
				case __sub_operator:
				case __nor_operator:
				case __id:
				case __number:
				case __string:
				case __left_curve:
					parse.push(this_token);
					temp.add_child(calculation_expr());
					check_semi_at_end();
					break;
				case __if:
					parse.push(this_token);
					temp.add_child(if_else_expr());
					break;
				case __while:
				case __for:
				case __foreach:
				case __forindex:
					parse.push(this_token);
					temp.add_child(loop_expr());
					break;
				case __continue:
				case __break:
					temp2.set_node_type(this_token.type);
					temp.add_child(temp2);
					check_semi_at_end();
					break;
				case __semi:break;
				case __return:
					temp.add_child(return_expr());
					check_semi_at_end();
					break;
				default:
					++error;
					std::cout<<">>[Error] line "<<this_token.line<<": \'";
					print_token(this_token.type);
					std::cout<<"\' when generating a statement."<<std::endl;
					return node;
					break;
			}
		}
	}
	else
		parse.push(this_token);
	return node;
}
abstract_syntax_tree nasal_parser::loop_expr()
{
	abstract_syntax_tree node;
	abstract_syntax_tree temp;
	get_token();
	if(this_token.type==__while)
	{
		node.set_node_type(__while);
		get_token();
		if(this_token.type!=__left_curve)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a '(' after 'while'."<<std::endl;
			return node;
		}
		get_token();
		switch(this_token.type)
		{
			case __left_curve:
			case __sub_operator:
			case __nor_operator:
			case __number:
			case __string:
			case __id:
				parse.push(this_token);
				node.add_child(calculation_expr());
				break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": expect a condition."<<std::endl;
				return node;
				break;
		}
		get_token();
		if(this_token.type!=__right_curve)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ')' after 'while('."<<std::endl;
			return node;
		}
		get_token();
		if(this_token.type==__left_brace)
		{
			// if without { then only one statement is behind it
			parse.push(this_token);
			node.add_child(statements_block());
		}
		else
		{

			switch(this_token.type)
			{
				case __var:
					node.add_child(definition_expr());
					check_semi_at_end();
					break;
				case __sub_operator:
				case __nor_operator:
				case __id:
				case __number:
				case __string:
				case __left_curve:
					parse.push(this_token);
					node.add_child(calculation_expr());
					check_semi_at_end();
					break;
				case __if:
					parse.push(this_token);
					node.add_child(if_else_expr());
					break;
				case __while:
				case __for:
				case __foreach:
				case __forindex:
					parse.push(this_token);
					node.add_child(loop_expr());
					break;
				case __continue:
				case __break:
					temp.set_node_type(this_token.type);
					node.add_child(temp);
					check_semi_at_end();
					break;
				case __semi:break;
				case __return:
					node.add_child(return_expr());
					check_semi_at_end();
					break;
				default:
					++error;
					std::cout<<">>[Error] line "<<this_token.line<<": \'";
					print_token(this_token.type);
					std::cout<<"\' when creating a new loop."<<std::endl;
					return node;
					break;
			}
		}
	}
	else if(this_token.type==__for)
	{
		node.set_node_type(__for);
		get_token();
		if(this_token.type!=__left_curve)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a '(' after 'for'."<<std::endl;
			return node;
		}
		get_token();
		switch(this_token.type)
		{
			case __var:
				node.add_child(definition_expr());
				break;
			case __id:
				node.add_child(calculation_expr());
				break;
			case __semi:parse.push(this_token);break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' when creating a new loop."<<std::endl;
				return node;
				break;
		}
		check_semi_at_end();
		get_token();
		switch(this_token.type)
		{
			case __left_curve:
			case __sub_operator:
			case __nor_operator:
			case __id:
			case __number:
			case __string:
				parse.push(this_token);
				node.add_child(calculation_expr());
				break;
			case __semi:parse.push(this_token);break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' when creating a new loop."<<std::endl;
				return node;
				break;
		}
		check_semi_at_end();
		get_token();
		switch(this_token.type)
		{
			case __left_curve:
			case __id:
			case __number:
			case __string:
				parse.push(this_token);
				node.add_child(calculation_expr());
				break;
			case __right_curve:parse.push(this_token);break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' when creating a new loop."<<std::endl;
				return node;
				break;
		}
		get_token();
		if(this_token.type!=__right_curve)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ')' after 'for('."<<std::endl;
			return node;
		}
		get_token();
		if(this_token.type==__left_brace)
		{
			// if without { then only one statement is behind it
			parse.push(this_token);
			node.add_child(statements_block());
		}
		else
		{
			switch(this_token.type)
			{
				case __var:
					node.add_child(definition_expr());
					check_semi_at_end();
					break;
				case __sub_operator:
				case __nor_operator:
				case __id:
				case __number:
				case __string:
				case __left_curve:
					parse.push(this_token);
					node.add_child(calculation_expr());
					check_semi_at_end();
					break;
				case __if:
					parse.push(this_token);
					node.add_child(if_else_expr());
					break;
				case __while:
				case __for:
				case __foreach:
				case __forindex:
					parse.push(this_token);
					node.add_child(loop_expr());
					break;
				case __continue:
				case __break:
					temp.set_node_type(this_token.type);
					node.add_child(temp);
					check_semi_at_end();
					break;
				case __semi:break;
				case __return:
					node.add_child(return_expr());
					check_semi_at_end();
					break;
				default:
					++error;
					std::cout<<">>[Error] line "<<this_token.line<<": \'";
					print_token(this_token.type);
					std::cout<<"\' when creating a new loop."<<std::endl;
					return node;
					break;
			}
		}
	}
	else if(this_token.type==__forindex || this_token.type==__foreach)
	{
		node.set_node_type(this_token.type);
		get_token();
		if(this_token.type!=__left_curve)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a '(' after 'forindex' or 'foreach'."<<std::endl;
			return node;
		}
		get_token();
		switch(this_token.type)
		{
			case __var:
				node.add_child(definition_expr());
				break;
			case __id:
				parse.push(this_token);
				node.add_child(calculation_expr());
				break;
			case __semi:parse.push(this_token);break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' when creating a new loop."<<std::endl;
				return node;
				break;
		}
		check_semi_at_end();
		get_token();
		switch(this_token.type)
		{
			case __left_curve:
			case __id:
			case __number:
			case __string:
				parse.push(this_token);
				node.add_child(calculation_expr());
				break;
			case __left_bracket:
				node.add_child(list_generate_expr());
				break;
			case __right_curve:parse.push(this_token);break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' when creating a new loop."<<std::endl;
				return node;
				break;
		}
		get_token();
		if(this_token.type!=__right_curve)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ')' after 'forindex(' or 'foreach('."<<std::endl;
			return node;
		}
		get_token();
		if(this_token.type==__left_brace)
		{
			// if without { then only one statement is behind it
			parse.push(this_token);
			node.add_child(statements_block());
		}
		else
		{
			switch(this_token.type)
			{
				case __var:
					node.add_child(definition_expr());
					check_semi_at_end();
					break;
				case __sub_operator:
				case __nor_operator:
				case __id:
				case __number:
				case __string:
				case __left_curve:
					parse.push(this_token);
					node.add_child(calculation_expr());
					check_semi_at_end();
					break;
				case __if:
					parse.push(this_token);
					node.add_child(if_else_expr());
					break;
				case __while:
				case __for:
				case __foreach:
				case __forindex:
					parse.push(this_token);
					node.add_child(loop_expr());
					break;
				case __continue:
				case __break:
					temp.set_node_type(this_token.type);
					node.add_child(temp);
					check_semi_at_end();
					break;
				case __semi:break;
				case __return:
					node.add_child(return_expr());
					check_semi_at_end();
					break;
				default:
					++error;
					std::cout<<">>[Error] line "<<this_token.line<<": \'";
					print_token(this_token.type);
					std::cout<<"\' when creating a new loop."<<std::endl;
					return node;
					break;
			}
		}
	}
	else
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": incorrect token '";
		print_token(this_token.type);
		std::cout<<"' when creating a new loop."<<std::endl;
		return node;
	}
	return node;
}
abstract_syntax_tree nasal_parser::mul_div_operator_expr()
{
	abstract_syntax_tree node;
	abstract_syntax_tree temp;
	node.set_node_type(this_token.type);
	while(1)
	{
		temp.set_clear();
		get_token();
		switch(this_token.type)
		{
			case __number:
				temp.set_node_type(__number);
				temp.set_var_number(this_token.content);
				node.add_child(temp);
				break;
			case __string:
				temp.set_node_type(__string);
				temp.set_var_string(this_token.content);
				node.add_child(temp);
				break;
			case __id:
				temp.set_node_type(__id);
				temp.set_var_name(this_token.content);
				temp.add_child(identifier_call_expr());
				node.add_child(temp);
				break;
			case __left_curve:
				parse.push(this_token);
				node.add_child(calculation_expr());
				break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": expect a data after operator '*' or '/'."<<std::endl;
				return node;
				break;
		}
		get_token();
		if(this_token.type!=__mul_operator && this_token.type!=__div_operator)
		{
			parse.push(this_token);
			break;
		}
	}
	return node;
}
abstract_syntax_tree nasal_parser::unary_operator_expr()
{
	abstract_syntax_tree node;
	abstract_syntax_tree temp;
	node.set_node_type(__unary_operation);
	get_token();
	switch(this_token.type)
	{
		case __number:
			temp.set_node_type(__number);
			temp.set_var_number(this_token.content);
			node.add_child(temp);
			break;
		case __string:
			temp.set_node_type(__string);
			temp.set_var_string(this_token.content);
			node.add_child(temp);
			break;
		case __id:
			temp.set_node_type(__id);
			temp.set_var_name(this_token.content);
			temp.add_child(identifier_call_expr());
			node.add_child(temp);
			break;
		case __left_curve:
			parse.push(this_token);
			node.add_child(calculation_expr());
			break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a data after operator '!' or '-'."<<std::endl;
			return node;
			break;
	}
	return node;
}
abstract_syntax_tree nasal_parser::in_curve_calc_expr()
{
	abstract_syntax_tree node;
	node=calculation_expr();
	get_token();
	if(this_token.type!=__right_curve)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a ')' when generating __in_curve_calculation."<<std::endl;
		return node;
	}
	get_token();
	switch(this_token.type)
	{
		case __left_bracket:
			node.add_child(call_list_expr());
			break;
		case __left_curve:
			node.add_child(call_function_expr());
			break;
		default:parse.push(this_token);break;
	}
	return node;
}
abstract_syntax_tree nasal_parser::calculation_expr()
{
	abstract_syntax_tree node;
	abstract_syntax_tree temp;
	// number / string / identifier haven't been checked
	while(1)
	{
		get_token(); // check number / string / identifier
		if(this_token.type==__nor_operator || this_token.type==__sub_operator)
			node=unary_operator_expr();
		else if(this_token.type==__id)
			node=identifier_call_expr();
		else if(this_token.type==__number || this_token.type==__string)
		{
			node.set_node_type(this_token.type);
			switch(this_token.type)
			{
				case __number:node.set_node_type(__number);node.set_var_number(this_token.content);break;
				case __string:node.set_node_type(__string);node.set_var_string(this_token.content);break;
			}
		}
		else if(this_token.type==__left_curve)
			node=in_curve_calc_expr();
		else if(this_token.type==__func)
		{
			node=function_generate_expr();
			return node;
		}
		else
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a scalar before operator '+' , '-' , '*' , '/' or '~'."<<std::endl;
			return node;
		}
		get_token();
		switch(this_token.type)
		{
			case __cmp_equal:
			case __cmp_not_equal:
			case __cmp_less:
			case __cmp_more:
			case __cmp_less_or_equal:
			case __cmp_more_or_equal:
			case __and_operator:
			case __or_operator:
			case __add_operator:
			case __sub_operator:
			case __link_operator:
				temp.set_clear();
				temp.set_node_type(this_token.type);
				temp.add_child(node);
				node=temp;
				break;
			case __mul_operator:
			case __div_operator:
				temp.set_clear();
				temp.set_node_type(this_token.type);
				temp.add_child(node);
				temp.add_child(mul_div_operator_expr());
				node=temp;
				get_token();
				switch(this_token.type)
				{
					case __cmp_equal:
					case __cmp_not_equal:
					case __cmp_less:
					case __cmp_more:
					case __cmp_less_or_equal:
					case __cmp_more_or_equal:
					case __and_operator:
					case __or_operator:
					case __add_operator:
					case __sub_operator:
					case __link_operator:
						temp.set_clear();
						temp.set_node_type(this_token.type);
						temp.add_child(node);
						node=temp;
						break;
					default:parse.push(this_token);return node;break;
				}
				break;
			case __semi:parse.push(this_token);return node;break;
			case __unknown_operator:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": __unknown_operator '"<<this_token.content<<"'."<<std::endl;
				return node;break;
			default:parse.push(this_token);return node;break;
		}
	}
	return node;
}
abstract_syntax_tree nasal_parser::call_list_expr()
{
	abstract_syntax_tree node;
	node.set_node_type(__list_search);
	get_token();
	switch(this_token.type)
	{
		case __left_curve:
		case __number:
		case __string:
		case __id:
			parse.push(this_token);
			node.add_child(calculation_expr());
			break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": incorrect data type when calling a list."<<std::endl;
			return node;
			break;
	}
	get_token();
	if(this_token.type==__colon)// to check if there is [number:number] or [number:]
	{
		get_token();
		switch(this_token.type)
		{
			case __left_curve:
			case __number:
			case __string:
			case __id:
				parse.push(this_token);
				node.add_child(calculation_expr());
				break;
			case __right_bracket:parse.push(this_token);break;// this is [number:]
			default:parse.push(this_token);break;
		}
		get_token();
		if(this_token.type!=__right_bracket)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ']' when calling a list."<<std::endl;
			return node;
		}
		get_token();
		switch(this_token.type)
		{
			case __left_bracket:
				node.add_child(call_list_expr());
				break;
			case __left_curve:
				node.add_child(call_function_expr());
				break;
			case __dot:
				node.add_child(call_hash_expr());
				break;
			default:parse.push(this_token);break;
		}
	}
	else if(this_token.type==__comma)
	{
		while(this_token.type!=__right_bracket)
		{
			get_token();
			switch(this_token.type)
			{
				case __left_curve:
				case __number:
				case __string:
				case __id:
					parse.push(this_token);
					node.add_child(calculation_expr());
					break;
				case __right_bracket:parse.push(this_token);break;
				default:
					++error;
					std::cout<<">>[Error] line "<<this_token.line<<": incorrect data type when calling a list."<<std::endl;
					return node;
					break;
			}
			get_token();
			if(this_token.type!=__comma && this_token.type!=__right_bracket)
			{
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": expect a ',' or ']' when calling a list."<<std::endl;
				return node;
			}
		}
	}
	else if(this_token.type==__right_bracket)
	{
		get_token();
		switch(this_token.type)
		{
			case __left_bracket:
				node.add_child(call_list_expr());
				break;
			case __left_curve:
				node.add_child(call_function_expr());
				break;
			case __dot:
				node.add_child(call_hash_expr());
				break;
			default:parse.push(this_token);break;
		}
	}
	else
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a ']' when calling a list."<<std::endl;
		return node;
	}
	return node;
}
abstract_syntax_tree nasal_parser::parameter_function_expr()
{
	abstract_syntax_tree node;
	node.set_node_type(__function);
	get_token();
	if(this_token.type==__id)
	{
		parse.push(this_token);
		node.add_child(calculation_expr());
		return node;
	}
	else if(this_token.type==__left_curve || this_token.type==__left_brace)
	{
		parse.push(this_token);
		node.add_child(function_generate_expr());
		return node;
	}
	else
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect an __id or '(' or '{' when calling or generating a function."<<std::endl;
		return node;
	}
	return node;
}
abstract_syntax_tree nasal_parser::call_function_expr()
{
	abstract_syntax_tree node;
	node.set_node_type(__call_function);
	get_token();
	while(this_token.type!=__right_curve)
	{
		abstract_syntax_tree temp;
		switch(this_token.type)
		{
			case __left_curve:
			case __nor_operator:
			case __sub_operator:
			case __number:
			case __string:
			case __id:
				parse.push(this_token);
				temp=calculation_expr();
				break;
			case __left_bracket:
				temp=list_generate_expr();
				break;
			case __left_brace:
				temp=hash_generate_expr();
				break;
			case __func:
				temp=parameter_function_expr();
				break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": incorrect token '";
				print_token(this_token.type);
				std::cout<<"' when calling a function."<<std::endl;
				return node;
				break;
		}
		get_token();
		if(this_token.type==__colon)
		{
			get_token();
			switch(this_token.type)
			{
				case __left_curve:
				case __nor_operator:
				case __sub_operator:
				case __number:
				case __string:
				case __id:
					parse.push(this_token);
					temp.add_child(calculation_expr());
					break;
				case __left_bracket:
					temp.add_child(list_generate_expr());
					break;
				case __left_brace:
					temp.add_child(hash_generate_expr());
					break;
				case __func:
					temp.add_child(parameter_function_expr());
					break;
				default:
					++error;
					std::cout<<">>[Error] line "<<this_token.line<<": incorrect token '";
					print_token(this_token.type);
					std::cout<<"' when calling a function."<<std::endl;
					return node;
					break;
			}
		}
		else
			parse.push(this_token);
		node.add_child(temp);
		get_token();
		if(this_token.type!=__comma && this_token.type!=__right_curve)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ',' after parameter or ')' to end the call."<<std::endl;
			return node;
		}
		else if(this_token.type==__comma)
			get_token();
	}
	get_token();
	switch(this_token.type)
	{
		case __left_bracket:
			node.add_child(call_list_expr());
			break;
		case __left_curve:
			node.add_child(call_function_expr());
			break;
		case __dot:
			node.add_child(call_hash_expr());
			break;
		default:parse.push(this_token);break;
	}
	return node;
}
abstract_syntax_tree nasal_parser::call_hash_expr()
{
	abstract_syntax_tree node;
	node.set_node_type(__hash_search);
	get_token();
	if(this_token.type!=__id)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": missing identifier when calling __hash_member."<<std::endl;
		return node;
	}
	node.add_child(identifier_call_expr());
	return node;
}
abstract_syntax_tree nasal_parser::identifier_call_expr()
{
	abstract_syntax_tree node;
	node.set_var_name(this_token.content);
	// __id has been checked
	get_token();
	switch(this_token.type)
	{
		case __left_bracket:
			node.set_node_type(__list_search);
			node.add_child(call_list_expr());
			break;
		case __left_curve:
			node.set_node_type(__call_function);
			node.add_child(call_function_expr());
			break;
		case __dot:
			node.set_node_type(__hash_search);
			node.add_child(call_hash_expr());
			break;
		default:
			node.set_node_type(__id);
			parse.push(this_token);
			break;
	}
	abstract_syntax_tree temp;
	get_token();
	switch(this_token.type)
	{
		case __equal:
		case __add_equal:
		case __sub_equal:
		case __mul_equal:
		case __div_equal:
		case __link_equal:
			temp.set_node_type(__assignment);
			temp.add_child(node);
			temp.add_child(assignment_expr());
			node.set_clear();
			node=temp;
			break;
		default:parse.push(this_token);break;
	}
	return node;
}
void nasal_parser::parse_main_work()
{
	root.set_clear();
	root.set_node_type(__root);
	error=0;
	warning=0;
	while(!parse.empty())
	{
		get_token();
		switch(this_token.type)
		{
			case __var:
				root.add_child(definition_expr());
				check_semi_at_end();
				break;
			case __sub_operator:
			case __nor_operator:
			case __id:
			case __number:
			case __string:
			case __left_curve:
				parse.push(this_token);
				root.add_child(calculation_expr());
				check_semi_at_end();
				break;
			case __if:
				parse.push(this_token);
				root.add_child(if_else_expr());
				break;
			case __while:
			case __for:
			case __foreach:
			case __forindex:
				parse.push(this_token);
				root.add_child(loop_expr());
				break;
			case __semi:break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' when generating __main_statement_block."<<std::endl;
				if(this_token.type==0)
				{
					std::cout<<">>[Error] parse stack empty."<<std::endl;
					break;
				}
				break;
		}
	}
	std::cout<<">>[Parse] complete generation."<<error<<" error(s),"<<warning<<" warning(s)."<<std::endl;
	return;
}

#endif
