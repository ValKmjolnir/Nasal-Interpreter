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

enum parse_error_type
{
	parse_unknown_error=0,       // unknown error
	error_token_in_main,         // when a token should not be the begin of a statement in main
	error_token_in_block,        // when a token should not be the begin of a statement in block

	lack_semi,
	lack_id,
	lack_left_curve,
	lack_right_curve,
	lack_right_brace,

	definition_lack_id,          // lack identifier
	definition_lack_equal,       // lack '=' when not getting ';'
	assignment_begin_error,      // assignment begins with more than one identifier_call
	multi_definition_need_curve, // lack right curve when generating 'var (id,id,id)'

	multi_assignment_need_curve, // lack right curve when generating (scalar,scalar)=(scalar,scalar)
	multi_assignment_need_equal, // lack '=' when generating (scalar,scalar)=(scalar,scalar)

	error_begin_token_of_scalar, // in scalar_generate() 
	
	default_dynamic_parameter,   // default parameter should not be dynamic 
	parameter_lack_part,         // parameter lack a ')' or identifier
	parameter_lack_curve,        // parameter lack a ',' or ')'

	special_call_func_lack_id,
	special_call_func_lack_colon,
	call_func_lack_comma,        // lack comma when giving parameters to a function
	call_hash_lack_id,           // lack identifier when calling a hash
	call_vector_wrong_comma,     // wrong use of comma like this: id[0,4:6,7,] (the last comma is incorrect here)
	call_vector_lack_bracket,    // lack ']' when calling a vector
	call_vector_wrong_token,     // get wrong token when calling a vector

	vector_gen_lack_end,         // lack ',' or ')' when generating a vector
	hash_gen_lack_id,            // lack identifier or string when generating a hash
	hash_gen_lack_colon,         // lack ':' when generating a hash
	hash_gen_lack_end,           // lack ',' or '}' when generating a hash

