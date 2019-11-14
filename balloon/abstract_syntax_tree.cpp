#ifndef __ABSTRACT_SYNTAX_TREE_CPP__
#define __ABSTRACT_SYNTAX_TREE_CPP__
#include "abstract_syntax_tree.h"


int exit_type=0;
std::stack<var> ret_stack;// for function ret use
std::list<var> parameter;// for function call use

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
	else if(this->type==__array)
	{
		temp=this->array_generation();
		return temp;
	}
	else if(this->type==__hash)
	{
		temp=this->hash_generation();
		return temp;
	}
	else if(this->type==__function)
	{
		temp.set_type(__var_function);
		temp.set_function(*this);
		return temp;
	}
	if(this->type==__nor_operator)
	{
		temp.set_type(__var_number);
		temp=children.front().calculation();
		if(temp.get_type()!=__var_number)
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] line "<<line<<": must use a number to use \'!\' but use \'";
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
			std::cout<<">>[Runtime-error] line "<<line<<": must use a number to use \'+\' but use \'";
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
				std::cout<<">>[Runtime-error] line "<<line<<": must use a number to use \'-\' but use \'";
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
				std::cout<<">>[Runtime-error] line "<<line<<": must use a number to use \'-\' but use \'";
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
			std::cout<<">>[Runtime-error] line "<<line<<": must use a number to use \'*\' but use \'";
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
				std::cout<<">>[Runtime-error] line "<<line<<": get number \'NaN\' or \'Inf\'."<<std::endl;
			}
		}
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] line "<<line<<": must use a number to use \'/\' but use \'";
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
			std::cout<<">>[Runtime-error] line "<<line<<": must use a string to use \'~\' but use \'";
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
				std::cout<<">>[Runtime-error] line "<<line<<": must use number or string to use \'==\' but use \'";
				print_scalar(left_child.get_type());
				std::cout<<"\'."<<std::endl;
			}
		}
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] line "<<line<<": must use same type to use \'==\' but use \'";
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
				std::cout<<">>[Runtime-error] line "<<line<<": must use number or string to use \'!=\' but use \'";
				print_scalar(left_child.get_type());
				std::cout<<"\'."<<std::endl;
			}
		}
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] line "<<line<<": must use same type to use \'!=\' but use \'";
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
				std::cout<<">>[Runtime-error] line "<<line<<": must use number or string to use \'<\' but use \'";
				print_scalar(left_child.get_type());
				std::cout<<"\'."<<std::endl;
			}
		}
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] line "<<line<<": must use same type to use \'<\' but use \'";
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
				std::cout<<">>[Runtime-error] line "<<line<<": must use number or string to use \'>\' but use \'";
				print_scalar(left_child.get_type());
				std::cout<<"\'."<<std::endl;
			}
		}
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] line "<<line<<": must use same type to use \'>\' but use \'";
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
				std::cout<<">>[Runtime-error] line "<<line<<": must use number or string to use \'<=\' but use \'";
				print_scalar(left_child.get_type());
				std::cout<<"\'."<<std::endl;
			}
		}
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] line "<<line<<": must use same type to use \'<=\' but use \'";
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
				std::cout<<">>[Runtime-error] line "<<line<<": must use number or string to use \'>=\' but use \'";
				print_scalar(left_child.get_type());
				std::cout<<"\'."<<std::endl;
			}
		}
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] line "<<line<<": must use same type to use \'>=\' but use \'";
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
			std::cout<<">>[Runtime-error] line "<<line<<": must use same type to use \'or\' but use \'";
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
			std::cout<<">>[Runtime-error] line "<<line<<": must use same type to use \'and\' but use \'";
			print_scalar(left_child.get_type());
			std::cout<<"\' and \'";
			print_scalar(right_child.get_type());
			std::cout<<"\'."<<std::endl;
		}
	}
	else
	{
		exit_type=__error_value_type;
		std::cout<<">>[Runtime-error] line "<<line<<": error type \'";
		print_detail_token(this->type);
		std::cout<<"\' occurred when doing calculation."<<std::endl;
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
		std::cout<<">>[Runtime-error] line "<<line<<": must use a number to make a choice but use \'";
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
		std::cout<<">>[Runtime-error] line "<<line<<": cannot find a var named \'"<<name<<"\'."<<std::endl;
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
					std::cout<<">>[Runtime-error] line "<<line<<": ";
					print_detail_token(i->type);
					std::cout<<": incorrect type \'";
					print_scalar(temp.get_type());
					std::cout<<"\'."<<std::endl;
					break;
				}
			}
			else if(i->type==__call_hash && temp.get_type()==__var_hash)
			{
				temp=temp.get_hash_member(i->name);
				if(temp.get_type()==__null_type)
				{
					exit_type=__get_value_failure;
					std::cout<<">>[Runtime-error] line "<<line<<": cannot find a hash-member named \'"<<i->name<<"\' or this value is set to __null_type. detail: ";
					temp.print_var();
					std::cout<<"."<<std::endl;
					break;
				}
			}
			else if(i->type==__call_function && temp.get_type()==__var_function)
			{
				parameter.clear();
				for(std::list<abstract_syntax_tree>::iterator j=i->children.begin();j!=i->children.end();++j)
					parameter.push_back(j->calculation());
				temp=temp.get_function().run_func();
			}
			else
			{
				exit_type=__error_value_type;
				std::cout<<">>[Runtime-error] line "<<line<<": ";
				print_detail_token(i->type);
				std::cout<<": incorrect type \'";
				print_scalar(temp.get_type());
				std::cout<<"\'."<<std::endl;
				break;
			}
		}
	}
	return temp;
}

