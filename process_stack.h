#ifndef __PROCESS_STACK_H__
#define __PROCESS_STACK_H__

#include <iostream>
#include <cstring>
#include "nasal.h"
namespace nasal
{


struct process_stack_unit
{
	int line;           //place the unit first appear
	std::string content;//content of the unit or name of the var/class/function
	std::string type;   //var class function
	process_stack_unit *next;
	process_stack_unit *last;
};

class process_stack
{
	private:
		process_stack_unit *head;
		process_stack_unit *ptr;
	public:
		process_stack()
		{
			head->line=0;
			head->content="# Nasal language for FlightGear.";
			head->last=NULL;
			head->next=NULL;
			ptr=NULL;
		}
		~process_stack()
		{
			process_stack_unit *temp=head;
			while(temp->next)
			{
				temp=temp->next;
				delete head;
				head=temp;
			}
			delete head;
		}
		void stack_append(process_stack_unit &p)
		{
			process_stack_unit *temp=head;
			process_stack_unit *last_node;
			while(temp->next)
			{
				if(!temp->next->next)
					last_node=temp;
				temp=temp->next;
			}
			if(temp==head)
				last_node=head;
			temp=new process_stack_unit;
			temp->last=last_node;
			temp->next=NULL;
			temp->content=p.content;
			temp->line=p.last;
			temp->type=p.type;
			return;
		}
};




}

#endif
