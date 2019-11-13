#ifndef __ABSTRACT_SYNTAX_TREE_CPP__
#define __ABSTRACT_SYNTAX_TREE_CPP__
#include "abstract_syntax_tree.h"


int exit_type=0;
std::stack<var> ret_stack;// for function ret use

var abstract_syntax_tree::calculation()
{
	var temp;
	temp.set_type(__null_type);
	if(this->type==__number)
	{
		temp.set_type(__var_number);
		temp.set_number(number);
		return temp;
	}
	else if(this->type==__string)
	{
		temp.set_type(__var_string);
		temp.set_string(str);
		return temp;
	}
	else if(this->type==__id)
	{
		temp=this->call_identifier();
		return temp;
	}
	if(this->type==__nor_operator)
	{
		temp.set_type(__var_number);
		temp=children.front().calculation();
		if(temp.get_type()!=__var_number)
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] must use a number to use \'!\' but use \'";
			print_scalar(temp.get_type());
			std::cout<<"\'."<<std::endl;
		}
		else
		{
			if(temp.get_number())
				temp.set_number(0);
			else
				temp.set_number(1);
		}
		return temp;
	}
	else if(this->type==__add_operator)
	{
		temp.set_type(__var_number);
		var left_child=children.front().calculation();
		var right_child=children.back().calculation();
		if(left_child.get_type()==__var_number && right_child.get_type()==__var_number)
			temp.set_number(left_child.get_number()+right_child.get_number());
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] must use a number to use \'+\' but use \'";
			print_scalar(left_child.get_type());
			std::cout<<"\' and \'";
			print_scalar(right_child.get_type());
			std::cout<<"\'."<<std::endl;
		}
	}
	else if(this->type==__sub_operator)
	{
		temp.set_type(__var_number);
		std::list<abstract_syntax_tree>::iterator i=children.begin();
		++i;
		if(i==children.end())
		{
			temp=children.front().calculation();
			if(temp.get_type()==__var_number)
				temp.set_number(temp.get_number()*(-1));
			else
			{
				exit_type=__error_value_type;
				std::cout<<">>[Runtime-error] must use a number to use \'-\' but use \'";
				print_scalar(temp.get_type());
				std::cout<<"\'."<<std::endl;
			}
		}
		else
		{
			var left_child=children.front().calculation();
			var right_child=children.back().calculation();
			if(left_child.get_type()==__var_number && right_child.get_type()==__var_number)
				temp.set_number(left_child.get_number()-right_child.get_number());
			else
			{
				exit_type=__error_value_type;
				std::cout<<">>[Runtime-error] must use a number to use \'-\' but use \'";
				print_scalar(left_child.get_type());
				std::cout<<"\' and \'";
				print_scalar(right_child.get_type());
				std::cout<<"\'."<<std::endl;
			}
		}
	}
	else if(this->type==__mul_operator)
	{
		temp.set_type(__var_number);
		var left_child=children.front().calculation();
		var right_child=children.back().calculation();
		if(left_child.get_type()==__var_number && right_child.get_type()==__var_number)
			temp.set_number(left_child.get_number()*right_child.get_number());
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] must use a number to use \'*\' but use \'";
			print_scalar(left_child.get_type());
			std::cout<<"\' and \'";
			print_scalar(right_child.get_type());
			std::cout<<"\'."<<std::endl;
		}
	}
	else if(this->type==__div_operator)
	{
		temp.set_type(__var_number);
		var left_child=children.front().calculation();
		var right_child=children.back().calculation();
		if(left_child.get_type()==__var_number && right_child.get_type()==__var_number)
		{
			temp.set_number(left_child.get_number()/right_child.get_number());
			if(std::isnan(temp.get_number()) || std::isinf(temp.get_number()))
			{
				exit_type=__sigfpe_arithmetic_exception;
				std::cout<<">>[Runtime-error] get number \'NaN\' or \'Inf\'."<<std::endl;
			}
		}
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] must use a number to use \'/\' but use \'";
			print_scalar(left_child.get_type());
			std::cout<<"\' and \'";
			print_scalar(right_child.get_type());
			std::cout<<"\'."<<std::endl;
		}
	}
	else if(this->type==__link_operator)
	{
		temp.set_type(__var_string);
		var left_child=children.front().calculation();
		var right_child=children.back().calculation();
		if(left_child.get_type()==__var_string && right_child.get_type()==__var_string)
			temp.set_string(left_child.get_string()+right_child.get_string());
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] must use a string to use \'~\' but use \'";
			print_scalar(left_child.get_type());
			std::cout<<"\' and \'";
			print_scalar(right_child.get_type());
			std::cout<<"\'."<<std::endl;
		}
	}
	else if(this->type==__cmp_equal)
	{
		temp.set_type(__var_number);
		var left_child=children.front().calculation();
		var right_child=children.back().calculation();
		if(left_child.get_type()==right_child.get_type())
		{
			if(left_child.get_type()==__var_number)
				temp.set_number((double)(left_child.get_number()==right_child.get_number()));
			else if(left_child.get_type()==__var_string)
				temp.set_number((double)(left_child.get_string()==right_child.get_string()));
			else
			{
				exit_type=__error_value_type;
				std::cout<<">>[Runtime-error] must use number or string to use \'==\' but use \'";
				print_scalar(left_child.get_type());
				std::cout<<"\'."<<std::endl;
			}
		}
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] must use same type to use \'==\' but use \'";
			print_scalar(left_child.get_type());
			std::cout<<"\' and \'";
			print_scalar(right_child.get_type());
			std::cout<<"\'."<<std::endl;
		}
	}
	else if(this->type==__cmp_not_equal)
	{
		temp.set_type(__var_number);
		var left_child=children.front().calculation();
		var right_child=children.back().calculation();
		if(left_child.get_type()==right_child.get_type())
		{
			if(left_child.get_type()==__var_number)
				temp.set_number((double)(left_child.get_number()!=right_child.get_number()));
			else if(left_child.get_type()==__var_string)
				temp.set_number((double)(left_child.get_string()!=right_child.get_string()));
			else
			{
				exit_type=__error_value_type;
				std::cout<<">>[Runtime-error] must use number or string to use \'!=\' but use \'";
				print_scalar(left_child.get_type());
				std::cout<<"\'."<<std::endl;
			}
		}
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] must use same type to use \'!=\' but use \'";
			print_scalar(left_child.get_type());
			std::cout<<"\' and \'";
			print_scalar(right_child.get_type());
			std::cout<<"\'."<<std::endl;
		}
	}
	else if(this->type==__cmp_less)
	{
		temp.set_type(__var_number);
		var left_child=children.front().calculation();
		var right_child=children.back().calculation();
		if(left_child.get_type()==right_child.get_type())
		{
			if(left_child.get_type()==__var_number)
				temp.set_number((double)(left_child.get_number()<right_child.get_number()));
			else if(left_child.get_type()==__var_string)
				temp.set_number((double)(left_child.get_string()<right_child.get_string()));
			else
			{
				exit_type=__error_value_type;
				std::cout<<">>[Runtime-error] must use number or string to use \'<\' but use \'";
				print_scalar(left_child.get_type());
				std::cout<<"\'."<<std::endl;
			}
		}
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] must use same type to use \'<\' but use \'";
			print_scalar(left_child.get_type());
			std::cout<<"\' and \'";
			print_scalar(right_child.get_type());
			std::cout<<"\'."<<std::endl;
		}
	}
	else if(this->type==__cmp_more)
	{
		temp.set_type(__var_number);
		var left_child=children.front().calculation();
		var right_child=children.back().calculation();
		if(left_child.get_type()==right_child.get_type())
		{
			if(left_child.get_type()==__var_number)
				temp.set_number((double)(left_child.get_number()>right_child.get_number()));
			else if(left_child.get_type()==__var_string)
				temp.set_number((double)(left_child.get_string()>right_child.get_string()));
			else
			{
				exit_type=__error_value_type;
				std::cout<<">>[Runtime-error] must use number or string to use \'>\' but use \'";
				print_scalar(left_child.get_type());
				std::cout<<"\'."<<std::endl;
			}
		}
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] must use same type to use \'>\' but use \'";
			print_scalar(left_child.get_type());
			std::cout<<"\' and \'";
			print_scalar(right_child.get_type());
			std::cout<<"\'."<<std::endl;
		}
	}
	else if(this->type==__cmp_less_or_equal)
	{
		temp.set_type(__var_number);
		var left_child=children.front().calculation();
		var right_child=children.back().calculation();
		if(left_child.get_type()==right_child.get_type())
		{
			if(left_child.get_type()==__var_number)
				temp.set_number((double)(left_child.get_number()<=right_child.get_number()));
			else if(left_child.get_type()==__var_string)
				temp.set_number((double)(left_child.get_string()<=right_child.get_string()));
			else
			{
				exit_type=__error_value_type;
				std::cout<<">>[Runtime-error] must use number or string to use \'<=\' but use \'";
				print_scalar(left_child.get_type());
				std::cout<<"\'."<<std::endl;
			}
		}
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] must use same type to use \'<=\' but use \'";
			print_scalar(left_child.get_type());
			std::cout<<"\' and \'";
			print_scalar(right_child.get_type());
			std::cout<<"\'."<<std::endl;
		}
	}
	else if(this->type==__cmp_more_or_equal)
	{
		temp.set_type(__var_number);
		var left_child=children.front().calculation();
		var right_child=children.back().calculation();
		if(left_child.get_type()==right_child.get_type())
		{
			if(left_child.get_type()==__var_number)
				temp.set_number((double)(left_child.get_number()>=right_child.get_number()));
			else if(left_child.get_type()==__var_string)
				temp.set_number((double)(left_child.get_string()>=right_child.get_string()));
			else
			{
				exit_type=__error_value_type;
				std::cout<<">>[Runtime-error] must use number or string to use \'>=\' but use \'";
				print_scalar(left_child.get_type());
				std::cout<<"\'."<<std::endl;
			}
		}
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] must use same type to use \'>=\' but use \'";
			print_scalar(left_child.get_type());
			std::cout<<"\' and \'";
			print_scalar(right_child.get_type());
			std::cout<<"\'."<<std::endl;
		}
	}
	else if(this->type==__or_operator)
	{
		temp.set_type(__var_number);
		var left_child=children.front().calculation();
		var right_child=children.back().calculation();
		if(left_child.get_type()==__var_number && right_child.get_type()==__var_number)
			temp.set_number((double)(left_child.get_number() || right_child.get_number()));
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] must use same type to use \'or\' but use \'";
			print_scalar(left_child.get_type());
			std::cout<<"\' and \'";
			print_scalar(right_child.get_type());
			std::cout<<"\'."<<std::endl;
		}
	}
	else if(this->type==__and_operator)
	{
		temp.set_type(__var_number);
		var left_child=children.front().calculation();
		var right_child=children.back().calculation();
		if(left_child.get_type()==__var_number && right_child.get_type()==__var_number)
			temp.set_number((double)(left_child.get_number() && right_child.get_number()));
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] must use same type to use \'and\' but use \'";
			print_scalar(left_child.get_type());
			std::cout<<"\' and \'";
			print_scalar(right_child.get_type());
			std::cout<<"\'."<<std::endl;
		}
	}
	else
	{
		exit_type=__error_value_type;
		std::cout<<">>[Runtime-error] error type occurred when doing calculation."<<std::endl;
	}
	return temp;
}

