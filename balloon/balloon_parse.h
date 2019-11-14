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
		void print_generated_tree()
		{
			std::cout<<">>[Abstract-syntax-tree]"<<std::endl;
			root.print_tree(1);
			return;
		}
		void run_tree()
		{
			std::cout<<">>[Runtime] process begins at addr:"<<(void*)(&root)<<"."<<std::endl;
			root.run_root();
			return;
		}
		abstract_syntax_tree ret();
		abstract_syntax_tree choose();
		abstract_syntax_tree loop();
		abstract_syntax_tree definition();
		abstract_syntax_tree assignment();
		abstract_syntax_tree array_generate();
		abstract_syntax_tree hash_generate();
		abstract_syntax_tree check_number();
		abstract_syntax_tree check_string();
		abstract_syntax_tree check_unary();
		abstract_syntax_tree block();
		abstract_syntax_tree func_generate();
		abstract_syntax_tree call_identifier();
		abstract_syntax_tree calculation();
		abstract_syntax_tree calculation_or();
		abstract_syntax_tree calculation_and();
		abstract_syntax_tree calculation_cmp();
		abstract_syntax_tree calculation_additive();
		abstract_syntax_tree calculation_multive();
		abstract_syntax_tree scalar();
		void check_semi();
		void parse_main();
};

abstract_syntax_tree balloon_parse::ret()
{
	abstract_syntax_tree new_node;
	new_node.set_type(__return);
	get_token();
	if(this_token.type!=__return)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": must use \'return\' here."<<std::endl;
		return new_node;
	}
	new_node.set_line(this_token.line);
	get_token();
	if(this_token.type==__semi)
		parse.push(this_token);
	else
	{
		parse.push(this_token);
		new_node.add_child(scalar());
	}
	return new_node;
}

abstract_syntax_tree balloon_parse::choose()
{
	abstract_syntax_tree new_node;
	abstract_syntax_tree temp;
	new_node.set_type(__ifelse);
	temp.set_type(__if);
	get_token();
	if(this_token.type!=__if)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": must use \'if\' when generating an if-else statement."<<std::endl;
		return new_node;
	}
	new_node.set_line(this_token.line);
	temp.set_line(this_token.line);
	get_token();
	if(this_token.type!=__left_curve)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \'(\'."<<std::endl;
		return new_node;
	}
	temp.add_child(scalar());
	get_token();
	if(this_token.type!=__right_curve)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \')\'."<<std::endl;
		return new_node;
	}
	temp.add_child(block());
	new_node.add_child(temp);
	while(1)
	{
		temp.set_clear();
		get_token();
		if(this_token.type!=__elsif && this_token.type!=__else)
		{
			parse.push(this_token);
			break;
		}
		if(this_token.type==__else)
		{
			get_token();
			if(this_token.type==__if)// else if
			{
				temp.set_type(__elsif);
				temp.set_line(this_token.line);
				get_token();
				if(this_token.type!=__left_curve)
				{
					++error;
					std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \'(\'."<<std::endl;
					return new_node;
				}
				temp.add_child(scalar());
				get_token();
				if(this_token.type!=__right_curve)
				{
					++error;
					std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \')\'."<<std::endl;
					return new_node;
				}
				temp.add_child(block());
				new_node.add_child(temp);
			}
			else// real else
			{
				temp.set_type(__else);
				temp.set_line(this_token.line);
				parse.push(this_token);
				temp.add_child(block());
				new_node.add_child(temp);
				break;
			}
		}
		else if(this_token.type==__elsif)
		{
			temp.set_type(__elsif);
			temp.set_line(this_token.line);
			get_token();
			if(this_token.type!=__left_curve)
			{
				++error;
				std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \'(\'."<<std::endl;
				return new_node;
			}
			temp.add_child(scalar());
			get_token();
			if(this_token.type!=__right_curve)
			{
				++error;
				std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \')\'."<<std::endl;
				return new_node;
			}
			temp.add_child(block());
			new_node.add_child(temp);
		}
	}
	return new_node;
}

abstract_syntax_tree balloon_parse::loop()
{
	abstract_syntax_tree new_node;
	new_node.set_type(__while);
	get_token();
	if(this_token.type!=__while)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": must use \'while\' when generating a loop."<<std::endl;
		return new_node;
	}
	new_node.set_line(this_token.line);
	get_token();
	if(this_token.type!=__left_curve)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \'(\'."<<std::endl;
		return new_node;
	}
	new_node.add_child(scalar());
	get_token();
	if(this_token.type!=__right_curve)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \')\'."<<std::endl;
		return new_node;
	}
	new_node.add_child(block());
	return new_node;
}

