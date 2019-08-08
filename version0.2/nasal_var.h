#ifndef __NASAL_VAR_H__
#define __NASAL_VAR_H__

#define VAR_NONE   0
#define VAR_LLINT  1
#define VAR_DOUBLE 2
#define VAR_CHAR   3
#define VAR_STRING 4
#define VAR_LIST   5
#define VAR_HASH   6
#define VAR_FUNC   7

class var
{
	public:
		int type;
		void *data;
		var();
		var(const var&);
		~var();
		var& operator=(const var&);
};

#endif
