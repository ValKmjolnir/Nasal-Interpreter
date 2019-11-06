#ifndef __ABSTRACT_SYNTAX_TREE_CPP__
#define __ABSTRACT_SYNTAX_TREE_CPP__

balloon_scope global;
var abstract_syntax_tree::get_value()
{
	var temp;
	temp.set_type(type);
	if(type==__number)
		temp.set_number(number);
	else if(type==__string)
		temp.set_string(str);
	else if(type==__array)
	{
		var new_var;
	}
	else if(type==__hash)
	{
		var new_var;
	}
	else if(type==__function)
		temp.set_function(*this);
	if(name.length()!=0)
		temp.set_name(name);
	return temp;
}
void abstract_syntax_tree::run_root()
{
	global.set_clear();
	int beg_time,end_time;
	int exit_type=__process_exited_successfully;
	beg_time=time(NULL);
	for(std::list<abstract_syntax_tree>::iterator i=children.begin();i!=children.end();++i)
	{
		if(i->type==__definition)
		{
			var new_var;
			std::list<abstract_syntax_tree>::iterator j=i->children.begin();
			std::string _name=j->name;
			if(!global.search_var(_name))
			{
				new_var.set_name(_name);
				global.add_var(new_var);
			}
			else
			{
				std::cout<<">>[Runtime-error] redeclaration of \'"<<_name<<"\'."<<std::endl;
				exit_type=__redeclaration;
				break;
			}
		}
	}
	end_time=time(NULL);
	std::cout<<">>[Runtime] process exited after "<<end_time-beg_time<<" sec(s) with returned state \'";
	print_exit_type(exit_type);
	std::cout<<"\'."<<std::endl;
	return;
}


		

		

#endif
