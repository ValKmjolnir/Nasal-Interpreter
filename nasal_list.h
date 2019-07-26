#ifndef __NASAL_LIST_H__
#define __NASAL_LIST_H__

#include <iostream>
#include <cstring>


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
		NasalList();
		NasalList(NasalList &);
		~NasalList();
		void PrintList();
		void Append(void *,const char *);
		NasalList& operator=(const NasalList &);
};

}

#endif
