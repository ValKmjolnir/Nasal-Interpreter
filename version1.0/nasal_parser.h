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
				std::cout<<">>[Parse] "<<error<<" error(s) occurred,stop."<<std::endl;
			return;
		}
		void parse_main_work();
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
void nasal_parser::check_semi_at_end()
{
	get_token();
	if(this_token.type!=__semi)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a ';' at the end of this line."<<std::endl;
	}
	return;
}
void nasal_parser::return_expr()
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
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a data or ';'."<<std::endl;
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
		std::cout<<">>[Error] line "<<this_token.line<<": expect a '{' at this place."<<std::endl;
		return;
	}
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
			default:
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' in an incorrect place."<<std::endl;
				++error;
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
				if(this_token.type!=__right_curve && this_token.type!=__comma)
				{
					++error;
					std::cout<<">>[Error] line "<<this_token.line<<": expect a ',' or ')' when creating a new function."<<std::endl;
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
					std::cout<<">>[Error] line "<<this_token.line<<": only ')' can be put after dynamic identifier."<<std::endl;
					return;
				}
				parse.push(this_token);
			}
			else
			{
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": expect identifiers and dynamic identifier only."<<std::endl;
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
		std::cout<<"' when creating a new function."<<std::endl;
		return;
	}
	statements_block();
	return;
}
void nasal_parser::list_generate_expr()
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
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": incorrect token '";
				print_token(this_token.type);
				std::cout<<"' when creating a new list."<<std::endl;
				return;
				break;
		}
		get_token();
		if(this_token.type!=__comma && this_token.type!=__right_bracket)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ',' or ']'."<<std::endl;
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
		switch(this_token.type)
		{
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __id:identifier_begin_expr();break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": incorrect token '";
				print_token(this_token.type);
				std::cout<<"' when creating a new hash member."<<std::endl;
				return;
				break;
		}
		get_token();
		if(this_token.type!=__colon)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ':' when creating a new hash member."<<std::endl;
			return;
		}
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
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": incorrect token '";
				print_token(this_token.type);
				std::cout<<"' when creating a new hash member."<<std::endl;
				return;
				break;
		}
		get_token();
		if(this_token.type!=__comma && this_token.type!=__right_brace)
		{
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ',' or '}'."<<std::endl;
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
	if(this_token.type!=__id && this_token.type!=__left_curve)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect an identifier."<<std::endl;
		return;
	}
	if(this_token.type==__left_curve)
	{
		while(this_token.type!=__right_curve)
		{
			get_token();
			if(this_token.type!=__id)
			{
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": the expect type is identifier."<<std::endl;
				return;
			}
			get_token();
			if(this_token.type!=__comma && this_token.type!=__right_curve)
			{
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": expect a ',' or ')'."<<std::endl;
				return;
			}
			if(this_token.type==__right_curve)
				break;
		}
	}
	get_token();
	if(this_token.type!=__equal && this_token.type!=__semi)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a '=' after identifier."<<std::endl;
		return;
	}
	else if(this_token.type==__semi)
	{
		parse.push(this_token);// for semi check
//		++warning;
//		std::cout<<">>[Warning] line "<<this_token.line<<": better initializing this."<<std::endl;
		return;
	}
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
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": incorretc data type when doing assignment."<<std::endl;
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
			this_token=temp;// to avoid when recognizing 'else' without 'if' 
			
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
		case __add_operator:
		case __sub_operator:
		case __nor_operator:one_operator_expr();break;
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __id:identifier_begin_expr();break;
		case __left_curve:in_curve_calc_expr();break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a correct data."<<std::endl;
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
	statements_block();
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
			case __add_operator:
			case __sub_operator:
			case __nor_operator:one_operator_expr();break;
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __id:identifier_begin_expr();break;
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": expect a correct data."<<std::endl;
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
		statements_block();
		if(parse.empty())
			return;
		get_token();
	}
	if(this_token.type==__else)
		statements_block();
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
			case __add_operator:
			case __sub_operator:
			case __nor_operator:one_operator_expr();break;
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __id:identifier_begin_expr();break;
			case __left_curve:in_curve_calc_expr();break;
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
			std::cout<<">>[Error] line "<<this_token.line<<": expect a ')' after 'while'."<<std::endl;
			return;
		}
		statements_block();
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
			case __id:identifier_begin_expr();break;
			case __semi:parse.push(this_token);break;
			default:
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' in an incorrect place."<<std::endl;
				++error;
				break;
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
			default:
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' in an incorrect place."<<std::endl;
				++error;
				break;
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
			default:
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' in an incorrect place."<<std::endl;
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
		statements_block();
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
			case __id:identifier_begin_expr();break;
			case __semi:parse.push(this_token);break;
			default:
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' in an incorrect place."<<std::endl;
				++error;
				break;
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
			default:
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' in an incorrect place."<<std::endl;
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
		statements_block();
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
void nasal_parser::add_sub_operator_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __id:identifier_begin_expr();break;
		case __left_curve:in_curve_calc_expr();break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a data after this operator."<<std::endl;
			return;
			break;
	}
	return;
}
void nasal_parser::mul_div_operator_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __id:identifier_begin_expr();break;
		case __left_curve:in_curve_calc_expr();break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a data after this operator."<<std::endl;
			return;
			break;
	}
	return;
}
void nasal_parser::link_operator_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __id:identifier_begin_expr();break;
		case __left_curve:in_curve_calc_expr();break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a data after this operator."<<std::endl;
			return;
			break;
	}
	return;
}
void nasal_parser::compare_operator_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __id:identifier_begin_expr();break;
		case __left_curve:in_curve_calc_expr();break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a data after this operator."<<std::endl;
			return;
			break;
	}
	return;
}
void nasal_parser::one_operator_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __id:identifier_begin_expr();break;
		case __left_curve:in_curve_calc_expr();break;
		default:
			++error;
			std::cout<<">>[Error] line "<<this_token.line<<": expect a data after this operator."<<std::endl;
			return;
			break;
	}
	return;
}
void nasal_parser::in_curve_calc_expr()
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
	if(this_token.type!=__right_curve)
	{
		++error;
		std::cout<<">>[Error] line "<<this_token.line<<": expect a ')' at this line."<<std::endl;
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
		default:parse.push(this_token);break;
	}
	return;
}
void nasal_parser::number_begin_expr()
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
void nasal_parser::string_begin_expr()
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
void nasal_parser::call_list_expr()
{
	get_token();
	switch(this_token.type)
	{
		case __number:number_begin_expr();break;
		case __string:string_begin_expr();break;
		case __id:identifier_begin_expr();break;
		case __left_curve:in_curve_calc_expr();break;
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
			case __number:number_begin_expr();break;
			case __string:string_begin_expr();break;
			case __id:identifier_begin_expr();break;
			case __left_curve:in_curve_calc_expr();break;
			case __right_bracket:return;break;// this is [number:]
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
void nasal_parser::call_function_expr()
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
			default:
				++error;
				std::cout<<">>[Error] line "<<this_token.line<<": incorrect token '";
				print_token(this_token.type);
				std::cout<<"' when calling a function."<<std::endl;
				return;
				break;
		}
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
		std::cout<<">>[Error] line "<<this_token.line<<": missing identifier when calling hash members."<<std::endl;
		return;
	}
	identifier_begin_expr();
	return;
}
void nasal_parser::identifier_begin_expr()
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
void nasal_parser::parse_main_work()
{
	error=0;
	warning=0;
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
			default:
				std::cout<<">>[Error] line "<<this_token.line<<": \'";
				print_token(this_token.type);
				std::cout<<"\' in an incorrect place."<<std::endl;
				++error;
				break;
		}
	}
	std::cout<<">>[Parse] complete generation."<<error<<" error(s),"<<warning<<" warning(s)."<<std::endl;
	return;
}

#endif
