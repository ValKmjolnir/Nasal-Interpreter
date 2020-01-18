#ifndef __NASAL_PARSE_H__
#define __NASAL_PARSE_H__

class nasal_parse
{
	private:
		std::stack<token> parse_token_stream;
		std::stack<token> checked_tokens;
		token this_token;
		int error;
		int warning;
		abstract_syntax_tree root;
	public:
		// basic
		void print_detail_token();
		void get_token_list(std::list<token>&);
		void get_token();
		void push_token();
		int get_error();
		abstract_syntax_tree& get_root();
		
		// abstract_syntax_tree generation
		bool check_comma_in_curve();
		void main_generate();
		abstract_syntax_tree calculation();
		abstract_syntax_tree and_calculation();
		abstract_syntax_tree or_calculation();
		abstract_syntax_tree additive_calculation();
		abstract_syntax_tree multive_calculation();
		abstract_syntax_tree scalar_generate();
		abstract_syntax_tree hash_generate();
		abstract_syntax_tree vector_generate();
		abstract_syntax_tree function_generate();
		abstract_syntax_tree var_outside_definition();
		abstract_syntax_tree var_inside_definition();
		abstract_syntax_tree normal_assignment();
		abstract_syntax_tree in_curve_assignment();
		abstract_syntax_tree loop_expr();
		abstract_syntax_tree choose_expr();
};

void nasal_parse::print_detail_token()
{
	std::stack<token> tmp=parse_token_stream;
	std::string space="";
	int line=1;
	std::cout<<line<<"\t";
	while(tmp.top().type!=__stack_end)
	{
		if(tmp.top().line!=line)
		{
			for(int i=line+1;i<tmp.top().line;++i)
				std::cout<<std::endl<<i<<"\t";
			line=tmp.top().line;
			std::cout<<std::endl<<line<<"\t"<<space;
		}
		print_parse_token(tmp.top().type);
		if(tmp.top().type==__left_brace)
			space+=' ';
		std::cout<<" ";
		tmp.pop();
		if(!tmp.empty() && tmp.top().type==__right_brace)
			space.pop_back();
	}
	std::cout<<std::endl;
	return;
}

void nasal_parse::get_token_list(std::list<token>& detail_token_stream)
{
	while(!parse_token_stream.empty())
		parse_token_stream.pop();
	while(!checked_tokens.empty())
		checked_tokens.pop();
	token end_token;
	end_token.line=0;
	end_token.str="stack_end";
	end_token.type=__stack_end;
	parse_token_stream.push(end_token);
	checked_tokens.push(end_token);
	// clear stacks and initialize them with end_token

	std::stack<token> backward_tmp;
	// backward_tmp is used to backward detail_token_stream
	// to the parse_token_stream so get_token can get token
	// according to the correct subsequence.
	for(std::list<token>::iterator i=detail_token_stream.begin();i!=detail_token_stream.end();++i)
		backward_tmp.push(*i);
	while(!backward_tmp.empty())
	{
		parse_token_stream.push(backward_tmp.top());
		backward_tmp.pop();
	}
	return;
}

void nasal_parse::get_token()
{
	this_token=parse_token_stream.top();
	parse_token_stream.pop();
	checked_tokens.push(this_token);
	if(this_token.type==__stack_end)
	{
		std::cout<<">>[Parse-error] fatal error occurred."<<std::endl;
		std::cout<<">>[Parse-error] empty token stack."<<std::endl;
	}
	return;
}

void nasal_parse::push_token()
{
	parse_token_stream.push(checked_tokens.top());
	this_token=checked_tokens.top();
	checked_tokens.pop();
	if(this_token.type==__stack_end)
	{
		std::cout<<">>[Parse-error] fatal error occurred."<<std::endl;
		std::cout<<">>[Parse-error] empty checked-token stack."<<std::endl;
	}
	return;
}

int nasal_parse::get_error()
{
	// if error is larger than 0,
	// the process will stop.
	return error;
}

abstract_syntax_tree& nasal_parse::get_root()
{
	std::cout<<">>[Abstract-syntax-tree] get root address: "<<(&root)<<" ."<<std::endl;
	return root;
}

bool nasal_parse::check_comma_in_curve()
{
	bool ret=false;
	int cnt=0;
	while(this_token.type!=__right_curve)
	{
		++cnt;
		this->get_token();
		if(this_token.type==__comma)
		{
			ret=true;
			break;
		}
	}
	for(int i=0;i<cnt;++i)
		this->push_token();
	return ret;
}

