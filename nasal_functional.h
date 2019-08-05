#ifndef __NASAL_FUNCTIONAL_H__
#define __NASAL_FUNCTIONAL_H__

#include <iostream>
#include <cstring>

namespace nasal
{

#define FUNC_OPERATOR    1
#define FUNC_IDENTIFIER  2
#define FUNC_NUMBER      3
#define FUNC_RESERVEWORD 4
#define FUNC_STRING      5

struct token_unit
{
	std::string type;
	std::string content;
	int line;
	token_unit *next;
};

class token_list
{
	private:
		token_unit *head;
	public:
		token_list()
		{
			head=new token_unit;
			head->type="Begin";
			head->content="__process_begin";
			head->line=0;
			head->next=NULL;
		}
		~token_list()
		{
			token_unit *temp=head;
			token_unit *this_node=NULL;
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
			token_unit *temp=head;
			token_unit *this_node=NULL;
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
			token_unit *temp=head;
			while(temp->next)
				temp=temp->next;
			temp->next=new token_unit;
			temp=temp->next;
			temp->next=NULL;
			temp->type=_type;
			temp->line=_line;
			temp->content=_content;
			return;
		}
		void print()
		{
			token_unit *temp=head;
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
		void run()
		{
			token_unit *temp=head;
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
		}
};

}


#endif
