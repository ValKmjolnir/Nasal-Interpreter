#ifndef __NASAL_LIST_CPP__
#define __NASAL_LIST_CPP__

#include "nasal_list.h"
#include "nasal_hash.h"
namespace nasal
{

NasalList::NasalList()
{
	head=new ListUnit;
	head->data=NULL;
	head->next=NULL;
}

NasalList::NasalList(NasalList &Source)
{
	ListUnit *temp;
	ListUnit *SourceTemp=Source.head;
	
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
		else if(temp->Type=="float")
		{
			temp->data=new float;
			*((float *)temp->data)=*((float *)SourceTemp->data);
		}
		else if(temp->Type=="double")
		{
			temp->data=new double;
			*((double *)temp->data)=*((double *)SourceTemp->data);
		}
		else if(temp->Type=="char")
		{
			temp->data=new char;
			*((char *)temp->data)=*((char *)SourceTemp->data);
		}
		else if(temp->Type=="long long int")
		{
			temp->data=new long long int;
			*((long long int *)temp->data)=*((long long int *)SourceTemp->data);
		}
		else if(temp->Type=="string")
		{
			temp->data=new std::string;
			*((std::string *)temp->data)=*((std::string *)SourceTemp->data);
		}
		else if(temp->Type=="array")
		{
			temp->data=new NasalList;
			*((NasalList *)temp->data)=*((NasalList *)SourceTemp->data);
		}
		else if(temp->Type=="hash")
		{
			temp->data=new NasalHash;
			*((NasalHash *)temp->data)=*((NasalHash *)SourceTemp->data);
		}
		else if(temp->Type=="null")
		{
			temp->data=NULL;
		}
		temp->next=NULL;
	}
}

NasalList::~NasalList()
{
	ListUnit *temp=head;
	while(temp->next)
	{
		head=temp->next;
		if(temp->data)
		{
			if(temp->Type=="int")
				delete (int *)temp->data;
			else if(temp->Type=="float")
				delete (float *)temp->data;
			else if(temp->Type=="double")
				delete (double *)temp->data;
			else if(temp->Type=="char")
				delete (char *)temp->data;
			else if(temp->Type=="long long int")
				delete (long long int *)temp->data;
			else if(temp->Type=="string")
				delete (std::string *)temp->data;
			else if(temp->Type=="array")
				delete (NasalList *)temp->data;
			else if(temp->Type=="hash")
				delete (NasalHash *)temp->data;
		}
		delete temp;
		temp=head;
	}
	if(temp->data)
	{
		if(temp->Type=="int")
			delete (int *)temp->data;
		else if(temp->Type=="float")
			delete (float *)temp->data;
		else if(temp->Type=="double")
			delete (double *)temp->data;
		else if(temp->Type=="char")
			delete (char *)temp->data;
		else if(temp->Type=="long long int")
			delete (long long int *)temp->data;
		else if(temp->Type=="string")
			delete (std::string *)temp->data;
		else if(temp->Type=="array")
			delete (NasalList *)temp->data;
		else if(temp->Type=="hash")
			delete (NasalHash *)temp->data;
	}
	delete temp;
}

void NasalList::PrintList()
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
			else if(temp->Type=="float")
				std::cout<<*((float *)temp->data)<<", ";
			else if(temp->Type=="double")
				std::cout<<*((double *)temp->data)<<", ";
			else if(temp->Type=="char")
				std::cout<<"\""<<*((char *)temp->data)<<"\", ";
			else if(temp->Type=="long long int")
				std::cout<<*((long long int *)temp->data)<<", ";
			else if(temp->Type=="string")
				std::cout<<"\""<<*((std::string *)temp->data)<<"\", ";
			else if(temp->Type=="array")
			{
				((NasalList *)temp->data)->PrintList();
				std::cout<<", ";
			}
			else if(temp->Type=="hash")
			{
				((NasalHash *)temp->data)->PrintHash();
				std::cout<<", ";
			}
			else if(temp->Type=="null")
			{
				std::cout<<"NULL, ";
			}
		}
		else
		{
			if(temp->Type=="int")
				std::cout<<*((int *)temp->data);
			else if(temp->Type=="float")
				std::cout<<*((float *)temp->data);
			else if(temp->Type=="double")
				std::cout<<*((double *)temp->data);
			else if(temp->Type=="char")
				std::cout<<"\""<<*((char *)temp->data)<<"\"";
			else if(temp->Type=="long long int")
				std::cout<<*((long long int *)temp->data);
			else if(temp->Type=="string")
				std::cout<<"\""<<*((std::string *)temp->data)<<"\"";
			else if(temp->Type=="array")
				((NasalList *)temp->data)->PrintList();
			else if(temp->Type=="hash")
				((NasalHash *)temp->data)->PrintHash();
			else if(temp->Type=="null")
				std::cout<<"NULL";
		}
	}
	std::cout<<"]";
}
	
