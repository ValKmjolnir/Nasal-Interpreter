#ifndef __NASAL_HASH_H__
#define __NASAL_HASH_H__

#include <iostream>
#include <cstring>
#include "nasal_list.h"

namespace nasal
{
#ifndef nil
#define nil -1
#endif

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
		NasalHash();
		NasalHash(NasalHash&);
		~NasalHash();
		void PrintHash();
		void Append(const char *,void *,const char *);
		int Contains(const char *);
		NasalList Keys();
		void Delete(const char *);
		NasalHash& operator=(const NasalHash&);
};

}

#endif
