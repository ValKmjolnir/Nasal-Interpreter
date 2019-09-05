#ifndef __NASAL_PARSER_H__
#define __NASAL_PARSER_H__

#include <stack>
#include <cstring>

enum token_type
{
	__stack_end=1,
	__equal,// =
	__cmp_equal,__cmp_not_equal,// == !=
	__cmp_less,__cmp_less_or_equal,// < <=
	__cmp_more,__cmp_more_or_equal,// > >=
	__and_operator,__or_operator,__nor_operator,// and or !
	__add_operator,__sub_operator,// + -
	__mul_operator,__div_operator,__link_operator,// * / ~
	__add_equal,__sub_equal,// += -=
	__mul_equal,__div_equal,__link_equal,// *= /= ~=
	__left_brace,__right_brace,// {}
	__left_bracket,__right_bracket,// []
	__left_curve,__right_curve,// ()
	__semi,__comma,__colon,__dot,// ; , : .

	__var,__func,__return,
	__if,__elsif,__else,
	__id,__dynamic_id,
	__continue,__break,
	__for,__forindex,__foreach,__while,
	//end of operators & reserve words
	__two_operator,
	__scalar,__data_list,
	__number,__string,
	__list,
	__hash,
	__hash_member,__hash_member_list,
	__identifier,
	__statement,__statements,
	__function,
	__call_function,
	__call_list,
	__call_hash,
	__call,
	__definition,__return_scalar,
	__assignment,__pre_assignment,__assignment_operator,
	__calculation,
	__loop,
	__choose,__if_choose,__elsif_choose,__else_choose,
	__program
};

