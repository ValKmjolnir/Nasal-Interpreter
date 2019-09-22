#ifndef __NASAL_PARSER_H__
#define __NASAL_PARSER_H__

#include "nasal_token_type.h"

class nasal_parser
{
	private:
		ast_tree_node root;
		std::stack<token> parse;
		std::stack<ast_tree_node> node_cache;
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
							std::cout<<"line "<<line<<"    ";
						else
							std::cout<<std::endl<<"line "<<line<<"    ";
					}
					else
					{
						while(line!=temp.top().line)
						{
							++line;
							std::cout<<std::endl<<"line "<<line<<"    ";
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
				std::cout<<">>[Parse] [-Warning] Empty lexer list."<<std::endl;
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
			std::cout<<">>[Abstract-syntax-tree]"<<std::endl;
			root.print(0);
			return;
		}
		void run()
		{
			if(!error)
				root.run();
			else
				std::cout<<">>[Parse] "<<error<<"error(s) occurred,stop."<<std::endl;
			return;
		}
		void parse_main_work();
		void definition_expr();
		void assignment_expr();
		void while_loop_expr();
		void for_loop_expr();
		void foreach_index_loop_expr();
		void loop_expr();
		void if_else_expr();
		void function_expr();
		
		void list_init_generator();
		void hash_init_generator();
		void list_search_expr();
		void hash_search_expr();
		void call_function_expr();
		void identifier_begin_expr();
		void number_begin_expr();
		void string_begin_expr();
		void link_expr();
		void add_sub_expr();
		void mul_div_expr();
		void compare_operator_expr();
};
void nasal_parser::definition_expr()
{
	get_token();
	if(this_token.type!=__id)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": missing identifier after declarator."<<std::endl;
		return;
	}
	get_token();
	if(this_token.type!=__equal && this_token.type!=__semi)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": must have a \'=\' after identifier."<<std::endl;
		return;
	}
	else if(this_token.type==__semi)
	{
		++warning;
		std::cout<<">>[Warning] line "<<this_token.line<<": variable without initializing."<<std::endl;
		return;
	}
	get_token();
	switch(this_token.type)
	{
		case __id:identifier_begin_expr();break;
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __left_brace:hash_init_generator();break;
		case __left_bracket:list_init_generator();break;
		case __func:function_expr();break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": must have a correct data type after operator = ."<<std::endl;
			return;
			break;
	}
	get_token();
	if(this_token.type!=__semi)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a \';\' at the end of this line."<<std::endl;
		parse.push(this_token);
		return;
	}
	return;
}
void nasal_parser::assignment_expr()
{
	get_token();
	if(this_token.type!=__equal && this_token.type!=__add_equal
		&& this_token.type!=__sub_equal && this_token.type!=__mul_equal
		&& this_token.type!=__div_equal && this_token.type!=__link_equal)
	{
		parse.push(this_token);
		return;
	}
	get_token();
	switch(this_token.type)
	{
		case __id:identifier_begin_expr();break;
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __left_brace:hash_init_generator();break;
		case __left_bracket:list_init_generator();break;
		case __func:function_expr();break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": must have a correct data type after operator."<<std::endl;
			return;
			break;
	}
	get_token();
	if(this_token.type!=__semi)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a \';\' at the end of this line."<<std::endl;
		parse.push(this_token);
		return;
	}
	return;
}
void nasal_parser::while_loop_expr()
{
	get_token();
	if(this_token.type!=__left_curve)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": missing condition after \'while\' ."<<std::endl;
		return;
	}
	get_token();
	switch(this_token.type)
	{
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __id:identifier_begin_expr();break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": incorrect token \'";
			print_token(this_token.type);
			std::cout<<"\' in this line."<<std::endl;
			return;
			break;
	}
	get_token();
	if(this_token.type!=__right_curve)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a \')\' at the end of condition."<<std::endl;
		return;
	}
	get_token();
	if(this_token.type==__left_brace)
	{
		while(this_token.type!=__right_brace)
		{
			get_token();
			switch(this_token.type)
			{
				case __var:definition_expr();break;
				case __id:identifier_begin_expr();break;
				case __number:number_begin_expr();break;
				case __string:string_begin_expr();break;
				case __if:
					parse.push(this_token);
					if_else_expr();
					break;
				case __while:
				case __for:
				case __foreach:
				case __forindex:
					parse.push(this_token);
					loop_expr();
					break;
				case __continue:
				case __break:
					get_token();
					if(this_token.type!=__semi)
					{
						++error;
						std::cout<<">>[Error] line "<<this_token.line<<": expect a \';\' at the end of this statement."<<std::endl;
					}
					break;
				case __semi:break;
				case __right_brace:break;
				default:
					std::cout<<">>[Error] line "<<this_token.line<<": ";
					print_token(this_token.type);
					std::cout<<" in an incorrect place."<<std::endl;
					++error;
					break;
			}
		}
	}
	else
	{
		get_token();
		switch(this_token.type)
		{
			case __var:definition_expr();break;
			case __id:identifier_begin_expr();break;
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __if:
				parse.push(this_token);
				if_else_expr();
				break;
			case __while:
			case __for:
			case __foreach:
			case __forindex:
				parse.push(this_token);
				loop_expr();
				break;
			case __continue:
			case __break:
				get_token();
				if(this_token.type!=__semi)
				{
					++error;
					std::cout<<">>[Error] line "<<this_token.line<<": expect a \';\' at the end of this statement."<<std::endl;
				}
				break;
			case __semi:break;
			default:
				std::cout<<">>[Error] line "<<this_token.line<<": ";
				print_token(this_token.type);
				std::cout<<" in an incorrect place."<<std::endl;
				++error;
				break;
		}
	}
	return;
}
void nasal_parser::for_loop_expr()
{
	get_token();
	if(this_token.type!=__left_curve)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": missing condition after \'for\' ."<<std::endl;
		return;
	}
	return;
}
void nasal_parser::foreach_index_loop_expr()
{
	get_token();
	if(this_token.type!=__left_curve)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": missing condition after \'foreach\' or \'forindex\' ."<<std::endl;
		return;
	}
	return;
}
void nasal_parser::loop_expr()
{
	get_token();
	if(this_token.type==__while)
		while_loop_expr();
	else if(this_token.type==__for)
		for_loop_expr();
	else if(this_token.type==__foreach || this_token.type==__forindex)
		foreach_index_loop_expr();
	return;
}
void nasal_parser::if_else_expr()
{
	return;
}
void nasal_parser::function_expr()
{
	return;
}
void nasal_parser::list_init_generator()
{
	get_token();
	while(this_token.type!=__right_bracket && this_token.type!=__semi)
	{
		switch(this_token.type)
		{
			case __id:identifier_begin_expr();break;
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __left_brace:hash_init_generator();break;
			case __left_bracket:list_init_generator();break;
			case __func:function_expr();break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": incorrect token when generating a new list."<<std::endl;
				return;
				break;
		}
		get_token();
		if(this_token.type!=__comma && this_token.type!=__right_bracket && this_token.type!=__semi)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a \',\' at this line."<<std::endl;
			return;
		}
		else if(this_token.type!=__right_bracket)
			get_token();
	}
	if(this_token.type==__semi)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a \']\' at this line."<<std::endl;
		return;
	}
	return;
}
void nasal_parser::hash_init_generator()
{
	get_token();
	while(this_token.type!=__right_brace && this_token.type!=__semi)
	{
		if(this_token.type!=__id)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": hashmember should have this type \'id : scalar\' but there is no identifier."<<std::endl;
			return;
		}
		get_token();
		if(this_token.type!=__colon)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": hashmember should have this type \'id : scalar\' but lacks a \':\'."<<std::endl;
			return;
		}
		get_token();
		switch(this_token.type)
		{
			case __id:identifier_begin_expr();break;
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __left_brace:hash_init_generator();break;
			case __left_bracket:list_init_generator();break;
			case __func:function_expr();break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": incorrect token when generating a new list."<<std::endl;
				return;
				break;
		}
		get_token();
		if(this_token.type!=__comma && this_token.type!=__right_brace && this_token.type!=__semi)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a \',\'."<<std::endl;
			return;
		}
		if(this_token.type!=__right_brace)
			get_token();
	}
	if(this_token.type==__semi)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a \']\'."<<std::endl;
		return;
	}
	return;
}
void nasal_parser::list_search_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __number:number_begin_expr();break;
		case __id:identifier_begin_expr();break;
		case __string:string_begin_expr();break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": incorrect token \"";
			print_token(this_token.type);
			std::cout<<"\" in list call."<<std::endl;
			break;
	}
	get_token();
	if(this_token.type!=__right_bracket)
	{
		if(this_token.type==__semi)
			parse.push(this_token);
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a \']\' when calling a list."<<std::endl;
		return;
	}
	get_token();
	switch(this_token.type)
	{
		case __left_bracket:list_search_expr();break;
		case __dot:hash_search_expr();break;
		default:
			parse.push(this_token);
			break;
	}
	return;
}
void nasal_parser::hash_search_expr()
{
	get_token();
	if(this_token.type!=__id)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": missing identifier after \'.\' ."<<std::endl;
		return;
	}
	get_token();
	switch(this_token.type)
	{
		case __left_curve:call_function_expr();break;
		case __left_bracket:list_search_expr();break;
		case __dot:hash_search_expr();break;
		case __semi:parse.push(this_token);break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": incorrect token \"";
			print_token(this_token.type);
			std::cout<<"\" at this line."<<std::endl;
			break;
	}
	return;
}
void nasal_parser::call_function_expr()
{
	get_token();
	while(this_token.type!=__right_curve && this_token.type!=__semi)
	{
		switch(this_token.type)
		{
			case __id:identifier_begin_expr();break;
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __left_brace:hash_init_generator();break;
			case __left_bracket:list_init_generator();break;
			case __func:function_expr();break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": incorrect token when generating a new list."<<std::endl;
				return;
				break;
		}
		get_token();
		if(this_token.type!=__comma && this_token.type!=__right_curve && this_token.type!=__semi)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a \',\'."<<std::endl;
			return;
		}
		else if(this_token.type!=__right_curve)
			get_token();
	}
	if(this_token.type==__semi)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a \')\'."<<std::endl;
		return;
	}
	get_token();
	switch(this_token.type)
	{
		case __left_bracket:list_search_expr();break;
		case __dot:hash_search_expr();break;
		default:
			parse.push(this_token);
			break;
	}
	return;
}
void nasal_parser::identifier_begin_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __add_operator:
		case __sub_operator:add_sub_expr();break;
		case __mul_operator:
		case __div_operator:mul_div_expr();break;
		case __link_operator:link_expr();break;
		case __and_operator:
		case __or_operator:
		case __cmp_equal:
		case __cmp_not_equal:
		case __cmp_less:
		case __cmp_more:
		case __cmp_less_or_equal:
		case __cmp_more_or_equal:
			parse.push(this_token);
			compare_operator_expr();
			break;
		case __equal:assignment_expr();break;
		case __left_curve:call_function_expr();break;
		case __left_bracket:list_search_expr();break;
		case __dot:hash_search_expr();break;
		case __comma:
		case __right_curve:
		case __right_bracket:
		case __right_brace:
		case __semi:parse.push(this_token);break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": incorrect token \"";
			print_token(this_token.type);
			std::cout<<"\" at this line."<<std::endl;
			break;
	}
	get_token();
	if(this_token.type==__semi)
		parse.push(this_token);
	else
	{
		switch(this_token.type)
		{
			case __equal:
			case __add_equal:
			case __sub_equal:
			case __mul_equal:
			case __div_equal:
			case __link_equal:
				parse.push(this_token);
				assignment_expr();
				break;
			default:
				parse.push(this_token);
				break;
		}
	}
	return;
}
void nasal_parser::number_begin_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __add_operator:
		case __sub_operator:add_sub_expr();break;
		case __mul_operator:
		case __div_operator:mul_div_expr();break;
		case __link_operator:link_expr();break;
		case __comma:
		case __right_curve:
		case __right_bracket:
		case __right_brace:
		case __semi:parse.push(this_token);break;
		case __and_operator:
		case __or_operator:
		case __cmp_equal:
		case __cmp_not_equal:
		case __cmp_less:
		case __cmp_more:
		case __cmp_less_or_equal:
		case __cmp_more_or_equal:
			parse.push(this_token);
			compare_operator_expr();
			break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": incorrect token \"";
			print_token(this_token.type);
			std::cout<<"\" after number."<<std::endl;
			break;
	}
	return;
}
void nasal_parser::string_begin_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __add_operator:
		case __sub_operator:add_sub_expr();break;
		case __mul_operator:
		case __div_operator:mul_div_expr();break;
		case __link_operator:link_expr();break;
		case __comma:
		case __right_curve:
		case __right_bracket:
		case __right_brace:
		case __semi:parse.push(this_token);break;
		case __and_operator:
		case __or_operator:
		case __cmp_equal:
		case __cmp_not_equal:
		case __cmp_less:
		case __cmp_more:
		case __cmp_less_or_equal:
		case __cmp_more_or_equal:
			parse.push(this_token);
			compare_operator_expr();
			break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": incorrect token \"";
			print_token(this_token.type);
			std::cout<<"\" after string."<<std::endl;
			break;
	}
	return;
}
void nasal_parser::link_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __id:identifier_begin_expr();break;
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a scalar after operator ~ ."<<std::endl;
			break;
	}
	return;
}
void nasal_parser::add_sub_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __id:identifier_begin_expr();break;
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a scalar after operator + or - ."<<std::endl;
			break;
	}
	return;
}
void nasal_parser::mul_div_expr()
{
	get_token();
	while(1)
	{
		switch(this_token.type)
		{
			case __id:identifier_begin_expr();break;
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": expect a scalar after operator * or / ."<<std::endl;
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
void nasal_parser::compare_operator_expr()
{
	get_token();
	if(this_token.type!=__cmp_equal && this_token.type!=__cmp_not_equal && this_token.type!=__cmp_less && this_token.type!=__cmp_more
		&& this_token.type!=__cmp_less_or_equal && this_token.type!=__cmp_more_or_equal
		&& this_token.type!=__and_operator && this_token.type!=__or_operator)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": incorrect comparation operator."<<std::endl;
		return;
	}
	get_token();
	switch(this_token.type)
	{
		case __id:identifier_begin_expr();break;
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a scalar after comparation operator."<<std::endl;
			return;
			break;
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
			case __var:definition_expr();break;
			case __id:identifier_begin_expr();break;
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __if:
				parse.push(this_token);
				if_else_expr();
				break;
			case __while:
			case __for:
			case __foreach:
			case __forindex:
				parse.push(this_token);
				loop_expr();
				break;
			case __continue:
			case __break:
				get_token();
				if(this_token.type!=__semi)
				{
					++error;
					std::cout<<">>[Error] line "<<this_token.line<<": expect a \';\' at the end of this statement."<<std::endl;
				}
				break;
			case __semi:break;
			default:
				std::cout<<">>[Error] line "<<this_token.line<<": ";
				print_token(this_token.type);
				std::cout<<" in an incorrect place."<<std::endl;
				++error;
				break;
		}
	}
	std::cout<<">>[Parse] complete generation."<<error<<" error(s),"<<warning<<" warning(s)."<<std::endl;
	return;
}

#endif
