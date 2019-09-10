#ifndef __NASAL_PARSER_H__
#define __NASAL_PARSER_H__

#include "nasal_token_type.h"

struct parse_unit
{
	int line;
	int type;
	std::string content;
};


class nasal_parser
{
	private:
		std::stack<parse_unit> parser;
	public:
		void print_parser_stack()
		{
			if(parser.empty())
				return;
			int line=0;
			std::stack<parse_unit> temp;
			while(!parser.empty())
			{
				temp.push(parser.top());
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
				print_token(temp.top().type);
				std::cout<<" ";
				parser.pop();
			}
			while(!temp.empty())
			{
				parser.push(temp.top());
				temp.pop();
			}
			std::cout<<std::endl;
			return;
		}
		std::stack<parse_unit>& return_stack()
		{
			return parser;
		}
		void parse_process(std::list<token>& lexer)
		{
			while(!parser.empty())
				parser.pop();
			std::stack<parse_unit> temp;
			for(std::list<token>::iterator i=lexer.begin();i!=lexer.end();++i)
			{
				parse_unit temp_parse;
				temp_parse.line=i->line;
				temp_parse.content=i->content;
				if((*i).type==RESERVEWORD)
				{
					if((*i).content=="var")
						temp_parse.type=__var;
					else if((*i).content=="func")
						temp_parse.type=__func;
					else if((*i).content=="return")
						temp_parse.type=__return;
					else if((*i).content=="nil")
						temp_parse.type=__number;
					else if((*i).content=="continue")
						temp_parse.type=__continue;
					else if((*i).content=="break")
						temp_parse.type=__break;
					else if((*i).content=="and")
						temp_parse.type=__and_operator;
					else if((*i).content=="or")
						temp_parse.type=__or_operator;
					else if((*i).content=="for")
						temp_parse.type=__for;
					else if((*i).content=="forindex")
						temp_parse.type=__forindex;
					else if((*i).content=="foreach")
						temp_parse.type=__foreach;
					else if((*i).content=="while")
						temp_parse.type=__while;
					else if((*i).content=="if")
						temp_parse.type=__if;
					else if((*i).content=="else")
						temp_parse.type=__else;
					else if((*i).content=="elsif")
						temp_parse.type=__elsif;
				}
				else if(((*i).content=="==") || ((*i).content=="!=") || ((*i).content==">") || ((*i).content==">=") || ((*i).content=="<") || ((*i).content=="<="))
				{
					if((*i).content=="==")
						temp_parse.type=__cmp_equal;
					else if((*i).content=="!=")
						temp_parse.type=__cmp_not_equal;
					else if((*i).content==">")
						temp_parse.type=__cmp_more;
					else if((*i).content==">=")
						temp_parse.type=__cmp_more_or_equal;
					else if((*i).content=="<")
						temp_parse.type=__cmp_less;
					else if((*i).content=="<=")
						temp_parse.type=__cmp_less_or_equal;
				}
				else if(((*i).content==";") || ((*i).content==",") || ((*i).content=="=") || ((*i).content==":") || ((*i).content=="."))
				{
					char c=(*i).content[0];
					switch(c)
					{
						case ';':temp_parse.type=__semi;break;
						case ',':temp_parse.type=__comma;break;
						case '=':temp_parse.type=__equal;break;
						case ':':temp_parse.type=__colon;break;
						case '.':temp_parse.type=__dot;break;
					}
				}
				else if(((*i).type==NUMBER) || ((*i).type==STRING) || ((*i).type==IDENTIFIER) || ((*i).type==DYNAMIC_ID))
				{
					int t=(*i).type;
					switch(t)
					{
						case NUMBER:temp_parse.type=__number;break;
						case STRING:temp_parse.type=__string;break;
						case IDENTIFIER:temp_parse.type=__id;break;
						case DYNAMIC_ID:temp_parse.type=__dynamic_id;break;
					}
				}
				else if(((*i).content=="+") || ((*i).content=="-") || ((*i).content=="*") || ((*i).content=="/") || ((*i).content=="~") || ((*i).content=="!"))
				{
					char c=(*i).content[0];
					switch(c)
					{
						case '+':temp_parse.type=__add_operator;break;
						case '-':temp_parse.type=__sub_operator;break;
						case '*':temp_parse.type=__mul_operator;break;
						case '/':temp_parse.type=__div_operator;break;
						case '~':temp_parse.type=__link_operator;break;
						case '!':temp_parse.type=__nor_operator;break;
					}
				}
				else if(((*i).content=="+=") || ((*i).content=="-=") || ((*i).content=="*=") || ((*i).content=="/=") || ((*i).content=="~="))
				{
					char c=(*i).content[0];
					switch(c)
					{
						case '+':temp_parse.type=__add_equal;break;
						case '-':temp_parse.type=__sub_equal;break;
						case '*':temp_parse.type=__mul_equal;break;
						case '/':temp_parse.type=__div_equal;break;
						case '~':temp_parse.type=__link_equal;break;
					}
				}
				else if(((*i).content=="(") || ((*i).content==")") || ((*i).content=="[") || ((*i).content=="]") || ((*i).content=="{") || ((*i).content=="}"))
				{
					char c=(*i).content[0];
					switch(c)
					{
						case '(':temp_parse.type=__left_curve;break;
						case ')':temp_parse.type=__right_curve;break;
						case '[':temp_parse.type=__left_bracket;break;
						case ']':temp_parse.type=__right_bracket;break;
						case '{':temp_parse.type=__left_brace;break;
						case '}':temp_parse.type=__right_brace;break;
					}
				}
				temp.push(temp_parse);//push this into stack
			}
			if(temp.empty())
			{
				std::cout<<">>[Warning] Empty lexer."<<std::endl;
				std::cout<<">>[Parse] Parser exited."<<std::endl;
				return;
			}
			while(!temp.empty())
			{
				parser.push(temp.top());
				temp.pop();
			}
			return;
		}
};

#endif
