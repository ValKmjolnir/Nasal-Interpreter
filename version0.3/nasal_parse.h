#ifndef __NASAL_PARSE_H__
#define __NASAL_PARSE_H__
#include "nasal_lexer.h"
#include "nasal_token.h"

enum token_type
{
	__stack_end,
	__equal,// =
	__add_operator,__sub_operator,__mul_operator,__div_operator,__link_operator,// + - * / ~
	__add_equal,__sub_equal,__mul_equal,__div_equal,__link_equal,// += -= *= /= ~=
	__left_brace,__right_brace,// {}
	__left_bracket,__right_bracket,// []
	__left_curve,__right_curve,// ()
	__semi,// ;
	__comma,// ,
	__colon,// :
	__dot,// .
	__var,// var reserve word
	__func,// func reserve word
	__print,// print reserve word
	__identifier,__identifiers,
	__scalar,__scalars,
	__hash_member,__hash_members,
	__array_search,__hash_search,
	__statement,__statements,
	__function,//function(){}
	__use_function,__class_function,
	__definition,__assignment,
	__print_function,// print()
	__loop,__loop_continue,__loop_break,// for()while() continue; break;
	__choose,// if else if else
	__return,__func_return
};

struct parse_unit
{
	int type;
	int line;
};

class parse
{
	public:
		std::stack<parse_unit> parser;
	public:
		void func_return_reduction();
		void class_function_reduction();
		void array_search_reduction();
		void hash_search_reduction();
		void function_call_reduction();
		void hash_member_reduction();
		void hash_members_reduction();
		void definition_reduction();
		void assignment_reduction();
		void identifier_reduction();
		void scalar_reduction();
		void print_reduction();
		void statement_reduction();
		void statements_reduction();
		void function_reduction();
		void parse_work(token_list&);
		void print_stack();
		void stack_set_empty();
};

