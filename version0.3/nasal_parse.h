#ifndef __NASAL_PARSE_H__
#define __NASAL_PARSE_H__
#include "nasal_lexer.h"
#include "nasal_token.h"

enum token_type
{
	__stack_end,__identifier,__use_identifier,
	__scalar,__function,__var,__equal,__semi,__definition
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
	if(tbl[0]==__var && tbl[1]==__identifier && tbl[2]==__equal && tbl[3]==__scalar && tbl[4]==__semi)
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
		parser.push(temp_parse);
		definition_reduction();
		if(parser.top().type==__definition)
			std::cout<<"Definition in "<<parser.top().line<<std::endl;
	}
	return;
}

parse nasal_parse;

#endif
