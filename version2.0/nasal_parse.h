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
		void main_generate();
		abstract_syntax_tree scalar_generate();
		abstract_syntax_tree var_outside_definition();
		abstract_syntax_tree loop_expr();
		abstract_syntax_tree choose_expr();
};

void nasal_parse::print_detail_token()
{
	std::stack<token> tmp=parse_token_stream;
	std::string space="";
	int line=1;
	std::cout<<line<<"\t";
	while(!tmp.empty())
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
		{
			std::string str="";
			for(int i=0;i<space.length()-1;++i)
				str+=space[i];
			space=str;
		}
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

void nasal_parse::main_generate()
{
	error=0;
	warning=0;
	// initialize error and warning
	root.set_clear();
	root.set_line(1);
	root.set_type(__root);
	// initialize root node
	while(!parse_token_stream.empty())
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
			case __left_curve:   case __left_bracket: case __left_brace:
			case __func:
				this->push_token();
				root.add_child(scalar_generate());
				break;
			case __if:
				this->push_token();
				root.add_child(choose_expr());
				break;
			case __while: case __for: case __foreach: case __forindex:
				this->push_token();
				root.add_child(loop_expr());
				break;
			case __semi:break;
			default:
				++error;
				print_parse_error(error_token_in_main,this_token.line,this_token.type);
				break;
		}
	}
	std::cout<<">>[Parse] complete generation. "<<error<<" error(s), "<<warning<<" warning(s)."<<std::endl;
	return;
}

abstract_syntax_tree nasal_parse::scalar_generate()
{
	this->get_token();
	abstract_syntax_tree scalar_node;
	switch(this_token.type)
	{
		case __nor_operator:
		case __sub_operator:
			break;
		case __number:
		case __string:
		case __nil:
		case __id:
			break;
		case __left_curve:break;
		case __left_brace:break;
		case __left_bracket:break;
		case __func:break;
	}
	return scalar_node;
}

abstract_syntax_tree nasal_parse::var_outside_definition()
{
	abstract_syntax_tree definition_node;
	definition_node.set_type(__definition);
	this->get_token();// get 'var'
	definition_node.set_line(this_token.line);
	this->get_token();
	if(this_token.type==__id)
	{
		abstract_syntax_tree new_var_identifier;
		new_var_identifier.set_type(__id);
		new_var_identifier.set_line(this_token.line);
		new_var_identifier.set_name(this_token.str);
		definition_node.add_child(new_var_identifier);
		this->get_token();
		if(this_token.type==__semi)
			this->push_token();// var id
		else if(this_token.type==__equal)
			definition_node.add_child(scalar_generate());// var id = scalar
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
		multi_identifier.set_type(__multi_id);
		multi_identifier.set_line(this_token.line);
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
				multi_identifier.add_child(scalar_generate());
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
		definition_node.add_child(multi_identifier);
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
	return definition_node;
}

abstract_syntax_tree nasal_parse::loop_expr()
{
	abstract_syntax_tree loop_main_node;
	loop_main_node.set_type(__loop);
	this->get_token();
	loop_main_node.set_line(this_token.line);
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
	choose_main_node.set_type(__ifelse);
	this->get_token();// get 'if'
	choose_main_node.set_line(this_token.line);
	return choose_main_node;
}
#endif
