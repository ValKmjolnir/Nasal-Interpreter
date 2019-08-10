#ifndef __NASAL_VAR_STACK_H__
#define __NASAL_VAR_STACK_H__

#include "nasal_print.h"

struct var_stack_unit
{
	std::string var_name;
	var var_detail;
	var_stack_unit *next;
};

class var_stack
{
	private:
		var_stack_unit *head;
	public:
		var_stack()
		{
			head=new var_stack_unit;
			head->var_name="null";
			head->next=NULL;
		}
		~var_stack()
		{
			var_stack_unit *temp=head;
			var_stack_unit *this_node=NULL;
			if(head->next)
			{
				while(temp->next)
				{
					this_node=temp;
					temp=temp->next;
					delete this_node;
				}
				delete temp;
			}
			else
				delete head;
		}
		void append_var(std::string& varia_name,var& temp_var)
		{
			var_stack_unit *temp=head;
			while(temp->next)
				temp=temp->next;
			temp->next=new var_stack_unit;
			temp=temp->next;
			temp->var_name=varia_name;
			temp->var_detail=temp_var;
			temp->next=NULL;
		}
		void print_var()
		{
			var_stack_unit *temp=head;
			while(temp->next)
			{
				temp=temp->next;
				std::cout<<"[";
				switch(temp->var_detail.type)
				{
					case VAR_NONE:
						std::cout<<"null";
						break;
					case VAR_LLINT:
						std::cout<<"int";
						break;
					case VAR_DOUBLE:
						std::cout<<"float";
						break;
					case VAR_CHAR:
						std::cout<<"char";
						break;
					case VAR_STRING:
						std::cout<<"string";
						break;
					case VAR_LIST:
						std::cout<<"array";
						break;
					case VAR_HASH:
						std::cout<<"hash";
						break;
				}
				std::cout<<"]: "<<temp->var_name<<" : ";
				if(temp->var_detail.type!=VAR_STRING)
					PrintVar(temp->var_detail);
				else
					std::cout<<*((std::string *)temp->var_detail.data);
				std::cout<<std::endl;
			}
			return;
		}
		var search_var(std::string varia_name)
		{
			var temp_var;
			temp_var.data=NULL;
			temp_var.type=VAR_NONE;
			var_stack_unit *temp=head;
			while(temp->next)
			{
				temp=temp->next;
				if(temp->var_name==varia_name)
				{
					temp_var=temp->var_detail;
					break;
				}
			}
			return temp_var;
		}
		void edit_var(std::string varia_name,var &temp_var)
		{
			var_stack_unit *temp=head;
			while(temp->next)
			{
				temp=temp->next;
				if(temp->var_name==varia_name)
				{
					temp->var_detail=temp_var;
					break;
				}
			}
			return;
		}
		void pop_var()
		{
			var_stack_unit *temp=head;
			var_stack_unit *end_temp;
			if(!head->next)
				return;
			while(temp->next)
			{
				end_temp=temp;
				temp=temp->next;
			}
			end_temp->next=NULL;
			delete temp;
		}
		void delete_all()
		{
			var_stack_unit *temp=head->next;
			var_stack_unit *this_node=NULL;
			head->next=NULL;
			if(!temp)
				return;
			while(temp->next)
			{
				this_node=temp;
				temp=temp->next;
				delete this_node;
			}
			delete temp;
			return;
		}
};
var_stack nasal_var_stack;

#endif
