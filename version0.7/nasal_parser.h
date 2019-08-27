#ifndef __NASAL_PARSER_H__
#define __NASAL_PARSER_H__

#include <stack>

enum token_type
{
	__stack_end,
	__null_end,// $
	__equal,// =
	__cmp_equal,// ==
	__cmp_not_equal,// !=
	__cmp_less,__cmp_less_or_equal,// < <=
	__cmp_more,__cmp_more_or_equal,// > >=
	__and_operator,__or_operator,__nor_operator,// and or !
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
	__id,__identifier,__identifiers,__identifier_end,__lacked_identifier,
	__parameter,__parameters,__parameter_end,
	__number,__string,
	__scalar,__scalars,__scalar_end,
	__list,__hash,
	__hash_member,__hash_members,__hash_member_end,__hash_suffix,
	__statement,__statements,
	__function,//function(){}
	__definition,
	__assignment,__pre_assignment,
	__calculation,
	__loop,__continue,__break,__for,__forindex,__foreach,__while,// for()while() continue; break;
	__choose,__if,__elsif,__else,// if else if else
	__return
};
void print_token_type(int type)
{
	std::string context="";
	switch(type)
	{
		case __stack_end:
			context="__stack_end";
			break;
		case __equal:
			context="=";
			break;
		case __cmp_equal:
			context="==";
			break;
		case __cmp_not_equal:
			context="!=";
			break;
		case __cmp_less:
			context="<";
			break;
		case __cmp_less_or_equal:
			context="<=";
			break;
		case __cmp_more:
			context=">";
			break;
		case __cmp_more_or_equal:
			context=">=";
			break;
		case __and_operator:
			context="and";
			break;
		case __or_operator:
			context="or";
			break;
		case __nor_operator:
			context="!";
			break;
		case __add_operator:
			context="+";
			break;
		case __sub_operator:
			context="-";
			break;
		case __mul_operator:
			context="*";
			break;
		case __div_operator:
			context="/";
			break;
		case __link_operator:
			context="~";
			break;
		case __add_equal:
			context="+=";
			break;
		case __sub_equal:
			context="-=";
			break;
		case __mul_equal:
			context="*=";
			break;
		case __div_equal:
			context="/=";
			break;
		case __link_equal:
			context="~=";
			break;
		case __left_brace:
			context="{";
			break;
		case __right_brace:
			context="}";
			break;
		case __left_bracket:
			context="[";
			break;
		case __right_bracket:
			context="]";
			break;
		case __left_curve:
			context="(";
			break;
		case __right_curve:
			context=")";
			break;
		case __semi:
			context=";";
			break;
		case __comma:
			context=",";
			break;
		case __colon:
			context=":";
			break;
		case __dot:
			context=".";
			break;
		case __var:
			context="var";
			break;
		case __func:
			context="func";
			break;
		case __id:
			context="id";
			break;
		case __identifier:
			context="identifier";
			break;
		case __identifiers:
			context="identifiers";
			break;
		case __scalar:
			context="scalar";
			break;
		case __scalars:
			context="scalars";
			break;
		case __list:
			context="list";
			break;
		case __hash:
			context="hash";
			break;
		case __hash_member:
			context="hash_member";
			break;
		case __hash_members:
			context="hash_members";
			break;
		case __continue:
			context="continue";
			break;
		case __break:
			context="break";
			break;
		case __for:
			context="for";
			break;
		case __forindex:
			context="forindex";
			break;
		case __foreach:
			context="foreach";
			break;
		case __while:
			context="while";
			break;
		case __if:
			context="if";
			break;
		case __elsif:
			context="elsif";
			break;
		case __else:
			context="else";
			break;
		case __return:
			context="return";
			break;
		case __calculation:
			context="calculation";
			break;
		case __function:
			context="function_module";
			break;
		case __definition:
			context="definition";
			break;
		case __number:
			context="number";
			break;
		case __string:
			context="string";
			break;
	}
	std::cout<<context;
	return;
}

