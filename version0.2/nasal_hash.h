#ifndef __NASAL_HASH_H__
#define __NASAL_HASH_H__


struct nasal_hash_unit
{
	std::string name;
	std::string type;
	void *data;
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
		void append_var(var&);
		void append_list(nasal_list&);
		void append_hash(nasal_hash&);
		int contains(std::string&);
		int delete_element(std::string&);
		nasal_list keys();
};

#endif
