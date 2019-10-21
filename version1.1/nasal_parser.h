#ifndef __NASAL_PARSER_H__
#define __NASAL_PARSER_H__

class nasal_parser
{
	private:
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
		void in_curve_calc_expr();
		void calculation_expr();
		void identifier_call_expr();
		void call_list_expr();
		void parameter_function_expr();
		void call_function_expr();
		void call_hash_expr();
		void list_generate_expr();
		void hash_generate_expr();
		void definition_expr();
		void assignment_expr();
		void loop_expr();
		bool else_if_check();
		void if_else_expr();
		void mul_div_operator_expr();
		void one_operator_expr();
		void check_semi_at_end();
		void statements_block();
		void function_generate_expr();
		void return_expr();
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
void nasal_parser::return_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __left_curve:
		case __number:
		case __string:
		case __id:parse.push(this_token);calculation_expr();break;
		case __func:function_generate_expr();break;
		case __left_bracket:list_generate_expr();break;
		case __left_brace:hash_generate_expr();break;
		case __semi:parse.push(this_token);break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a data or ';' after __return_expr."<<std::endl;
			return;
			break;
	}
	return;
}
void nasal_parser::statements_block()
{
	get_token();
	if(this_token.type!=__left_brace)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a '{' when generating a __block."<<std::endl;
		return;
	}
	get_token();
	while(this_token.type!=__right_brace)
	{
		switch(this_token.type)
		{
			case __var:definition_expr();check_semi_at_end();break;
			case __left_curve:
			case __sub_operator:
			case __nor_operator:
			case __id:
			case __number:
			case __string:parse.push(this_token);calculation_expr();check_semi_at_end();break;
			case __if:parse.push(this_token);if_else_expr();break;
			case __while:
			case __for:
			case __foreach:
			case __forindex:parse.push(this_token);loop_expr();break;
			case __continue:
			case __break:check_semi_at_end();break;
			case __semi:break;
			case __return:return_expr();check_semi_at_end();break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": '";
				print_token(this_token.type);
				std::cout<<"' incorrect token as the beginning of statement."<<std::endl;
				return;
				break;
		}
		get_token();
	}
	return;
}
void nasal_parser::function_generate_expr()
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
						case __sub_operator:parse.push(this_token);calculation_expr();break;
						case __left_brace:hash_generate_expr();break;
						case __left_bracket:list_generate_expr();break;
						case __func:function_generate_expr();break;
						default:
							++error;
							std::cout<<">>[Error] line "<<this_token.line<<": expect a scalar after operator '=' ."<<std::endl;
							return;break;
					}
				}
				get_token();
				if(this_token.type!=__right_curve && this_token.type!=__comma)
				{
					++error;
					std::cout<<">>[Error] line "<<this_token.line<<": expect a ',' or ')' when creating a __function."<<std::endl;
					return;
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
					return;
				}
				parse.push(this_token);
			}
			else
			{
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": expect __id and __dynamic_id only."<<std::endl;
				return;
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
		return;
	}
	get_token();
	if(this_token.type==__left_brace)
	{
		parse.push(this_token);
		statements_block();
	}
	else
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": function must have a statement block begin with '{' ."<<std::endl;
		return;
	}
	return;
}
void nasal_parser::list_generate_expr()
{
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
			case __id:parse.push(this_token);calculation_expr();break;
			case __left_bracket:list_generate_expr();break;
			case __left_brace:hash_generate_expr();break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": incorrect token '";
				print_token(this_token.type);
				std::cout<<"' when creating a __list."<<std::endl;
				return;
				break;
		}
		get_token();
		if(this_token.type!=__comma && this_token.type!=__right_bracket)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ',' or ']' but get '";
			print_token(this_token.type);
			std::cout<<"' when creating a __list."<<std::endl;
			return;
		}
		else if(this_token.type==__comma)
			get_token();
	}
	return;
}
void nasal_parser::hash_generate_expr()
{
	get_token();
	while(this_token.type!=__right_brace)
	{
		if(this_token.type!=__id && this_token.type!=__string)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": incorrect token '";
			print_token(this_token.type);
			std::cout<<"' when creating a __hash_member."<<std::endl;
			return;
		}
		if(this_token.type==__string || this_token.type==__number)
		{
			parse.push(this_token);
			calculation_expr();
		}
		get_token();
		if(this_token.type!=__colon)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ':' when creating a __hash_member."<<std::endl;
			return;
		}
		get_token();
		switch(this_token.type)
		{
			case __left_curve:
			case __number:
			case __string:
			case __id:parse.push(this_token);calculation_expr();break;
			case __func:
				get_token();
				if(this_token.type==__id)
				{
					parse.push(this_token);
					calculation_expr();
				}
				else
				{
					parse.push(this_token);
					function_generate_expr();
				}
				break;
			case __left_bracket:list_generate_expr();break;
			case __left_brace:hash_generate_expr();break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": incorrect token '";
				print_token(this_token.type);
				std::cout<<"' when creating a __hash_member."<<std::endl;
				return;
				break;
		}
		get_token();
		if(this_token.type!=__comma && this_token.type!=__right_brace)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ',' or '}' but get '";
			print_token(this_token.type);
			std::cout<<"' when creating a __hash."<<std::endl;
			return;
		}
		else if(this_token.type==__comma)
			get_token();
	}
	return;
}
void nasal_parser::definition_expr()
{
	get_token();
	if(this_token.type!=__id)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect an __id."<<std::endl;
		return;
	}
	get_token();
	if(this_token.type!=__equal && this_token.type!=__semi)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a '=' after __id."<<std::endl;
		return;
	}
	else if(this_token.type==__semi)
	{
		parse.push(this_token);// for semi check
		++warning;
		std::cout<<">>[Warning] line "<<this_token.line<<": better initializing this."<<std::endl;
		return;
	}
	get_token();
	token t;
	t.line=this_token.line;
	t.type=__semi;
	switch(this_token.type)
	{
		case __left_curve:
		case __sub_operator:
		case __nor_operator:
		case __number:
		case __string:
		case __id:parse.push(this_token);calculation_expr();break;
		case __func:function_generate_expr();parse.push(t);break;
		case __left_bracket:list_generate_expr();break;
		case __left_brace:hash_generate_expr();break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a data after this operator."<<std::endl;
			return;
			break;
	}
	return;
}
void nasal_parser::assignment_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __left_curve:
		case __sub_operator:
		case __nor_operator:
		case __number:
		case __string:
		case __id:parse.push(this_token);calculation_expr();break;
		case __func:function_generate_expr();break;
		case __left_bracket:list_generate_expr();break;
		case __left_brace:hash_generate_expr();break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": incorrect data type when doing assignment."<<std::endl;
			return;
			break;
	}
	return;
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
void nasal_parser::if_else_expr()
{
	get_token();
	if(this_token.type!=__if)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a 'if' when creating new if-else statement."<<std::endl;
		return;
	}
	get_token();
	if(this_token.type!=__left_curve)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a '(' when making a condition."<<std::endl;
		return;
	}
	get_token();
	switch(this_token.type)
	{
		case __left_curve:
		case __sub_operator:
		case __nor_operator:
		case __number:
		case __string:
		case __id:parse.push(this_token);calculation_expr();break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a correct condition."<<std::endl;
			return;
			break;
	}
	get_token();
	if(this_token.type!=__right_curve)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a ')' when making a condition."<<std::endl;
		return;
	}
	get_token();
	if(this_token.type==__left_brace)
	{
		// if without { then only one statement is behind it
		parse.push(this_token);
		statements_block();
	}
	else
	{
		switch(this_token.type)
		{
			case __var:definition_expr();check_semi_at_end();break;
			case __id:
			case __number:
			case __string:
			case __left_curve:parse.push(this_token);calculation_expr();check_semi_at_end();break;
			case __if:parse.push(this_token);if_else_expr();break;
			case __sub_operator:
			case __nor_operator:parse.push(this_token);calculation_expr();break;
			case __while:
			case __for:
			case __foreach:
			case __forindex:parse.push(this_token);loop_expr();break;
			case __continue:
			case __break:check_semi_at_end();break;
			case __semi:break;
			case __return:return_expr();check_semi_at_end();break;
			default:
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' when generating a statement."<<std::endl;
				++error;
				break;
		}
	}
	if(parse.empty())
		return;
	get_token();
	while(this_token.type==__elsif || else_if_check())
	{
		get_token();
		if(this_token.type!=__left_curve)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a '(' when making a condition."<<std::endl;
			return;
		}
		get_token();
		switch(this_token.type)
		{
			case __left_curve:
			case __sub_operator:
			case __nor_operator:
			case __number:
			case __string:
			case __id:parse.push(this_token);calculation_expr();break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": expect a correct condition."<<std::endl;
				return;
				break;
		}
		get_token();
		if(this_token.type!=__right_curve)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ')' when making a condition."<<std::endl;
			return;
		}
		get_token();
		if(this_token.type==__left_brace)
		{
			// if without { then only one statement is behind it
			parse.push(this_token);
			statements_block();
		}
		else
		{
			switch(this_token.type)
			{
				case __var:definition_expr();check_semi_at_end();break;
				case __sub_operator:
				case __nor_operator:
				case __id:
				case __number:
				case __string:
				case __left_curve:parse.push(this_token);calculation_expr();check_semi_at_end();break;
				case __if:parse.push(this_token);if_else_expr();break;
				case __while:
				case __for:
				case __foreach:
				case __forindex:parse.push(this_token);loop_expr();break;
				case __continue:
				case __break:check_semi_at_end();break;
				case __semi:break;
				case __return:return_expr();check_semi_at_end();break;
				default:
					std::cout<<">>[Error] line "<<this_token.line<<": \'";
					print_token(this_token.type);
					std::cout<<"\' when generating a statement."<<std::endl;
					++error;
					break;
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
			switch(this_token.type)
			{
				case __var:definition_expr();check_semi_at_end();break;
				case __sub_operator:
				case __nor_operator:
				case __id:
				case __number:
				case __string:
				case __left_curve:parse.push(this_token);calculation_expr();check_semi_at_end();break;
				case __if:parse.push(this_token);if_else_expr();break;
				case __while:
				case __for:
				case __foreach:
				case __forindex:parse.push(this_token);loop_expr();break;
				case __continue:
				case __break:check_semi_at_end();break;
				case __semi:break;
				case __return:return_expr();check_semi_at_end();break;
				default:
					std::cout<<">>[Error] line "<<this_token.line<<": \'";
					print_token(this_token.type);
					std::cout<<"\' when generating a statement."<<std::endl;
					++error;
					break;
			}
		}
	}
	else
		parse.push(this_token);
	return;
}
void nasal_parser::loop_expr()
{
	get_token();
	if(this_token.type==__while)
	{
		get_token();
		if(this_token.type!=__left_curve)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a '(' after 'while'."<<std::endl;
			return;
		}
		get_token();
		switch(this_token.type)
		{
			case __left_curve:
			case __sub_operator:
			case __nor_operator:
			case __number:
			case __string:
			case __id:parse.push(this_token);calculation_expr();break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": expect a condition."<<std::endl;
				return;
				break;
		}
		get_token();
		if(this_token.type!=__right_curve)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ')' after 'while('."<<std::endl;
			return;
		}
		get_token();
		if(this_token.type==__left_brace)
		{
			// if without { then only one statement is behind it
			parse.push(this_token);
			statements_block();
		}
		else
		{

			switch(this_token.type)
			{
				case __var:definition_expr();check_semi_at_end();break;
				case __sub_operator:
				case __nor_operator:
				case __id:
				case __number:
				case __string:
				case __left_curve:parse.push(this_token);calculation_expr();check_semi_at_end();break;
				case __if:parse.push(this_token);if_else_expr();break;
				case __while:
				case __for:
				case __foreach:
				case __forindex:parse.push(this_token);loop_expr();break;
				case __continue:
				case __break:check_semi_at_end();break;
				case __semi:break;
				case __return:return_expr();check_semi_at_end();break;
				default:
					std::cout<<">>[Error] line "<<this_token.line<<": \'";
					print_token(this_token.type);
					std::cout<<"\' when creating a new loop."<<std::endl;
					++error;
					break;
			}
		}
	}
	else if(this_token.type==__for)
	{
		get_token();
		if(this_token.type!=__left_curve)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a '(' after 'for'."<<std::endl;
			return;
		}
		get_token();
		switch(this_token.type)
		{
			case __var:definition_expr();break;
			case __id:calculation_expr();break;
			case __semi:parse.push(this_token);break;
			default:
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' when creating a new loop."<<std::endl;
				++error;
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
			case __string:parse.push(this_token);calculation_expr();break;
			case __semi:parse.push(this_token);break;
			default:
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' when creating a new loop."<<std::endl;
				++error;
				break;
		}
		check_semi_at_end();
		get_token();
		switch(this_token.type)
		{
			case __left_curve:
			case __id:
			case __number:
			case __string:parse.push(this_token);calculation_expr();break;
			case __right_curve:parse.push(this_token);break;
			default:
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' when creating a new loop."<<std::endl;
				++error;
				break;
		}
		get_token();
		if(this_token.type!=__right_curve)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ')' after 'for('."<<std::endl;
			return;
		}
		get_token();
		if(this_token.type==__left_brace)
		{
			// if without { then only one statement is behind it
			parse.push(this_token);
			statements_block();
		}
		else
		{
			switch(this_token.type)
			{
				case __var:definition_expr();check_semi_at_end();break;
				case __sub_operator:
				case __nor_operator:
				case __id:
				case __number:
				case __string:
				case __left_curve:parse.push(this_token);calculation_expr();check_semi_at_end();break;
				case __if:parse.push(this_token);if_else_expr();break;
				case __while:
				case __for:
				case __foreach:
				case __forindex:parse.push(this_token);loop_expr();break;
				case __continue:
				case __break:check_semi_at_end();break;
				case __semi:break;
				case __return:return_expr();check_semi_at_end();break;
				default:
					std::cout<<">>[Error] line "<<this_token.line<<": \'";
					print_token(this_token.type);
					std::cout<<"\' when creating a new loop."<<std::endl;
					++error;
					break;
			}
		}
	}
	else if(this_token.type==__forindex || this_token.type==__foreach)
	{
		get_token();
		if(this_token.type!=__left_curve)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a '(' after 'forindex' or 'foreach'."<<std::endl;
			return;
		}
		get_token();
		switch(this_token.type)
		{
			case __var:definition_expr();break;
			case __id:parse.push(this_token);calculation_expr();break;
			case __semi:parse.push(this_token);break;
			default:
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' when creating a new loop."<<std::endl;
				++error;
				break;
		}
		check_semi_at_end();
		get_token();
		switch(this_token.type)
		{
			case __left_curve:
			case __id:
			case __number:
			case __string:parse.push(this_token);calculation_expr();break;
			case __left_bracket:list_generate_expr();break;
			case __right_curve:parse.push(this_token);break;
			default:
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' when creating a new loop."<<std::endl;
				++error;
				break;
		}
		get_token();
		if(this_token.type!=__right_curve)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ')' after 'forindex(' or 'foreach('."<<std::endl;
			return;
		}
		get_token();
		if(this_token.type==__left_brace)
		{
			// if without { then only one statement is behind it
			parse.push(this_token);
			statements_block();
		}
		else
		{
			switch(this_token.type)
			{
				case __var:definition_expr();check_semi_at_end();break;
				case __sub_operator:
				case __nor_operator:
				case __id:
				case __number:
				case __string:
				case __left_curve:parse.push(this_token);calculation_expr();check_semi_at_end();break;
				case __if:parse.push(this_token);if_else_expr();break;
				case __while:
				case __for:
				case __foreach:
				case __forindex:parse.push(this_token);loop_expr();break;
				case __continue:
				case __break:check_semi_at_end();break;
				case __semi:break;
				case __return:return_expr();check_semi_at_end();break;
				default:
					std::cout<<">>[Error] line "<<this_token.line<<": \'";
					print_token(this_token.type);
					std::cout<<"\' when creating a new loop."<<std::endl;
					++error;
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
		return;
	}
	return;
}
void nasal_parser::mul_div_operator_expr()
{
	while(1)
	{
		get_token();
		switch(this_token.type)
		{
			case __number:break;
			case __string:break;
			case __id:identifier_call_expr();break;
			case __left_curve:parse.push(this_token);calculation_expr();break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": expect a data after operator '*' or '/'."<<std::endl;
				return;
				break;
		}
		get_token();
		if(this_token.type!=__mul_operator && this_token.type!=__div_operator)
		{
			parse.push(this_token);
			break;
		}
	}
	return;
}
void nasal_parser::one_operator_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __number:break;
		case __string:break;
		case __id:identifier_call_expr();break;
		case __left_curve:parse.push(this_token);calculation_expr();break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a data after operator '!' or '-'."<<std::endl;
			return;
			break;
	}
	return;
}
void nasal_parser::in_curve_calc_expr()
{
	calculation_expr();
	get_token();
	if(this_token.type!=__right_curve)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a ')' when generating __in_curve_calculation."<<std::endl;
	}
	get_token();
	switch(this_token.type)
	{
		case __left_bracket:call_list_expr();break;
		case __left_curve:call_function_expr();break;
		default:parse.push(this_token);break;
	}
	return;
}
void nasal_parser::calculation_expr()
{
	// number / string / identifier haven't been checked
	while(1)
	{
		get_token(); // check number / string / identifier
		if(this_token.type==__nor_operator || this_token.type==__sub_operator)
			one_operator_expr();
		else if(this_token.type==__id)
			identifier_call_expr();
		else if(this_token.type==__number || this_token.type==__string)
			;
		else if(this_token.type==__left_curve)
			in_curve_calc_expr();
		else if(this_token.type==__func)
		{
			function_generate_expr();
			return;
		}
		else
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a scalar before operator '+' , '-' , '*' , '/' or '~'."<<std::endl;
			return;
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
			case __link_operator:break;
			case __mul_operator:
			case __div_operator:
				mul_div_operator_expr();
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
					case __link_operator:break;
					default:parse.push(this_token);return;break;
				}
				break;
			case __semi:parse.push(this_token);return;break;
			case __unknown_operator:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": __unknown_operator '"<<this_token.content<<"'."<<std::endl;
				return;break;
			default:parse.push(this_token);return;break;
		}
	}
	return;
}
void nasal_parser::call_list_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __left_curve:
		case __number:
		case __string:
		case __id:parse.push(this_token);calculation_expr();break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": incorrect data type when calling a list."<<std::endl;
			return;
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
			case __id:parse.push(this_token);calculation_expr();break;
			case __right_bracket:parse.push(this_token);break;// this is [number:]
			default:parse.push(this_token);break;
		}
		get_token();
		if(this_token.type!=__right_bracket)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ']' when calling a list."<<std::endl;
			return;
		}
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
	else
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a ']' when calling a list."<<std::endl;
		return;
	}
	return;

}
void nasal_parser::parameter_function_expr()
{
	get_token();
	if(this_token.type==__id)
	{
		parse.push(this_token);
		calculation_expr();
		return;
	}
	else if(this_token.type==__left_curve || this_token.type==__left_brace)
	{
		parse.push(this_token);
		function_generate_expr();
		return;
	}
	else
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect an __id or '(' or '{' when calling or generating a function."<<std::endl;
		return;
	}
	return;
}
void nasal_parser::call_function_expr()
{
	get_token();
	while(this_token.type!=__right_curve)
	{
		switch(this_token.type)
		{
			case __left_curve:
			case __nor_operator:
			case __sub_operator:
			case __number:
			case __string:
			case __id:parse.push(this_token);calculation_expr();break;
			case __left_bracket:list_generate_expr();break;
			case __left_brace:hash_generate_expr();break;
			case __func:parameter_function_expr();break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": incorrect token '";
				print_token(this_token.type);
				std::cout<<"' when calling a function."<<std::endl;
				return;
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
				case __id:parse.push(this_token);calculation_expr();break;
				case __left_bracket:list_generate_expr();break;
				case __left_brace:hash_generate_expr();break;
				case __func:parameter_function_expr();break;
				default:
					++error;
					std::cout<<">>[Error] line "<<this_token.line<<": incorrect token '";
					print_token(this_token.type);
					std::cout<<"' when calling a function."<<std::endl;
					return;
					break;
			}
		}
		else
			parse.push(this_token);
		get_token();
		if(this_token.type!=__comma && this_token.type!=__right_curve)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ',' after parameter or ')' to end the call."<<std::endl;
			return;
		}
		else if(this_token.type==__comma)
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
void nasal_parser::call_hash_expr()
{
	get_token();
	if(this_token.type!=__id)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": missing identifier when calling __hash_member."<<std::endl;
		return;
	}
	identifier_call_expr();
	return;
}
void nasal_parser::identifier_call_expr()
{
	// __id has been checked
	get_token();
	switch(this_token.type)
	{
		case __left_bracket:call_list_expr();break;
		case __left_curve:call_function_expr();break;
		case __dot:call_hash_expr();break;
		default:parse.push(this_token);break;
	}
	get_token();
	switch(this_token.type)
	{
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
void nasal_parser::parse_main_work()
{
	error=0;
	warning=0;
	while(!parse.empty())
	{
		get_token();
		switch(this_token.type)
		{
			case __var:definition_expr();check_semi_at_end();break;
			case __sub_operator:
			case __nor_operator:
			case __id:
			case __number:
			case __string:
			case __left_curve:parse.push(this_token);calculation_expr();check_semi_at_end();break;
			case __if:parse.push(this_token);if_else_expr();break;
			case __while:
			case __for:
			case __foreach:
			case __forindex:parse.push(this_token);loop_expr();break;
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
