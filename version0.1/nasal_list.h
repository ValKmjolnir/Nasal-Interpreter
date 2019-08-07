#ifndef __NASAL_LIST_H__
#define __NASAL_LIST_H__

#include <iostream>
#include <cstring>


namespace nasal
{
#ifndef nil
#define nil -1
#endif

class var;

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
		void Append(void *,std::string &);
		NasalList& operator=(const NasalList &);
		void SetSize(const int);
		NasalList SubVec(const int,const int);
		var Pop();
		NasalList Sort(const int,const int);
		ListUnit SearchElement(const int);
};

class var
{
	public:
		std::string Type;
		void *data;
		bool isGlobal;
		var()
		{
			data=NULL;
		}
		var(const var &);
		~var();
		var& operator=(const var &);
		void Print();
};

}

#endif