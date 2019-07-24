#ifndef __NASAL_LIST_H__
#define __NASAL_LIST_H__

#include<iostream>
#include<cstring>
namespace nasal
{


struct ListUnit
{
	std::string Type;
	void *data;
	ListUnit* next;
};

class NasalList
{
	private:
		ListUnit *head;
	public:
		NasalList()
		{
			head=new ListUnit;
			head->next=NULL;
		}
		~NasalList()
		{
			ListUnit *temp=head;
			while(temp->next)
			{
				head=temp->next;
				delete temp;
				temp=head;
			}
			delete temp;
		}
		void PrintList()
		{
			ListUnit *temp=head;
			std::cout<<"[ ";
			while(temp->next)
			{
				temp=temp->next;
				if(temp->next)
				{
					if(temp->Type=="int")
						std::cout<<*((int *)temp->data)<<", ";
					if(temp->Type=="float")
						std::cout<<*((float *)temp->data)<<", ";
					if(temp->Type=="double")
						std::cout<<*((double *)temp->data)<<", ";
					if(temp->Type=="char")
						std::cout<<"\""<<*((char *)temp->data)<<"\", ";
					if(temp->Type=="long long int")
						std::cout<<*((long long int *)temp->data)<<", ";
				}
				else
				{
					if(temp->Type=="int")
						std::cout<<*((int *)temp->data);
					if(temp->Type=="float")
						std::cout<<*((float *)temp->data);
					if(temp->Type=="double")
						std::cout<<*((double *)temp->data);
					if(temp->Type=="char")
						std::cout<<"\""<<*((char *)temp->data)<<"\"";
					if(temp->Type=="long long int")
						std::cout<<*((long long int *)temp->data);
				}
			}
			std::cout<<"]";
		}
		void Append(void *AppendData,const char *TypeName)
		{
			ListUnit *temp=head;
			while(temp->next)
			{
				temp=temp->next;
			}
			ListUnit *NewListMember=new ListUnit;
			temp->next=NewListMember;
			NewListMember->data=AppendData;
			NewListMember->Type=TypeName;
			NewListMember->next=NULL;
		}
};

}

#endif
