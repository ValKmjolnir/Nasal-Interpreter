#ifndef __NASAL_HASH_H__
#define __NASAL_HASH_H__

#include<iostream>
#include<cstring>
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
			NewHashMember->data=AppendData;
			NewHashMember->VarName=VariaName;
			NewHashMember->Type=TypeName;
			NewHashMember->next=NULL;
		}
};

}

#endif
