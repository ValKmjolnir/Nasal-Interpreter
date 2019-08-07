#ifndef __NASAL_VAR_CPP__
#define __NASAL_VAR_CPP__

var::var()
{
	type=VAR_NONE;
	data=NULL;
}
var::var(const var &temp)
{
	type=temp.type;
	switch(type)
	{
		case VAR_NONE:
			data=NULL;
			break;
		case VAR_LLINT:
			data=new long long int;
			*((long long int *)data)=*((long long int *)temp.data);
			break;
		case VAR_DOUBLE:
			data=new double;
			*((double *)data)=*((double *)temp.data);
			break;
		case VAR_CHAR:
			data=new char;
			*((char *)data)=*((char *)temp.data);
			break;
		case VAR_STRING:
			data=new std::string;
			*((std::string *)data)=*((std::string *)temp.data);
			break;
		case VAR_LIST:
			data=new nasal_list;
			*((nasal_list *)data)=*((nasal_list *)temp.data);
			break;
		case VAR_HASH:
			data=new nasal_hash;
			*((nasal_hash *)data)=*((nasal_hash *)temp.data);
			break;
	}
}
var::~var()
{
	if(data)
		switch(type)
		{
			case VAR_LLINT:
				delete (long long int *)data;
				break;
			case VAR_DOUBLE:
				delete (double *)data;
				break;
			case VAR_CHAR:
				delete (char *)data;
				break;
			case VAR_STRING:
				delete (std::string *)data;
				break;
			case VAR_LIST:
				delete (nasal_list *)data;
				break;
			case VAR_HASH:
				delete (nasal_hash *)data;
				break;
		}
}
var& var::operator=(const var &temp)
{
	if(data)
		switch(type)
		{
			case VAR_LLINT:
				delete (long long int *)data;
				break;
			case VAR_DOUBLE:
				delete (double *)data;
				break;
			case VAR_CHAR:
				delete (char *)data;
				break;
			case VAR_STRING:
				delete (std::string *)data;
				break;
			case VAR_LIST:
				delete (nasal_list *)data;
				break;
			case VAR_HASH:
				delete (nasal_hash *)data;
				break;
		}
	type=temp.type;
	switch(type)
	{
		case VAR_NONE:
			data=NULL;
			break;
		case VAR_LLINT:
			data=new long long int;
			*((long long int *)data)=*((long long int *)temp.data);
			break;
		case VAR_DOUBLE:
			data=new double;
			*((double *)data)=*((double *)temp.data);
			break;
		case VAR_CHAR:
			data=new char;
			*((char *)data)=*((char *)temp.data);
			break;
		case VAR_STRING:
			data=new std::string;
			*((std::string *)data)=*((std::string *)temp.data);
			break;
		case VAR_LIST:
			data=new nasal_list;
			*((nasal_list *)data)=*((nasal_list *)temp.data);
			break;
		case VAR_HASH:
			data=new nasal_hash;
			*((nasal_hash *)data)=*((nasal_hash *)temp.data);
			break;
	}
	return *this;
}

#endif