void NasalList::Append(void *AppendData,const char *TypeName)
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
	NewListMember->data=NULL;
	NewListMember->Type=TypeName;
	if(TypeName=="int")
	{
		NewListMember->data=new int;
		*((int *)NewListMember->data)=*((int *)AppendData);
	}
	else if(TypeName=="float")
	{
		NewListMember->data=new float;
		*((float *)NewListMember->data)=*((float *)AppendData);
	}
	else if(TypeName=="double")
	{
		NewListMember->data=new double;
		*((double *)NewListMember->data)=*((double *)AppendData);
	}
	else if(TypeName=="char")
	{
		NewListMember->data=new char;
		*((char *)NewListMember->data)=*((char *)AppendData);
	}
	else if(TypeName=="long long int")
	{
		NewListMember->data=new long long int;
		*((long long int *)NewListMember->data)=*((long long int *)AppendData);
	}
	else if(TypeName=="string")
	{
		NewListMember->data=new std::string;
		*((std::string *)NewListMember->data)=*((std::string *)AppendData);
	}
	else if(TypeName=="array")
	{
		NewListMember->data=new NasalList;
		*((NasalList *)NewListMember->data)=TempList;
	}
	else if(TypeName=="hash")
	{
		NewListMember->data=new NasalHash;
		*((NasalHash *)NewListMember->data)=*((NasalHash *)AppendData);
	}
	else if(TypeName=="var")
	{
		NewListMember->Type=(*(var *)AppendData).Type;
		if((*(var *)AppendData).Type=="int")
		{
			NewListMember->data=new int;
			*((int *)NewListMember->data)=*((int *)(*(var *)AppendData).data);
		}
		else if((*(var *)AppendData).Type=="float")
		{
			NewListMember->data=new float;
			*((float *)NewListMember->data)=*((float *)(*(var *)AppendData).data);
		}
		else if((*(var *)AppendData).Type=="double")
		{
			NewListMember->data=new double;
			*((double *)NewListMember->data)=*((double *)(*(var *)AppendData).data);
		}
		else if((*(var *)AppendData).Type=="char")
		{
			NewListMember->data=new char;
			*((char *)NewListMember->data)=*((char *)(*(var *)AppendData).data);
		}
		else if((*(var *)AppendData).Type=="long long int")
		{
			NewListMember->data=new long long int;
			*((long long int *)NewListMember->data)=*((long long int *)(*(var *)AppendData).data);
		}
		else if((*(var *)AppendData).Type=="string")
		{
			NewListMember->data=new std::string;
			*((std::string *)NewListMember->data)=*((std::string *)(*(var *)AppendData).data);
		}
		else if((*(var *)AppendData).Type=="array")
		{
			NewListMember->data=new NasalList;
			*((NasalList *)NewListMember->data)=*((NasalList *)(*(var *)AppendData).data);
		}
		else if((*(var *)AppendData).Type=="hash")
		{
			NewListMember->data=new NasalHash;
			*((NasalHash *)NewListMember->data)=*((NasalHash *)(*(var *)AppendData).data);
		}
	}
	NewListMember->next=NULL;
}

void NasalList::Append(void *AppendData,std::string &TypeName)
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
	NewListMember->data=NULL;
	NewListMember->Type=TypeName;
	if(TypeName=="int")
	{
		NewListMember->data=new int;
		*((int *)NewListMember->data)=*((int *)AppendData);
	}
	else if(TypeName=="float")
	{
		NewListMember->data=new float;
		*((float *)NewListMember->data)=*((float *)AppendData);
	}
	else if(TypeName=="double")
	{
		NewListMember->data=new double;
		*((double *)NewListMember->data)=*((double *)AppendData);
	}
	else if(TypeName=="char")
	{
		NewListMember->data=new char;
		*((char *)NewListMember->data)=*((char *)AppendData);
	}
	else if(TypeName=="long long int")
	{
		NewListMember->data=new long long int;
		*((long long int *)NewListMember->data)=*((long long int *)AppendData);
	}
	else if(TypeName=="string")
	{
		NewListMember->data=new std::string;
		*((std::string *)NewListMember->data)=*((std::string *)AppendData);
	}
	else if(TypeName=="array")
	{
		NewListMember->data=new NasalList;
		*((NasalList *)NewListMember->data)=TempList;
	}
	else if(TypeName=="hash")
	{
		NewListMember->data=new NasalHash;
		*((NasalHash *)NewListMember->data)=*((NasalHash *)AppendData);
	}
	NewListMember->next=NULL;
}