abstract_syntax_tree balloon_parse::definition()
{
	abstract_syntax_tree new_node;
	abstract_syntax_tree temp;
	new_node.set_type(__definition);
	temp.set_type(__id);
	get_token();
	if(this_token.type!=__var)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \'var\' here."<<std::endl;
		return new_node;
	}
	new_node.set_line(this_token.line);
	temp.set_line(this_token.line);
	get_token();
	if(this_token.type!=__id)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect an identifier here."<<std::endl;
		return new_node;
	}
	temp.set_name(this_token.str);
	new_node.add_child(temp);
	get_token();
	if(this_token.type!=__equal)
	{
		++warning;
		std::cout<<">>[Parse-warning] line "<<this_token.line<<": every new var should be initialized."<<std::endl;
		parse.push(this_token);
		return new_node;
	}
	new_node.add_child(scalar());
	return new_node;
}

abstract_syntax_tree balloon_parse::assignment()
{
	abstract_syntax_tree new_node;
	get_token();
	if(!(this_token.type==__equal || this_token.type==__add_equal
	|| this_token.type==__sub_equal || this_token.type==__mul_equal
	|| this_token.type==__div_equal || this_token.type==__link_equal))
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect an operator for assignment here."<<std::endl;
		return new_node;
	}
	new_node.set_line(this_token.line);
	new_node.set_type(this_token.type);
	new_node.add_child(scalar());
	return new_node;
}

abstract_syntax_tree balloon_parse::array_generate()
{
	abstract_syntax_tree new_node;
	new_node.set_type(__array);
	get_token();
	if(this_token.type!=__left_bracket)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \'[\' here."<<std::endl;
		return new_node;
	}
	new_node.set_line(this_token.line);
	while(1)
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
			case __id:parse.push(this_token);new_node.add_child(scalar());break;
			case __right_bracket:parse.push(this_token);break;
			default:
				++error;
				std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a scalar or \']\' here."<<std::endl;
				return new_node;
				break;
		}
		get_token();
		if(this_token.type!=__comma && this_token.type!=__right_bracket)
		{
			++error;
			std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \',\' or \']\' here."<<std::endl;
			return new_node;
		}
		else if(this_token.type==__right_bracket)
			break;
	}
	return new_node;
}

abstract_syntax_tree balloon_parse::hash_generate()
{
	abstract_syntax_tree new_node;
	abstract_syntax_tree temp;
	new_node.set_type(__hash);
	get_token();
	if(this_token.type!=__left_brace)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \'{\' here."<<std::endl;
		return new_node;
	}
	new_node.set_line(this_token.line);
	get_token();
	if(this_token.type!=__right_brace)
		parse.push(this_token);
	while(this_token.type!=__right_brace)
	{
		temp.set_clear();
		temp.set_type(__id);
		get_token();
		if(this_token.type!=__id)
		{
			++error;
			std::cout<<">>[Parse-error] line "<<this_token.line<<": expect an identifier here."<<std::endl;
			return new_node;
		}
		temp.set_line(this_token.line);
		temp.set_name(this_token.str);
		get_token();
		if(this_token.type!=__colon)
		{
			++error;
			std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \':\' here."<<std::endl;
			return new_node;
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
			case __id:parse.push(this_token);temp.add_child(scalar());break;
			case __right_brace:parse.push(this_token);break;
			default:
				++error;
				std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a scalar or \'}\' here."<<std::endl;
				return new_node;
				break;
		}
		new_node.add_child(temp);
		get_token();
		if(this_token.type!=__comma && this_token.type!=__right_brace)
		{
			++error;
			std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \',\' or \'}\' here."<<std::endl;
			return new_node;
		}
	}
	return new_node;
}

abstract_syntax_tree balloon_parse::check_number()
{
	abstract_syntax_tree new_node;
	new_node.set_type(__number);
	get_token();
	if(this_token.type!=__number)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a number here."<<std::endl;
		return new_node;
	}
	new_node.set_line(this_token.line);
	new_node.set_number(this_token.str);
	return new_node;
}

abstract_syntax_tree balloon_parse::check_string()
{
	abstract_syntax_tree new_node;
	new_node.set_type(__string);
	get_token();
	if(this_token.type!=__string)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a string here."<<std::endl;
		return new_node;
	}
	new_node.set_line(this_token.line);
	new_node.set_string(this_token.str);
	return new_node;
}