void parse::func_return_reduction()
{
	int tbl[3]={0};
	std::stack<parse_unit> temp;
	for(int i=2;i>=0;--i)
	{
		if(parser.empty())
			break;
		temp.push(parser.top());
		tbl[i]=temp.top().type;
		parser.pop();
	}
	for(int i=0;i<3;++i)
	{
		if(temp.empty())
			break;
		parser.push(temp.top());
		temp.pop();
	}
	if(tbl[0]==__return && (tbl[1]==__scalar || tbl[1]==__identifier || tbl[1]==__use_function) && tbl[2]==__semi)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__func_return;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<3;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	return;
}
void parse::class_function_reduction()
{
	int tbl[10]={0};
	std::stack<parse_unit> temp;
	for(int i=9;i>=0;--i)
	{
		if(parser.empty())
			break;
		temp.push(parser.top());
		tbl[i]=temp.top().type;
		parser.pop();
	}
	for(int i=0;i<10;++i)
	{
		if(temp.empty())
			break;
		parser.push(temp.top());
		temp.pop();
	}
	if(tbl[3]==__func && tbl[4]==__left_curve
		&& (tbl[5]==__identifier || tbl[5]==__identifiers) && tbl[6]==__right_curve
		&& tbl[7]==__left_brace && (tbl[8]==__statement || tbl[8]==__statements) && tbl[9]==__right_brace)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__class_function;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<7;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	else if(tbl[4]==__func && tbl[5]==__left_curve
		&& tbl[6]==__right_curve && tbl[7]==__left_brace
		&& (tbl[8]==__statement || tbl[8]==__statements) && tbl[9]==__right_brace)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__class_function;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<6;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	else if(tbl[4]==__func && tbl[5]==__left_curve
		&& (tbl[6]==__identifier || tbl[6]==__identifiers) && tbl[7]==__right_curve
		&& tbl[8]==__left_brace && tbl[9]==__right_brace)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__class_function;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<6;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	else if(tbl[5]==__func
		&& tbl[6]==__left_curve && tbl[7]==__right_curve
		&& tbl[8]==__left_brace && tbl[9]==__right_brace)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__class_function;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<5;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	return;
}
void parse::array_search_reduction()
{
	int tbl[4]={0};
	std::stack<parse_unit> temp;
	for(int i=3;i>=0;--i)
	{
		if(parser.empty())
			break;
		temp.push(parser.top());
		tbl[i]=temp.top().type;
		parser.pop();
	}
	for(int i=0;i<4;++i)
	{
		if(temp.empty())
			break;
		parser.push(temp.top());
		temp.pop();
	}
	if(tbl[0]==__identifier && tbl[1]==__left_bracket
		&& (tbl[2]==__identifier || tbl[2]==__scalar || tbl[2]==__use_function)
		&& tbl[3]==__right_bracket)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__identifier;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<4;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	return;
}
void parse::hash_search_reduction()
{
	int tbl[3]={0};
	std::stack<parse_unit> temp;
	for(int i=2;i>=0;--i)
	{
		if(parser.empty())
			break;
		temp.push(parser.top());
		tbl[i]=temp.top().type;
		parser.pop();
	}
	for(int i=0;i<3;++i)
	{
		if(temp.empty())
			break;
		parser.push(temp.top());
		temp.pop();
	}
	if(tbl[0]==__identifier && tbl[1]==__dot && (tbl[2]==__identifier || tbl[2]==__function || tbl[2]==__use_function))
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__identifier;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<3;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	return;
}
void parse::function_call_reduction()
{
	int tbl[5]={0};
	std::stack<parse_unit> temp;
	for(int i=4;i>=0;--i)
	{
		if(parser.empty())
			break;
		temp.push(parser.top());
		tbl[i]=temp.top().type;
		parser.pop();
	}
	for(int i=0;i<5;++i)
	{
		if(temp.empty())
			break;
		parser.push(temp.top());
		temp.pop();
	}
	if(tbl[0]==__identifier && tbl[1]==__left_curve
		&& (tbl[2]==__identifier || tbl[2]==__identifiers || tbl[2]==__scalar || tbl[2]==__scalars)
		&& tbl[3]==__right_curve)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__use_function;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<5;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	else if(tbl[1]==__identifier && tbl[2]==__left_curve && tbl[3]==__right_curve)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__use_function;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<4;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	return;
}
void parse::hash_member_reduction()
{
	int tbl[3]={0};
	std::stack<parse_unit> temp;
	for(int i=2;i>=0;--i)
	{
		if(parser.empty())
			break;
		temp.push(parser.top());
		tbl[i]=temp.top().type;
		parser.pop();
	}
	for(int i=0;i<3;++i)
	{
		if(temp.empty())
			break;
		parser.push(temp.top());
		temp.pop();
	}
	if(tbl[0]==__identifier && tbl[1]==__colon && (tbl[2]==__identifier || tbl[2]==__scalar || tbl[2]==__class_function || tbl[2]==__use_function))
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__hash_member;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<3;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	return;
}
void parse::hash_members_reduction()
{
	int tbl[3]={0};
	std::stack<parse_unit> temp;
	for(int i=2;i>=0;--i)
	{
		if(parser.empty())
			break;
		temp.push(parser.top());
		tbl[i]=temp.top().type;
		parser.pop();
	}
	for(int i=0;i<3;++i)
	{
		if(temp.empty())
			break;
		parser.push(temp.top());
		temp.pop();
	}
	if((tbl[0]==__hash_member || tbl[0]==__hash_members) && tbl[1]==__comma && tbl[2]==__hash_member)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__hash_members;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<3;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	return;
}
void parse::definition_reduction()
{
	int tbl[7]={0};
	std::stack<parse_unit> temp;
	for(int i=6;i>=0;--i)
	{
		if(parser.empty())
			break;
		temp.push(parser.top());
		tbl[i]=temp.top().type;
		parser.pop();
	}
	for(int i=0;i<7;++i)
	{
		if(temp.empty())
			break;
		parser.push(temp.top());
		temp.pop();
	}
	if(tbl[2]==__var && tbl[3]==__identifier && tbl[4]==__equal && (tbl[5]==__scalar || tbl[5]==__identifier || tbl[5]==__use_function) && tbl[6]==__semi)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__definition;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<5;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	else if(tbl[0]==__var && tbl[1]==__identifier && tbl[2]==__equal && tbl[3]==__left_bracket
		&& (tbl[4]==__identifier || tbl[4]==__identifiers || tbl[4]==__scalar || tbl[4]==__scalars || tbl[4]==__use_function || tbl[4]==__array_search || tbl[4]==__hash_search)
		&& tbl[5]==__right_bracket && tbl[6]==__semi)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__definition;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<7;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	else if(tbl[1]==__var && tbl[2]==__identifier && tbl[3]==__equal && tbl[4]==__left_bracket
		&& tbl[5]==__right_bracket && tbl[6]==__semi)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__definition;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<6;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	else if(tbl[0]==__var && tbl[1]==__identifier && tbl[2]==__equal && tbl[3]==__left_brace
		&& (tbl[4]==__hash_member || tbl[4]==__hash_members)
		&& tbl[5]==__right_brace && tbl[6]==__semi)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__definition;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<7;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	else if(tbl[1]==__var && tbl[2]==__identifier && tbl[3]==__equal && tbl[4]==__left_brace
		&& tbl[5]==__right_brace && tbl[6]==__semi)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__definition;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<6;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	else if(tbl[3]==__var && tbl[4]==__identifier && tbl[5]==__equal && tbl[6]==__use_function)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__definition;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<4;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	return;
}
void parse::assignment_reduction()
{
	int tbl[4]={0};
	std::stack<parse_unit> temp;
	for(int i=3;i>=0;--i)
	{
		if(parser.empty())
			break;
		temp.push(parser.top());
		tbl[i]=temp.top().type;
		parser.pop();
	}
	for(int i=0;i<4;++i)
	{
		if(temp.empty())
			break;
		parser.push(temp.top());
		temp.pop();
	}
	if(tbl[0]==__identifier
	&& (tbl[1]==__equal || tbl[1]==__add_equal || tbl[1]==__sub_equal || tbl[1]==__mul_equal || tbl[1]==__div_equal || tbl[1] ==__link_equal)
	&& (tbl[2]==__scalar || tbl[2]==__identifier || tbl[2]==__use_function) && tbl[3]==__semi)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__assignment;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<4;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	else if(tbl[1]==__identifier
		&& (tbl[2]==__equal || tbl[2]==__add_equal || tbl[2]==__sub_equal || tbl[2]==__mul_equal || tbl[2]==__div_equal || tbl[2] ==__link_equal)
		&& tbl[3]==__use_function)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__assignment;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<3;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	return;
}
void parse::identifier_reduction()
{
	int tbl[3]={0};
	std::stack<parse_unit> temp;
	for(int i=2;i>=0;--i)
	{
		if(parser.empty())
			break;
		temp.push(parser.top());
		tbl[i]=temp.top().type;
		parser.pop();
	}
	for(int i=0;i<3;++i)
	{
		if(temp.empty())
			break;
		parser.push(temp.top());
		temp.pop();
	}
	if((tbl[0]==__identifier || tbl[0]==__identifiers) && tbl[1]==__comma && tbl[2]==__identifier)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__identifiers;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<3;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	return;
}
void parse::scalar_reduction()
{
	int tbl[3]={0};
	std::stack<parse_unit> temp;
	for(int i=2;i>=0;--i)
	{
		if(parser.empty())
			break;
		temp.push(parser.top());
		tbl[i]=temp.top().type;
		parser.pop();
	}
	for(int i=0;i<3;++i)
	{
		if(temp.empty())
			break;
		parser.push(temp.top());
		temp.pop();
	}
	if(((tbl[0]==__scalar || tbl[0]==__scalars || tbl[0]==__identifier || tbl[0]==__identifiers || tbl[0]==__use_function) && tbl[1]==__comma && (tbl[2]==__scalar || tbl[2]==__use_function))
		|| ((tbl[0]==__scalar || tbl[0]==__scalars || tbl[0]==__use_function) && tbl[1]==__comma && tbl[2]==__identifier))
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__scalars;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<3;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	return;
}
void parse::print_reduction()
{
	int tbl[5]={0};
	std::stack<parse_unit> temp;
	for(int i=4;i>=0;--i)
	{
		if(parser.empty())
			break;
		temp.push(parser.top());
		tbl[i]=temp.top().type;
		parser.pop();
	}
	for(int i=0;i<5;++i)
	{
		if(temp.empty())
			break;
		parser.push(temp.top());
		temp.pop();
	}
	if(tbl[0]==__print && tbl[1]==__left_curve
	&& (tbl[2]==__scalar || tbl[2]==__scalars || tbl[2]==__identifier || tbl[2]==__identifiers || tbl[2]==__use_function)
	&& tbl[3]==__right_curve && tbl[4]==__semi)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__print_function;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<5;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	else if(tbl[1]==__print && tbl[2]==__left_curve	&& tbl[3]==__right_curve && tbl[4]==__semi)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__print_function;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<4;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	return;
}
void parse::statement_reduction()
{
	int top_type=parser.top().type;
	if(top_type==__definition || top_type==__assignment || top_type==__print_function || top_type==__use_function
	|| top_type==__loop || top_type==__loop_continue || top_type==__loop_break || top_type==__choose || top_type==__func_return)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__statement;
		temp_parse_unit.line=parser.top().line;
		parser.pop();
		parser.push(temp_parse_unit);
	}
	return;
}
void parse::statements_reduction()
{
	int tbl[2]={0};
	std::stack<parse_unit> temp;
	for(int i=1;i>=0;--i)
	{
		if(parser.empty())
			break;
		temp.push(parser.top());
		tbl[i]=temp.top().type;
		parser.pop();
	}
	for(int i=0;i<2;++i)
	{
		if(temp.empty())
			break;
		parser.push(temp.top());
		temp.pop();
	}
	if((tbl[0]==__statement || tbl[0]==__statements) && tbl[1]==__statement)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__statements;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<2;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	return;
}
void parse::function_reduction()
{
	int tbl[10]={0};
	std::stack<parse_unit> temp;
	for(int i=9;i>=0;--i)
	{
		if(parser.empty())
			break;
		temp.push(parser.top());
		tbl[i]=temp.top().type;
		parser.pop();
	}
	for(int i=0;i<10;++i)
	{
		if(temp.empty())
			break;
		parser.push(temp.top());
		temp.pop();
	}
	if(tbl[0]==__var && tbl[1]==__identifier && tbl[2]==__equal && tbl[3]==__func && tbl[4]==__left_curve
		&& (tbl[5]==__identifier || tbl[5]==__identifiers) && tbl[6]==__right_curve
		&& tbl[7]==__left_brace && (tbl[8]==__statement || tbl[8]==__statements) && tbl[9]==__right_brace)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__function;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<10;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	else if(tbl[1]==__var && tbl[2]==__identifier && tbl[3]==__equal && tbl[4]==__func && tbl[5]==__left_curve
		&& tbl[6]==__right_curve && tbl[7]==__left_brace
		&& (tbl[8]==__statement || tbl[8]==__statements) && tbl[9]==__right_brace)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__function;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<9;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	else if(tbl[1]==__var && tbl[2]==__identifier && tbl[3]==__equal && tbl[4]==__func && tbl[5]==__left_curve
		&& (tbl[6]==__identifier || tbl[6]==__identifiers) && tbl[7]==__right_curve
		&& tbl[8]==__left_brace && tbl[9]==__right_brace)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__function;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<9;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	else if(tbl[2]==__var && tbl[3]==__identifier && tbl[4]==__equal && tbl[5]==__func
		&& tbl[6]==__left_curve && tbl[7]==__right_curve
		&& tbl[8]==__left_brace && tbl[9]==__right_brace)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__function;
		temp_parse_unit.line=parser.top().line;
		for(int i=0;i<8;++i)
			parser.pop();
		parser.push(temp_parse_unit);
	}
	return;
}

