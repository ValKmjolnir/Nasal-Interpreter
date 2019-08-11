#ifndef __NASAL_FUNC_STACK_H__
#define __NASAL_FUNC_STACK_H__

#include "nasal_functional.h"


struct func_stack_unit
{
	std::string func_name;
	func func_statement;
	func_stack_unit *next;
};

class func_stack
{
	private:
		func_stack_unit *head;
	public:
		func_stack()
		{
			head=new func_stack_unit;
			head->func_name="null";
			head->next=NULL;
		}
		~func_stack()
		{
			func_stack_unit *temp=head;
			func_stack_unit *this_node=NULL;
			while(temp->next)
			{
				this_node=temp;
				temp=temp->next;
				delete this_node;
			}
			delete temp;
		}
		void append_function(std::string &function_name,func &temp_func)
		{
			func_stack_unit *temp=head;
			while(temp->next)
			{
				temp=temp->next;
				if(temp->func_name==function_name)
				{
					std::cout<<"[Error] Redeclaration of function \""<<function_name<<"\""<<std::endl;
					return;
				}
			}
			temp->next=new func_stack_unit;
			temp=temp->next;
			temp->next=NULL;
			temp->func_name=function_name;
			temp->func_statement=temp_func;
			return;
		}
//		void run_function(std::string &function_name)
//		{
//			func_stack_unit *temp=head;
//			while(temp->next)
//			{
//				temp=temp->next;
//				if(temp->func_name==function_name)
//				{
//					temp->func_statement.run();
//					return;
//				}
//			}
//			std::cout<<"[Error] Could not find this function."<<std::endl;
//			return;
//		}
		void print_function()
		{
			func_stack_unit *temp=head;
			while(temp->next)
			{
				temp=temp->next;
				std::cout<<"function: "<<temp->func_name<<std::endl;
			}
			return;
		}
		void pop_function()
		{
			func_stack_unit *temp=head;
			func_stack_unit *end_temp;
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
			func_stack_unit *temp=head->next;
			func_stack_unit *this_node=NULL;
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
func_stack nasal_func_stack;

#endif
