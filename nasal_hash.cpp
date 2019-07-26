#ifndef __NASAL_HASH_CPP__
#define __NASAL_HASH_CPP__

#include "nasal_hash.h"
#include "nasal_list.h"
namespace nasal
{

NasalHash::NasalHash()
{
	head=new HashUnit;
	head->data=NULL;
	head->next=NULL;
}
NasalHash::NasalHash(NasalHash &Source)
{
	HashUnit *temp;
	HashUnit *SourceTemp=Source.head;
	
	head=new HashUnit;
	head->next=NULL;
	head->data=NULL;
	
	temp=head;
	while(SourceTemp->next)
	{
		SourceTemp=SourceTemp->next;
		temp->next=new HashUnit;
		temp=temp->next;
		temp->Type=SourceTemp->Type;
		temp->VarName=SourceTemp->VarName;
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
		{
			temp->data=new NasalHash;
			*((NasalHash *)temp->data)=*((NasalHash *)SourceTemp->data);
		}
		temp->next=NULL;
	}
}
NasalHash::~NasalHash()
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
}
void NasalHash::PrintHash()
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
void NasalHash::Append(const char *VariaName,void *AppendData,const char *TypeName)
{
	NasalHash TempHash;//sometimes user may use n.append(n)
	if(TypeName=="hash")
		TempHash=*((NasalHash *)AppendData);
	HashUnit *temp=head;
	while(temp->next)
	{
		temp=temp->next;
	}
	HashUnit *NewHashMember=new HashUnit;
	NewHashMember->data=NULL;
	temp->next=NewHashMember;
	NewHashMember->VarName=VariaName;
	NewHashMember->Type=TypeName;
	if(TypeName=="int")
	{
		NewHashMember->data=new int;
		*((int *)NewHashMember->data)=*((int *)AppendData);
	}
	else if(TypeName=="float")
	{
		NewHashMember->data=new float;
		*((float *)NewHashMember->data)=*((float *)AppendData);
	}
	else if(TypeName=="double")
	{
		NewHashMember->data=new double;
		*((double *)NewHashMember->data)=*((double *)AppendData);
	}
	else if(TypeName=="char")
	{
		NewHashMember->data=new char;
		*((char *)NewHashMember->data)=*((char *)AppendData);
	}
	else if(TypeName=="long long int")
	{
		NewHashMember->data=new long long int;
		*((long long int *)NewHashMember->data)=*((long long int *)AppendData);
	}
	else if(TypeName=="string")
	{
		NewHashMember->data=new std::string;
		*((std::string *)NewHashMember->data)=*((std::string *)AppendData);
	}
	else if(TypeName=="array")
	{
		NewHashMember->data=new NasalList;
		*((NasalList *)NewHashMember->data)=*((NasalList *)AppendData);
	}
	else if(TypeName=="hash")
	{
		NewHashMember->data=new NasalHash;
		*((NasalHash *)NewHashMember->data)=TempHash;
	}
	NewHashMember->next=NULL;
}
int NasalHash::Contains(const char *VariaName)
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
NasalList NasalHash::Keys()
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
void NasalHash::Delete(const char *VariaName)
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
NasalHash& NasalHash::operator=(const NasalHash &Source)
{
	HashUnit *temp=head;
	HashUnit *SourceTemp=Source.head;
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
	head=new HashUnit;
	head->next=NULL;
	
	temp=head;
	while(SourceTemp->next)
	{
		SourceTemp=SourceTemp->next;
		temp->next=new HashUnit;
		temp=temp->next;
		temp->Type=SourceTemp->Type;
		temp->VarName=SourceTemp->VarName;
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
		{
			temp->data=new NasalHash;
			*((NasalHash *)temp->data)=*((NasalHash *)SourceTemp->data);
		}
		temp->next=NULL;
	}
	return *this;
}

}
#endif
