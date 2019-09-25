#ifndef __NASAL_VAR_CPP__
#define __NASAL_VAR_CPP__

var::var()
{
	ptr=NULL;
	type=var_null;
}
var::var(const var& temp)
{
	switch(temp.type)
	{
		case var_null:ptr=NULL;break;
		case var_number:ptr=new double;*((double*)ptr)=*((double*)temp.ptr);break;
		case var_string:ptr=new std::string;*((std::string*)ptr)=*((std::string*)temp.ptr);break;
		case var_list:ptr=new nasal_list;*((nasal_list*)ptr)=*((nasal_list*)temp.ptr);break;
		case var_hash:ptr=new nasal_hash;*((nasal_hash*)ptr)=*((nasal_hash*)temp.ptr);break;
	}
	type=temp.type;
}
var::~var()
{
	switch(type)
	{
		case var_null:break;
		case var_number:delete (double*)ptr;break;
		case var_string:delete (std::string*)ptr;break;
		case var_list:delete (nasal_list*)ptr;break;
		case var_hash:delete (nasal_hash*)ptr;break;
	}
}

nasal_list::nasal_list()
{
	elem.clear();
}
nasal_list::nasal_list(const nasal_list& temp)
{
	elem=temp.elem;
}

nasal_hash::nasal_hash()
{
	elem.clear();
	elem_name.clear();
}
nasal_hash::nasal_hash(const nasal_hash& temp)
{
	elem=temp.elem;
	elem_name=temp.elem_name;
}

nasal_function::nasal_function()
{
	root.clear();
}
nasal_function::nasal_function(const nasal_function& temp)
{
	root=temp.root;
}
#endif
