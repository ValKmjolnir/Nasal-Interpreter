#ifndef __NASAL_PARSER_H__
#define __NASAL_PARSER_H__

#include <stack>

enum token_type
{
	__stack_end=1,
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
	__var,
	__func,
	__id,
	__return,
	__if,__elsif,__else,
	__continue,__break,__for,__forindex,__foreach,__while,
	
	__scalar,
	__number,__string,__char,
	__list,
	__hash,
	__hash_member,
	__identifier,
	__statement,
	__function,
	__call_function,
	__definition,
	__assignment,
	__calculation,
	__loop,
	__choose,
	__program
};

const int max_len=10;
struct cmp_seq
{
	int tokens[max_len];
	int res;
};
// must put in a reverse way
cmp_seq par[]=
{
	{{__program,__statement},                                                __program},
	
	{{__calculation,__add_operator,__calculation},                       __calculation},
	{{__calculation,__add_operator,__id},                                __calculation},
	{{__id,__add_operator,__calculation},                                __calculation},
	{{__id,__add_operator,__id},                                         __calculation},
	
	{{__calculation,__sub_operator,__calculation},                       __calculation},
	{{__calculation,__sub_operator,__id},                                __calculation},
	{{__id,__sub_operator,__calculation},                                __calculation},
	{{__id,__sub_operator,__id},                                         __calculation},
	
	{{__calculation,__mul_operator,__calculation},                       __calculation},
	{{__calculation,__mul_operator,__id},                                __calculation},
	{{__id,__mul_operator,__calculation},                                __calculation},
	{{__id,__mul_operator,__id},                                         __calculation},
	
	{{__calculation,__div_operator,__calculation},                       __calculation},
	{{__calculation,__div_operator,__id},                                __calculation},
	{{__id,__div_operator,__calculation},                                __calculation},
	{{__id,__div_operator,__id},                                         __calculation},
	
	{{__calculation,__link_operator,__calculation},                      __calculation},
	{{__calculation,__link_operator,__id},                               __calculation},
	{{__id,__link_operator,__calculation},                               __calculation},
	{{__id,__link_operator,__id},                                        __calculation},
	
	{{__right_curve,__id,__left_curve},                                  __calculation},
	{{__right_curve,__calculation,__left_curve},                         __calculation},
	
	{{__semi,__calculation,__equal,__id,__var},                           __definition},
	{{__semi,__number,__equal,__id,__var},                                __definition},
	{{__semi,__string,__equal,__id,__var},                                __definition},
	{{__semi,__id,__equal,__id,__var},                                    __definition},
	{{__semi,__char,__equal,__id,__var},                                  __definition},
	{{__semi,__right_brace,__left_brace,__equal,__id,__var},              __definition},
	{{__semi,__right_bracket,__left_bracket,__equal,__id,__var},          __definition},
	
	{{__semi,__calculation,__add_equal,__id},                             __assignment},
	{{__semi,__number,__add_equal,__id},                                  __assignment},
	{{__semi,__string,__add_equal,__id},                                  __assignment},
	{{__semi,__id,__add_equal,__id},                                      __assignment},
	{{__semi,__char,__add_equal,__id},                                    __assignment},
	
	{{__semi,__calculation,__sub_equal,__id},                             __assignment},
	{{__semi,__number,__sub_equal,__id},                                  __assignment},
	{{__semi,__string,__sub_equal,__id},                                  __assignment},
	{{__semi,__id,__sub_equal,__id},                                      __assignment},
	{{__semi,__char,__sub_equal,__id},                                    __assignment},
	
	{{__semi,__calculation,__mul_equal,__id},                             __assignment},
	{{__semi,__number,__mul_equal,__id},                                  __assignment},
	{{__semi,__string,__mul_equal,__id},                                  __assignment},
	{{__semi,__id,__mul_equal,__id},                                      __assignment},
	{{__semi,__char,__mul_equal,__id},                                    __assignment},
	
	{{__semi,__calculation,__div_equal,__id},                             __assignment},
	{{__semi,__number,__div_equal,__id},                                  __assignment},
	{{__semi,__string,__div_equal,__id},                                  __assignment},
	{{__semi,__id,__div_equal,__id},                                      __assignment},
	{{__semi,__char,__div_equal,__id},                                    __assignment},
	
	{{__semi,__calculation,__link_equal,__id},                            __assignment},
	{{__semi,__number,__link_equal,__id},                                 __assignment},
	{{__semi,__string,__link_equal,__id},                                 __assignment},
	{{__semi,__id,__link_equal,__id},                                     __assignment},
	{{__semi,__char,__link_equal,__id},                                   __assignment},
	
	{{__definition},                                                       __statement},
	{{__assignment},                                                       __statement},
	{{__statement,__statement},                                            __statement}
};
int num_of_par=sizeof(par)/sizeof(cmp_seq);