bool abstract_syntax_tree::condition_check()
{
	bool ret=false;
	var temp=calculation();
	if(temp.get_type()==__var_number)
	{
		if(temp.get_number())
			ret=true;
	}
	else
	{
		exit_type=__error_value_type;
		std::cout<<">>[Runtime-error] must use a number to make a choice but use \'";
		print_scalar(temp.get_type());
		std::cout<<"\'."<<std::endl;
	}
	return ret;
}

var abstract_syntax_tree::call_identifier()
{
	var temp;
	temp.set_type(__null_type);
	if(scope.search_var(name))
		temp=scope.get_var(name);
	else
	{
		std::cout<<">>[Runtime-error] cannot find a var named \'"<<name<<"\'."<<std::endl;
		exit_type=__find_var_failure;
		return temp;
	}
	if(!children.empty())
	{
		for(std::list<abstract_syntax_tree>::iterator i=children.begin();i!=children.end();++i)
		{
			if(i->type==__call_array && temp.get_type()==__var_array)
			{
				var place=i->children.front().calculation();
				if(place.get_type()==__var_number)
					temp=temp.get_array_member((int)place.get_number());
				else
				{
					exit_type=__error_value_type;
					std::cout<<">>[Runtime-error] ";
					print_detail_token(i->type);
					std::cout<<":incorrect type \'";
					print_scalar(temp.get_type());
					std::cout<<"\'."<<std::endl;
					break;
				}
			}
			else if(i->type==__call_hash && temp.get_type()==__var_hash)
			{
				
			}
			else if(i->type==__call_function && temp.get_type()==__var_function)
			{
				
			}
			else
			{
				exit_type=__error_value_type;
				std::cout<<">>[Runtime-error] ";
				print_detail_token(i->type);
				std::cout<<":incorrect type \'";
				print_scalar(temp.get_type());
				std::cout<<"\'."<<std::endl;
			}
		}
	}
	return temp;
}

