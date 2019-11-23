#ifndef __BALLOON_SCOPE_H__
#define __BALLOON_SCOPE_H__

class balloon_scope
{
	private:
		std::list<var> global;
		std::list<std::list<std::list<var> > > scope_list;
		/*
			example:
				std::list<var> global                  // global scope
				std::list<std::list<std::list<var> > > // total scope
					-> std::list<std::list<var> >      // block scope
						->std::list<var>               // local scope
							-> var -> var -> var -> var
		*/
	public:
		void set_clear()
		{
			global.clear();
			scope_list.clear();
			return;
		}
		bool check_redefine(std::string name)
		{
			if(!scope_list.empty() && !scope_list.back().empty())
			{
				// check the last scope
				std::list<std::list<var> >::iterator i=scope_list.back().end();
				--i;
				for(std::list<var>::iterator j=i->begin();j!=i->end();++j)
					if(j->get_name()==name)
						return true;
				return false;
			}
			if(!global.empty())
			{
				for(std::list<var>::iterator i=global.begin();i!=global.end();++i)
					if(i->get_name()==name)
						return true;
				return false;
			}
			return false;
		}
		bool search_var(std::string name)
		{
			if(!scope_list.empty() && !scope_list.back().empty())
			{
				std::list<std::list<var> >::iterator i=scope_list.back().end();
				--i;
				for(;;--i)
				{
					if(!i->empty())// avoid sigsegv
					{
						for(std::list<var>::iterator j=i->begin();j!=i->end();++j)
							if(j->get_name()==name)
								return true;
					}
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
			if(!scope_list.empty() && !scope_list.back().empty())
			{
				std::list<std::list<var> >::iterator i=scope_list.back().end();
				--i;
				i->push_back(t);
				return;
			}
			global.push_back(t);
			return;
		}
		void print_last_block()
		{
			if(!scope_list.empty() && !scope_list.back().empty())
			{
				std::list<std::list<var> >::iterator i=scope_list.back().end();
				--i;
				// get the last scope block(std::list<std::list<var> >)
				for(;;--i)
				{
					if(!i->empty())// avoid sigsegv
					{
						for(std::list<var>::iterator j=i->begin();j!=i->end();++j)
							std::cout<<j->get_name()<<std::endl;
					
					}
					if(i==scope_list.back().begin())
						break;
				}
			}
			return;
		}
		var get_var(std::string name)
		{
			if(!scope_list.empty() && !scope_list.back().empty())
			{
				std::list<std::list<var> >::iterator i=scope_list.back().end();
				--i;
				// get the last scope block(std::list<std::list<var> >)
				for(;;--i)
				{
					if(!i->empty())// avoid sigsegv
					{
						for(std::list<var>::iterator j=i->begin();j!=i->end();++j)
							if(j->get_name()==name)
								return *j;
					}
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
		var* append_get_addr(std::string name)
		{
			var* addr=NULL;
			if(!scope_list.empty())
			{
				int cnt=1;
				// append function will get array's address from parameters' scope so the last two blocks will be checked
				// because when calling a function,a new block scope will be created.
				// but the array does not exist in this block,
				// so you must find the block before this block.
				std::list<std::list<std::list<var> > >::iterator blk=scope_list.end();
				--blk;
				for(;;--blk,++cnt)
				{
					if(!blk->empty())// avoid sigsegv
					{
						std::list<std::list<var> >::iterator i=blk->end();
						--i;
						for(;;--i)
						{
							if(!i->empty())// avoid sigsegv
							{
								for(std::list<var>::iterator j=i->begin();j!=i->end();++j)
									if(j->get_name()==name)
									{
										addr=&(*j);
										return addr;
									}
							}
							if(i==blk->begin())
								break;
						}	
					}
					if(cnt==2 || blk==scope_list.begin())
						break;
				}
			}
			if(!global.empty())// if can't find array's address,find the global scope
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
		var* get_addr(std::string name)
		{
			var* addr=NULL;
			if(!scope_list.empty())
			{
				std::list<std::list<var> >::iterator i=scope_list.back().end();
				--i;
				for(;;--i)
				{
					if(!i->empty())// avoid sigsegv
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
			if(!scope_list.empty() && !scope_list.back().empty())
				scope_list.back().pop_back();
			return;
		}
};

balloon_scope scope;

#endif
