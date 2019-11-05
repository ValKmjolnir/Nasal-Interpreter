#ifndef __BALLOON_PARSE_H__
#define __BALLOON_PARSE_H__

class balloon_parse
{
	private:
		std::stack<token> parse;
		token this_token;
		int error;
		int warning;
		abstract_syntax_tree root;
	public:
		void get_token()
		{
			if(!parse.empty())
			{
				this_token=parse.top();
				parse.pop();
			}
			else
				this_token.type=0;
			return;
		}
		void get_detail_token_stream(std::list<token>& tk_list)
		{
			root.set_clear();
			while(!parse.empty())
				parse.pop();
			if(tk_list.empty())
				return;
			std::stack<token> temp;
			for(std::list<token>::iterator i=tk_list.begin();i!=tk_list.end();++i)
				temp.push(*i);
			while(!temp.empty())
			{
				parse.push(temp.top());
				temp.pop();
			}
			return;
		}
		void print_parse_stack()
		{
			if(parse.empty())
				return;
			std::stack<token> temp;
			int line=parse.top().line;
			std::cout<<line<<"	";
			while(!parse.empty())
			{
				if(parse.top().line!=line)
				{
					line=parse.top().line;
					std::cout<<std::endl<<line<<"	";
				}
				temp.push(parse.top());
				std::cout<<" ";
				print_detail_token(temp.top().type);
				parse.pop();
			}
			while(!temp.empty())
			{
				parse.push(temp.top());
				temp.pop();
			}
			std::cout<<std::endl;
			return;
		}
		int get_error()
		{
			return error;
		}
		void definition();
		void assignment();
		void array_generate();
		void hash_generate();
		void check_number();
		void check_string();
		void check_unary();
		void block();
		void func_generate();
		void call_identifier();
		void calculation();
		void calculation_or();
		void calculation_and();
		void calculation_cmp();
		void calculation_additive();
		void calculation_multive();
		void scalar();
		void check_semi();
		void parse_main();
};

void balloon_parse::definition()
{
	get_token();
	if(this_token.type!=__var)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \'var\' here."<<std::endl;
		return;
	}
	get_token();
	if(this_token.type!=__id)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect an identifier here."<<std::endl;
		return;
	}
	get_token();
	if(this_token.type!=__equal)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \'=\' here."<<std::endl;
		return;
	}
	scalar();
	return;
}

void balloon_parse::assignment()
{
	get_token();
	if(!(this_token.type==__equal || this_token.type==__add_equal
	|| this_token.type==__sub_equal || this_token.type==__mul_equal
	|| this_token.type==__div_equal || this_token.type==__link_equal))
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect an operator for assignment here."<<std::endl;
		return;
	}
	scalar();
	return;
}

void balloon_parse::array_generate()
{
	get_token();
	if(this_token.type!=__left_bracket)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \'[\' here."<<std::endl;
		return;
	}
	get_token();
	if(this_token.type!=__right_bracket)
		parse.push(this_token);
	while(this_token.type!=__right_bracket)
	{
		get_token();
		switch(this_token.type)
		{
			case __func:
			case __left_curve:
			case __left_bracket:
			case __left_brace:
			case __nor_operator:
			case __sub_operator:
			case __number:
			case __string:
			case __id:parse.push(this_token);scalar();break;
			case __right_bracket:parse.push(this_token);break;
			default:
				++error;
				std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a scalar or \']\' here."<<std::endl;
				return;
				break;
		}
		get_token();
		if(this_token.type!=__comma && this_token.type!=__right_bracket)
		{
			++error;
			std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \',\' or \']\' here."<<std::endl;
			return;
		}
	}
	return;
}

