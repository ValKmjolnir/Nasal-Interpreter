#ifndef __NASAL_FUNC_H__
#define __NASAL_FUNC_H__

#include <iostream>
#include <cstring>

namespace nasal
{

struct func_unit
{
	std::string type;
	std::string content;
	int line;
	func_unit *next;
};

class func
{
	private:
		func_unit *head;
	public:
		func()
		{
			head=new func_unit;
			head->type="Begin";
			head->content="__process_begin";
			head->line=0;
			head->next=NULL;
		}
		~func()
		{
			func_unit *temp=head;
			func_unit *this_node=NULL;
			while(temp->next)
			{
				this_node=temp;
				temp=temp->next;
				delete this_node;
			}
			delete temp;
		}
		void delete_all()
		{
			if(!head->next)
				return;
			func_unit *temp=head;
			func_unit *this_node=NULL;
			temp=temp->next;
			head->next=NULL;
			while(temp->next)
			{
				this_node=temp;
				temp=temp->next;
				delete this_node;
			}
			delete temp;
			return;
		}
		void append(const char *_type,std::string &_content,const int _line)
		{
			func_unit *temp=head;
			while(temp->next)
				temp=temp->next;
			temp->next=new func_unit;
			temp=temp->next;
			temp->next=NULL;
			temp->type=_type;
			temp->line=_line;
			temp->content=_content;
			return;
		}
		void print()
		{
			func_unit *temp=head;
			std::cout<<"line "<<temp->line<<": "<<"( ProcessBegin | "<<temp->content<<" )"<<std::endl;
			if(!head->next)
				return;
			while(temp->next)
			{
				temp=temp->next;
				std::cout<<"line "<<temp->line<<": ";
				if(temp->type=="Operator")
					std::cout<<"( Operator     | ";
				else if(temp->type=="Identifier")
					std::cout<<"( Identifier   | ";
				else if(temp->type=="Number")
					std::cout<<"( Number       | ";
				else if(temp->type=="ReserveWord")
					std::cout<<"( ReserveWord  | ";
				else if(temp->type=="String")
					std::cout<<"( String       | ";
				std::cout<<temp->content<<" )"<<std::endl;
			}
			return;
		}
		func& operator=(const func &p)
		{
			delete_all();
			func_unit *temp=head;
			func_unit *ptemp=p.head;
			while(ptemp->next)
			{
				ptemp=ptemp->next;
				temp=new func_unit;
				temp->type=ptemp->type;
				temp->content=ptemp->content;
				temp->line=ptemp->line;
				temp->next=NULL;
			}
			temp=new func_unit;
			temp->type=ptemp->type;
			temp->content=ptemp->content;
			temp->line=ptemp->line;
			temp->next=NULL;
			return *this;
		}
		void run()
		{
			func_unit *temp=head;
			if(!head->next)
			{
				std::cout<<"Running complete."<<std::endl;
				return;
			}
			while(temp->next)
			{
				temp=temp->next;
				if(temp->type=="Operator")
				{
					;
				}
				else if(temp->type=="Identifier")
				{
					;
				}
				else if(temp->type=="Number")
				{
					;
				}
				else if(temp->type=="ReserveWord")
				{
					;
				}
				else if(temp->type=="String")
				{
					;
				}
			}
			return;
		}
};

}

#endif
