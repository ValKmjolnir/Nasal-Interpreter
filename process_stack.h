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
	std::string Name;//content of the unit or name of the var/class/function
	std::string type;   //var class function
	var unitdata;
	bool global;
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
			head=new process_stack_unit;
			head->line=0;
			head->Name="InterpreterInfo";
			head->global=false;
			head->type="information";
			head->unitdata.Type="string";
			head->unitdata.data=new std::string;
			*((std::string *)head->unitdata.data)="# Nasal language for FlightGear.";
			
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
				temp=temp->next;
			}
			temp->next=new process_stack_unit;
			
			last_node=temp;
			temp=temp->next;
			
			temp->last=last_node;
			last_node->next=temp;
			temp->next=NULL;
			
			temp->Name=p.Name;
			temp->line=p.line;
			temp->type=p.type;
			temp->global=p.global;
			temp->unitdata=p.unitdata;
			
			return;
		}
		void stack_print(bool reverse_mode_used)
		{
			process_stack_unit *temp=head;
			std::cout<<"In stack: "<<std::endl;
			std::cout<<temp->line<<": |"<<temp->type<<"| \""<<temp->Name<<"\""<<std::endl;
			while(temp->next)
			{
				temp=temp->next;
				std::cout<<temp->line<<": |"<<temp->type<<"| \""<<temp->Name<<"\""<<std::endl;
			}
			if(reverse_mode_used)
				while(temp->last)
				{
					std::cout<<temp->line<<": |"<<temp->type<<"| \""<<temp->Name<<"\""<<std::endl;
					temp=temp->last;
				}
			std::cout<<"End."<<std::endl;
			return;
		}
		bool check_stack(std::string &ElementName)
		{
			process_stack_unit *temp=head;
			while(temp->next)
			{
				temp=temp->next;
				if(temp->Name==ElementName)
					return true;
			}
			return false;
		}
		void stack_content_print(std::string &ElementName)
		{
			process_stack_unit *temp=head;
			while(temp->next)
			{
				temp=temp->next;
				if(temp->Name==ElementName)
				{
					temp->unitdata.Print();
					return;
				}
			}
			return;
		}
};




}

#endif
