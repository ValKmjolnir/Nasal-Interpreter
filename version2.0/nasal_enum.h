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

// parse_gen_type include enums for parser and ast
enum parse_gen_type
{
	__stack_end=1,

	// operators
	__cmp_equal,__cmp_not_equal,__cmp_less,__cmp_less_or_equal,__cmp_more,__cmp_more_or_equal,
	// == != < <= > >= 
	__and_operator,__or_operator,__nor_operator,__add_operator,__sub_operator,__mul_operator,__div_operator,__link_operator,
	// and or ! + - * / ~
	__equal,__add_equal,__sub_equal,__mul_equal,__div_equal,__link_equal,
	// = += -= *= /= ~=
	__left_brace,__right_brace,                    // {}
	__left_bracket,__right_bracket,                // []
	__left_curve,__right_curve,                    // ()
	__semi,__comma,__colon,__dot,__ques_mark,      // ; , : . ?
	__unknown_operator,
	
	// reserve words
	__var,__func,__return,__nil,
	__if,__elsif,__else,
	__continue,__break,
	__for,__forindex,__foreach,__while,
	
	// basic scalar type: number string identifier dynamic_identifier
	__number,__string,__id,__dynamic_id,
	
	// abstract_syntax_tree type below
	// abstract_syntax_tree also uses the types above, such as operators
	__root,
	__null_type,
	__multi_id,__multi_scalar,
	__parameters,__special_parameter,__default_parameter,
	__vector,__sub_vector,__call_vector,
	__hash,__hash_member,__call_hash,
	__function,__call_function,
	__normal_statement_block,
	__definition,
	__conditional
};
void print_parse_token(int type)
{
	std::string context="";
	switch(type)
	{
		case __stack_end:         context=" # ";  break;
		case __cmp_equal:         context=" == "; break;
		case __cmp_not_equal:     context=" != "; break;
		case __cmp_less:          context=" < ";  break;
		case __cmp_less_or_equal: context=" <= "; break;
		case __cmp_more:          context=" > ";  break;
		case __cmp_more_or_equal: context=" >= "; break;
		
		case __and_operator:      context=" and ";break;
		case __or_operator:       context=" or "; break;
		case __nor_operator:      context=" ! ";  break;
		case __add_operator:      context=" + ";  break;
		case __sub_operator:      context=" - ";  break;
		case __mul_operator:      context=" * ";  break;
		case __div_operator:      context=" / ";  break;
		case __link_operator:     context=" ~ ";  break;
		
		case __equal:             context=" = ";  break;
		case __add_equal:         context=" += "; break;
		case __sub_equal:         context=" -= "; break;
		case __mul_equal:         context=" *= "; break;
		case __div_equal:         context=" /= "; break;
		case __link_equal:        context=" ~= "; break;
		
		case __left_brace:        context="{";    break;
		case __right_brace:       context="}";    break;
		case __left_bracket:      context="[";    break;
		case __right_bracket:     context="]";    break;
		case __left_curve:        context="(";    break;
		case __right_curve:       context=")";    break;
		
		case __semi:              context=";";    break;
		case __comma:             context=",";    break;
		case __colon:             context=":";    break;
		case __dot:               context=".";    break;
		case __ques_mark:         context="?";    break;
		
		case __unknown_operator:  context="unknown_operator";break;
		
		case __var:               context="var ";      break;
		case __func:              context="func ";     break;
		case __continue:          context="continue";  break;
		case __break:             context="break";     break;
		case __for:               context="for";       break;
		case __forindex:          context="forindex";  break;
		case __foreach:           context="foreach ";  break;
		case __while:             context="while";     break;
		case __if:                context="if ";       break;
		case __elsif:             context="elsif ";    break;
		case __else:              context="else ";     break;
		case __return:            context="return ";   break;
		case __nil:               context="nil";       break;
		
		case __id:                context="id";        break;
		case __dynamic_id:        context="id...";     break;
		case __number:            context="num";       break;
		case __string:            context="str";       break;
		
		default:                  context="undefined";   break;
	}
	std::cout<<context;
	return;
}
void print_ast_type(int type)
{
	std::string context="";
	switch(type)
	{
		case __cmp_equal:         context=" == "; break;
		case __cmp_not_equal:     context=" != "; break;
		case __cmp_less:          context=" < ";  break;
		case __cmp_less_or_equal: context=" <= "; break;
		case __cmp_more:          context=" > ";  break;
		case __cmp_more_or_equal: context=" >= "; break;
		
		case __and_operator:      context=" and ";break;
		case __or_operator:       context=" or "; break;
		case __nor_operator:      context=" ! ";  break;
		case __add_operator:      context=" + ";  break;
		case __sub_operator:      context=" - ";  break;
		case __mul_operator:      context=" * ";  break;
		case __div_operator:      context=" / ";  break;
		case __link_operator:     context=" ~ ";  break;
		
		case __equal:             context=" = ";  break;
		case __add_equal:         context=" += "; break;
		case __sub_equal:         context=" -= "; break;
		case __mul_equal:         context=" *= "; break;
		case __div_equal:         context=" /= "; break;
		case __link_equal:        context=" ~= "; break;
		
		case __ques_mark:         context="?";    break;

		case __var:               context="var ";      break;
		case __func:              context="func ";     break;
		case __continue:          context="continue";  break;
		case __break:             context="break";     break;
		case __for:               context="for";       break;
		case __forindex:          context="forindex";  break;
		case __foreach:           context="foreach ";  break;
		case __while:             context="while";     break;
		case __if:                context="if ";       break;
		case __elsif:             context="elsif ";    break;
		case __else:              context="else ";     break;
		case __return:            context="return ";   break;
		case __nil:               context="nil";       break;
		
		case __id:                context="id";        break;
		case __dynamic_id:        context="id...";     break;
		case __number:            context="num";       break;
		case __string:            context="str";       break;
		
		case __root:              context="root";              break; // root of the ast that parser generates
		case __null_type:         context="null_type";         break; // type of the node of the tree is unknown
		case __multi_id:          context="identifiers";       break; // id,id,id,id                |often used in multi-definition or multi-assignment
		case __multi_scalar:      context="scalars";           break; // scalar,scalar,scalar,scalar|often used in multi-definition or multi-assignment
		case __parameters:        context="parameters";        break; // parameter list
		case __special_parameter: context="special_parameter"; break; // identifier:scalar          |special way of calling a function
		case __default_parameter: context="default_parameter"; break; // identifier=scalar          |default parameter when generating a new function
		case __vector:            context="vector";            break; // vector
		case __sub_vector:        context="sub_vector";        break; // the same as subvec() but more flexible to use
		case __call_vector:       context="call_vector";       break; // call vector member
		case __hash:              context="hash";              break; // hash
		case __hash_member:       context="hash_member";       break; // hash member
		case __call_hash:         context="call_hash";         break; // call hash member
		case __function:          context="function";          break; // function
		case __call_function:     context="call_function";     break; // call function
		case __normal_statement_block:context="block";         break; // block
		case __definition:        context="definition";        break; // definition
		case __conditional:       context="conditional";       break; // if-else
		
		default:                  context="undefined";         break;
	}
	std::cout<<context;
	return;
}

enum scalar_type
{
	scalar_nil=0,
	scalar_number,
	scalar_string,
	scalar_vector,
	scalar_hash,
	scalar_function
};
void print_scalar_type(const int type)
{
	switch(type)
	{
		case scalar_nil:      std::cout<<"nil";break;
		case scalar_number:
		case scalar_string:   std::cout<<"scalar";break;
		case scalar_vector:   std::cout<<"vector";break;
		case scalar_hash:     std::cout<<"hash";break;
		case scalar_function: std::cout<<"function";break;
		default:              std::cout<<"nil";break;
	}
	return;
}

#endif