const int max_token_len=20;
struct cmp_seq
{
	int tokens[max_token_len];
	int res;
}par[]=
{
	{{__var,__id,__equal,__string,__semi},__definition},
	{{__number},                              __scalar},
	{{__var,__id,__equal,__scalar,__semi},__definition},
	{{__var,__id,__equal,__id,__semi},    __definition}
};
int num_of_par=sizeof(par)/sizeof(cmp_seq);


struct parse_unit
{
	int line;
	int type;
};
class PDA
{
	private:
		std::stack<parse_unit> main_stack;
		std::stack<parse_unit> error_stack;
	public:
		void stack_input(std::stack<parse_unit>& temp)
		{
			while(!temp.empty())
			{
				main_stack.push(temp.top());
				temp.pop();
			}
			return;
		}
		void print_error()
		{
			if(!error_stack.empty())
			{
				std::stack<parse_unit> temp;
				while(!error_stack.empty())
				{
					temp.push(error_stack.top());
					error_stack.pop();
				}
				std::cout<<">>[Error] Parse error:";
				int now_line=0;
				while(!temp.empty())
				{
					if(temp.top().line!=now_line)
					{
						now_line=temp.top().line;
						std::cout<<std::endl<<"line "<<now_line<<" ";
					}
					print_token_type(temp.top().type);
					std::cout<<" ";
					temp.pop();
				}
				std::cout<<std::endl;
			}
			else
				std::cout<<">>[Parse] 0 error occurred."<<std::endl;
			return;
		}
		bool extend_comp_progress(const int type)
		{
			std::stack<parse_unit> recognized_stack;
			std::stack<parse_unit> comp_stack;
			parse_unit temp;
			temp.line=0;
			temp.type=0;
			for(int i=0;i<num_of_par;++i)
			{
				if(par[i].res==type)
				{
					while(!comp_stack.empty())
						comp_stack.pop();
					for(int j=max_token_len-1;j>=0;--j)
					{
						if(par[i].tokens[j])
						{
							temp.type=par[i].tokens[j];
							comp_stack.push(temp);
						}
					}
					while((!comp_stack.empty()) && (!main_stack.empty()))
					{
						if(comp_stack.top().type==main_stack.top().type)
						{
							comp_stack.pop();
							recognized_stack.push(main_stack.top());
							main_stack.pop();
						}
						else if(comp_stack.top().type==__null_end)
							comp_stack.pop();
						else if(comp_stack.top().type!=main_stack.top().type)
						{
							if(!extend_comp_progress(comp_stack.top().type))
							{
								while(!recognized_stack.empty())
								{
									main_stack.push(recognized_stack.top());
									recognized_stack.pop();
								}
								break;
							}
							else
								comp_stack.pop();
						}
					}
					if(comp_stack.empty())
					{
						while(!recognized_stack.empty())
							recognized_stack.pop();
						return true;
					}
				}
			}
			return false;
		}
		void main_comp_progress()
		{
			std::stack<parse_unit> recognized_stack;
			std::stack<parse_unit> comp_stack;
			parse_unit temp;
			temp.line=0;
			temp.type=0;
			
			while(!main_stack.empty())
			{
				bool ispar=false;
				for(int i=0;i<num_of_par;++i)
				{
					while(!comp_stack.empty())
						comp_stack.pop();
					for(int j=max_token_len-1;j>=0;--j)
					{
						if(par[i].tokens[j])
						{
							temp.type=par[i].tokens[j];
							comp_stack.push(temp);
						}
					}
					while((!comp_stack.empty()) && (!main_stack.empty()))
					{
						if(comp_stack.top().type==main_stack.top().type)
						{
							comp_stack.pop();
							recognized_stack.push(main_stack.top());
							main_stack.pop();
						}
						else if(comp_stack.top().type==__null_end)
							comp_stack.pop();
						else if(comp_stack.top().type!=main_stack.top().type)
						{
							if(!extend_comp_progress(comp_stack.top().type))
							{
								while(!recognized_stack.empty())
								{
									main_stack.push(recognized_stack.top());
									recognized_stack.pop();
								}
								break;
							}
							else
								comp_stack.pop();
						}
					}
					if(comp_stack.empty())
					{
						while(!recognized_stack.empty())
							recognized_stack.pop();
						ispar=true;
						break;
					}
				}
				if(!ispar && !main_stack.empty())
				{
					error_stack.push(main_stack.top());
					main_stack.pop();
				}
			}
			print_error();
			return;
		}
};