NasalList& NasalList::operator=(const NasalList &Source)
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
			else if(temp->Type=="float")
				delete (float *)temp->data;
			else if(temp->Type=="double")
				delete (double *)temp->data;
			else if(temp->Type=="char")
				delete (char *)temp->data;
			else if(temp->Type=="long long int")
				delete (long long int *)temp->data;
			else if(temp->Type=="string")
				delete (std::string *)temp->data;
			else if(temp->Type=="array")
				delete (NasalList *)temp->data;
			else if(temp->Type=="hash")
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
		else if(temp->Type=="float")
		{
			temp->data=new float;
			*((float *)temp->data)=*((float *)SourceTemp->data);
		}
		else if(temp->Type=="double")
		{
			temp->data=new double;
			*((double *)temp->data)=*((double *)SourceTemp->data);
		}
		else if(temp->Type=="char")
		{
			temp->data=new char;
			*((char *)temp->data)=*((char *)SourceTemp->data);
		}
		else if(temp->Type=="long long int")
		{
			temp->data=new long long int;
			*((long long int *)temp->data)=*((long long int *)SourceTemp->data);
		}
		else if(temp->Type=="string")
		{
			temp->data=new std::string;
			*((std::string *)temp->data)=*((std::string *)SourceTemp->data);
		}
		else if(temp->Type=="array")
		{
			temp->data=new NasalList;
			*((NasalList *)temp->data)=*((NasalList *)SourceTemp->data);
		}
		else if(temp->Type=="hash")
		{
			temp->data=new NasalHash;
			*((NasalHash *)temp->data)=*((NasalHash *)SourceTemp->data);
		}
		else if(temp->Type=="null")
		{
			temp->data=NULL;
		}
		temp->next=NULL;
	}
	return *this;
}

void NasalList::SetSize(const int Size)
{
	if(Size==0)
	{
		ListUnit *temp=head;
		ListUnit *DeleteNode;
		temp=temp->next;
		head->next=NULL;
		while(temp->next)
		{
			DeleteNode=temp;
			if(temp->data)
			{
				if(temp->Type=="int")
					delete (int *)temp->data;
				else if(temp->Type=="float")
					delete (float *)temp->data;
				else if(temp->Type=="double")
					delete (double *)temp->data;
				else if(temp->Type=="char")
					delete (char *)temp->data;
				else if(temp->Type=="long long int")
					delete (long long int *)temp->data;
				else if(temp->Type=="string")
					delete (std::string *)temp->data;
				else if(temp->Type=="array")
					delete (NasalList *)temp->data;
				else if(temp->Type=="hash")
					delete (NasalHash *)temp->data;
			}
			temp=temp->next;
			delete DeleteNode;
		}
		if(temp->data)
		{
			if(temp->Type=="int")
				delete (int *)temp->data;
			else if(temp->Type=="float")
				delete (float *)temp->data;
			else if(temp->Type=="double")
				delete (double *)temp->data;
			else if(temp->Type=="char")
				delete (char *)temp->data;
			else if(temp->Type=="long long int")
				delete (long long int *)temp->data;
			else if(temp->Type=="string")
				delete (std::string *)temp->data;
			else if(temp->Type=="array")
				delete (NasalList *)temp->data;
			else if(temp->Type=="hash")
				delete (NasalHash *)temp->data;
		}
		delete temp;
		return;
	}
	int cnt=0;
	ListUnit *temp=head;
	while(temp->next)
	{
		temp=temp->next;
		++cnt;
		if(cnt==Size)
		{
			ListUnit *DeleteNode=temp->next;
			temp->next=NULL;
			temp=DeleteNode;
			while(DeleteNode->next)
			{
				DeleteNode=DeleteNode->next;
				if(temp->data)
				{
					if(temp->Type=="int")
						delete (int *)temp->data;
					else if(temp->Type=="float")
						delete (float *)temp->data;
					else if(temp->Type=="double")
						delete (double *)temp->data;
					else if(temp->Type=="char")
						delete (char *)temp->data;
					else if(temp->Type=="long long int")
						delete (long long int *)temp->data;
					else if(temp->Type=="string")
						delete (std::string *)temp->data;
					else if(temp->Type=="array")
						delete (NasalList *)temp->data;
					else if(temp->Type=="hash")
						delete (NasalHash *)temp->data;
				}
				delete temp;
				temp=DeleteNode;
			}
			if(temp->data)
			{
				if(temp->Type=="int")
					delete (int *)temp->data;
				else if(temp->Type=="float")
					delete (float *)temp->data;
				else if(temp->Type=="double")
					delete (double *)temp->data;
				else if(temp->Type=="char")
					delete (char *)temp->data;
				else if(temp->Type=="long long int")
					delete (long long int *)temp->data;
				else if(temp->Type=="string")
					delete (std::string *)temp->data;
				else if(temp->Type=="array")
					delete (NasalList *)temp->data;
				else if(temp->Type=="hash")
					delete (NasalHash *)temp->data;
			}
			delete temp;
			return;
		}
	}
	while(cnt<Size)
	{
		temp->next=new ListUnit;
		temp=temp->next;
		temp->Type="null";
		temp->data=NULL;
		++cnt;
	}
	temp->next=NULL;
	return;
}
NasalList NasalList::SubVec(const int start,const int length=nil)
{
	int cnt=-1;
	ListUnit *temp=head;
	while(temp->next)
	{
		temp=temp->next;
		++cnt;
		if(cnt==start)
		{
			NasalList NewNasalList;
			while(temp->next)
			{
				NewNasalList.Append(temp->data,temp->Type);
				temp=temp->next;
				if(cnt==length)
					return NewNasalList;
				++cnt;
			}
			if(length!=nil&&cnt<length)
				std::cout<<"[Error] Length out of range: (length) "<<length<<"|(range) "<<cnt<<"."<<std::endl;
			NewNasalList.Append(temp->data,temp->Type);
			return NewNasalList;
		}
	}
	std::cout<<"[Error] Starting place out of range: (start) "<<start<<"|(range) "<<cnt<<"."<<std::endl;
	NasalList NullList;
	NullList.SetSize(1);
	return NullList;
}

