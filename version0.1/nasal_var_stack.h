#ifndef __NASAL_VAR_STACK_H__
#define __NASAL_VAR_STACK_H__

#include "nasal_list.h"
#include "nasal_print.h"


namespace nasal
{

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
		void append_var(std::string &varia_name,var &temp_var)
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
				std::cout<<"["<<temp->var_detail.Type<<"]: "<<temp->var_name<<" : ";
				if(temp->var_detail.Type!="string")
					PrintVar(temp->var_detail);
				else
					std::cout<<*((std::string *)temp->var_detail.data);
				std::cout<<std::endl;
			}
			return;
		}
		var SearchVar(std::string varia_name)
		{
			var temp_var;
			temp_var.data=NULL;
			temp_var.Type="null";
			temp_var.isGlobal=false;
			var_stack_unit *temp=head;
			while(temp->next)
			{
				temp=temp->next;
				if(temp->var_name==varia_name)
					temp_var=temp->var_detail;
			}
			return temp_var;
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

}

#endif