void balloon_parse::hash_generate()
{
	get_token();
	if(this_token.type!=__left_brace)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \'{\' here."<<std::endl;
		return;
	}
	get_token();
	if(this_token.type!=__right_brace)
		parse.push(this_token);
	while(this_token.type!=__right_brace)
	{
		get_token();
		if(this_token.type!=__id)
		{
			++error;
			std::cout<<">>[Parse-error] line "<<this_token.line<<": expect an identifier here."<<std::endl;
			return;
		}
		get_token();
		if(this_token.type!=__colon)
		{
			++error;
			std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \':\' here."<<std::endl;
			return;
		}
		get_token();
		switch(this_token.type)
		{
			case __func:
			case __left_curve:
			case __left_bracket:
			case __left_brace:
			case __nor_operator:
			case __sub_operator:
			case __number:
			case __string:
			case __id:parse.push(this_token);scalar();break;
			case __right_brace:parse.push(this_token);break;
			default:
				++error;
				std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a scalar or \'}\' here."<<std::endl;
				return;
				break;
		}
		get_token();
		if(this_token.type!=__comma && this_token.type!=__right_brace)
		{
			++error;
			std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \',\' or \'}\' here."<<std::endl;
			return;
		}
	}
	return;
}

void balloon_parse::check_number()
{
	get_token();
	if(this_token.type!=__number)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a number here."<<std::endl;
	}
	return;
}

void balloon_parse::check_string()
{
	get_token();
	if(this_token.type!=__string)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a string here."<<std::endl;
	}
	return;
}

void balloon_parse::check_unary()
{
	get_token();
	if(this_token.type!=__nor_operator && this_token.type!=__sub_operator)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a unary operator here."<<std::endl;
	}
	get_token();
	switch(this_token.type)
	{
		case __number:parse.push(this_token);check_number();break;
		case __string:parse.push(this_token);check_string();break;
		case __id:parse.push(this_token);call_identifier();break;
		case __left_curve:parse.push(this_token);scalar();break;
		case __nor_operator:parse.push(this_token);check_unary();break;
		default:++error;std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a scalar here."<<std::endl;break;
	}
	return;
}

void balloon_parse::block()
{
	get_token();
	if(this_token.type!=__left_brace)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \'{\' ."<<std::endl;
		return;
	}
	while(this_token.type!=__right_brace)
	{
		get_token();
		switch(this_token.type)
		{
			case __continue:
			case __break:check_semi();break;
			case __right_brace:break;
		}
	}
	return;
}

void balloon_parse::func_generate()
{
	get_token();
	if(this_token.type!=__func)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \'func\' here."<<std::endl;
		return;
	}
	get_token();
	if(this_token.type==__left_curve)
	{
		while(this_token.type!=__right_curve)
		{
			get_token();
			if(this_token.type!=__id && this_token.type!=__dynamic_id)
			{
				++error;
				std::cout<<">>[Parse-error] line "<<this_token.line<<": expect identifier when declaring a new function."<<std::endl;
				return;
			}
			if(this_token.type==__dynamic_id)
			{
				get_token();
				if(this_token.type!=__right_curve)
				{
					++error;
					std::cout<<">>[Parse-error] line "<<this_token.line<<": dynamic identifier should have \')\' behind it ."<<std::endl;
					return;
				}
			}
			else
			{
				get_token();
				if(this_token.type!=__comma && this_token.type!=__right_curve)
				{
					++error;
					std::cout<<">>[Parse-error] line "<<this_token.line<<": expect \',\' or \')\' here."<<std::endl;	
					return;
				}
			}
		}
	}
	else
		parse.push(this_token);
	block();
	return;
}

void balloon_parse::call_identifier()
{
	get_token();
	if(this_token.type!=__id)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": must have an identifier here."<<std::endl;
		return;
	}
	while(1)
	{
		get_token();
		if(this_token.type==__left_curve)
		{
			while(this_token.type!=__right_curve)
			{
				scalar();
				get_token();
				if(this_token.type!=__comma && this_token.type!=__right_curve)
				{
					++error;
					std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \',\' or \')\' here."<<std::endl;
					return;
				}
			}
		}
		else if(this_token.type==__left_bracket)
		{
			scalar();
			get_token();
			if(this_token.type!=__right_bracket)
			{
				++error;
				std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \']\' ."<<std::endl;
				return;
			}
		}
		else if(this_token.type==__dot)
			call_identifier();
		else
		{
			parse.push(this_token);
			break;
		}
	}
	get_token();
	if(this_token.type==__equal || this_token.type==__add_equal
	|| this_token.type==__sub_equal || this_token.type==__mul_equal
	|| this_token.type==__div_equal || this_token.type==__link_equal)
	{
		parse.push(this_token);
		assignment();
	}
	else
		parse.push(this_token);
	return;
}

