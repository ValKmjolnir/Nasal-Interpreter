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
	__identifier,__identifiers,
	__scalar,__scalars,
	__hash_member,__hash_members,
	__statement,__statements,
	__function,//function(){}
	__definition,__assignment,
	__loop,__continue,__break,// for()while() continue; break;
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
		bool scalars_reduction();
		bool identifier_check();
		bool identifiers_reduction();
		bool hashmember_check();
		bool hashmembers_reduction();
		bool statement_check();
		bool statements_reduction();
		bool definition_check();
		bool assignment_check();
		void parse_work(token_list&);
		void print_stack();
		void stack_set_empty();
};

bool parse::scalars_reduction()
{
	int tbl[3]={0};
	std::stack<parse_unit> temp;
	for(int i=0;i<3;++i)
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
	if(((tbl[2]==__scalar) || (tbl[2]==__scalars)) && (tbl[1]==__comma) && ((tbl[0]==__scalar) || (tbl[0]==__identifier)))
	{
		parse_unit t;
		t.type=__scalars;
		t.line=parser.top().line;
		for(int i=0;i<3;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	else if(((tbl[2]==__identifier) || (tbl[2]==__identifiers)) && (tbl[1]==__comma) && (tbl[0]==__scalar))
	{
		parse_unit t;
		t.type=__scalars;
		t.line=parser.top().line;
		for(int i=0;i<3;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	return false;
}
bool parse::identifier_check()
{
	int tbl[4]={0};
	std::stack<parse_unit> temp;
	for(int i=0;i<4;++i)
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
	if((tbl[3]==__identifier) && (tbl[2]==__left_bracket) && ((tbl[1]==__identifier) || (tbl[1]==__scalar)) && (tbl[0]==__right_bracket))
	{
		parse_unit t;
		t.type=__identifier;
		t.line=parser.top().line;
		for(int i=0;i<4;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	else if((tbl[2]==__identifier) && (tbl[1]==__dot) && (tbl[0]==__identifier))
	{
		parse_unit t;
		t.type=__identifier;
		t.line=parser.top().line;
		for(int i=0;i<3;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	else if((tbl[3]==__identifier) && (tbl[2]==__left_curve) && ((tbl[1]==__identifier) || (tbl[1]==__identifiers) || (tbl[1]==__scalar) || (tbl[1]==__scalars)) && (tbl[0]==__right_curve))
	{
		parse_unit t;
		t.type=__identifier;
		t.line=parser.top().line;
		for(int i=0;i<4;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	else if((tbl[2]==__identifier) && (tbl[1]==__left_curve) && (tbl[0]==__right_curve))
	{
		parse_unit t;
		t.type=__identifier;
		t.line=parser.top().line;
		for(int i=0;i<3;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	return false;
}
bool parse::identifiers_reduction()
{
	int tbl[3]={0};
	std::stack<parse_unit> temp;
	for(int i=0;i<3;++i)
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
	if(((tbl[2]==__identifier) || (tbl[2]==__identifiers)) && (tbl[1]==__comma) && (tbl[0]==__identifier))
	{
		parse_unit t;
		t.type=__identifiers;
		t.line=parser.top().line;
		for(int i=0;i<3;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	return false;
}
bool parse::hashmember_check()
{
	int tbl[9]={0};
	std::stack<parse_unit> temp;
	for(int i=0;i<9;++i)
	{
		if(parser.empty())
			break;
		temp.push(parser.top());
		tbl[i]=temp.top().type;
		parser.pop();
	}
	for(int i=0;i<9;++i)
	{
		if(temp.empty())
			break;
		parser.push(temp.top());
		temp.pop();
	}
	if((tbl[2]==__identifier) && (tbl[1]==__colon) && ((tbl[0]==__identifier) || (tbl[0]==__scalar)))
	{
		parse_unit t;
		t.type=__hash_member;
		t.line=parser.top().line;
		for(int i=0;i<3;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	else if((tbl[6]==__identifier) && (tbl[5]==__colon) && (tbl[4]==__func) && (tbl[3]==__left_curve) && (tbl[2]==__right_curve) && (tbl[1]==__left_brace) && (tbl[0]==__right_brace))
	{
		parse_unit t;
		t.type=__hash_member;
		t.line=parser.top().line;
		for(int i=0;i<7;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	else if((tbl[7]==__identifier) && (tbl[6]==__colon) && (tbl[5]==__func) && (tbl[4]==__left_curve) && ((tbl[3]==__identifier) || (tbl[3]==__identifiers)) && (tbl[2]==__right_curve) && (tbl[1]==__left_brace) && (tbl[0]==__right_brace))
	{
		parse_unit t;
		t.type=__hash_member;
		t.line=parser.top().line;
		for(int i=0;i<8;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	else if((tbl[7]==__identifier) && (tbl[6]==__colon) && (tbl[5]==__func) && (tbl[4]==__left_curve) && (tbl[3]==__right_curve) && (tbl[2]==__left_brace) && ((tbl[1]==__statement) || (tbl[1]==__statements)) && (tbl[0]==__right_brace))
	{
		parse_unit t;
		t.type=__hash_member;
		t.line=parser.top().line;
		for(int i=0;i<8;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	else if((tbl[8]==__identifier) && (tbl[7]==__colon) && (tbl[6]==__func) && (tbl[5]==__left_curve) && ((tbl[4]==__identifier) || (tbl[4]==__identifiers)) && (tbl[3]==__right_curve) && (tbl[2]==__left_brace) && ((tbl[1]==__statement) || (tbl[1]==__statements)) && (tbl[0]==__right_brace))
	{
		parse_unit t;
		t.type=__hash_member;
		t.line=parser.top().line;
		for(int i=0;i<9;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	return false;
}
bool parse::hashmembers_reduction()
{
	int tbl[3]={0};
	std::stack<parse_unit> temp;
	for(int i=0;i<3;++i)
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
	if(((tbl[2]==__hash_member) || (tbl[2]==__hash_members)) && (tbl[1]==__comma) && (tbl[0]==__hash_member))
	{
		parse_unit t;
		t.type=__hash_members;
		t.line=parser.top().line;
		for(int i=0;i<3;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	return false;
}
bool parse::definition_check()
{
	int tbl[7]={0};
	std::stack<parse_unit> temp;
	for(int i=0;i<7;++i)
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
	if((tbl[4]==__var) && (tbl[3]==__identifier) && (tbl[2]==__equal) && ((tbl[1]==__identifier) || (tbl[1]==__scalar)) && (tbl[0]==__semi))
	{
		parse_unit t;
		t.type=__definition;
		t.line=parser.top().line;
		for(int i=0;i<5;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	else if((tbl[5]==__var) && (tbl[4]==__identifier) && (tbl[3]==__equal) && (tbl[2]==__left_bracket) && (tbl[1]==__right_bracket) && (tbl[0]==__semi))
	{
		parse_unit t;
		t.type=__definition;
		t.line=parser.top().line;
		for(int i=0;i<6;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	else if((tbl[5]==__var) && (tbl[4]==__identifier) && (tbl[3]==__equal) && (tbl[2]==__left_brace) && (tbl[1]==__right_brace) && (tbl[0]==__semi))
	{
		parse_unit t;
		t.type=__definition;
		t.line=parser.top().line;
		for(int i=0;i<6;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	else if((tbl[6]==__var) && (tbl[5]==__identifier) && (tbl[4]==__equal) && (tbl[3]==__left_bracket) && ((tbl[2]==__scalar) || (tbl[2]==__scalars) || (tbl[2]==__identifier) || (tbl[2]==__identifiers)) && (tbl[1]==__right_bracket) && (tbl[0]==__semi))
	{
		parse_unit t;
		t.type=__definition;
		t.line=parser.top().line;
		for(int i=0;i<7;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	else if((tbl[6]==__var) && (tbl[5]==__identifier) && (tbl[4]==__equal) && (tbl[3]==__left_brace) && ((tbl[2]==__hash_member) || (tbl[2]==__hash_members)) && (tbl[1]==__right_brace) && (tbl[0]==__semi))
	{
		parse_unit t;
		t.type=__definition;
		t.line=parser.top().line;
		for(int i=0;i<7;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	return false;
}
bool parse::assignment_check()
{
	int tbl[6]={0};
	std::stack<parse_unit> temp;
	for(int i=0;i<6;++i)
	{
		if(parser.empty())
			break;
		temp.push(parser.top());
		tbl[i]=temp.top().type;
		parser.pop();
	}
	for(int i=0;i<6;++i)
	{
		if(temp.empty())
			break;
		parser.push(temp.top());
		temp.pop();
	}
	if((tbl[3]==__identifier) && (tbl[2]==__equal) && ((tbl[1]==__identifier) || (tbl[1]==__scalar)) && (tbl[0]==__semi))
	{
		parse_unit t;
		t.type=__assignment;
		t.line=parser.top().line;
		for(int i=0;i<4;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	else if((tbl[4]==__identifier) && (tbl[3]==__equal) && (tbl[2]==__left_bracket) && (tbl[1]==__right_bracket) && (tbl[0]==__semi))
	{
		parse_unit t;
		t.type=__assignment;
		t.line=parser.top().line;
		for(int i=0;i<5;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	else if((tbl[4]==__identifier) && (tbl[3]==__equal) && (tbl[2]==__left_brace) && (tbl[1]==__right_brace) && (tbl[0]==__semi))
	{
		parse_unit t;
		t.type=__assignment;
		t.line=parser.top().line;
		for(int i=0;i<5;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	else if((tbl[5]==__identifier) && (tbl[4]==__equal) && (tbl[3]==__left_bracket) && ((tbl[2]==__scalar) || (tbl[2]==__scalars) || (tbl[2]==__identifier) || (tbl[2]==__identifiers)) && (tbl[1]==__right_bracket) && (tbl[0]==__semi))
	{
		parse_unit t;
		t.type=__assignment;
		t.line=parser.top().line;
		for(int i=0;i<6;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	else if((tbl[5]==__identifier) && (tbl[4]==__equal) && (tbl[3]==__left_brace) && ((tbl[2]==__hash_member) || (tbl[2]==__hash_members)) && (tbl[1]==__right_brace) && (tbl[0]==__semi))
	{
		parse_unit t;
		t.type=__assignment;
		t.line=parser.top().line;
		for(int i=0;i<6;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	return false;
}
bool parse::statement_check()
{
	int tbl[3]={0};
	std::stack<parse_unit> temp;
	for(int i=0;i<3;++i)
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
	if((tbl[0]==__definition) || (tbl[0]==__assignment) || (tbl[0]==__func) || (tbl[0]==__loop) || (tbl[0]==__choose))
	{
		parse_unit t;
		t.type=__statement;
		t.line=parser.top().line;
		parser.pop();
		parser.push(t);
		return true;
	}
	else if((tbl[2]==__return) && ((tbl[1]==__identifier) || (tbl[1]==__scalar)) && (tbl[0]==__semi))
	{
		parse_unit t;
		t.type=__statement;
		t.line=parser.top().line;
		for(int i=0;i<3;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	else if((tbl[2]!=__return) && ((tbl[1]==__identifier) || (tbl[1]==__continue) || (tbl[1]==__break)) && (tbl[0]==__semi))
	{
		parse_unit t;
		t.type=__statement;
		t.line=parser.top().line;
		for(int i=0;i<2;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	return false;
}
bool parse::statements_reduction()
{
	int tbl[2]={0};
	std::stack<parse_unit> temp;
	for(int i=0;i<2;++i)
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
	if(((tbl[1]==__statement) || (tbl[1]==__statements)) && (tbl[0]==__statement))
	{
		parse_unit t;
		t.type=__statements;
		t.line=parser.top().line;
		for(int i=0;i<2;++i)
			parser.pop();
		parser.push(t);
		return true;
	}
	return false;
}
void parse::stack_set_empty()
{
	while(!parser.empty())
		parser.pop();
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
		if((temp->content=="var") || (temp->content=="func") || (temp->content=="return") || (temp->content=="nil") || (temp->content=="continue") || (temp->content=="break"))
		{
			if(temp->content=="var")
				temp_parse.type=__var;
			else if(temp->content=="func")
				temp_parse.type=__func;
			else if(temp->content=="return")
				temp_parse.type=__return;
			else if(temp->content=="nil")
				temp_parse.type=__scalar;
			else if(temp->content=="continue")
				temp_parse.type=__continue;
			else if(temp->content=="break")
				temp_parse.type=__break;
		}
		else if(temp->type==IDENTIFIER)
		{
			temp_parse.type=__identifier;
		}
		else if((temp->content==";") || (temp->content==",") || (temp->content=="=") || (temp->content==":") || (temp->content=="."))
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
		else if((temp->type==NUMBER) || (temp->type==STRING))
		{
			temp_parse.type=__scalar;
		}
		else if((temp->content=="+") || (temp->content=="-") || (temp->content=="*") || (temp->content=="/") || (temp->content=="~"))
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
		else if((temp->content=="+=") || (temp->content=="-=") || (temp->content=="*=") || (temp->content=="/=") || (temp->content=="~="))
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
		else if((temp->content=="(") || (temp->content==")") || (temp->content=="[") || (temp->content=="]") || (temp->content=="{") || (temp->content=="}"))
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
		
		bool reduction_complete=false;
		while(!reduction_complete)
		{
			if(scalars_reduction())
			{
				std::cout<<"line "<<parser.top().line<<": Scalars"<<std::endl;
				continue;
			}
			if(identifier_check())
			{
				std::cout<<"line "<<parser.top().line<<": Identifier | Call function | Call hash | Call array"<<std::endl;
				continue;
			}
			if(identifiers_reduction())
			{
				std::cout<<"line "<<parser.top().line<<": Identifiers"<<std::endl;
				continue;
			}
			if(hashmember_check())
			{
				std::cout<<"line "<<parser.top().line<<": Hash member"<<std::endl;
				continue;
			}
			if(hashmembers_reduction())
			{
				std::cout<<"line "<<parser.top().line<<": Hash members"<<std::endl;
				continue;
			}
			if(definition_check())
			{
				std::cout<<"line "<<parser.top().line<<": Definition"<<std::endl;
				continue;
			}
			//assignment check must be put behind the definition check
			//because the assignment check has the same method to check
			//but assignment checks without a "var" so if you put the
			//assignment before definition,there may be a mistake.
			if(assignment_check())
			{
				std::cout<<"line "<<parser.top().line<<": Assignment"<<std::endl;
				continue;
			}
			if(statement_check())
			{
				std::cout<<"line "<<parser.top().line<<": Statement"<<std::endl;
				if(statements_reduction())
					std::cout<<"line "<<parser.top().line<<": Statements"<<std::endl;
				continue;
			}
			reduction_complete=true;
		}
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


parse nasal_parse;

#endif
