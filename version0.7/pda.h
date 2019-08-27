#ifndef __PDA_H__
#define __PDA_H__

#include <stack>

struct parse_unit
{
	int line;
	int type;
};

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

const int max_token_len=20;
struct cmp_seq
{
	int tokens[max_token_len];
	int res;
}par[]=
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
int num_of_par=sizeof(par)/sizeof(cmp_seq);

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