void balloon_parse::calculation()
{
	calculation_or();
	return;
}

void balloon_parse::calculation_or()
{
	while(1)
	{
		calculation_and();
		get_token();
		if(this_token.type!=__or_operator)
		{
			parse.push(this_token);
			break;
		}
	}
	return;
}

void balloon_parse::calculation_and()
{
	while(1)
	{
		calculation_cmp();
		get_token();
		if(this_token.type!=__and_operator)
		{
			parse.push(this_token);
			break;
		}
	}
	return;
}

void balloon_parse::calculation_cmp()
{
	while(1)
	{
		calculation_additive();
		get_token();
		if(this_token.type!=__cmp_equal && this_token.type!=__cmp_not_equal
		&& this_token.type!=__cmp_less && this_token.type!=__cmp_less_or_equal
		&& this_token.type!=__cmp_more && this_token.type!=__cmp_more_or_equal)
		{
			parse.push(this_token);
			break;
		}
	}
}

void balloon_parse::calculation_additive()
{
	while(1)
	{
		calculation_multive();
		get_token();
		if(this_token.type!=__add_operator && this_token.type!=__sub_operator && this_token.type!=__link_operator)
		{
			parse.push(this_token);
			break;
		}
	}
	return;
}

void balloon_parse::calculation_multive()
{
	while(1)
	{
		get_token();
		switch(this_token.type)
		{
			case __left_curve:
				scalar();get_token();
				if(this_token.type!=__right_curve)
				{
					++error;
					std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \')\' here."<<std::endl;
					return;
				}
				break;
			case __number:parse.push(this_token);check_number();break;
			case __string:parse.push(this_token);check_string();break;
			case __id:parse.push(this_token);call_identifier();break;
			case __nor_operator:
			case __sub_operator:parse.push(this_token);check_unary();break;
			default:
				++error;
				std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a scalar here."<<std::endl;
				return;
				break;
		}
		get_token();
		if(this_token.type!=__mul_operator && this_token.type!=__div_operator)
		{
			parse.push(this_token);
			break;
		}
	}
	return;
}

void balloon_parse::scalar()
{
	get_token();
	switch(this_token.type)
	{
		case __func:parse.push(this_token);func_generate();break;
		case __left_bracket:parse.push(this_token);array_generate();break;
		case __left_brace:parse.push(this_token);hash_generate();break;
		case __left_curve:
		case __nor_operator:
		case __sub_operator:
		case __number:
		case __string:
		case __id:parse.push(this_token);calculation();break;
		default:
			++error;
			std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a scalar at this place."<<std::endl;
			parse.push(this_token);
			break;
	}
	return;
}

void balloon_parse::check_semi()
{
	get_token();
	if(this_token.type!=__semi)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \';\' ."<<std::endl;
	}
	return;
}

void balloon_parse::parse_main()
{
	root.set_type(__root);
	error=0;
	warning=0;
	
	while(!parse.empty())
	{
		get_token();
		switch(this_token.type)
		{
			case __var:parse.push(this_token);definition();check_semi();break;
			case __left_curve:
			case __left_bracket:
			case __left_brace:
			case __nor_operator:
			case __sub_operator:
			case __number:
			case __string:
			case __id:parse.push(this_token);scalar();check_semi();break;
			case __if:parse.push(this_token);break;
			case __while:parse.push(this_token);break;
			case __semi:break;
			default:
				++error;
				std::cout<<">>[Parse-error] line "<<this_token.line<<": \'";
				print_detail_token(this_token.type);
				std::cout<<"\' should not appear in this scope."<<std::endl;
				break;
		}
	}
	std::cout<<">>[Parse] complete generating."<<error<<" error(s),"<<warning<<" warning(s)."<<std::endl;
	return;
}
#endif