const int max_len=15;
struct cmp_seq
{
	int tokens[max_len];
	int res;
};
// must put in a reverse way
cmp_seq par[]=
{
	{{__program,__statement},   __program},
	{{__add_operator},__two_operator},
	{{__sub_operator},__two_operator},
	{{__mul_operator},__two_operator},
	{{__div_operator},__two_operator},
	{{__link_operator},__two_operator},
	{{__and_operator},__two_operator},
	{{__or_operator},__two_operator},
	{{__cmp_equal},__two_operator},
	{{__cmp_not_equal},__two_operator},
	{{__cmp_less},__two_operator},
	{{__cmp_less_or_equal},__two_operator},
	{{__cmp_more},__two_operator},
	{{__cmp_more_or_equal},__two_operator},
	{{__add_equal},__assignment_operator},
	{{__sub_equal},__assignment_operator},
	{{__mul_equal},__assignment_operator},
	{{__div_equal},__assignment_operator},
	{{__link_equal},__assignment_operator},

	{{__number},__scalar},
	{{__string},__scalar},
	{{__calculation},__scalar},

	{{__call_list},__call},
	{{__call_hash},__call},
	{{__call_function},__call},

	{{__id,__comma,__id},__data_list},
	{{__id,__comma,__scalar},__data_list},
	{{__id,__comma,__call},__data_list},
	{{__id,__comma,__function},__data_list},
	{{__id,__comma,__list},__data_list},
	{{__id,__comma,__hash},__data_list},
	{{__scalar,__comma,__id},__data_list},
	{{__scalar,__comma,__scalar},__data_list},
	{{__scalar,__comma,__call},__data_list},
	{{__scalar,__comma,__function},__data_list},
	{{__scalar,__comma,__list},__data_list},
	{{__scalar,__comma,__hash},__data_list},
	{{__call,__comma,__id},__data_list},
	{{__call,__comma,__scalar},__data_list},
	{{__call,__comma,__call},__data_list},
	{{__call,__comma,__function},__data_list},
	{{__call,__comma,__list},__data_list},
	{{__call,__comma,__hash},__data_list},
	{{__function,__comma,__id},__data_list},
	{{__function,__comma,__scalar},__data_list},
	{{__function,__comma,__call},__data_list},
	{{__function,__comma,__function},__data_list},
	{{__function,__comma,__list},__data_list},
	{{__function,__comma,__hash},__data_list},
	{{__list,__comma,__id},__data_list},
	{{__list,__comma,__scalar},__data_list},
	{{__list,__comma,__call},__data_list},
	{{__list,__comma,__function},__data_list},
	{{__list,__comma,__list},__data_list},
	{{__list,__comma,__hash},__data_list},
	{{__hash,__comma,__id},__data_list},
	{{__hash,__comma,__scalar},__data_list},
	{{__hash,__comma,__call},__data_list},
	{{__hash,__comma,__function},__data_list},
	{{__hash,__comma,__list},__data_list},
	{{__hash,__comma,__hash},__data_list},
	{{__data_list,__comma,__id},__data_list},
	{{__data_list,__comma,__scalar},__data_list},
	{{__data_list,__comma,__call},__data_list},
	{{__data_list,__comma,__function},__data_list},
	{{__data_list,__comma,__list},__data_list},
	{{__data_list,__comma,__hash},__data_list},

	{{__id,__colon,__id},__hash_member},
	{{__scalar,__colon,__id},__hash_member},
	{{__call,__colon,__id},__hash_member},
	{{__function,__colon,__id},__hash_member},

	{{__hash_member,__comma,__hash_member},__hash_member_list},
	{{__hash_member_list,__comma,__hash_member},__hash_member_list},

	{{__right_bracket,__id,__left_bracket},__list},
	{{__right_bracket,__scalar,__left_bracket},__list},
	{{__right_bracket,__call,__left_bracket},__list},
	{{__right_bracket,__function,__left_bracket},__list},
	{{__right_bracket,__data_list,__left_bracket},__list},

	{{__right_brace,__hash_member,__left_brace},__hash},
	{{__right_brace,__hash_member_list,__left_brace},__hash},

	{{__list,__id},__call_list},
	{{__list,__call},__call_list},
	{{__right_bracket,__hash_member,__left_bracket,__id},__call_list},
	{{__right_bracket,__hash_member,__left_bracket,__call},__call_list},
	{{__right_bracket,__colon,__id,__left_bracket,__id},__call_list},
	{{__right_bracket,__colon,__scalar,__left_bracket,__id},__call_list},
	{{__right_bracket,__colon,__call,__left_bracket,__id},__call_list},
	{{__right_bracket,__colon,__id,__left_bracket,__call},__call_list},
	{{__right_bracket,__colon,__scalar,__left_bracket,__call},__call_list},
	{{__right_bracket,__colon,__call,__left_bracket,__call},__call_list},

	{{__id,__dot,__id},__call_hash},
	{{__id,__dot,__call},__call_hash},
	{{__scalar,__dot,__id},__call_hash},
	{{__scalar,__dot,__call},__call_hash},
	{{__call,__dot,__id},__call_hash},
	{{__call,__dot,__call},__call_hash},

	{{__right_curve,__left_curve,__id},__call_function},
	{{__right_curve,__id,__left_curve,__id},__call_function},
	{{__right_curve,__scalar,__left_curve,__id},__call_function},
	{{__right_curve,__call,__left_curve,__id},__call_function},
	{{__right_curve,__list,__left_curve,__id},__call_function},
	{{__right_curve,__hash,__left_curve,__id},__call_function},
	{{__right_curve,__function,__left_curve,__id},__call_function},
	{{__right_curve,__data_list,__left_curve,__id},__call_function},

	{{__id,__two_operator,__id},__calculation},
	{{__id,__two_operator,__scalar},__calculation},
	{{__id,__two_operator,__call},__calculation},
	{{__scalar,__two_operator,__id},__calculation},
	{{__scalar,__two_operator,__scalar},__calculation},
	{{__scalar,__two_operator,__call},__calculation},
	{{__call,__two_operator,__id},__calculation},
	{{__call,__two_operator,__scalar},__calculation},
	{{__call,__two_operator,__call},__calculation},
	{{__id,__nor_operator},__calculation},
	{{__scalar,__nor_operator},__calculation},
	{{__call,__nor_operator},__calculation},
	{{__right_curve,__id,__left_curve},__calculation},
	{{__right_curve,__scalar,__left_curve},__calculation},
	{{__right_curve,__call,__left_curve},__calculation},
	
	{{__semi,__id,__return},__return_scalar},
	{{__semi,__scalar,__return},__return_scalar},
	{{__semi,__call,__return},__return_scalar},
	{{__semi,__function,__return},__return_scalar},
	{{__semi,__list,__return},__return_scalar},
	{{__semi,__hash,__return},__return_scalar},
	{{__semi,__return},__return_scalar},
	
	{{__right_brace,__left_brace,__func},__function},
	{{__right_brace,__statement,__left_brace,__func},__function},
	
	{{__right_brace,__left_brace,__right_curve,__left_curve,__func},__function},
	{{__right_brace,__statement,__left_brace,__right_curve,__left_curve,__func},__function},
	
	{{__right_brace,__left_brace,__right_curve,__id,__left_curve,__func},__function},
	{{__right_brace,__statement,__left_brace,__right_curve,__id,__left_curve,__func},__function},
	{{__right_brace,__left_brace,__right_curve,__scalar,__left_curve,__func},__function},
	{{__right_brace,__statement,__left_brace,__right_curve,__scalar,__left_curve,__func},__function},
	{{__right_brace,__left_brace,__right_curve,__call,__left_curve,__func},__function},
	{{__right_brace,__statement,__left_brace,__right_curve,__call,__left_curve,__func},__function},
	{{__right_brace,__left_brace,__right_curve,__list,__left_curve,__func},__function},
	{{__right_brace,__statement,__left_brace,__right_curve,__list,__left_curve,__func},__function},
	{{__right_brace,__left_brace,__right_curve,__hash,__left_curve,__func},__function},
	{{__right_brace,__statement,__left_brace,__right_curve,__hash,__left_curve,__func},__function},
	{{__right_brace,__left_brace,__right_curve,__function,__left_curve,__func},__function},
	{{__right_brace,__statement,__left_brace,__right_curve,__function,__left_curve,__func},__function},
	{{__right_brace,__left_brace,__right_curve,__dynamic_id,__left_curve,__func},__function},
	{{__right_brace,__statement,__left_brace,__right_curve,__dynamic_id,__left_curve,__func},__function},
	{{__right_brace,__left_brace,__right_curve,__data_list,__left_curve,__func},__function},
	{{__right_brace,__statement,__left_brace,__right_curve,__data_list,__left_curve,__func},__function},
	
	{{__right_brace,__left_brace,__right_curve,__id,__left_curve,__if},__choose},
	{{__right_brace,__left_brace,__right_curve,__scalar,__left_curve,__if},__choose},
	{{__right_brace,__left_brace,__right_curve,__call,__left_curve,__if},__choose},
	{{__right_brace,__statement,__left_brace,__right_curve,__id,__left_curve,__if},__choose},
	{{__right_brace,__statement,__left_brace,__right_curve,__scalar,__left_curve,__if},__choose},
	{{__right_brace,__statement,__left_brace,__right_curve,__call,__left_curve,__if},__choose},
	{{__right_brace,__left_brace,__right_curve,__id,__left_curve,__if,__else},__choose},
	{{__right_brace,__left_brace,__right_curve,__scalar,__left_curve,__if,__else},__choose},
	{{__right_brace,__left_brace,__right_curve,__call,__left_curve,__if,__else},__choose},
	{{__right_brace,__statement,__left_brace,__right_curve,__id,__left_curve,__if,__else},__choose},
	{{__right_brace,__statement,__left_brace,__right_curve,__scalar,__left_curve,__if,__else},__choose},
	{{__right_brace,__statement,__left_brace,__right_curve,__call,__left_curve,__if,__else},__choose},
	{{__right_brace,__left_brace,__right_curve,__id,__left_curve,__elsif},__choose},
	{{__right_brace,__left_brace,__right_curve,__scalar,__left_curve,__elsif},__choose},
	{{__right_brace,__left_brace,__right_curve,__call,__left_curve,__elsif},__choose},
	{{__right_brace,__statement,__left_brace,__right_curve,__id,__left_curve,__elsif},__choose},
	{{__right_brace,__statement,__left_brace,__right_curve,__scalar,__left_curve,__elsif},__choose},
	{{__right_brace,__statement,__left_brace,__right_curve,__call,__left_curve,__elsif},__choose},
	{{__right_brace,__left_brace,__else},__choose},
	{{__right_brace,__statement,__left_brace,__else},__choose},
	
	{{__semi,__id,__equal,__id,__var},__definition},
	{{__semi,__right_bracket,__left_bracket,__equal,__id,__var},__definition},
	{{__semi,__right_brace,__left_brace,__equal,__id,__var},__definition},
	{{__semi,__scalar,__equal,__id,__var},__definition},
	{{__semi,__call,__equal,__id,__var},__definition},
	{{__semi,__function,__equal,__id,__var},__definition},
	{{__semi,__list,__equal,__id,__var},__definition},
	{{__semi,__hash,__equal,__id,__var},__definition},
	{{__semi,__id,__var},__definition},
	
	{{__id,__equal,__id},__pre_assignment},
	{{__scalar,__equal,__id},__pre_assignment},
	{{__call,__equal,__id},__pre_assignment},
	{{__function,__equal,__id},__pre_assignment},
	{{__list,__equal,__id},__pre_assignment},
	{{__hash,__equal,__id},__pre_assignment},
	{{__id,__equal,__call},__pre_assignment},
	{{__scalar,__equal,__call},__pre_assignment},
	{{__call,__equal,__call},__pre_assignment},
	{{__function,__equal,__call},__pre_assignment},
	{{__list,__equal,__call},__pre_assignment},
	{{__hash,__equal,__call},__pre_assignment},
	{{__id,__assignment_operator,__id},__pre_assignment},
	{{__scalar,__assignment_operator,__id},__pre_assignment},
	{{__call,__assignment_operator,__id},__pre_assignment},
	{{__id,__assignment_operator,__call},__pre_assignment},
	{{__scalar,__assignment_operator,__call},__pre_assignment},
	{{__call,__assignment_operator,__call},__pre_assignment},
	{{__semi,__pre_assignment},__assignment},
	
	{{__right_brace,__left_brace,__right_curve,__id,__left_curve,__while},__loop},
	{{__right_brace,__left_brace,__right_curve,__scalar,__left_curve,__while},__loop},
	{{__right_brace,__left_brace,__right_curve,__call,__left_curve,__while},__loop},
	{{__right_brace,__statement,__left_brace,__right_curve,__id,__left_curve,__while},__loop},
	{{__right_brace,__statement,__left_brace,__right_curve,__scalar,__left_curve,__while},__loop},
	{{__right_brace,__statement,__left_brace,__right_curve,__call,__left_curve,__while},__loop},
	{{__right_brace,__left_brace,__right_curve,__id,__statement,__left_curve,__forindex},__loop},
	{{__right_brace,__left_brace,__right_curve,__call,__statement,__left_curve,__forindex},__loop},
	{{__right_brace,__left_brace,__right_curve,__list,__statement,__left_curve,__forindex},__loop},
	{{__right_brace,__statement,__left_brace,__right_curve,__id,__statement,__left_curve,__forindex},__loop},
	{{__right_brace,__statement,__left_brace,__right_curve,__call,__statement,__left_curve,__forindex},__loop},
	{{__right_brace,__statement,__left_brace,__right_curve,__list,__statement,__left_curve,__forindex},__loop},
	{{__right_brace,__left_brace,__right_curve,__id,__statement,__left_curve,__foreach},__loop},
	{{__right_brace,__left_brace,__right_curve,__call,__statement,__left_curve,__foreach},__loop},
	{{__right_brace,__left_brace,__right_curve,__list,__statement,__left_curve,__foreach},__loop},
	{{__right_brace,__statement,__left_brace,__right_curve,__id,__statement,__left_curve,__foreach},__loop},
	{{__right_brace,__statement,__left_brace,__right_curve,__call,__statement,__left_curve,__foreach},__loop},
	{{__right_brace,__statement,__left_brace,__right_curve,__list,__statement,__left_curve,__foreach},__loop},
	{{__right_brace,__left_brace,__right_curve,__pre_assignment,__statement,__left_curve,__for},__loop},
	{{__right_brace,__statement,__left_brace,__right_curve,__pre_assignment,__statement,__left_curve,__for},__loop},
	
	{{__return_scalar},__statement},
	{{__semi,__continue},__statement},
	{{__semi,__break},__statement},
	
	{{__semi,__id,__semi},__semi},
	{{__semi,__call,__semi},__semi},
	{{__semi,__scalar,__semi},__semi},
	{{__semi,__list,__semi},__semi},
	{{__semi,__hash,__semi},__semi},
	{{__semi,__id,__right_brace},__right_brace},
	{{__semi,__call,__right_brace},__right_brace},
	{{__semi,__scalar,__right_brace},__right_brace},
	{{__semi,__list,__right_brace},__right_brace},
	{{__semi,__hash,__right_brace},__right_brace},
	{{__semi,__id,__left_brace},__left_brace},
	{{__semi,__call,__left_brace},__left_brace},
	{{__semi,__scalar,__left_brace},__left_brace},
	{{__semi,__list,__left_brace},__left_brace},
	{{__semi,__hash,__left_brace},__left_brace},
	{{__semi,__id,__left_curve},__left_curve},
	{{__semi,__call,__left_curve},__left_curve},
	{{__semi,__scalar,__left_curve},__left_curve},
	{{__semi,__list,__left_curve},__left_curve},
	{{__semi,__hash,__left_curve},__left_curve},
	
	{{__loop},__statement},
	{{__choose},__statement},
	{{__definition},__statement},
	{{__assignment},__statement},
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
		case __dynamic_id:
			context="id...";
			break;
		case __number:
			context="number";
			break;
		case __string:
			context="string";
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
		case __statements:
			context="statements";
			break;
		case __function:
			context="function";
			break;
		case __call_function:
			context="call_func";
			break;
		case __call_list:
			context="call_list";
			break;
		case __call_hash:
			context="call_hash";
			break;
		case __definition:
			context="definition;";
			break;
		case __assignment:
			context="assignment;";
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
		case __if_choose:
			context="if_choose";
			break;
		case __elsif_choose:
			context="else_if_choose";
			break;
		case __else_choose:
			context="else_choose";
			break;
		case __program:
			context="program";
			break;
		
		case __call:
			context="call";
			break;
		case __data_list:
			context="datas";
			break;
		case __hash_member_list:
			context="hash_members";
			break;
		case __pre_assignment:
			context="pre_assignment";
			break;
		case __two_operator:
			context="operator";
			break;
		case __assignment_operator:
			context="operator";
			break;
		case __return_scalar:
			context="return_scl;";
			break;
		default:
			context="unknown_type";
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
			main_stack.push(__var);
			main_stack.push(__id);
			main_stack.push(__semi);
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
					int t=comp_stack.top();
					if((t==__right_brace) || (t==__semi))
						std::cout<<std::endl;
				}
				comp_stack.pop();
			}
			std::cout<<std::endl;
			return;
		}
		void main_comp_progress(bool show) // show is used to print parser stack 
		{
			if(show)
				print_main_and_comp();
			while(main_stack.top()!=__stack_end)
			{
				comp_stack.push(main_stack.top());
				main_stack.pop();
				if((comp_stack.top()==__left_curve) && ((main_stack.top()==__id) || (main_stack.top()==__if) || (main_stack.top()==__elsif) || (main_stack.top()==__while) || (main_stack.top()==__func)))
				{
					comp_stack.push(main_stack.top());
					main_stack.pop();
				}
				else if((comp_stack.top()==__id) && (main_stack.top()==__var))
				{
					comp_stack.push(main_stack.top());
					main_stack.pop();
				}
				else if(((comp_stack.top()==__sub_operator) || (comp_stack.top()==__add_operator)) && (main_stack.top()!=__id) && (main_stack.top()!=__right_curve) && (main_stack.top()!=__right_bracket))
				{
					main_stack.push(__number);
				}
				if((comp_stack.top()==__if) && (main_stack.top()==__else))
				{
					comp_stack.push(main_stack.top());
					main_stack.pop();
				}
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
				else if((*i).type==DYNAMIC_ID)
				{
					temp_parse.type=__dynamic_id;
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
					else if((*i).type==STRING)
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
