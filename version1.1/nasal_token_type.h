#ifndef __NASAL_TOKEN_TYPE_H__
#define __NASAL_TOKEN_TYPE_H__

#include <cstring>

enum token_type
{
	__stack_end=1,
	__equal,                                       // =
	__cmp_equal,__cmp_not_equal,                   // == !=
	__cmp_less,__cmp_less_or_equal,                // < <=
	__cmp_more,__cmp_more_or_equal,                // > >=
	__and_operator,__or_operator,__nor_operator,   // and or !
	__add_operator,__sub_operator,                 // + -
	__mul_operator,__div_operator,__link_operator, // * / ~
	__add_equal,__sub_equal,                       // += -=
	__mul_equal,__div_equal,__link_equal,          // *= /= ~=
	__left_brace,__right_brace,                    // {}
	__left_bracket,__right_bracket,                // []
	__left_curve,__right_curve,                    // ()
	__semi,__comma,__colon,__dot,                  // ; , : .
	__unknown_operator,
	__var,__func,__return,
	__if,__elsif,__else,
	__continue,__break,
	__for,__forindex,__foreach,__while,
	//operators & reserve words
	
	__number,__string,__id,__dynamic_id,
	//basic elements
	
	__root,
	__list,__hash,
	__hash_member,
	__call_function,__list_search,__hash_search,
	__normal_statement_block,
	__definition,__assignment,
	__function,__loop,__ifelse
};

void print_token(int type)
{
	std::string context="";
	switch(type)
	{
		case __stack_end:         context="#"; break;
		case __equal:             context="="; break;
		case __cmp_equal:         context="==";break;
		case __cmp_not_equal:     context="!=";break;
		case __cmp_less:          context="<"; break;
		case __cmp_less_or_equal: context="<=";break;
		case __cmp_more:          context=">"; break;
		case __cmp_more_or_equal: context=">=";break;
		case __and_operator:      context="and";break;
		case __or_operator:       context="or"; break;
		case __nor_operator:      context="!"; break;
		case __add_operator:      context="+"; break;
		case __sub_operator:      context="-"; break;
		case __mul_operator:      context="*"; break;
		case __div_operator:      context="/"; break;
		case __link_operator:     context="~"; break;
		case __add_equal:         context="+=";break;
		case __sub_equal:         context="-=";break;
		case __mul_equal:         context="*=";break;
		case __div_equal:         context="/=";break;
		case __link_equal:        context="~=";break;
		case __left_brace:        context="{"; break;
		case __right_brace:       context="}"; break;
		case __left_bracket:      context="["; break;
		case __right_bracket:     context="]"; break;
		case __left_curve:        context="("; break;
		case __right_curve:       context=")"; break;
		case __semi:              context=";"; break;
		case __comma:             context=","; break;
		case __colon:             context=":"; break;
		case __dot:               context="."; break;
		case __unknown_operator:  context="unknown_operator";break;
		case __var:               context="var"; break;
		case __func:              context="func";break;
		case __continue:          context="continye"; break;
		case __break:             context="break"; break;
		case __for:               context="for"; break;
		case __forindex:          context="forindex";break;
		case __foreach:           context="foreach";break;
		case __while:             context="while";break;
		case __if:                context="if";break;
		case __elsif:             context="elsif";break;
		case __else:              context="else";break;
		case __return:            context="return";break;
		
		case __id:                context="id";break;
		case __dynamic_id:        context="id...";break;
		case __number:            context="num";break;
		case __string:            context="str";break;
		
		case __root:              context="root";break;
		case __list:              context="list";break;
		case __hash:              context="hash";break;
		case __hash_member:       context="hash_member";break;
		case __call_function:     context="call_func";break;
		case __list_search:       context="call_list";break;
		case __hash_search:       context="call_hash";break;
		case __normal_statement_block:context="block";break;
		case __definition:        context="definition";break;
		case __assignment:        context="assignment";break;
		case __function:          context="function";break;
		case __loop:              context="loop";break;
		case __ifelse:            context="if-else";break;
		default:                  context="unknown_token";break;
	}
	std::cout<<context;
	return;
}


#endif
