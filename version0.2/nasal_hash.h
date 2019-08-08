#ifndef __NASAL_HASH_H__
#define __NASAL_HASH_H__

#include "nasal_var.h"

struct nasal_hash_unit
{
	std::string name;
	var hash_var;
	nasal_hash_unit *next;
};

class nasal_hash
{
	private:
		nasal_hash_unit *head;
	public:
		nasal_hash();
		~nasal_hash();
		nasal_hash& operator=(const nasal_hash&);
		void append(std::string&,var&);
		int contains(std::string&);
		int delete_element(std::string&);
		nasal_list keys();
};

#endif
