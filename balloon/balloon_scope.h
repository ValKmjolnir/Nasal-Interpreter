#ifndef __BALLOON_SCOPE_H__
#define __BALLOON_SCOPE_H__

class balloon_scope
{
	private:
		std::list<var> global;
		std::list<std::list<std::list<var> > > scope_list;
	public:
		void set_clear()
		{
			global.clear();
			scope_list.clear();
			return;
		}
		bool search_var(std::string name)
		{
			if(!scope_list.empty())
			{
				std::list<std::list<var> >::iterator i=scope_list.back().end();
				--i;
				for(;;--i)
				{
					for(std::list<var>::iterator j=i->begin();j!=i->end();++j)
						if(j->get_name()==name)
							return true;
					if(i==scope_list.back().begin())
						break;
				}
			}
			if(!global.empty())
			{
				for(std::list<var>::iterator i=global.begin();i!=global.end();++i)
					if(i->get_name()==name)
						return true;
			}
			return false;
		}
		void add_new_var(var t)
		{
			if(!scope_list.empty())
			{
				std::list<std::list<var> >::iterator i=scope_list.back().end();
				--i;
				i->push_back(t);
				return;
			}
			global.push_back(t);
			return;
		}
		var get_var(std::string name)
		{
			if(!scope_list.empty())
			{
				std::list<std::list<var> >::iterator i=scope_list.back().end();
				--i;
				// get the last scope block(std::list<std::list<var> >)
				for(;;--i)
				{
					for(std::list<var>::iterator j=i->begin();j!=i->end();++j)
						if(j->get_name()==name)
							return *j;
					if(i==scope_list.back().begin())
						break;
				}
			}
			if(!global.empty())
			{
				for(std::list<var>::iterator i=global.begin();i!=global.end();++i)
					if(i->get_name()==name)
						return *i;
			}
			return error_var;
		}
		var* get_addr(std::string name)
		{
			var* addr=NULL;
			if(!scope_list.empty())
			{
				std::list<std::list<var> >::iterator i=scope_list.back().end();
				--i;
				// get the last scope block(std::list<std::list<var> >)
				for(;;--i)
				{
					for(std::list<var>::iterator j=i->begin();j!=i->end();++j)
						if(j->get_name()==name)
						{
							addr=&(*j);
							return addr;
						}
					if(i==scope_list.back().begin())
						break;
				}
			}
			if(!global.empty())
			{
				for(std::list<var>::iterator i=global.begin();i!=global.end();++i)
					if(i->get_name()==name)
					{
						addr=&(*i);
						return addr;
					}
			}
			return &error_var;
		}
		void add_new_block_scope()
		{
			std::list<std::list<var> > new_list;
			scope_list.push_back(new_list);
			return;
		}
		void add_new_local_scope()
		{
			if(!scope_list.empty())
			{
				std::list<std::list<std::list<var> > >::iterator i=scope_list.end();
				--i;
				std::list<var> new_list;
				i->push_back(new_list);
			}
			return;
		}
		void pop_last_block_scope()
		{
			if(!scope_list.empty())
				scope_list.pop_back();
			return;
		}
		void pop_last_local_scope()
		{
			if(!scope_list.empty())
			{
				std::list<std::list<var> > temp=scope_list.back();
				temp.pop_back();
			}
			return;
		}
};

balloon_scope scope;

#endif