var abstract_syntax_tree::array_generation()
{
	var new_var;
	new_var.set_type(__var_array);
	if(!children.empty())
		for(std::list<abstract_syntax_tree>::iterator i=children.begin();i!=children.end();++i)
			new_var.append_array(i->calculation());
	return new_var;
}

var abstract_syntax_tree::hash_generation()
{
	var new_var;
	new_var.set_type(__var_hash);
	if(!children.empty())
		for(std::list<abstract_syntax_tree>::iterator i=children.begin();i!=children.end();++i)
		{
			var temp;
			temp=i->children.front().calculation();
			temp.set_name(i->name);
			new_var.append_hash(temp);
		}
	return new_var;
}

var* abstract_syntax_tree::get_var_addr()
{
	var* addr=&error_var;
	if(scope.search_var(name))
		addr=scope.get_addr(name);
	else
	{
		std::cout<<">>[Runtime-error] line "<<line<<": cannot find a var named \'"<<name<<"\'."<<std::endl;
		exit_type=__find_var_failure;
		return addr;
	}
	if(!children.empty())
	{
		for(std::list<abstract_syntax_tree>::iterator i=children.begin();i!=children.end();++i)
		{
			if(i->type==__call_array && addr->get_type()==__var_array)
			{
				var place=i->children.front().calculation();
				if(place.get_type()==__var_number)
					addr=addr->get_array_member_addr((int)place.get_number());
				else
				{
					exit_type=__error_value_type;
					std::cout<<">>[Runtime-error] line "<<line<<": ";
					print_detail_token(i->type);
					std::cout<<": incorrect type \'";
					print_scalar(addr->get_type());
					std::cout<<"\'."<<std::endl;
					break;
				}
			}
			else if(i->type==__call_hash && addr->get_type()==__var_hash)
			{
				addr=addr->get_hash_member_addr(i->name);
				if(addr->get_type()==__null_type)
				{
					exit_type=__get_value_failure;
					std::cout<<">>[Runtime-error] line "<<line<<": cannot find a hash-member named \'"<<i->name<<"\'."<<std::endl;
					break;
				}
			}
			else if(i->type==__call_function && addr->get_type()==__var_function)
			{
				exit_type=__error_value_type;
				std::cout<<">>[Runtime-error] line "<<line<<": function-returned value cannot be assigned."<<std::endl;
				break;
			}
			else
			{
				exit_type=__error_value_type;
				std::cout<<">>[Runtime-error] line "<<line<<": ";
				print_detail_token(i->type);
				std::cout<<": incorrect type \'";
				print_scalar(addr->get_type());
				std::cout<<"\'."<<std::endl;
				break;
			}
		}
	}
	return addr;
}

