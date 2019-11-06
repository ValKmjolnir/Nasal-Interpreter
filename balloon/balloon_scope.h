#ifndef __BALLOON_SCOPE_H__
#define __BALLOON_SCOPE_H__

class balloon_scope
{
	private:
		std::list<var> scope;
		var error_var;
	public:
		void set_clear()
		{
			scope.clear();
			return;
		}
		bool search_var(std::string name)
		{
			for(std::list<var>::iterator i=scope.begin();i!=scope.end();++i)
				if(i->get_name()==name)
					return true;
			return false;
		}
		void add_var(var t)
		{
			scope.push_back(t);
			return;
		}
		var& get_var(std::string name)
		{
			for(std::list<var>::iterator i=scope.begin();i!=scope.end();++i)
				if(i->get_name()==name)
					return *i;
			std::cout<<">>[Runtime-error] \'"<<name<<"\' was not declared in this scope."<<std::endl;
			return error_var;
		}
};

#endif
