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
	__var,// var
	__identifier,__scalar,__function,
	__definition,__assignment,
	
};
token_type token_type_tbl;

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
		void definition_reduction();
		void assignment_reduction();
		void parse_work(token_list&);
};

void parse::definition_reduction()
{
	int tbl[5];
	std::stack<parse_unit> temp;
	for(int i=4;i>=0;--i)
	{
		if(parser.empty())
			break;
		temp.push(parser.top());
		tbl[i]=temp.top().type;
		parser.pop();
	}
	if(tbl[0]==__var && tbl[1]==__identifier && tbl[2]==__equal && (tbl[3]==__scalar || tbl[3]==__identifier || tbl[3]==__function) && tbl[4]==__semi)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__definition;
		temp_parse_unit.line=temp.top().line;
		parser.push(temp_parse_unit);
	}
	else
		for(int i=0;i<5;++i)
		{
			if(temp.empty())
				break;
			parser.push(temp.top());
			temp.pop();
		}
	return;
}
void parse::assignment_reduction()
{
	int tbl[4];
	std::stack<parse_unit> temp;
	for(int i=3;i>=0;--i)
	{
		if(parser.empty())
			break;
		temp.push(parser.top());
		tbl[i]=temp.top().type;
		parser.pop();
	}
	if(tbl[0]==__identifier && tbl[1]==__equal && (tbl[2]==__scalar || tbl[2]==__identifier) && tbl[3]==__semi)
	{
		parse_unit temp_parse_unit;
		temp_parse_unit.type=__assignment;
		temp_parse_unit.line=temp.top().line;
		parser.push(temp_parse_unit);
	}
	else
		for(int i=0;i<4;++i)
		{
			if(temp.empty())
				break;
			parser.push(temp.top());
			temp.pop();
		}
	return;
}

void parse::parse_work(token_list& lexer)
{
	parse_unit temp_parse;
	token_unit *temp=lexer.get_head();
	while(temp->next)
	{
		temp=temp->next;
		temp_parse.line=temp->line;
		if(temp->content=="var")
		{
			token_type_tbl=__var;
			temp_parse.type=token_type_tbl;
		}
		else if(temp->content=="=")
		{
			token_type_tbl=__equal;
			temp_parse.type=token_type_tbl;
		}
		else if(temp->type==IDENTIFIER)
		{
			token_type_tbl=__identifier;
			temp_parse.type=token_type_tbl;
		}
		else if(temp->content==";")
		{
			token_type_tbl=__semi;
			temp_parse.type=token_type_tbl;
		}
		else if(temp->type==NUMBER || temp->type==STRING)
		{
			token_type_tbl=__scalar;
			temp_parse.type=token_type_tbl;
		}
		else if(temp->content=="+" || temp->content=="-" || temp->content=="*" || temp->content=="/" || temp->content=="~")
		{
			if(temp->content=="+")
				token_type_tbl=__add_operator;
			else if(temp->content=="-")
				token_type_tbl=__sub_operator;
			else if(temp->content=="*")
				token_type_tbl=__mul_operator;
			else if(temp->content=="/")
				token_type_tbl=__div_operator;
			else if(temp->content=="~")
				token_type_tbl=__link_operator;
			temp_parse.type=token_type_tbl;
		}
		else if(temp->content=="+=" || temp->content=="-=" || temp->content=="*=" || temp->content=="/=" || temp->content=="~=")
		{
			if(temp->content=="+=")
				token_type_tbl=__add_equal;
			else if(temp->content=="-=")
				token_type_tbl=__sub_equal;
			else if(temp->content=="*=")
				token_type_tbl=__mul_equal;
			else if(temp->content=="/=")
				token_type_tbl=__div_equal;
			else if(temp->content=="~=")
				token_type_tbl=__link_equal;
			temp_parse.type=token_type_tbl;
		}
		parser.push(temp_parse);
		definition_reduction();
		assignment_reduction();
		if(parser.top().type==__definition)
			std::cout<<"line "<<parser.top().line<<": definition"<<std::endl;
		else if(parser.top().type==__assignment)
			std::cout<<"line "<<parser.top().line<<": assignment"<<std::endl;
	}
	return;
}

parse nasal_parse;

#endif