class nasal_parser
{
	private:
		std::stack<parse_unit> parser;
	public:
		void parse_quiet_process(std::list<token>& lexer)
		{
			while(!parser.empty())
				parser.pop();
			for(std::list<token>::iterator i=lexer.begin();i!=lexer.end();++i)
			{
				parse_unit temp_parse;
				temp_parse.line=(*i).line;
				if(((*i).content=="var") || ((*i).content=="func") || ((*i).content=="return") || ((*i).content=="nil") || ((*i).content=="continue") || ((*i).content=="break") || ((*i).content=="and") || ((*i).content=="or"))
				{
					if((*i).content=="var")
						temp_parse.type=__var;
					else if((*i).content=="func")
						temp_parse.type=__func;
					else if((*i).content=="return")
						temp_parse.type=__return;
					else if((*i).content=="nil")
						temp_parse.type=__scalar;
					else if((*i).content=="continue")
						temp_parse.type=__continue;
					else if((*i).content=="break")
						temp_parse.type=__break;
					else if((*i).content=="and")
						temp_parse.type=__and_operator;
					else if((*i).content=="or")
						temp_parse.type=__or_operator;
				}
				else if((*i).type==IDENTIFIER)
				{
					temp_parse.type=__id;
				}
				else if(((*i).content=="for") || ((*i).content=="foreach") || ((*i).content=="while") || ((*i).content=="forindex"))
				{
					if((*i).content=="for")
						temp_parse.type=__for;
					else if((*i).content=="forindex")
						temp_parse.type=__forindex;
					else if((*i).content=="foreach")
						temp_parse.type=__foreach;
					else if((*i).content=="while")
						temp_parse.type=__while;
				}
				else if(((*i).content=="if") || ((*i).content=="else") || ((*i).content=="elsif"))
				{
					if((*i).content=="if")
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
					if((*i).content==";")
						temp_parse.type=__semi;
					else if((*i).content==",")
						temp_parse.type=__comma;
					else if((*i).content=="=")
						temp_parse.type=__equal;
					else if((*i).content==":")
						temp_parse.type=__colon;
					else if((*i).content==".")
						temp_parse.type=__dot;
				}
				else if(((*i).type==NUMBER) || ((*i).type==STRING))
				{
					if((*i).type==NUMBER)
						temp_parse.type=__number;
					else
						temp_parse.type=__string;
				}
				else if(((*i).content=="+") || ((*i).content=="-") || ((*i).content=="*") || ((*i).content=="/") || ((*i).content=="~") || ((*i).content=="!"))
				{
					if((*i).content=="+")
						temp_parse.type=__add_operator;
					else if((*i).content=="-")
						temp_parse.type=__sub_operator;
					else if((*i).content=="*")
						temp_parse.type=__mul_operator;
					else if((*i).content=="/")
						temp_parse.type=__div_operator;
					else if((*i).content=="~")
						temp_parse.type=__link_operator;
					else if((*i).content=="!")
						temp_parse.type=__nor_operator;
				}
				else if(((*i).content=="+=") || ((*i).content=="-=") || ((*i).content=="*=") || ((*i).content=="/=") || ((*i).content=="~="))
				{
					if((*i).content=="+=")
						temp_parse.type=__add_equal;
					else if((*i).content=="-=")
						temp_parse.type=__sub_equal;
					else if((*i).content=="*=")
						temp_parse.type=__mul_equal;
					else if((*i).content=="/=")
						temp_parse.type=__div_equal;
					else if((*i).content=="~=")
						temp_parse.type=__link_equal;
				}
				else if(((*i).content=="(") || ((*i).content==")") || ((*i).content=="[") || ((*i).content=="]") || ((*i).content=="{") || ((*i).content=="}"))
				{
					char c=(*i).content[0];
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
				parser.push(temp_parse);//push this into stack
			}
			PDA automata;
			automata.stack_input(parser);
			automata.main_comp_progress();
			return;
		}
};

#endif
