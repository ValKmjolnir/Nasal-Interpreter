#ifndef __NASAL_HASH_CPP__
#define __NASAL_HASH_CPP__


#include "nasal_hash.h"

nasal_hash::nasal_hash()
{
	head=new nasal_hash_unit;
	head->name="";
	head->hash_var.type=VAR_NONE;
	head->hash_var.data=NULL;
	head->next=NULL;
}
nasal_hash::~nasal_hash()
{
	nasal_hash_unit *temp=head;
	nasal_hash_unit *this_node=NULL;
	while(temp->next)
	{
		this_node=temp;
		temp=temp->next;
		delete this_node;
	}
	delete temp;
}
nasal_hash& nasal_hash::operator=(const nasal_hash &p)
{
	nasal_hash_unit *temp=head;
	nasal_hash_unit *this_node=NULL;
	if(head->next)
	{
		temp=temp->next;
		head->next=NULL;
		while(temp->next)
		{
			this_node=temp;
			temp=temp->next;
			delete this_node;
		}
		delete temp;
	}
	temp=head;
	nasal_hash_unit *temp_p=p.head;
	
	while(temp_p->next)
	{
		temp_p=temp_p->next;
		temp->next=new nasal_hash_unit;
		temp=temp->next;
		temp->next=NULL;
		temp->hash_var=temp_p->hash_var;
		temp->name=temp_p->name;
	}
	return *this;
}
void nasal_hash::append(std::string& var_name,var& p)
{
	nasal_hash_unit *temp=head;
	while(temp->next)
		temp=temp->next;
	temp->next=new nasal_hash_unit;
	temp=temp->next;
	temp->next=NULL;
	temp->hash_var=p;
	temp->name=var_name;
	return;
}
int nasal_hash::contains(std::string& var_name)
{
	nasal_hash_unit *temp=head;
	while(temp->next)
	{
		temp=temp->next;
		if(temp->name==var_name)
			return 1;
	}
	return 0;
}
int nasal_hash::delete_element(std::string& var_name)
{
	nasal_hash_unit *temp=head;
	nasal_hash_unit *last_node=NULL;
	while(temp->next)
	{
		last_node=temp;
		temp=temp->next;
		if(temp->name==var_name)
		{
			last_node->next=temp->next;
			delete temp;
			return 1;
		}
	}
	return 0;
}
nasal_list nasal_hash::keys()
{
	var assist_var;
	assist_var.type=VAR_STRING;
	nasal_list temp_list;
	nasal_hash_unit *temp=head;
	while(temp->next)
	{
		temp=temp->next;
		assist_var.data=new std::string;
		*((std::string *)assist_var.data)=temp->name;
		temp_list.append(assist_var);
	}
	return temp_list;
}

#endif
