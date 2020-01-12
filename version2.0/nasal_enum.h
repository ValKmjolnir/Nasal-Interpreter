#ifndef __NASAL_ENUM_H__
#define __NASAL_ENUM_H__

enum lexer_token_type
{
	__token_reserve_word=1,
	__token_identifier,
	__token_number,
	__token_string,
	__token_operator,
};
void print_lexer_token(int type)
{
	switch(type)
	{
		case __token_reserve_word:std::cout<<"reserve word";break;
		case __token_identifier:  std::cout<<"identifier  ";break;
		case __token_number:      std::cout<<"number      ";break;
		case __token_string:      std::cout<<"string      ";break;
		case __token_operator:    std::cout<<"operator    ";break;
	}
	return;
}

enum parse_token_type
{
	__stack_end=1,
	__cmp_equal,__cmp_not_equal,__cmp_less,__cmp_less_or_equal,__cmp_more,__cmp_more_or_equal,
	// == != < <= > >= 
	__and_operator,__or_operator,__nor_operator,__add_operator,__sub_operator,__mul_operator,__div_operator,__link_operator,
	// and or ! + - * / ~
	__equal,__add_equal,__sub_equal,__mul_equal,__div_equal,__link_equal,
	// = += -= *= /= ~=
	__left_brace,__right_brace,                    // {}
	__left_bracket,__right_bracket,                // []
	__left_curve,__right_curve,                    // ()
	__semi,__comma,__colon,__dot,__ques_mark,       // ; , : . ?
	__unknown_operator,
	// operators
	__var,__func,__return,__nil,
	__if,__elsif,__else,
	__continue,__break,
	__for,__forindex,__foreach,__while,
	// reserve words
	__number,__string,__id,__dynamic_id,
	// basic scalar type: number string identifier dynamic_identifier
	
	// absttract_syntax_tree type below
	__root,
	__null_type,
	__multi_id,
	__parameters,
	__list,__hash,
	__hash_member,
	__call_function,__call_array,__call_hash,
	__normal_statement_block,
	__definition,__assignment,
	__function,__loop,__ifelse
};

void print_parse_token(int type)
{
	std::string context="";
	switch(type)
	{
		case __stack_end:         context="#";  break;
		
		case __cmp_equal:         context="=="; break;
		case __cmp_not_equal:     context="!="; break;
		case __cmp_less:          context="<";  break;
		case __cmp_less_or_equal: context="<="; break;
		case __cmp_more:          context=">";  break;
		case __cmp_more_or_equal: context=">="; break;
		
		case __and_operator:      context="and";break;
		case __or_operator:       context="or"; break;
		case __nor_operator:      context="!";  break;
		case __add_operator:      context="+";  break;
		case __sub_operator:      context="-";  break;
		case __mul_operator:      context="*";  break;
		case __div_operator:      context="/";  break;
		case __link_operator:     context="~";  break;
		
		case __equal:             context="=";  break;
		case __add_equal:         context="+="; break;
		case __sub_equal:         context="-="; break;
		case __mul_equal:         context="*="; break;
		case __div_equal:         context="/="; break;
		case __link_equal:        context="~="; break;
		
		case __left_brace:        context="{";  break;
		case __right_brace:       context="}";  break;
		case __left_bracket:      context="[";  break;
		case __right_bracket:     context="]";  break;
		case __left_curve:        context="(";  break;
		case __right_curve:       context=")";  break;
		
		case __semi:              context=";";  break;
		case __comma:             context=",";  break;
		case __colon:             context=":";  break;
		case __dot:               context=".";  break;
		case __ques_mark:         context="?";  break;
		
		case __unknown_operator:  context="unknown_operator";break;
		
		case __var:               context="var";      break;
		case __func:              context="func";     break;
		case __continue:          context="continye"; break;
		case __break:             context="break";    break;
		case __for:               context="for";      break;
		case __forindex:          context="forindex"; break;
		case __foreach:           context="foreach";  break;
		case __while:             context="while";    break;
		case __if:                context="if";       break;
		case __elsif:             context="elsif";    break;
		case __else:              context="else";     break;
		case __return:            context="return";   break;
		case __nil:               context="nil";      break;
		
		case __id:                context="identifier";   break;
		case __dynamic_id:        context="identifier...";break;
		case __number:            context="number";       break;
		case __string:            context="string";       break;
		
		case __root:              context="root";        break;
		case __null_type:         context="null_type";   break;
		case __multi_id:          context="identifiers"; break;
		case __parameters:        context="parameters";  break;
		case __list:              context="list";        break;
		case __hash:              context="hash";        break;
		case __hash_member:       context="hash_member"; break;
		case __call_function:     context="call_func";   break;
		case __call_array:        context="call_array";  break;
		case __call_hash:         context="call_hash";   break;
		case __normal_statement_block:context="block";   break;
		case __definition:        context="definition";  break;
		case __assignment:        context="assignment";  break;
		case __function:          context="function";    break;
		case __loop:              context="loop";        break;
		case __ifelse:            context="if-else";     break;
		
		default:                  context="undefined_token";break;
	}
	std::cout<<context;
	return;
}

enum parse_error_type
{
	parse_unknown_error,         // unknown error
	error_token_in_main,         // when a token should not be the begin of a statement in main
	definition_lack_id,          // lack identifier
	definition_lack_equal,       // lack '=' when not getting ';'
	definition_wrong_type,       // need identifier but get number or string
	multi_definition_need_curve, // lack right curve when generating 'var (id,id,id)'
	error_begin_token_of_scalar, // in scalar_generate() 
	lack_left_curve,             // lack left curve
	lack_right_curve,            // lack right curve
};

void print_parse_error(int error_type,int line,int error_token_type=__stack_end)
{
	std::string error_info_head=">>[Parse-error] line ";
	std::string warning_info_head=">> [Parse-warning] line ";
	switch(error_type)
	{
		case parse_unknown_error:
			std::cout<<error_info_head<<line<<": unknown parse error. error id: parse_unknown_error."<<std::endl;break;
		case error_token_in_main:
			std::cout<<error_info_head<<line<<": statements should not begin with \'";
			print_parse_token(error_token_type);
			std::cout<<"\' in main scope."<<std::endl;
			break;
		case definition_lack_id:
			std::cout<<error_info_head<<line<<": expect identifier(s) after \'var\'."<<std::endl;break;
		case definition_lack_equal:
			std::cout<<error_info_head<<line<<": expect a \'=\' here."<<std::endl;break;
		case definition_wrong_type:
			std::cout<<error_info_head<<line<<": expect an identifier here but get other types."<<std::endl;break;
		case multi_definition_need_curve:
			std::cout<<error_info_head<<line<<": expect a \')\' here."<<std::endl;break;
		case error_begin_token_of_scalar:
			std::cout<<error_info_head<<line<<": expect a scalar here but get \'";
			print_parse_token(error_token_type);
			std::cout<<"\' ."<<std::endl;
			break;
		case lack_left_curve:
			std::cout<<error_info_head<<line<<": expect a \'(\' here."<<std::endl;break;
		case lack_right_curve:
			std::cout<<error_info_head<<line<<": expect a \')\' here."<<std::endl;break;
		default:
			std::cout<<error_info_head<<line<<": unknown parse error. error id: other_type."<<std::endl;break;
	}
	return;
}

#endif