	ternary_operator_lack_colon, // lack ':'
};
void print_parse_error(int error_type,int line,int error_token_type=__stack_end)
{
	std::string error_info_head=">> [Parse-error] line ";
	std::string warning_info_head=">> [Parse-warning] line ";
	switch(error_type)
	{
		case parse_unknown_error:
			std::cout<<error_info_head<<line<<": unknown parse error.(token id: "<<error_token_type<<")."<<std::endl;break;
		case error_token_in_main:
			std::cout<<error_info_head<<line<<": statements should not begin with \'";
			print_parse_token(error_token_type);
			std::cout<<"\' in main scope."<<std::endl;
			break;
		case error_token_in_block:
			std::cout<<error_info_head<<line<<": statements should not begin with \'";
			print_parse_token(error_token_type);
			std::cout<<"\' in block scope."<<std::endl;
			break;
		
		case lack_semi:
			std::cout<<error_info_head<<line<<": expect a \';\' at the end of the statement."<<std::endl;break;
		case lack_id:
			std::cout<<error_info_head<<line<<": expect an identifier here."<<std::endl;break;
		case lack_left_curve:
			std::cout<<error_info_head<<line<<": expect a \'(\' here."<<std::endl;break;
		case lack_right_curve:
			std::cout<<error_info_head<<line<<": expect a \')\' here."<<std::endl;break;
		case lack_right_brace:
			std::cout<<error_info_head<<line<<": expect a \'}\' here."<<std::endl;break;
		
		case definition_lack_id:
			std::cout<<error_info_head<<line<<": expect identifier(s) after \'var\'."<<std::endl;break;
		case definition_lack_equal:
			std::cout<<error_info_head<<line<<": expect a \'=\' here but get \'";
			print_parse_token(error_token_type);
			std::cout<<"\' when generating definition."<<std::endl;
			break;
		case assignment_begin_error:
			std::cout<<error_info_head<<line<<": assignment should not begin with more than one identifier_call(such as a*b=c)."<<std::endl;
			break;
		case multi_definition_need_curve:
			std::cout<<error_info_head<<line<<": expect a \')\' here but get \'";
			print_parse_token(error_token_type);
			std::cout<<"\' ."<<std::endl;
			break;
		
		case multi_assignment_need_curve:
			std::cout<<error_info_head<<line<<": expect a \')\' here but get \'";
			print_parse_token(error_token_type);
			std::cout<<"\' ."<<std::endl;
			break;
		case multi_assignment_need_equal:
			std::cout<<error_info_head<<line<<": expect a \'=\' here but get \'";
			print_parse_token(error_token_type);
			std::cout<<"\' ."<<std::endl;
			break;
		
		case error_begin_token_of_scalar:
			std::cout<<error_info_head<<line<<": expect a scalar here but get \'";
			print_parse_token(error_token_type);
			std::cout<<"\' ."<<std::endl;
			break;
		
		case default_dynamic_parameter:
			std::cout<<error_info_head<<line<<": dynamic parameter should not have a default value."<<std::endl;break;
		case parameter_lack_part:
			std::cout<<error_info_head<<line<<": expect a \')\' or identifier here when generating parameter_list."<<std::endl;break;
		case parameter_lack_curve:
			std::cout<<error_info_head<<line<<": expect a \')\' or \',\' here when generating parameter_list."<<std::endl;break;
		
		case special_call_func_lack_id:
			std::cout<<error_info_head<<line<<": expect an identifier here but get \'";
			print_parse_token(error_token_type);
			std::cout<<"\' when calling functions."<<std::endl;
			break;
		case special_call_func_lack_colon:
			std::cout<<error_info_head<<line<<": expect an \':\' here but get \'";
			print_parse_token(error_token_type);
			std::cout<<"\' when calling functions."<<std::endl;
			break;
		case call_func_lack_comma:
			std::cout<<error_info_head<<line<<": expect a \',\' when calling a function but get \'";
			print_parse_token(error_token_type);
			std::cout<<"\' ."<<std::endl;
			break;
		case call_hash_lack_id:
			std::cout<<error_info_head<<line<<": expect an identifier after \'.\' ."<<std::endl;break;
		case call_vector_wrong_comma:
			std::cout<<error_info_head<<line<<": expect a scalar after \',\' but get \']\' ."<<std::endl;
			break;
		case call_vector_lack_bracket:
			std::cout<<error_info_head<<line<<": expect a \']\' here but get \'";
			print_parse_token(error_token_type);
			std::cout<<"\' ."<<std::endl;
			break;
		case call_vector_wrong_token:
			std::cout<<error_info_head<<line<<": expect \':\' or ',' or ']' here but get \'";
			print_parse_token(error_token_type);
			std::cout<<"\' ."<<std::endl;
			break;
		
		case vector_gen_lack_end:
			std::cout<<error_info_head<<line<<": expect a \',\' or \')\' here but get \'";
			print_parse_token(error_token_type);
			std::cout<<"\' ."<<std::endl;
			break;
		case hash_gen_lack_id:
			std::cout<<error_info_head<<line<<": expect an identifier or string here but get \'";
			print_parse_token(error_token_type);
			std::cout<<"\' ."<<std::endl;
			break;
		case hash_gen_lack_colon:
			std::cout<<error_info_head<<line<<": expect a \':\' here but get \'";
			print_parse_token(error_token_type);
			std::cout<<"\' ."<<std::endl;
			break;
		case hash_gen_lack_end:
			std::cout<<error_info_head<<line<<": expect a \',\' or \'}\' here but get \'";
			print_parse_token(error_token_type);
			std::cout<<"\' ."<<std::endl;
			break;
		
		case ternary_operator_lack_colon:
			std::cout<<error_info_head<<line<<": expect a \':\' here but get \'";
			print_parse_token(error_token_type);
			std::cout<<"\' ."<<std::endl;
			break;
		default:
			std::cout<<error_info_head<<line<<": unknown parse error.(token id: "<<error_token_type<<")."<<std::endl;break;
	}
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