var NasalList::Pop()
{
	ListUnit *temp=head;
	ListUnit *NewLastUnit;
	while(temp->next)
	{
		NewLastUnit=temp;
		temp=temp->next;
	}
	NewLastUnit->next=NULL;
	var TempVar;
	TempVar.Type=temp->Type;
	if(TempVar.Type=="int")
	{
		TempVar.data=new int;
		*((int *)TempVar.data)=*((int *)temp->data);
	}
	else if(TempVar.Type=="float")
	{
		TempVar.data=new float;
		*((float *)TempVar.data)=*((float *)temp->data);
	}
	else if(TempVar.Type=="double")
	{
		TempVar.data=new double;
		*((double *)TempVar.data)=*((double *)temp->data);
	}
	else if(TempVar.Type=="char")
	{
		TempVar.data=new char;
		*((char *)TempVar.data)=*((char *)temp->data);
	}
	else if(TempVar.Type=="long long int")
	{
		TempVar.data=new long long int;
		*((long long int *)TempVar.data)=*((long long int *)temp->data);
	}
	else if(TempVar.Type=="string")
	{
		TempVar.data=new std::string;
		*((std::string *)TempVar.data)=*((std::string *)temp->data);
	}
	else if(TempVar.Type=="array")
	{
		TempVar.data=new NasalList;
		*((NasalList *)TempVar.data)=*((NasalList *)temp->data);
	}
	else if(TempVar.Type=="hash")
	{
		TempVar.data=new NasalHash;
		*((NasalHash *)TempVar.data)=*((NasalHash*)temp->data);
	}
	else if(TempVar.Type=="null")
		TempVar.data=NULL;
	
	if(temp->data)
	{
		if(temp->Type=="int")
			delete (int *)temp->data;
		else if(temp->Type=="float")
			delete (float *)temp->data;
		else if(temp->Type=="double")
			delete (double *)temp->data;
		else if(temp->Type=="char")
			delete (char *)temp->data;
		else if(temp->Type=="long long int")
			delete (long long int *)temp->data;
		else if(temp->Type=="string")
			delete (std::string *)temp->data;
		else if(temp->Type=="array")
			delete (NasalList *)temp->data;
		else if(temp->Type=="hash")
			delete (NasalHash *)temp->data;
	}
	delete temp;
	
	return TempVar;
}

#ifndef __SORT_TYPE__
#define SORT_INT 1
#define SORT_STRING 2
#endif

