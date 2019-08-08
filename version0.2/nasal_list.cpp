#ifndef __NASAL_LIST_CPP__
#define __NASAL_LIST_CPP__

#include "nasal_var.h"
#include "nasal_list.h"

nasal_list::nasal_list()
{
	head=new nasal_list_unit;
	head->list_var.type=VAR_NONE;
	head->list_var.data=NULL;
	head->next=NULL;
}
nasal_list::~nasal_list()
{
	nasal_list_unit *temp=head;
	nasal_list_unit *this_node=NULL;
	while(temp->next)
	{
		this_node=temp;
		temp=temp->next;
		delete this_node;
	}
	delete temp;
}
nasal_list& nasal_list::operator=(const nasal_list &p)
{
	nasal_list_unit *temp=head;
	nasal_list_unit *this_node=NULL;
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
	nasal_list_unit *temp_p=p.head;
	
	while(temp_p->next)
	{
		temp_p=temp_p->next;
		temp->next=new nasal_list_unit;
		temp=temp->next;
		temp->next=NULL;
		temp->list_var=temp_p->list_var;
	}
	return *this;
}
void nasal_list::append(var& p)
{
	nasal_list_unit *temp=head;
	while(temp->next)
		temp=temp->next;
	temp->next=new nasal_list_unit;
	temp=temp->next;
	temp->next=NULL;
	temp->list_var=p;
	return;
}
void nasal_list::setsize(const int list_size)
{
	nasal_list_unit *temp=head;
	int cnt=0;
	while(temp->next)
	{
		temp=temp->next;
		++cnt;
		if(cnt==list_size)
		{
			nasal_list_unit *this_node=NULL;
			nasal_list_unit *t=temp->next;
			temp->next=NULL;
			if(!t)
				return;
			while(t->next)
			{
				this_node=t;
				t=t->next;
				delete this_node;
			}
			delete t;
			return;
		}
	}
	while(cnt<list_size)
	{
		temp->next=new nasal_list_unit;
		temp=temp->next;
		temp->list_var.type=VAR_NONE;
		temp->list_var.data=NULL;
		temp->next=NULL;
		++cnt;
	}
	return;
}
nasal_list nasal_list::subvec(const int list_begin=0,const int list_end=-1)
{
	nasal_list temp_list;
	int cnt=-1;
	nasal_list_unit *temp=head;
	
	int beg=list_begin;
	int end=list_end;
	if(list_end==-1)
	{
		int end_place=-1;
		while(temp->next)
		{
			temp=temp->next;
			++end_place;
		}
		temp=head;
		end=end_place;
	}
	while(temp->next)
	{
		temp=temp->next;
		++cnt;
		if(beg<=cnt && cnt<=end)
			temp_list.append(temp->list_var);
	}
	return temp_list;
}
var nasal_list::pop()
{
	nasal_list_unit *temp=head;
	nasal_list_unit *this_node;
	while(temp->next)
	{
		this_node=temp;
		temp=temp->next;
	}
	this_node->next=NULL;
	var temp_var=temp->list_var;
	delete temp;
	return temp_var;
}
nasal_list nasal_list::sort_list(const int sort_type,bool cmp_rule=true)
{
	nasal_list temp_list;
	if(sort_type==SORT_INT)
	{
		nasal_list_unit *temp=head;
		while(temp->next)
		{
			temp=temp->next;
			if(temp->list_var.type!=VAR_LLINT)
			{
				std::cout<<"[Error] Incorrect type inside: "<<temp->list_var.type<<".But type must be long int."<<std::endl;
				temp_list.setsize(1);
				return temp_list;
			}
		}
		if(temp->list_var.type!=VAR_LLINT)
		{
			std::cout<<"[Error] Incorrect type inside: "<<temp->list_var.type<<".But type must be long int."<<std::endl;
			temp_list.setsize(1);
			return temp_list;
		}
		temp_list=*this;
	}
	else if(sort_type==SORT_STR)
	{
		nasal_list_unit *temp=head;
		while(temp->next)
		{
			temp=temp->next;
			if(temp->list_var.type!=VAR_STRING)
			{
				std::cout<<"[Error] Incorrect type inside: "<<temp->list_var.type<<".But type must be string."<<std::endl;
				temp_list.setsize(1);
				return temp_list;
			}
		}
		if(temp->list_var.type!=VAR_STRING)
		{
			std::cout<<"[Error] Incorrect type inside: "<<temp->list_var.type<<".But type must be string."<<std::endl;
			temp_list.setsize(1);
			return temp_list;
		}
		temp_list=*this;
	}
	return temp_list;
}

#endif
