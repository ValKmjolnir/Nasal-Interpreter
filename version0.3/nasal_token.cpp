#ifndef __NASAL_TOKEN_CPP__
#define __NASAL_TOKEN_CPP__

#include "nasal_token.h"
#include "nasal_var_stack.h"
#include "nasal_func_stack.h"

token_list::token_list()
{
	list_range=0;
	head=new token_unit;
	head->type=FUNC_BEGIN;
	head->content="__process_begin";
	head->line=0;
	head->next=NULL;
}
token_list::~token_list()
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
token_unit* token_list::get_head()
{
	return head;
}
int token_list::get_list_range()
{
	return list_range;
}
void token_list::print_line_token(const int _line)
{
	std::cout<<"line "<<_line<<": ";
	token_unit *temp=head;
	while(temp->next)
	{
		temp=temp->next;
		if(temp->line==_line)
			std::cout<<temp->content<<" ";
		else if(temp->line>_line)
			break;
	}
	std::cout<<std::endl;
	return;
}
void token_list::delete_all()
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
void token_list::append(const int _line,const int _type,std::string &_content)
{
	token_unit *temp=head;
	while(temp->next)
		temp=temp->next;
	++list_range;
	temp->next=new token_unit;
	temp=temp->next;
	temp->next=NULL;
	temp->type=_type;
	temp->line=_line;
	temp->content=_content;
	return;
}
void token_list::print()
{
	token_unit *temp=head;
	std::cout<<"line "<<temp->line<<": "<<"( ProcessBegin | "<<temp->content<<" )"<<std::endl;
	if(!head->next)
		return;
	while(temp->next)
	{
		temp=temp->next;
		std::cout<<"line "<<temp->line<<": ";
		if(temp->type==OPERATOR)
			std::cout<<"( Operator     | ";
		else if(temp->type==IDENTIFIER)
			std::cout<<"( Identifier   | ";
		else if(temp->type==NUMBER)
			std::cout<<"( Number       | ";
		else if(temp->type==RESERVEWORD)
			std::cout<<"( ReserveWord  | ";
		else if(temp->type==STRING)
			std::cout<<"( String       | ";
		std::cout<<temp->content<<" )"<<std::endl;
	}
	return;
}

#endif
