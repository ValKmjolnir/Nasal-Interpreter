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

struct cmp_seq
{
	int tokens[20];
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

class pda
{
	private:
		std::stack<parse_unit> main_stack;
		std::stack<parse_unit> cmp_stack;
		std::stack<parse_unit> recog_stack;
	public:
		
};