void nasal_parse::main_generate()
{
	error=0;
	warning=0;
	// initialize error and warning
	root.set_clear();
	root.set_node_line(1);
	root.set_node_type(__root);
	// initialize root node
	while(parse_token_stream.top().type!=__stack_end)
	{
		this->get_token();
		switch(this_token.type)
		{
			case __var:
				this->push_token();
				root.get_children().push_back(var_outside_definition());
				break;
			case __nor_operator: case __sub_operator:
			case __number:       case __nil:          case __string:     case __id:
			case __left_bracket: case __left_brace:
			case __func:
				this->push_token();
				root.add_children(calculation());
				break;
			case __left_curve:
				this->push_token();
				if(check_comma_in_curve())
				{
					this->get_token();// get '('
					this->get_token();// check if there is a 'var'
					if(this_token.type==__var)
					{
						this->push_token();
						this->push_token();
						root.add_children(var_inside_definition());
					}
					else
					{
						this->push_token();
						this->push_token();
						root.add_children(in_curve_assignment());
					}
				}
				else
					root.add_children(calculation());
			// '(' is the beginning of too many statements
			// '(' var id,id,id ')'
			// '(' calculation ')'
			// '(' scalar,scalar,scalar ')' '=' '(' scalar,scalar,scalar ')'
			// but these statements can be distinguished by commas in them
				break;
			case __if:
				this->push_token();
				root.add_children(choose_expr());
				break;
			case __while: case __for: case __foreach: case __forindex:
				this->push_token();
				root.add_children(loop_expr());
				break;
			case __semi:break;
			case __stack_end:break;
			default:
				++error;
				print_parse_error(error_token_in_main,this_token.line,this_token.type);
				break;
		}
	}
	std::cout<<">>[Parse] complete generation. "<<error<<" error(s), "<<warning<<" warning(s)."<<std::endl;
	return;
}

abstract_syntax_tree nasal_parse::calculation()
{
	abstract_syntax_tree calc_node;
	abstract_syntax_tree tmp_node;
	calc_node=and_calculation();
	this->get_token();
	while(this_token.type==__or_operator)
	{
		tmp_node.set_node_line(this_token.line);
		tmp_node.set_node_type(this_token.type);
		tmp_node.add_children(calc_node);
		tmp_node.add_children(and_calculation());
		calc_node=tmp_node;
		this->get_token();
	}
	if(this_token.type==__ques_mark)
	{
		// <expr> '?' <expr> ';' <expr>
		tmp_node.set_node_line(this_token.line);
		tmp_node.set_node_type(__ques_mark);
		tmp_node.add_children(calc_node);
		tmp_node.add_children(calculation());
		this->get_token();
		if(this_token.type!=__colon)
		{
			++error;
			print_parse_error(ternary_operator_lack_colon,this_token.line,this_token.type);
		}
		tmp_node.add_children(calculation());
		calc_node=tmp_node;
	}
	else
		this->push_token();
	return calc_node;
}

abstract_syntax_tree nasal_parse::and_calculation()
{
	abstract_syntax_tree calc_node;
	abstract_syntax_tree tmp_node;
	calc_node=or_calculation();
	this->get_token();
	while(this_token.type==__and_operator)
	{
		tmp_node.set_node_line(this_token.line);
		tmp_node.set_node_type(this_token.type);
		tmp_node.add_children(calc_node);
		tmp_node.add_children(or_calculation());
		calc_node=tmp_node;
		this->get_token();
	}
	this->push_token();
	return calc_node;
}

abstract_syntax_tree nasal_parse::or_calculation()
{
	abstract_syntax_tree calc_node;
	abstract_syntax_tree tmp_node;
	calc_node=additive_calculation();
	this->get_token();
	while(this_token.type==__or_operator)
	{
		tmp_node.set_node_line(this_token.line);
		tmp_node.set_node_type(this_token.type);
		tmp_node.add_children(calc_node);
		tmp_node.add_children(additive_calculation());
		calc_node=tmp_node;
		this->get_token();
	}
	this->push_token();
	return calc_node;
}

abstract_syntax_tree nasal_parse::additive_calculation()
{
	abstract_syntax_tree calc_node;
	abstract_syntax_tree tmp_node;
	calc_node=multive_calculation();
	this->get_token();
	while((this_token.type==__add_operator) || (this_token.type==__sub_operator))
	{
		tmp_node.set_node_line(this_token.line);
		tmp_node.set_node_type(this_token.type);
		tmp_node.add_children(calc_node);
		tmp_node.add_children(multive_calculation());
		calc_node=tmp_node;
		this->get_token();
	}
	this->push_token();
	return calc_node;
}