void parse::parse_work(token_list& lexer)
{
	parse_unit temp_parse;
	token_unit *temp=lexer.get_head();
	stack_set_empty();
	while(temp->next)
	{
		temp=temp->next;
		temp_parse.line=temp->line;
		if(temp->content=="var" || temp->content=="print" || temp->content=="func" || temp->content=="return" || temp->content=="nil")
		{
			if(temp->content=="var")
				temp_parse.type=__var;
			else if(temp->content=="print")
				temp_parse.type=__print;
			else if(temp->content=="func")
				temp_parse.type=__func;
			else if(temp->content=="return")
				temp_parse.type=__return;
			else if(temp->content=="nil")
				temp_parse.type=__scalar;
		}
		else if(temp->type==IDENTIFIER)
		{
			temp_parse.type=__identifier;
		}
		else if(temp->content==";" || temp->content=="," || temp->content=="=" || temp->content==":" || temp->content==".")
		{
			if(temp->content==";")
				temp_parse.type=__semi;
			else if(temp->content==",")
				temp_parse.type=__comma;
			else if(temp->content=="=")
				temp_parse.type=__equal;
			else if(temp->content==":")
				temp_parse.type=__colon;
			else if(temp->content==".")
				temp_parse.type=__dot;
		}
		else if(temp->type==NUMBER || temp->type==STRING)
		{
			temp_parse.type=__scalar;
		}
		else if(temp->content=="+" || temp->content=="-" || temp->content=="*" || temp->content=="/" || temp->content=="~")
		{
			if(temp->content=="+")
				temp_parse.type=__add_operator;
			else if(temp->content=="-")
				temp_parse.type=__sub_operator;
			else if(temp->content=="*")
				temp_parse.type=__mul_operator;
			else if(temp->content=="/")
				temp_parse.type=__div_operator;
			else if(temp->content=="~")
				temp_parse.type=__link_operator;
		}
		else if(temp->content=="+=" || temp->content=="-=" || temp->content=="*=" || temp->content=="/=" || temp->content=="~=")
		{
			if(temp->content=="+=")
				temp_parse.type=__add_equal;
			else if(temp->content=="-=")
				temp_parse.type=__sub_equal;
			else if(temp->content=="*=")
				temp_parse.type=__mul_equal;
			else if(temp->content=="/=")
				temp_parse.type=__div_equal;
			else if(temp->content=="~=")
				temp_parse.type=__link_equal;
		}
		else if(temp->content=="(" || temp->content==")" || temp->content=="[" || temp->content=="]" || temp->content=="{" || temp->content=="}")
		{
			char c=temp->content[0];
			switch(c)
			{
				case '(':
					temp_parse.type=__left_curve;
					break;
				case ')':
					temp_parse.type=__right_curve;
					break;
				case '[':
					temp_parse.type=__left_bracket;
					break;
				case ']':
					temp_parse.type=__right_bracket;
					break;
				case '{':
					temp_parse.type=__left_brace;
					break;
				case '}':
					temp_parse.type=__right_brace;
					break;
			}
		}
		parser.push(temp_parse);
		
		identifier_reduction();
		scalar_reduction();
		array_search_reduction();
		hash_search_reduction();
		print_reduction();
		function_call_reduction();
		func_return_reduction();
		definition_reduction();
		assignment_reduction();
		if(parser.top().type==__definition)
			std::cout<<"line "<<parser.top().line<<": definition"<<std::endl;
		else if(parser.top().type==__assignment)
			std::cout<<"line "<<parser.top().line<<": assignment"<<std::endl;
		else if(parser.top().type==__print_function)
			std::cout<<"line "<<parser.top().line<<": print()"<<std::endl;
		else if(parser.top().type==__use_function)
			std::cout<<"line "<<parser.top().line<<": call func()"<<std::endl;
		statement_reduction();//must be put before statements
		statements_reduction();
		function_reduction();
		class_function_reduction();
		if(parser.top().type==__function)
			std::cout<<"line "<<parser.top().line<<": function()"<<std::endl;
		hash_member_reduction();
		hash_members_reduction();
	}
	return;
}

void parse::print_stack()
{
	std::cout<<">> parser stack:"<<std::endl;
	while(!parser.empty())
	{
		if(parser.top().type==__definition)
			std::cout<<"line "<<parser.top().line<<": definition"<<std::endl;
		else if(parser.top().type==__assignment)
			std::cout<<"line "<<parser.top().line<<": assignment"<<std::endl;
		else if(parser.top().type==__print_function)
			std::cout<<"line "<<parser.top().line<<": print()"<<std::endl;
		else if(parser.top().type==__statement)
			std::cout<<"line "<<parser.top().line<<": statement"<<std::endl;
		else if(parser.top().type==__statements)
			std::cout<<"line "<<parser.top().line<<": statements"<<std::endl;
		else if(parser.top().type==__function)
			std::cout<<"line "<<parser.top().line<<": function()"<<std::endl;
		else
			std::cout<<"line "<<parser.top().line<<": "<<parser.top().type<<std::endl;
		parser.pop();
	}
	return;
}
void parse::stack_set_empty()
{
	while(!parser.empty())
		parser.pop();
	return;
}

parse nasal_parse;

#endif
