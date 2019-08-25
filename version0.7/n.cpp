#include "nasal_lexer.h"
#include <stack>

enum token_type
{
	__stack_end,
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
	__unknown_type_id,__identifier,__identifiers,
	__scalar,__scalars,__list,__hash,
	__hash_member,__hash_members,
	__statement,__statements,
	__function,//function(){}
	__definition,__assignment,__calculation,
	__loop,__continue,__break,__for,__forindex,__foreach,__while,// for()while() continue; break;
	__choose,__if,__elsif,__else,// if else if else
	__return
};

struct token_seq
{
	int tokens[15];
	int res;
}par[13]=
{
	{{__var,__identifier,__equal,__scalar,__semi},                                __definition},
	{{__var,__identifier,__equal,__identifier,__semi},                            __definition},
	{{__var,__identifier,__equal,__list,__semi},                                  __definition},
	{{__var,__identifier,__equal,__hash,__semi},                                  __definition},
	{{__var,__identifier,__equal,__function},                                     __definition},
	{{__var,__left_curve,__identifiers,__right_curve,__equal,__identifier,__semi},__definition},
	{{__var,__left_curve,__identifiers,__right_curve,__equal,__list,__semi},      __definition},
	{{__identifier,__dot,__identifier},                                           __identifier},
	{{__identifier,__left_bracket,__scalar,__right_bracket},                      __identifier},
	{{__identifier,__left_bracket,__identifier,__right_bracket},                  __identifier},
	{{__identifier,__left_bracket,__calculation,__right_bracket},                 __identifier},
	{{__identifier,__left_curve,__right_curve},                                   __identifier},
	{{__identifier,__left_curve,__scalar,__right_curve},                          __identifier}
};

struct par_info
{
	int res;
	int len;
};
par_info isPar(int *t)
{
	par_info temp;
	temp.len=0;
	temp.res=0;
	for(int i=0;i<13;++i)
	{
		int cnt=0;
		for(int j=0;j<15;++j)
		{
			if(par[i].tokens[j])
				++cnt;
			else
				break;
		}
		for(int j=0;j<cnt;++j)
		{
			if(par[i].tokens[j]!=t[15-cnt+j])
				break;
			if((par[i].tokens[j]==t[15-cnt+j]) && (j==cnt-1))
			{
				temp.len=cnt;
				temp.res=par[i].res;
				return temp;
			}
		}
	}
	return temp;
}


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
		case __unknown_type_id:
			context="unknown_id";
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
	}
	std::cout<<context;
	return;
}

struct parse_unit
{
	int line;
	int type;
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
			for(int i=0;i<15;++i)
			{
				parse_unit t;
				t.line=0;
				t.type=__stack_end;
				parser.push(t);
			}
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
					temp_parse.type=__identifier;
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
					temp_parse.type=__scalar;
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
			while(!parser.empty())
			{
				int tbl[15]={0};
				std::stack<parse_unit> temp;
				for(int i=0;i<15;++i)
				{
					if(!parser.empty())
					{
						temp.push(parser.top());
						parser.pop();
					}
					else
						break;
				}
				for(int i=0;i<15;++i)
				{
					if(!temp.empty())
					{
						tbl[i]=temp.top().type;
						parser.push(temp.top());
						temp.pop();
					}
					else
						break;
				}
				for(int i=0;i<15;++i)
				{
					if(tbl[i])
						break;
					if(!tbl[i] && i==14)
						return;
				}
				par_info tk=isPar(tbl);
				if(tk.res)
				{
					parse_unit temp;
					temp.line=parser.top().line;
					temp.type=tk.res;
					for(int i=0;i<tk.len;++i)
						parser.pop();
					parser.push(temp);
				}
				else
					break;
			}
			std::cout<<">>[Parser] Complete scanning."<<std::endl;
			return;
		}
		void print_error()
		{
			std::stack<parse_unit> temp_stack;
			while(!parser.empty())
			{
				if((parser.top().type!=__stack_end) && (parser.top().type!=__statement) && (parser.top().type!=__statements) && (parser.top().type!=__function) && (parser.top().type!=__definition) && (parser.top().type!=__assignment) && (parser.top().type!=__loop) && (parser.top().type!=__choose))
					temp_stack.push(parser.top());
				parser.pop();
			}
			if(!temp_stack.empty())
			{
				std::cout<<">>[Error] Parse error."<<std::endl;
				while(!temp_stack.empty())
				{
					int l=temp_stack.top().line;
					std::cout<<"line "<<l<<": ";
					while(l==temp_stack.top().line)
					{
						print_token_type(temp_stack.top().type);
						std::cout<<" ";
						temp_stack.pop();
						if(temp_stack.empty())
							break;
					}
					std::cout<<std::endl;
				}
			}
			else
				std::cout<<">>[Parser] No error occurred."<<std::endl;
			return;
		}
};

int main()
{
	nasal_lexer lex;
	nasal_parser pas;
	resource_programme_process prog;
	std::string str="a.txt";
	prog.input_file(str);
	lex.lexer_process(prog.use_file());
	pas.parse_quiet_process(lex.return_list());
	pas.print_error();
	return 0;
}