abstract_syntax_tree nasal_parse::multive_calculation()
{
	abstract_syntax_tree calc_node;
	abstract_syntax_tree tmp_node;
	this->get_token();
	if((this_token.type==__sub_operator) || (this_token.type==__nor_operator))
	{
		calc_node.set_node_line(this_token.line);
		calc_node.set_node_type(this_token.type);
		abstract_syntax_tree null_node;
		null_node.set_node_line(this_token.line);
		null_node.set_node_type(__number);
		null_node.set_var_number("0");
		calc_node.add_children(null_node);
		calc_node.add_children(scalar_generate());
	}
	else
	{
		this->push_token();
		calc_node=scalar_generate();
	}
	this->get_token();
	while((this_token.type==__mul_operator) || (this_token.type==__div_operator))
	{
		tmp_node.set_node_line(this_token.line);
		tmp_node.set_node_type(this_token.type);
		tmp_node.add_children(calc_node);
		tmp_node.add_children(scalar_generate());
		calc_node=tmp_node;
		this->get_token();
	}
	this->push_token();
	return calc_node;
}

abstract_syntax_tree nasal_parse::scalar_generate()
{
	this->get_token();
	abstract_syntax_tree scalar_node;
	scalar_node.set_node_line(this_token.line);

	switch(this_token.type)
	{
		case __number:scalar_node.set_node_type(__number);scalar_node.set_var_number(this_token.str);break;
		case __string:scalar_node.set_node_type(__string);scalar_node.set_var_string(this_token.str);break;
		case __nil:scalar_node.set_node_type(__nil);break;
		case __id:scalar_node.set_node_type(__id);scalar_node.set_var_name(this_token.str);break;
		case __left_curve:
			scalar_node=calculation();
			this->get_token();
			if(this_token.type!=__right_curve)
			{
				++error;
				print_parse_error(lack_right_curve,this_token.line);
			}
			break;
		case __left_brace:// hash
			this->push_token();
			scalar_node=hash_generate();
			break;
		case __left_bracket:// vector
			this->push_token();
			scalar_node=vector_generate();
			break;
		case __func:
			this->get_token();
			if(this_token.type!=__id)
			{
				this->push_token();
				this->push_token();
				scalar_node=function_generate();
				// function
			}
			else
			{
				scalar_node.set_node_type(__id);
				scalar_node.set_var_name(this_token.str);
				// func id
			}
			break;
		default:
			++error;
			print_parse_error(error_begin_token_of_scalar,this_token.line,this_token.type);
			break;
	}
	this->get_token(); // check if there is a '(' or '[' or '{' after id
	while((this_token.type==__left_curve) || (this_token.type==__left_bracket) || (this_token.type==__dot))
	{
		if(this_token.type==__left_curve)
		{
			// call function
			abstract_syntax_tree call_func_node;
			call_func_node.set_node_line(this_token.line);
			call_func_node.set_node_type(__call_function);
		}
		else if(this_token.type==__left_bracket)
		{
			abstract_syntax_tree call_vector_node;
			call_vector_node.set_node_line(this_token.line);
			call_vector_node.set_node_type(__call_vector);
			// there are many kinds of ways to call a vector
			// such as: id[0] id[0:12] id[-2:0] id[2:] id[4,3,1,5,2]
			abstract_syntax_tree tmp=calculation();
			this->get_token();
			if(this_token.type==__colon)
			{
				abstract_syntax_tree subvec_node;
				
				this->get_token();
				subvec_node.set_node_line(this_token.line);
				subvec_node.set_node_type(__sub_vector);
				subvec_node.add_children(tmp);
				if(this_token.type!=__right_bracket)
				{
					this->push_token();
					subvec_node.add_children(calculation());
				}
				else
					this->push_token();
				call_vector_node.add_children(subvec_node);
			}
			else if(this_token.type==__comma)
			{
				call_vector_node.add_children(tmp);
				while(this_token.type!=__right_bracket)
				{
					call_vector_node.add_children(calculation());
					this->get_token();
					if(this_token.type!=__comma && this_token.type!=__right_bracket)
					{
						++error;
						print_parse_error(call_vector_lack_bracket,this_token.line,this_token.type);
						break;
					}
				}
				this->push_token();
			}
			else if(this_token.type==__right_bracket)
			{
				this->push_token();
				call_vector_node.add_children(tmp);
			}
			this->get_token();
			if(this_token.type!=__right_bracket)
			{
				++error;
				print_parse_error(call_vector_lack_bracket,this_token.line,this_token.type);
				break;
			}
			scalar_node.add_children(call_vector_node);
		}
		else if(this_token.type==__dot)
		{
			this->get_token();
			if(this_token.type!=__id)
			{
				++error;
				print_parse_error(call_hash_lack_id,this_token.line);
				break;
			}
			abstract_syntax_tree call_hash_node;
			call_hash_node.set_node_line(this_token.line);
			call_hash_node.set_node_type(__call_hash);
			call_hash_node.set_var_name(this_token.str);
			scalar_node.add_children(call_hash_node);
		}
		this->get_token();
	}
	this->push_token();
	return scalar_node;
}