void print_token(int type)
{
	std::string context="";
	switch(type)
	{
		case __stack_end:
			context="#";
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
		case __number:
			context="number";
			break;
		case __string:
			context="string";
			break;
		case __char:
			context="char";
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
		case __scalar:
			context="scalar";
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
		case __identifier:
			context="identifier";
			break;
		case __statement:
			context="statement";
			break;
		case __function:
			context="function";
			break;
		case __call_function:
			context="call_func";
			break;
		case __definition:
			context="definition";
			break;
		case __assignment:
			context="assignment";
			break;
		case __calculation:
			context="calc";
			break;
		case __loop:
			context="loop";
			break;
		case __choose:
			context="choose";
			break;
		case __program:
			context="program";
			break;
	}
	std::cout<<context;
	return;
}

struct parse_unit
{
	int line;
	int type;
};
class PDA
{
	private:
		std::stack<int> main_stack;
		std::stack<int> comp_stack;
	public:
		void set_stack_empty()
		{
			while(!main_stack.empty())
				main_stack.pop();
			while(!comp_stack.empty())
				comp_stack.pop();
			main_stack.push(__stack_end);
			comp_stack.push(__stack_end);
			comp_stack.push(__program);
		}
		void stack_input(std::stack<parse_unit>& temp)
		{
			set_stack_empty();
			while(!temp.empty())
			{
				main_stack.push(temp.top().type);
				temp.pop();
			}
			return;
		}
		void print_stack(std::stack<int>& temp)
		{
			std::stack<int> t;
			while(!temp.empty())
			{
				t.push(temp.top());
				print_token(t.top());
				std::cout<<" ";
				temp.pop();
			}
			while(!t.empty())
			{
				temp.push(t.top());
				t.pop();
			}
			return;
		}
		void print_main_and_comp()
		{
			std::cout<<">>[Main stack] ";
			print_stack(main_stack);
			std::cout<<std::endl<<">>[Comp stack] ";
			print_stack(comp_stack);
			std::cout<<std::endl;
			return;
		}
		bool reducable()
		{
			std::stack<int> temp;
			for(int i=0;i<num_of_par;++i)
			{
				for(int j=max_len-1;j>=0;--j)
				{
					if(!par[i].tokens[j])
						continue;
					// if par[i].tokens[j] is 0 then continue until the tokens[j] is not 0
					temp.push(comp_stack.top());
					comp_stack.pop();
					if((par[i].tokens[j]!=temp.top()) || (comp_stack.empty() && par[i].tokens[j]))
					{
						while(!temp.empty())
						{
							comp_stack.push(temp.top());
							temp.pop();
						}
						break;
					}
					if((par[i].tokens[j]==temp.top()) && (j==0))
					{
						comp_stack.push(par[i].res);
						return true;
					}
				}
			}
			return false;
		}
		void print_error()
		{
			while(!comp_stack.empty())
			{
				if((comp_stack.top()!=__statement) && (comp_stack.top()!=__program) && (comp_stack.top()!=__stack_end))
				{
					print_token(comp_stack.top());
					std::cout<<" ";
				}
				comp_stack.pop();
			}
			std::cout<<std::endl;
			return;
		}
		void main_comp_progress(bool show)
		{
			if(show)
				print_main_and_comp();
			while(main_stack.top()!=__stack_end)
			{
				comp_stack.push(main_stack.top());
				main_stack.pop();
				if(show)
					print_main_and_comp();
				while(1)
				{
					if(!reducable())
						break;
					if(show)
						print_main_and_comp();
				}
			}
			if(comp_stack.top()==__program)
			{
				comp_stack.pop();
				if(comp_stack.top()==__stack_end)
					std::cout<<">>[Parse] 0 error(s)."<<std::endl;
				else
				{
					std::cout<<">>[Parse] Error:"<<std::endl;
					print_error();
				}
			}
			else
			{
				std::cout<<">>[Parse] Error:"<<std::endl;
				print_error();
			}
			std::cout<<">>[Parse] Complete checking."<<std::endl;
			return;
		}
};

class nasal_parser
{
	private:
		std::stack<parse_unit> parser;
	public:
		void parse_process(std::list<token>& lexer,bool show)
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
						temp_parse.type=__number;
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
				else if(((*i).type==NUMBER) || ((*i).type==STRING) || ((*i).type==CHAR))
				{
					if((*i).type==NUMBER)
						temp_parse.type=__number;
					else if((*i).type==STRING)
						temp_parse.type=__string;
					else if((*i).type==CHAR)
						temp_parse.type=__char;
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
			if(parser.empty())
			{
				std::cout<<">>[Warning] Empty lexer."<<std::endl;
				std::cout<<">>[Parse] Complete checking."<<std::endl;
				return;
			}
			std::stack<parse_unit> temp;
			while(!parser.empty())
			{
				temp.push(parser.top());
				parser.pop();
			}
			PDA automata;
			automata.stack_input(temp);
			automata.main_comp_progress(show);
			return;
		}
};

#endif
