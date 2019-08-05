#ifndef __PROCESS_STACK_H__
#define __PROCESS_STACK_H__

#include <iostream>
#include <cstring>
#include "nasal.h"
namespace nasal
{


struct process_stack_unit
{
	int line;                  //place the unit first appear
	std::string name;          //content of the unit or name of the var/class/function
	std::string format_type;   //var class function string info
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
			head->name="InterpreterInfo";
			head->format_type="Info";
			head->global=false;
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
			
			temp->name=p.name;
			temp->line=p.line;
			temp->format_type=p.format_type;
			temp->global=p.global;
			temp->unitdata=p.unitdata;
			
			return;
		}
		void stack_print(bool reverse_mode_used)
		{
			process_stack_unit *temp=head;
			std::cout<<"In stack: "<<std::endl;
			if(reverse_mode_used)
			{
				while(temp->next)
					temp=temp->next;
				while(temp->last)
				{
					std::cout<<"line "<<temp->line<<": |"<<temp->format_type<<"|"<<temp->name<<"|\n\t|";
					temp->unitdata.Print();
					std::cout<<std::endl;
					temp=temp->last;
				}
				std::cout<<"line "<<temp->line<<": |"<<temp->format_type<<"|"<<temp->name<<"|\n\t|";
				temp->unitdata.Print();
				std::cout<<std::endl;
			}
			else
			{
				std::cout<<"line "<<temp->line<<": |"<<temp->format_type<<"|"<<temp->name<<"|\n\t|";
				temp->unitdata.Print();
				std::cout<<std::endl;
				while(temp->next)
				{
					temp=temp->next;
					std::cout<<"line "<<temp->line<<": |"<<temp->format_type<<"|"<<temp->name<<"|\n\t|";
					temp->unitdata.Print();
					std::cout<<std::endl;
				}
			}
			std::cout<<"End."<<std::endl;
			return;
		}
		void pop()
		{
			process_stack_unit *temp=head;
			process_stack_unit *last_node;
			while(temp->next)
			{
				last_node=temp;
				temp=temp->next;
			}
			last_node->next=NULL;
			delete temp;
			return;
		}
		bool check_stack(std::string &ElementName)
		{
			process_stack_unit *temp=head;
			while(temp->next)
			{
				temp=temp->next;
				if(temp->name==ElementName)
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
				if(temp->name==ElementName)
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