abstract_syntax_tree balloon_parse::check_unary()
{
	abstract_syntax_tree new_node;
	get_token();
	if(this_token.type!=__nor_operator && this_token.type!=__sub_operator)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a unary operator here."<<std::endl;
		return new_node;
	}
	new_node.set_line(this_token.line);
	new_node.set_type(this_token.type);
	get_token();
	switch(this_token.type)
	{
		case __number:parse.push(this_token);new_node.add_child(check_number());break;
		case __string:parse.push(this_token);new_node.add_child(check_string());break;
		case __id:parse.push(this_token);new_node.add_child(call_identifier());break;
		case __left_curve:parse.push(this_token);new_node.add_child(scalar());break;
		case __nor_operator:parse.push(this_token);new_node.add_child(check_unary());break;
		default:++error;std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a scalar here."<<std::endl;break;
	}
	return new_node;
}

abstract_syntax_tree balloon_parse::block()
{
	abstract_syntax_tree new_node;
	abstract_syntax_tree temp;
	new_node.set_type(__block);
	get_token();
	if(this_token.type!=__left_brace)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \'{\' ."<<std::endl;
		return new_node;
	}
	new_node.set_line(this_token.line);
	while(1)
	{
		get_token();
		switch(this_token.type)
		{
			case __var:parse.push(this_token);new_node.add_child(definition());check_semi();break;
			case __left_curve:
			case __left_bracket:
			case __left_brace:
			case __nor_operator:
			case __sub_operator:
			case __number:
			case __string:
			case __id:parse.push(this_token);new_node.add_child(scalar());check_semi();break;
			case __if:parse.push(this_token);new_node.add_child(choose());break;
			case __for:
			case __forindex:
			case __foreach:
			case __while:parse.push(this_token);new_node.add_child(loop());break;
			case __semi:break;
			case __continue:
			case __break:temp.set_clear();temp.set_type(this_token.type);new_node.add_child(temp);check_semi();break;
			case __return:parse.push(this_token);new_node.add_child(ret());check_semi();break;
			case __right_brace:parse.push(this_token);break;
			default:
				++error;
				std::cout<<">>[Parse-error] line "<<this_token.line<<": \'";
				print_detail_token(this_token.type);
				std::cout<<"\' should not appear in this scope."<<std::endl;
				return new_node;
				break;
		}
		get_token();
		if(this_token.type==__right_brace)
			break;
		else
			parse.push(this_token);
	}
	return new_node;
}

abstract_syntax_tree balloon_parse::func_generate()
{
	abstract_syntax_tree new_node;
	abstract_syntax_tree para;
	abstract_syntax_tree temp;
	new_node.set_type(__function);
	para.set_type(__parameter);
	get_token();
	if(this_token.type!=__func)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \'func\' here."<<std::endl;
		return new_node;
	}
	new_node.set_line(this_token.line);
	para.set_line(this_token.line);
	get_token();
	if(this_token.type==__left_curve)
	{
		get_token();
		if(this_token.type!=__right_curve)
			parse.push(this_token);
		while(this_token.type!=__right_curve)
		{
			get_token();
			if(this_token.type!=__id && this_token.type!=__dynamic_id)
			{
				++error;
				std::cout<<">>[Parse-error] line "<<this_token.line<<": expect identifier when declaring a new function."<<std::endl;
				return new_node;
			}
			temp.set_clear();
			temp.set_line(this_token.line);
			temp.set_type(this_token.type);
			temp.set_name(this_token.str);
			para.add_child(temp);
			if(this_token.type==__dynamic_id)
			{
				get_token();
				if(this_token.type!=__right_curve)
				{
					++error;
					std::cout<<">>[Parse-error] line "<<this_token.line<<": dynamic identifier should have \')\' behind it ."<<std::endl;
					return new_node;
				}
			}
			else
			{
				get_token();
				if(this_token.type!=__comma && this_token.type!=__right_curve)
				{
					++error;
					std::cout<<">>[Parse-error] line "<<this_token.line<<": expect \',\' or \')\' here."<<std::endl;	
					return new_node;
				}
			}
		}
	}
	else
		parse.push(this_token);
	new_node.add_child(para);
	new_node.add_child(block());
	return new_node;
}

