#ifndef __NASAL_VAR_H__
#define __NASAL_VAR_H__

#include <cstring>
#include <list>

enum var_type
{
	var_null=0xaa55,
	var_number,
	var_string,
	var_list,
	var_hash
};

class nasal_list;
class nasal_hash;

class var
{
	private:
		void* ptr;
		int type;
	public:
		var();
		var(const var&);
		~var();
};

class nasal_list
{
	private:
		std::list<var> elem;
	public:
		nasal_list();
		nasal_list(const nasal_list&);
};

class nasal_hash
{
	private:
		std::list<var> elem;
		std::list<std::string> elem_name;
	public:
		nasal_hash();
		nasal_hash(const nasal_hash&);
};

#endif
