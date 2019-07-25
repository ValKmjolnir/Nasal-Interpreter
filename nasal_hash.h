#ifndef __NASAL_HASH_H__
#define __NASAL_HASH_H__

#include <iostream>
#include <cstring>
#include "nasal_list.h"

namespace nasal
{


struct HashUnit
{
	std::string VarName;
	std::string Type;
	void *data;
	HashUnit* next;
};

class NasalHash
{
	private:
		HashUnit *head;
	public:
		NasalHash()
		{
			head=new HashUnit;
			head->next=NULL;
		}
		~NasalHash()
		{
			HashUnit *temp=head;
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
						delete (NasalHash *)temp->data;
				}
				delete temp;
				temp=head;
			}
			delete temp;
		}
		void PrintHash()
		{
			HashUnit *temp=head;
			std::cout<<"{ ";
			while(temp->next)
			{
				temp=temp->next;
				std::cout<<temp->VarName<<":";
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
						((NasalHash *)temp->data)->PrintHash();
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
						((NasalHash *)temp->data)->PrintHash();
				}
			}
			std::cout<<"}";
		}
		void Append(const char *VariaName,void *AppendData,const char *TypeName)
		{
			HashUnit *temp=head;
			while(temp->next)
			{
				temp=temp->next;
			}
			HashUnit *NewHashMember=new HashUnit;
			temp->next=NewHashMember;
			NewHashMember->VarName=VariaName;
			NewHashMember->Type=TypeName;
			if(TypeName=="int")
			{
				NewHashMember->data=new int;
				*((int *)NewHashMember->data)=*((int *)AppendData);
			}
			if(TypeName=="float")
			{
				NewHashMember->data=new float;
				*((float *)NewHashMember->data)=*((float *)AppendData);
			}
			if(TypeName=="double")
			{
				NewHashMember->data=new double;
				*((double *)NewHashMember->data)=*((double *)AppendData);
			}
			if(TypeName=="char")
			{
				NewHashMember->data=new char;
				*((char *)NewHashMember->data)=*((char *)AppendData);
			}
			if(TypeName=="long long int")
			{
				NewHashMember->data=new long long int;
				*((long long int *)NewHashMember->data)=*((long long int *)AppendData);
			}
			if(TypeName=="string")
			{
				NewHashMember->data=new std::string;
				*((std::string *)NewHashMember->data)=*((std::string *)AppendData);
			}
			if(temp->Type=="array")
				;
			if(temp->Type=="hash")
				;
			NewHashMember->next=NULL;
		}
		int Contains(const char *VariaName)
		{
			HashUnit *temp=head;
			while(temp->next)
			{
				temp=temp->next;
				if(temp->VarName==VariaName)
					return 1;
			}
			return 0;
		}
		NasalList Keys()
		{
			NasalList FeedBackList;
			HashUnit *temp=head;
			while(temp->next)
			{
				temp=temp->next;
				FeedBackList.Append(&(temp->VarName),"string");
			}
			return FeedBackList;
		}
		void Delete(const char *VariaName)
		{
			HashUnit *temp=head;
			HashUnit *LastNode;
			while(temp->next)
			{
				LastNode=temp;
				temp=temp->next;
				if(temp->VarName==VariaName)
				{
					LastNode->next=temp->next;
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
						delete (NasalHash *)temp->data;
					delete temp;
					return;
				}
			}
			std::cout<<"[Error]: Could not find this element \""<<VariaName<<"\"."<<std::endl;
			return;
		}
};

}

#endif