var abstract_syntax_tree::get_value()
{
	var temp;
	temp.set_type(type);
	if(type==__id)
		temp=call_identifier();
	else if(type==__number)
		temp.set_number(number);
	else if(type==__string)
		temp.set_string(str);
	else if(type==__array)
	{
		var new_var;
		for(std::list<abstract_syntax_tree>::iterator i=children.begin();i!=children.end();++i)
		{
			new_var=i->get_value();
			temp.append_array(new_var);
		}
	}
	else if(type==__hash)
	{
		var new_var;
		for(std::list<abstract_syntax_tree>::iterator i=children.begin();i!=children.end();++i)
		{
			new_var=i->children.begin()->get_value();
			new_var.set_name(i->name);
			temp.append_array(new_var);
		}
	}
	else if(type==__function)
		temp.set_function(*this);
	else
	{
		std::cout<<">>[Runtime-error] incorrect value."<<std::endl;
		exit_type=__get_value_failure;
	}
	return temp;
}
void abstract_syntax_tree::run_root()
{
	while(!ret_stack.empty())ret_stack.pop();
	scope.set_clear();
	int beg_time,end_time;
	exit_type=__process_exited_successfully;
	beg_time=time(NULL);
	for(std::list<abstract_syntax_tree>::iterator i=children.begin();i!=children.end();++i)
	{
		if(i->type==__definition)
		{
			var new_var;
			std::list<abstract_syntax_tree>::iterator j=i->children.begin();
			std::string _name=j->name;
			if(!scope.search_var(_name))
			{
				++j;
				if(j!=i->children.end())
					new_var=j->calculation();
				new_var.set_name(_name);
				scope.add_new_var(new_var);
			}
			else
			{
				std::cout<<">>[Runtime-error] redeclaration of \'"<<_name<<"\'."<<std::endl;
				exit_type=__redeclaration;
				break;
			}
		}
		else if(i->type==__equal || i->type==__add_equal || i->type==__sub_equal || i->type==__mul_equal || i->type==__div_equal || i->type==__link_equal)
		{
			;
		}
		else if(i->type==__add_operator || i->type==__sub_operator || i->type==__mul_operator || i->type==__div_operator || i->type==__link_operator || i->type==__or_operator || i->type==__and_operator || i->type==__nor_operator)
		{
			var t=i->calculation();
			if(t.get_type()==__var_number)
				std::cout<<t.get_number()<<std::endl;
			else if(t.get_type()==__var_string)
				std::cout<<t.get_string()<<std::endl;
		}
		else if(i->type==__number)
			std::cout<<i->number<<std::endl;
		else if(i->type==__string)
			std::cout<<i->str<<std::endl;
		else if(i->type==__id)
		{
			var t=i->call_identifier();
			if(t.get_type()==__var_number)
				std::cout<<t.get_number()<<std::endl;
			else if(t.get_type()==__var_string)
				std::cout<<t.get_string()<<std::endl;
		}
		else if(i->type==__while)
		{
			scope.add_new_block_scope();
			i->run_loop();
			scope.pop_last_block_scope();
		}
		else if(i->type==__ifelse)
		{
			scope.add_new_block_scope();
			i->run_ifelse();
			scope.pop_last_block_scope();
		}
		if(exit_type!=__process_exited_successfully)
			break;
	}
	end_time=time(NULL);
	std::cout<<"--------------------------------------------------------------------------------------"<<std::endl;
	std::cout<<">>[Runtime] process exited after "<<end_time-beg_time<<" sec(s) with returned state \'";
	print_exit_type(exit_type);
	std::cout<<"\'."<<std::endl;
	scope.set_clear();
	return;
}