abstract_syntax_tree balloon_parse::call_identifier()
{
	abstract_syntax_tree new_node;
	abstract_syntax_tree temp;
	get_token();
	if(this_token.type!=__id)
	{
		++error;
		std::cout<<">>[Parse-error] line "<<this_token.line<<": must have an identifier here."<<std::endl;
		return new_node;
	}
	new_node.set_line(this_token.line);
	new_node.set_type(__id);
	new_node.set_name(this_token.str);
	while(1)
	{
		get_token();
		if(this_token.type==__left_curve)
		{
			temp.set_clear();
			temp.set_line(this_token.line);
			temp.set_type(__call_function);
			get_token();
			if(this_token.type!=__right_curve)
				parse.push(this_token);
			while(this_token.type!=__right_curve)
			{
				temp.add_child(scalar());
				get_token();
				if(this_token.type!=__comma && this_token.type!=__right_curve)
				{
					++error;
					std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \',\' or \')\' here."<<std::endl;
					return new_node;
				}
			}
			new_node.add_child(temp);
		}
		else if(this_token.type==__left_bracket)
		{
			temp.set_clear();
			temp.set_line(this_token.line);
			temp.set_type(__call_array);
			temp.add_child(scalar());
			get_token();
			if(this_token.type!=__right_bracket)
			{
				++error;
				std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \']\' ."<<std::endl;
				return new_node;
			}
			new_node.add_child(temp);
		}
		else if(this_token.type==__dot)
		{
			get_token();
			if(this_token.type!=__id)
			{
				++error;
				std::cout<<">>[Parse-error] line "<<this_token.line<<": expect an identifier when calling hash."<<std::endl;
				return new_node;
			}
			temp.set_clear();
			temp.set_line(this_token.line);
			temp.set_type(__call_hash);
			temp.set_name(this_token.str);
			new_node.add_child(temp);
		}
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
		temp=new_node;
		new_node=assignment();
		new_node.set_line(this_token.line);
		new_node.get_children().push_front(temp);
	}
	else
		parse.push(this_token);
	return new_node;
}

abstract_syntax_tree balloon_parse::calculation()
{
	abstract_syntax_tree new_node;
	new_node=calculation_or();
	return new_node;
}

abstract_syntax_tree balloon_parse::calculation_or()
{
	abstract_syntax_tree new_node;
	abstract_syntax_tree temp;
	new_node=calculation_and();
	get_token();
	if(this_token.type!=__or_operator)
	{
		parse.push(this_token);
		return new_node;
	}
	temp.set_line(this_token.line);
	temp.set_type(__or_operator);
	temp.add_child(new_node);
	while(1)
	{
		temp.add_child(calculation_and());
		new_node=temp;
		temp.set_clear();
		get_token();
		if(this_token.type!=__or_operator)
		{
			parse.push(this_token);
			break;
		}
		temp.set_line(this_token.line);
		temp.set_type(__or_operator);
		temp.add_child(new_node);
	}
	return new_node;
}

abstract_syntax_tree balloon_parse::calculation_and()
{
	abstract_syntax_tree new_node;
	abstract_syntax_tree temp;
	new_node=calculation_cmp();
	get_token();
	if(this_token.type!=__and_operator)
	{
		parse.push(this_token);
		return new_node;
	}
	temp.set_line(this_token.line);
	temp.set_type(__and_operator);
	temp.add_child(new_node);
	while(1)
	{
		temp.add_child(calculation_cmp());
		new_node=temp;
		temp.set_clear();
		get_token();
		if(this_token.type!=__and_operator)
		{
			parse.push(this_token);
			break;
		}
		temp.set_line(this_token.line);
		temp.set_type(__and_operator);
		temp.add_child(new_node);
	}
	return new_node;
}

abstract_syntax_tree balloon_parse::calculation_cmp()
{
	abstract_syntax_tree new_node;
	abstract_syntax_tree temp;
	new_node=calculation_additive();
	get_token();
	if(this_token.type!=__cmp_equal && this_token.type!=__cmp_not_equal
		&& this_token.type!=__cmp_less && this_token.type!=__cmp_less_or_equal
		&& this_token.type!=__cmp_more && this_token.type!=__cmp_more_or_equal)
	{
		parse.push(this_token);
		return new_node;
	}
	temp.set_line(this_token.line);
	temp.set_type(this_token.type);
	temp.add_child(new_node);
	while(1)
	{
		temp.add_child(calculation_additive());
		new_node=temp;
		temp.set_clear();
		get_token();
		if(this_token.type!=__cmp_equal && this_token.type!=__cmp_not_equal
		&& this_token.type!=__cmp_less && this_token.type!=__cmp_less_or_equal
		&& this_token.type!=__cmp_more && this_token.type!=__cmp_more_or_equal)
		{
			parse.push(this_token);
			break;
		}
		temp.set_line(this_token.line);
		temp.set_type(this_token.type);
		temp.add_child(new_node);
	}
	return new_node;
}