NasalList NasalList::Sort(const int SortType,const int _cmp)
{
	NasalList TempList;
	if(SortType==SORT_INT)
	{
		ListUnit *temp=head;
		while(temp->next)
		{
			temp=temp->next;
			if(temp->Type!="int")
			{
				std::cout<<"[Error] Incorrect type inside: "<<temp->Type<<".But type must be int."<<std::endl;
				TempList.SetSize(1);
				return TempList;
			}
		}
		if(temp->Type!="int")
		{
			std::cout<<"[Error] Incorrect type inside: "<<temp->Type<<".But type must be int."<<std::endl;
			TempList.SetSize(1);
			return TempList;
		}
		TempList=*this;
		
		
		ListUnit *FirstTempThis;
		ListUnit *SecondTempThis;
		ListUnit *NodeThis;
		FirstTempThis=TempList.head->next;
		while(FirstTempThis->next)
		{
			NodeThis=FirstTempThis;
			SecondTempThis=FirstTempThis->next;
			while(SecondTempThis->next)
			{
				if(_cmp>0 && *((int *)NodeThis->data)>*((int *)SecondTempThis->data))//from small to large
				{
					NodeThis=SecondTempThis;
				}
				else if(_cmp<=0 && *((int *)NodeThis->data)<*((int *)SecondTempThis->data))//from large to small
				{
					NodeThis=SecondTempThis;
				}
				SecondTempThis=SecondTempThis->next;
			}
			if(_cmp>0 && *((int *)NodeThis->data)>*((int *)SecondTempThis->data))//from small to large func(a,b) a-b
			{
				NodeThis=SecondTempThis;
			}
			else if(_cmp<=0 && *((int *)NodeThis->data)<*((int *)SecondTempThis->data))//from large to small func(a,b) b-a
			{
				NodeThis=SecondTempThis;
			}
			if(NodeThis!=FirstTempThis)
			{
				int t;
				t=*((int *)FirstTempThis->data);
				*((int *)FirstTempThis->data)=*((int *)NodeThis->data);
				*((int *)NodeThis->data)=t;
			}
			FirstTempThis=FirstTempThis->next;
		}
	}
	else if(SortType==SORT_STRING)
	{
		ListUnit *temp=head;
		while(temp->next)
		{
			temp=temp->next;
			if(temp->Type!="string")
			{
				std::cout<<"[Error] Incorrect type inside: "<<temp->Type<<".But type must be string."<<std::endl;
				TempList.SetSize(1);
				return TempList;
			}
		}
		if(temp->Type!="string")
		{
			std::cout<<"[Error] Incorrect type inside: "<<temp->Type<<".But type must be string."<<std::endl;
			TempList.SetSize(1);
			return TempList;
		}
		TempList=*this;
		
		
		ListUnit *FirstTempThis;
		ListUnit *SecondTempThis;
		ListUnit *NodeThis;
		FirstTempThis=TempList.head->next;
		while(FirstTempThis->next)
		{
			NodeThis=FirstTempThis;
			SecondTempThis=FirstTempThis->next;
			while(SecondTempThis->next)
			{
				if(_cmp>0 && *((std::string *)NodeThis->data)>*((std::string *)SecondTempThis->data))//from small to large
				{
					NodeThis=SecondTempThis;
				}
				else if(_cmp<=0 && *((std::string *)NodeThis->data)<*((std::string *)SecondTempThis->data))//from large to small
				{
					NodeThis=SecondTempThis;
				}
				SecondTempThis=SecondTempThis->next;
			}
			if(_cmp>0 && *((std::string *)NodeThis->data)>*((std::string *)SecondTempThis->data))//from small to large func(a,b) cmp(a,b)
			{
				NodeThis=SecondTempThis;
			}
			else if(_cmp<=0 && *((std::string *)NodeThis->data)<*((std::string *)SecondTempThis->data))//from large to small func(a,b) -cmp(a,b) or cmp(b,a)
			{
				NodeThis=SecondTempThis;
			}
			if(NodeThis!=FirstTempThis)
			{
				std::string t;
				t=*((std::string *)FirstTempThis->data);
				*((std::string *)FirstTempThis->data)=*((std::string *)NodeThis->data);
				*((std::string *)NodeThis->data)=t;
			}
			FirstTempThis=FirstTempThis->next;
		}
	}
	return TempList;
}

var::~var()
{
	if(data)
	{
		if(Type=="int")
			delete (int *)data;
		else if(Type=="float")
			delete (float *)data;
		else if(Type=="double")
			delete (double *)data;
		else if(Type=="char")
			delete (char *)data;
		else if(Type=="long long int")
			delete (long long int *)data;
		else if(Type=="string")
			delete (std::string *)data;
		else if(Type=="array")
			delete (NasalList *)data;
		else if(Type=="hash")
			delete (NasalHash *)data;
	}
}

}

#endif
