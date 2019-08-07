#ifndef __NASAL_LIST_H__
#define __NASAL_LIST_H__

class nasal_hash;

struct nasal_list_unit
{
	std::string type;
	void *data;
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
		void append_var(var&);
		void append_list(nasal_list&);
		void append_hash(nasal_hash&);
		void setsize(const int);
		nasal_list subvec(const int,const int);
		var pop();
		void sort_list(bool);
};

#endif
