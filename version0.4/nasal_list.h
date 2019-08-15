#ifndef __NASAL_LIST_H__
#define __NASAL_LIST_H__

#include "nasal_var.h"

#define SORT_INT 1
#define SORT_DBL 2
#define SORT_STR 3

class nasal_hash;

struct nasal_list_unit
{
	var list_var;
	nasal_list_unit *next;
};

class nasal_list
{
	private:
		nasal_list_unit *head;
	public:
		nasal_list();
		~nasal_list();
		nasal_list& operator=(const nasal_list&);
		void append(var&);
		void setsize(const int);
		nasal_list subvec(const int,const int);
		var pop();
		nasal_list sort_list(const int,const int);
};

#endif
