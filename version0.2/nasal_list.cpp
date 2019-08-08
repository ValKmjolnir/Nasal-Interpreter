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
nasal_list nasal_list::sort_list(const int sort_type,const int _cmp=1)
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
		nasal_list_unit *first_temp_this;
		nasal_list_unit *second_temp_this;
		nasal_list_unit *node_this;
		first_temp_this=temp_list.head->next;
		while(first_temp_this->next)
		{
			node_this=first_temp_this;
			second_temp_this=first_temp_this->next;
			while(second_temp_this->next)
			{
				if(_cmp>0 && *((int *)node_this->list_var.data)>*((int *)second_temp_this->list_var.data))//from small to large
					node_this=second_temp_this;
				else if(_cmp<=0 && *((int *)node_this->list_var.data)<*((int *)second_temp_this->list_var.data))//from large to small
					node_this=second_temp_this;
				second_temp_this=second_temp_this->next;
			}
			if(_cmp>0 && *((int *)node_this->list_var.data)>*((int *)second_temp_this->list_var.data))//from small to large func(a,b) a-b
				node_this=second_temp_this;
			else if(_cmp<=0 && *((int *)node_this->list_var.data)<*((int *)second_temp_this->list_var.data))//from large to small func(a,b) b-a
				node_this=second_temp_this;
			if(node_this!=first_temp_this)
			{
				int t;
				t=*((int *)first_temp_this->list_var.data);
				*((int *)first_temp_this->list_var.data)=*((int *)node_this->list_var.data);
				*((int *)node_this->list_var.data)=t;
			}
			first_temp_this=first_temp_this->next;
		}
	}
	else if(sort_type==SORT_DBL)
	{
		nasal_list_unit *temp=head;
		while(temp->next)
		{
			temp=temp->next;
			if(temp->list_var.type!=VAR_DOUBLE)
			{
				std::cout<<"[Error] Incorrect type inside: "<<temp->list_var.type<<".But type must be float."<<std::endl;
				temp_list.setsize(1);
				return temp_list;
			}
		}
		if(temp->list_var.type!=VAR_DOUBLE)
		{
			std::cout<<"[Error] Incorrect type inside: "<<temp->list_var.type<<".But type must be float."<<std::endl;
			temp_list.setsize(1);
			return temp_list;
		}
		temp_list=*this;
		nasal_list_unit *first_temp_this;
		nasal_list_unit *second_temp_this;
		nasal_list_unit *node_this;
		first_temp_this=temp_list.head->next;
		while(first_temp_this->next)
		{
			node_this=first_temp_this;
			second_temp_this=first_temp_this->next;
			while(second_temp_this->next)
			{
				if(_cmp>0 && *((double *)node_this->list_var.data)>*((double *)second_temp_this->list_var.data))//from small to large
					node_this=second_temp_this;
				else if(_cmp<=0 && *((double *)node_this->list_var.data)<*((double *)second_temp_this->list_var.data))//from large to small
					node_this=second_temp_this;
				second_temp_this=second_temp_this->next;
			}
			if(_cmp>0 && *((double *)node_this->list_var.data)>*((double *)second_temp_this->list_var.data))//from small to large func(a,b) a-b
				node_this=second_temp_this;
			else if(_cmp<=0 && *((double *)node_this->list_var.data)<*((double *)second_temp_this->list_var.data))//from large to small func(a,b) b-a
				node_this=second_temp_this;
			if(node_this!=first_temp_this)
			{
				double t;
				t=*((double *)first_temp_this->list_var.data);
				*((double *)first_temp_this->list_var.data)=*((double *)node_this->list_var.data);
				*((double *)node_this->list_var.data)=t;
			}
			first_temp_this=first_temp_this->next;
		}
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
		nasal_list_unit *first_temp_this;
		nasal_list_unit *second_temp_this;
		nasal_list_unit *node_this;
		first_temp_this=temp_list.head->next;
		while(first_temp_this->next)
		{
			node_this=first_temp_this;
			second_temp_this=first_temp_this->next;
			while(second_temp_this->next)
			{
				if(_cmp>0 && *((std::string *)node_this->list_var.data)>*((std::string *)second_temp_this->list_var.data))//from small to large
					node_this=second_temp_this;
				else if(_cmp<=0 && *((std::string *)node_this->list_var.data)<*((std::string *)second_temp_this->list_var.data))//from large to small
					node_this=second_temp_this;
				second_temp_this=second_temp_this->next;
			}
			if(_cmp>0 && *((std::string *)node_this->list_var.data)>*((std::string *)second_temp_this->list_var.data))//from small to large func(a,b) cmp(a,b)
				node_this=second_temp_this;
			else if(_cmp<=0 && *((std::string *)node_this->list_var.data)<*((std::string *)second_temp_this->list_var.data))//from large to small func(a,b) cmp(b,a) or -cmp(a,b)
				node_this=second_temp_this;
			if(node_this!=first_temp_this)
			{
				std::string t;
				t=*((std::string *)first_temp_this->list_var.data);
				*((std::string *)first_temp_this->list_var.data)=*((std::string *)node_this->list_var.data);
				*((std::string *)node_this->list_var.data)=t;
			}
			first_temp_this=first_temp_this->next;
		}
	}
	return temp_list;
}

#endif