abstract_syntax_tree nasal_parse::hash_generate()
{
	this->get_token(); // get '{'
	abstract_syntax_tree hash_node;
	hash_node.set_node_line(this_token.line);
	hash_node.set_node_type(__hash);
	this->get_token();
	if(this_token.type!=__right_brace)
	{
		this->push_token();
		while(this_token.type!=__right_brace)
		{
			abstract_syntax_tree hash_member_node;
			this->get_token();
			hash_member_node.set_node_line(this_token.line);
			hash_member_node.set_node_type(__hash_member);
			if(this_token.type==__id || this_token.type==__string)
			{
				abstract_syntax_tree member_id;
				member_id.set_node_line(this_token.line);
				member_id.set_node_type(__id);
				member_id.set_var_name(this_token.str);
				hash_member_node.add_children(member_id);
			}
			else
			{
				++error;
				print_parse_error(hash_gen_lack_id,this_token.line,this_token.type);
				break;
			}
			this->get_token();
			if(this_token.type!=__colon)
			{
				++error;
				print_parse_error(hash_gen_lack_colon,this_token.line,this_token.type);
				break;
			}
			hash_member_node.add_children(calculation());
			this->get_token();
			if(this_token.type!=__comma && this_token.type!=__right_brace)
			{
				++error;
				print_parse_error(hash_gen_lack_end,this_token.line,this_token.type);
				break;
			}
			if(this_token.type==__comma)
			{
				this->get_token();
				if(this_token.type!=__right_brace)
					this->push_token();
				// {name:scalar,} 
			}
			hash_node.add_children(hash_member_node);
		}
	}
	return hash_node;
}

abstract_syntax_tree nasal_parse::vector_generate()
{
	this->get_token(); // get '['
	abstract_syntax_tree vector_node;
	vector_node.set_node_line(this_token.line);
	vector_node.set_node_type(__vector);
	this->get_token();
	if(this_token.type!=__right_bracket)
	{
		this->push_token();
		while(this_token.type!=__right_bracket)
		{
			vector_node.add_children(calculation());
			this->get_token();
			if(this_token.type!=__comma && this_token.type!=__right_bracket)
			{
				++error;
				print_parse_error(vector_gen_lack_end,this_token.line,this_token.type);
				break;
			}
			if(this_token.type==__comma)
			{
				this->get_token();
				if(this_token.type!=__right_bracket)
					this->push_token();
				// [0,1,2,]
			}
		}
	}
	return vector_node;
}

abstract_syntax_tree nasal_parse::function_generate()
{
	abstract_syntax_tree function_node;
	abstract_syntax_tree parameter_list;
	this->get_token(); // get 'func'
	function_node.set_node_type(__function);
	function_node.set_node_line(this_token.line);
	parameter_list.set_node_type(__parameters);
	parameter_list.set_node_line(this_token.line);
	this->get_token();
	if(this_token.type==__left_curve)
	{
		while(this_token.type!=__right_curve)
		{
			// check identifier
			this->get_token();
			if(this_token.type==__id)
			{
				abstract_syntax_tree parameter;
				parameter.set_node_line(this_token.line);
				parameter.set_node_type(__id);
				parameter.set_var_name(this_token.str);
			}
			else if(this_token.type==__dynamic_id)
			{
				abstract_syntax_tree parameter;
				parameter.set_node_line(this_token.line);
				parameter.set_node_type(__dynamic_id);
				parameter.set_var_name(this_token.str);
			}
			else if(this_token.type==__right_curve)
				this->push_token();
			else
			{
				++error;
				print_parse_error(parameter_lack_part,this_token.line);
				break;
			}

			// check equal operator
			this->get_token();
			if(this_token.type==__equal)
			{

			}
			else if((this_token.type==__right_curve) || (this_token.type==__comma))
				this->push_token();
			else
			{
				++error;
				print_parse_error(parameter_lack_part,this_token.line);
				break;
			}

			// check comma or right_curve
			this->get_token();
			if((this_token.type!=__right_curve) && (this_token.type!=__comma))
			{
				++error;
				print_parse_error(parameter_lack_curve,this_token.line);
				break;
			}
		}
	}
	else
		this->push_token();
	return function_node;
}

