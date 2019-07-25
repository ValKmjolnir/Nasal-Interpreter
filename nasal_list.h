#ifndef __NASAL_LIST_H__
#define __NASAL_LIST_H__

#include <iostream>
#include <cstring>
#include "nasal_hash.h"

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
			head->data=NULL;
			head->next=NULL;
		}
		~NasalList()
		{
			ListUnit *temp=head;
			while(temp->next)
			{
				head=temp->next;
				if(temp->data)
				{
					if(temp->Type=="int")
						delete (int *)temp->data;
					if(temp->Type=="float")
						delete (float *)temp->data;
					if(temp->Type=="double")
						delete (double *)temp->data;
					if(temp->Type=="char")
						delete (char *)temp->data;
					if(temp->Type=="long long int")
						delete (long long int *)temp->data;
					if(temp->Type=="string")
						delete (std::string *)temp->data;
					if(temp->Type=="array")
						delete (NasalList *)temp->data;
					if(temp->Type=="hash");
						//delete (NasalHash *)temp->data;
				}
				delete temp;
				temp=head;
			}
			if(temp->data)
			{
				if(temp->Type=="int")
					delete (int *)temp->data;
				if(temp->Type=="float")
					delete (float *)temp->data;
				if(temp->Type=="double")
					delete (double *)temp->data;
				if(temp->Type=="char")
					delete (char *)temp->data;
				if(temp->Type=="long long int")
					delete (long long int *)temp->data;
				if(temp->Type=="string")
					delete (std::string *)temp->data;
				if(temp->Type=="array")
					delete (NasalList *)temp->data;
				if(temp->Type=="hash")
					;
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
					if(temp->Type=="string")
						std::cout<<"\""<<*((std::string *)temp->data)<<"\", ";
					if(temp->Type=="array")
					{
						((NasalList *)temp->data)->PrintList();
						std::cout<<", ";
					}
					if(temp->Type=="hash")
					{
						;//((NasalHash *)temp->data)->PrintHash();
						std::cout<<", ";
					}
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
					if(temp->Type=="string")
						std::cout<<"\""<<*((std::string *)temp->data)<<"\"";
					if(temp->Type=="array")
						((NasalList *)temp->data)->PrintList();
					if(temp->Type=="hash")
						;//((NasalHash *)temp->data)->PrintHash();
				}
			}
			std::cout<<"]";
		}
		void Append(void *AppendData,const char *TypeName)
		{
			NasalList TempList;//sometimes user may use n.append(n)
			if(TypeName=="array")
				TempList=*((NasalList *)AppendData);
			ListUnit *temp=head;
			while(temp->next)
			{
				temp=temp->next;
			}
			ListUnit *NewListMember=new ListUnit;
			temp->next=NewListMember;
			NewListMember->Type=TypeName;
			if(TypeName=="int")
			{
				NewListMember->data=new int;
				*((int *)NewListMember->data)=*((int *)AppendData);
			}
			if(TypeName=="float")
			{
				NewListMember->data=new float;
				*((float *)NewListMember->data)=*((float *)AppendData);
			}
			if(TypeName=="double")
			{
				NewListMember->data=new double;
				*((double *)NewListMember->data)=*((double *)AppendData);
			}
			if(TypeName=="char")
			{
				NewListMember->data=new char;
				*((char *)NewListMember->data)=*((char *)AppendData);
			}
			if(TypeName=="long long int")
			{
				NewListMember->data=new long long int;
				*((long long int *)NewListMember->data)=*((long long int *)AppendData);
			}
			if(TypeName=="string")
			{
				NewListMember->data=new std::string;
				*((std::string *)NewListMember->data)=*((std::string *)AppendData);
			}
			if(TypeName=="array")
			{
				NewListMember->data=new NasalList;
				*((NasalList *)NewListMember->data)=TempList;
			}
			if(TypeName=="hash")
				;
			NewListMember->next=NULL;
		}
		NasalList& operator=(const NasalList &Source)
		{
			ListUnit *temp=head;
			ListUnit *SourceTemp=Source.head;
			while(temp->next)
			{
				head=temp->next;
				if(temp->data)
				{
					if(temp->Type=="int")
						delete (int *)temp->data;
					if(temp->Type=="float")
						delete (float *)temp->data;
					if(temp->Type=="double")
						delete (double *)temp->data;
					if(temp->Type=="char")
						delete (char *)temp->data;
					if(temp->Type=="long long int")
						delete (long long int *)temp->data;
					if(temp->Type=="string")
						delete (std::string *)temp->data;
					if(temp->Type=="array")
						delete (NasalList *)temp->data;
					if(temp->Type=="hash")
						;
				}
				delete temp;
				temp=head;
			}
			if(temp->data)
			{
				if(temp->Type=="int")
					delete (int *)temp->data;
				if(temp->Type=="float")
					delete (float *)temp->data;
				if(temp->Type=="double")
					delete (double *)temp->data;
				if(temp->Type=="char")
					delete (char *)temp->data;
				if(temp->Type=="long long int")
					delete (long long int *)temp->data;
				if(temp->Type=="string")
					delete (std::string *)temp->data;
				if(temp->Type=="array")
					delete (NasalList *)temp->data;
				if(temp->Type=="hash")
					;
			}
			delete temp;
			head=new ListUnit;
			head->next=NULL;
			
			temp=head;
			while(SourceTemp->next)
			{
				SourceTemp=SourceTemp->next;
				temp->next=new ListUnit;
				temp=temp->next;
				temp->Type=SourceTemp->Type;
				if(temp->Type=="int")
				{
					temp->data=new int;
					*((int *)temp->data)=*((int *)SourceTemp->data);
				}
				if(temp->Type=="float")
				{
					temp->data=new float;
					*((float *)temp->data)=*((float *)SourceTemp->data);
				}
				if(temp->Type=="double")
				{
					temp->data=new double;
					*((double *)temp->data)=*((double *)SourceTemp->data);
				}
				if(temp->Type=="char")
				{
					temp->data=new char;
					*((char *)temp->data)=*((char *)SourceTemp->data);
				}
				if(temp->Type=="long long int")
				{
					temp->data=new long long int;
					*((long long int *)temp->data)=*((long long int *)SourceTemp->data);
				}
				if(temp->Type=="string")
				{
					temp->data=new std::string;
					*((std::string *)temp->data)=*((std::string *)SourceTemp->data);
				}
				if(temp->Type=="array")
				{
					temp->data=new NasalList;
					*((NasalList *)temp->data)=*((NasalList *)SourceTemp->data);
				}
				if(temp->Type=="hash")
					;
				temp->next=NULL;
			}
			return *this;
		}
};

}

#endif
