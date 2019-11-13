#ifndef __BALLOON_TYPE_H__
#define __BALLOON_TYPE_H__


enum token_type
{
	__token_number=1,
	__token_string,
	__reserve_word,
	__token_identifier,
	__token_operator
};
void print_token(int type)
{
	std::string str;
	switch(type)
	{
		case __token_number:    str="number       ";break;
		case __token_string:    str="string       ";break;
		case __reserve_word:    str="reserve word ";break;
		case __token_identifier:str="identifier   ";break;
		case __token_operator:  str="operator     ";break;
		default:                str="unknown      ";break;
	}
	std::cout<<str;
	return;
}

enum parse_type
{
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
	__var,__func,__return,
	__if,__elsif,__else,
	__continue,__break,
	__for,__forindex,__foreach,__while,
	__nil,
	//operators & reserve words
	__number,__string,__id,__dynamic_id,
	//basic elements
	
	__null_node,
	__block,
	__array,
	__hash,
	__root,
	__loop,
	__ifelse,
	__function,
	__parameter,
	__definition,
	__assignment,
	__call_array,
	__call_hash,
	__call_function
};
void print_detail_token(int type)
{
	std::string context="";
	switch(type)
	{
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
		case __nil:               context="nil";break;
		case __id:                context="id";break;
		case __dynamic_id:        context="id...";break;
		case __number:            context="num";break;
		case __string:            context="str";break;
		
		case __null_node:         context="null node";break;
		case __block:             context="block";break;
		case __array:             context="array";break;
		case __hash:              context="hash";break;
		case __root:              context="root";break;
		case __loop:              context="loop";break;
		case __ifelse:            context="if-else";break;
		case __function:          context="function";break;
		case __parameter:         context="parameter";break;
		case __definition:        context="definition";break;
		case __assignment:        context="assignment";break;
		case __call_array:        context="call array";break;
		case __call_hash:         context="call hash";break;
		case __call_function:     context="call func";break;
		default:                  context="undefined token";break;
	}
	std::cout<<context;
	return;
}

enum runtime_error_type
{
	__process_exited_successfully,
	__redeclaration,
	__get_value_failure,
	__find_var_failure,
	__error_value_type,
	__error_command_use,
	__sigfpe_arithmetic_exception,
	__sigsegv_segmentation_error,
	__terminal_interrupt
};

void print_exit_type(int type)
{
	std::string context;
	switch(type)
	{
		case __process_exited_successfully: context="process_success";break;
		case __redeclaration:               context="redeclaration";break;
		case __get_value_failure:           context="get_value_failure";break;
		case __find_var_failure:            context="find_var_failure";break;
		case __error_value_type:            context="value_type_error";break;
		case __error_command_use:           context="command_use_error(continue/break/return)";break;
		case __sigfpe_arithmetic_exception: context="SIGFPE";break;
		case __sigsegv_segmentation_error:  context="SIGSEGV";break;
		case __terminal_interrupt:          context="interrupt";break;
		default:                            context="unknown";break;
	}
	std::cout<<context;
	return;
}

#endif