abstract_syntax_tree balloon_parse::calculation_additive()
{
	abstract_syntax_tree new_node;
	abstract_syntax_tree temp;
	new_node=calculation_multive();
	get_token();
	if(this_token.type!=__add_operator && this_token.type!=__sub_operator && this_token.type!=__link_operator)
	{
		parse.push(this_token);
		return new_node;
	}
	temp.set_line(this_token.line);
	temp.set_type(this_token.type);
	temp.add_child(new_node);
	while(1)
	{
		temp.add_child(calculation_multive());
		new_node=temp;
		temp.set_clear();
		get_token();
		if(this_token.type!=__add_operator && this_token.type!=__sub_operator && this_token.type!=__link_operator)
		{
			parse.push(this_token);
			break;
		}
		temp.set_line(this_token.line);
		temp.set_type(this_token.type);
		temp.add_child(new_node);
	}
	return new_node;
}

abstract_syntax_tree balloon_parse::calculation_multive()
{
	abstract_syntax_tree new_node;
	abstract_syntax_tree temp;
	get_token();
	new_node.set_line(this_token.line);
	switch(this_token.type)
	{
		case __left_curve:
			new_node=scalar();get_token();
			if(this_token.type!=__right_curve)
			{
				++error;
				std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \')\' here."<<std::endl;
				return new_node;
			}
			break;
		case __number:parse.push(this_token);new_node=check_number();break;
		case __string:parse.push(this_token);new_node=check_string();break;
		case __id:parse.push(this_token);new_node=call_identifier();break;
		case __nor_operator:
		case __sub_operator:parse.push(this_token);new_node=check_unary();break;
		default:
			++error;
			std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a scalar here."<<std::endl;
			return new_node;
			break;
	}
	
	get_token();
	if(this_token.type!=__mul_operator && this_token.type!=__div_operator)
	{
		parse.push(this_token);
		return new_node;
	}
	temp.set_line(this_token.line);
	temp.set_type(this_token.type);
	temp.add_child(new_node);
	while(1)
	{
		get_token();
		switch(this_token.type)
		{
			case __left_curve:
				temp.add_child(scalar());get_token();
				if(this_token.type!=__right_curve)
				{
					++error;
					std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a \')\' here."<<std::endl;
					return new_node;
				}
				break;
			case __number:parse.push(this_token);temp.add_child(check_number());break;
			case __string:parse.push(this_token);temp.add_child(check_string());break;
			case __id:parse.push(this_token);temp.add_child(call_identifier());break;
			case __nor_operator:
			case __sub_operator:parse.push(this_token);temp.add_child(check_unary());break;
			default:
				++error;
				std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a scalar here."<<std::endl;
				return new_node;
				break;
		}
		new_node=temp;
		temp.set_clear();
		get_token();
		if(this_token.type!=__mul_operator && this_token.type!=__div_operator)
		{
			parse.push(this_token);
			break;
		}
		temp.set_line(this_token.line);
		temp.set_type(this_token.type);
		temp.add_child(new_node);
	}
	return new_node;
}

abstract_syntax_tree balloon_parse::scalar()
{
	abstract_syntax_tree new_node;
	get_token();
	new_node.set_line(this_token.line);
	switch(this_token.type)
	{
		case __func:parse.push(this_token);new_node=func_generate();break;
		case __left_bracket:parse.push(this_token);new_node=array_generate();break;
		case __left_brace:parse.push(this_token);new_node=hash_generate();break;
		case __left_curve:
		case __nor_operator:
		case __sub_operator:
		case __number:
		case __string:
		case __id:parse.push(this_token);new_node=calculation();break;
		default:
			++error;
			std::cout<<">>[Parse-error] line "<<this_token.line<<": expect a scalar at this place."<<std::endl;
			parse.push(this_token);
			break;
	}
	return new_node;
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
	root.set_clear();
	root.set_type(__root);
	root.set_line(1);
	error=0;
	warning=0;
	
	while(!parse.empty())
	{
		get_token();
		switch(this_token.type)
		{
			case __var:parse.push(this_token);root.add_child(definition());check_semi();break;
			case __left_curve:
			case __left_bracket:
			case __left_brace:
			case __nor_operator:
			case __sub_operator:
			case __number:
			case __string:
			case __id:parse.push(this_token);root.add_child(scalar());check_semi();break;
			case __if:parse.push(this_token);root.add_child(choose());break;
			case __for:
			case __forindex:
			case __foreach:
			case __while:parse.push(this_token);root.add_child(loop());break;
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