int abstract_syntax_tree::run_loop()
{
	int ret=0;
	scope.add_new_local_scope();
	
	abstract_syntax_tree condition=children.front();
	abstract_syntax_tree blk=children.back();
	while(condition.condition_check())
	{
		int type=blk.run_block();
		if(type==__break)
			break;
		else if(type==__return)
		{
			ret=__return;
			break;
		}
	}
	scope.pop_last_local_scope();
	return ret;
}

int abstract_syntax_tree::run_ifelse()
{
	int ret=0;
	scope.add_new_local_scope();
	for(std::list<abstract_syntax_tree>::iterator i=children.begin();i!=children.end();++i)
	{
		if(i->type!=__else && i->children.front().condition_check())
		{
			ret=i->children.back().run_block();
			break;
		}
		else
		{
			ret=i->children.back().run_block();
			break;
		}
	}
	scope.pop_last_local_scope();
	return ret;
}

var abstract_syntax_tree::run_func()
{
	var ret;
	scope.add_new_block_scope();
	scope.add_new_local_scope();
	
	//get return
	scope.pop_last_block_scope();
	ret=ret_stack.top();
	ret_stack.pop();
	return ret;
}

int abstract_syntax_tree::run_block()
{
	scope.add_new_local_scope();
	for(std::list<abstract_syntax_tree>::iterator i=children.begin();i!=children.end();++i)
	{
		if(i->type==__definition)
		{
			var new_var;
			std::list<abstract_syntax_tree>::iterator j=i->children.begin();
			std::string _name=j->name;
			if(!scope.search_var(_name))
			{
				++j;
				if(j!=i->children.end())
					new_var=j->calculation();
				new_var.set_name(_name);
				scope.add_new_var(new_var);
			}
			else
			{
				std::cout<<">>[Runtime-error] redeclaration of \'"<<_name<<"\'."<<std::endl;
				exit_type=__redeclaration;
			}
		}
		else if(i->type==__number)
			std::cout<<i->number<<std::endl;
		else if(i->type==__string)
			std::cout<<i->str<<std::endl;
		else if(i->type==__id)
		{
			var t=i->call_identifier();
			if(t.get_type()==__var_number)
				std::cout<<t.get_number()<<std::endl;
			else if(t.get_type()==__var_string)
				std::cout<<t.get_string()<<std::endl;
		}
		else if(i->type==__equal || i->type==__add_equal || i->type==__sub_equal || i->type==__mul_equal || i->type==__div_equal || i->type==__link_equal)
		{
			;
		}
		else if(i->type==__add_operator || i->type==__sub_operator || i->type==__mul_operator || i->type==__div_operator || i->type==__link_operator || i->type==__or_operator || i->type==__and_operator || i->type==__nor_operator)
		{
			var t=i->calculation();
			if(t.get_type()==__var_number)
				std::cout<<t.get_number()<<std::endl;
			else if(t.get_type()==__var_string)
				std::cout<<t.get_string()<<std::endl;
		}
		else if(i->type==__while)
		{
			int type=i->run_loop();
			if(type)
			{
				if(type==__return)
					return type;
				else
				{
					std::cout<<"[Runtime-error] incorrect use of break/continue."<<std::endl;
					exit_type=__error_command_use;
				}
			}
		}
		else if(i->type==__ifelse)
		{
			int type=i->run_ifelse();
			if(type)
				return type;
		}
		else if(i->type==__continue)
			return __continue;
		else if(i->type==__break)
			return __break;
		else if(i->type==__return)
		{
			var temp;
			temp.set_type(__null_type);
			if(!(i->children.empty()))
				temp=i->calculation();
			ret_stack.push(temp);
			return __return;
		}
		if(exit_type!=__process_exited_successfully)
			break;
	}
	scope.pop_last_local_scope();
	return 0;
}


#endif