var abstract_syntax_tree::assignment()
{
	var ret,temp;
	abstract_syntax_tree id=children.front();
	abstract_syntax_tree value=children.back();
	var* addr=id.get_var_addr();
	if(value.type==__equal || value.type==__add_equal || value.type==__sub_equal || value.type==__mul_equal || value.type==__div_equal || value.type==__link_equal)
		temp=value.assignment();
	else
		temp=value.calculation();
	if(type==__equal)
	{
		std::string tname=addr->get_name();
		*addr=temp;
		addr->set_name(tname);
	}
	else if(type==__add_equal || type==__sub_equal || type==__mul_equal || type==__div_equal)
	{
		if(addr->get_type()==__var_number && temp.get_type()==__var_number)
		{
			switch(type)
			{
				case __add_equal:addr->set_number(addr->get_number()+temp.get_number());break;
				case __sub_equal:addr->set_number(addr->get_number()-temp.get_number());break;
				case __mul_equal:addr->set_number(addr->get_number()*temp.get_number());break;
				case __div_equal:addr->set_number(addr->get_number()/temp.get_number());break;
			}
			if(std::isnan(addr->get_number()) || std::isinf(addr->get_number()))
			{
				exit_type=__sigfpe_arithmetic_exception;
				std::cout<<">>[Runtime-error] line "<<line<<": get number \'NaN\' or \'Inf\'."<<std::endl;
			}
		}
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] line "<<line<<": incorrect type \'";
			print_scalar(addr->get_type());
			std::cout<<"\' and \'";
			print_scalar(temp.get_type());
			std::cout<<"\' but this operator ";
			print_detail_token(type);
			std::cout<<" must use \'number\' and \'number\'."<<std::endl;
		}
	}
	else if(type==__link_equal)
	{
		if(addr->get_type()==__var_string && temp.get_type()==__string)
			addr->set_string(addr->get_string()+temp.get_string());
		else
		{
			exit_type=__error_value_type;
			std::cout<<">>[Runtime-error] line "<<line<<": incorrect type \'";
			print_scalar(addr->get_type());
			std::cout<<"\' and \'";
			print_scalar(temp.get_type());
			std::cout<<"\' but this operator ~ must use \'string\' and \'string\'."<<std::endl;
		}
	}
	
	ret=*addr;
	return ret;
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
				std::cout<<">>[Runtime-error] line "<<line<<": redeclaration of \'"<<_name<<"\'."<<std::endl;
				exit_type=__redeclaration;
				break;
			}
		}
		else if(i->type==__equal || i->type==__add_equal || i->type==__sub_equal || i->type==__mul_equal || i->type==__div_equal || i->type==__link_equal)
			i->assignment();
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
	std::cout<<"------------------------------------------------------------------------------"<<std::endl;
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
		if(exit_type!=__process_exited_successfully)
			break;
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
		if((i->type==__if || i->type==__elsif) && i->children.front().condition_check())
		{
			ret=i->children.back().run_block();
			break;
		}
		else if(i->type==__else)
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
	abstract_syntax_tree para=children.front();
	abstract_syntax_tree blk=children.back();
	std::list<abstract_syntax_tree>::iterator para_name=para.children.begin();
	std::list<var>::iterator para_value=parameter.begin();
	
	for(;para_name!=para.children.end();++para_name,++para_value)
	{
		if(para_value==parameter.end() && para_name!=para.children.end())
		{
			exit_type=__lack_parameter;
			std::cout<<">>[Runtime-error] line "<<line<<": lack parameter(s)."<<std::endl;
			break;
		}
		var new_var;
		new_var=*para_value;
		new_var.set_name(para_name->name);
		scope.add_new_var(new_var);
	}
	if(exit_type==__process_exited_successfully)
	{
		int _t=blk.run_block();
		if(_t!=__return)
			ret_stack.push(error_var);
	}
		
	else
		ret_stack.push(error_var);
	
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
				std::cout<<">>[Runtime-error] line "<<line<<": redeclaration of \'"<<_name<<"\'."<<std::endl;
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
			i->assignment();
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
					std::cout<<"[Runtime-error] line "<<line<<": incorrect use of break/continue."<<std::endl;
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
				temp=i->children.front().calculation();
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