abstract_syntax_tree nasal_parse::var_outside_definition()
{
	abstract_syntax_tree var_outsied_definition_node;
	var_outsied_definition_node.set_node_type(__definition);
	this->get_token();// get 'var'
	var_outsied_definition_node.set_node_line(this_token.line);
	this->get_token();
	if(this_token.type==__id)
	{
		abstract_syntax_tree new_var_identifier;
		new_var_identifier.set_node_type(__id);
		new_var_identifier.set_node_line(this_token.line);
		new_var_identifier.set_var_name(this_token.str);
		var_outsied_definition_node.add_children(new_var_identifier);
		this->get_token();
		if(this_token.type==__semi)
			this->push_token();// var id
		else if(this_token.type==__equal)
			var_outsied_definition_node.add_children(scalar_generate());// var id = scalar
		else
		{
			this->push_token();
			++error;
			print_parse_error(definition_lack_equal,this_token.line);
		}
	}
	else if(this_token.type==__left_curve)
	{
		abstract_syntax_tree multi_identifier;
		multi_identifier.set_node_type(__multi_id);
		multi_identifier.set_node_line(this_token.line);
		while(this_token.type!=__right_curve)
		{
			this->get_token();
			if(this_token.type!=__id)
			{
				this->push_token();
				++error;
				print_parse_error(definition_wrong_type,this_token.line);
				break;
			}
			else
			{
				this->push_token();
				multi_identifier.add_children(scalar_generate());
			}
			this->get_token();
			if(this_token.type!=__semi && this_token.type!=__right_curve)
			{
				this->push_token();
				++error;
				print_parse_error(multi_definition_need_curve,this_token.line);
				break;
			}
		}
		var_outsied_definition_node.add_children(multi_identifier);
		this->get_token();
		if(this_token.type==__semi)
			this->push_token();// var (id,id,id)
		else if(this_token.type==__equal)
		{
			;
		}
		else
		{
			this->push_token();
			++error;
			print_parse_error(definition_lack_equal,this_token.line);
		}
	}
	else
	{
		this->push_token();
		++error;
		print_parse_error(definition_lack_id,this_token.line);
	}
	return var_outsied_definition_node;
}

abstract_syntax_tree nasal_parse::var_inside_definition()
{
	abstract_syntax_tree var_inside_definition_node;
	var_inside_definition_node.set_node_type(__definition);
	this->get_token(); // get '('
	this->get_token(); // get 'var'
	var_inside_definition_node.set_node_line(this_token.line);
	return var_inside_definition_node;
}

abstract_syntax_tree nasal_parse::normal_assignment()
{
	abstract_syntax_tree assign_node;
	assign_node.set_node_type(__assignment);
	return assign_node;
}

abstract_syntax_tree nasal_parse::in_curve_assignment()
{
	abstract_syntax_tree assign_node;
	this->get_token(); // get '('
	assign_node.set_node_line(this_token.line);
	assign_node.set_node_type(__assignment);
	return assign_node;
}

abstract_syntax_tree nasal_parse::loop_expr()
{
	abstract_syntax_tree loop_main_node;
	loop_main_node.set_node_type(__loop);
	this->get_token(); // get the first token of loop
	loop_main_node.set_node_line(this_token.line);
	switch(this_token.type)
	{
		case __for:
		case __while:
		case __foreach:
		case __forindex:break;
	}
	return loop_main_node;
}

abstract_syntax_tree nasal_parse::choose_expr()
{
	abstract_syntax_tree choose_main_node;
	abstract_syntax_tree if_node;
	abstract_syntax_tree elsif_node;
	abstract_syntax_tree else_node;
	choose_main_node.set_node_type(__ifelse);
	// get 'if'
	this->get_token();
	choose_main_node.set_node_line(this_token.line);
	if_node.set_node_type(__if);
	if_node.set_node_line(this_token.line);
	this->get_token();
	if(this_token.type!=__left_curve)
	{
		++error;
		print_parse_error(lack_left_curve,this_token.line);
	}
	if_node.add_children(calculation());
	this->get_token();
	if(this_token.type!=__right_curve)
	{
		++error;
		print_parse_error(lack_right_curve,this_token.line);
	}
	// add statements

	// get elsif or else if
	return choose_main_node;
}
#endif
