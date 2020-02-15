#ifndef __NASAL_GC_H__
#define __NASAL_GC_H__

struct gc_unit
{
	// collected: If gc collected this item,it'll be set to true.Otherwise it is false.
	// elem:      Item that this unit stores
	// refcnt:    Reference counter
	bool         collected;
	nasal_scalar elem;
	int          refcnt;
	gc_unit()
	{
		collected=true;
		elem.set_clear();
		refcnt=0;
		return;
	}
	gc_unit(const gc_unit& tmp)
	{
		collected=tmp.collected;
		elem     =tmp.elem;
		refcnt   =tmp.refcnt;
		return;
	}
	void set_clear()
	{
		collected=true;
		elem.set_clear();
		refcnt=0;
		return;
	}
};

class gc_manager
{
	private:
		// free_space list is used to store space that is not in use.
		std::list<int> free_space;
		std::vector<gc_unit> memory;
	public:
		gc_manager()
		{
			memory.clear();
			free_space.clear();
			return;
		}
		~gc_manager()
		{
			memory.clear();
			free_space.clear();
			return;
		}
		void gc_init()
		{
			// this function must be called in class nasal_runtime
			// before running any codes
			std::vector<gc_unit> tmp_vec;
			memory.clear();
			memory.swap(tmp_vec);
			// clear the memory capacity by using tmp_vec.~vector<gc_unit>()
			free_space.clear();
			return;
		}
		void gc_scanner()
		{
			int memory_size=memory.size();
			for(int i=0;i<memory_size;++i)
				if((memory[i].refcnt<=0) && (!memory[i].collected))
				{
					memory[i].set_clear();
					free_space.push_back(i);
					std::cout<<">> [Gc] collected ";
					prt_hex(i);
					std::cout<<"."<<std::endl;
				}
			return;
		}
		int gc_alloc()
		{
			if(free_space.empty())
			{
				gc_unit new_unit;
				memory.push_back(new_unit);
				free_space.push_back(memory.size()-1);
			}
			int alloc_plc=free_space.front();
			free_space.pop_front();
			memory[alloc_plc].collected=false;
			memory[alloc_plc].refcnt=1;
			return alloc_plc;
		}
		bool place_check(const int place)
		{
			// check if this place is in memory
			// and this place is uncollected
			return (place<memory.size()) && (!memory[place].collected);
		}
		void reference_add(const int place)
		{
			if(place<memory.size())
				++memory[place].refcnt;
			else
			{
				std::cout<<">> [Gc] fatal error: unexpected memory place ";
				prt_hex(place);
				std::cout<<" ."<<std::endl;
			}
			return;
		}
		void reference_delete(const int place)
		{
			if(place<memory.size())
				--memory[place].refcnt;
			else
			{
				std::cout<<">> [Gc] fatal error: unexpected memory place ";
				prt_hex(place);
				std::cout<<" ."<<std::endl;
			}
			return;
		}
		void info_print()
		{
			std::cout<<">> [Gc] memory size:"<<memory.size()*sizeof(gc_unit)<<" byte."<<std::endl;
			std::cout<<">> [Gc] memory capacity:"<<memory.capacity()*sizeof(gc_unit)<<" byte."<<std::endl;
			std::cout<<">> [Gc] memory usage: "<<std::endl;
			int cnt=0;
			for(int i=0;i<memory.size();++i)
				if(!memory[i].collected)
				{
					prt_hex(i);
					std::cout<<"["<<memory[i].refcnt<<"]";
					// cnt is used to check if it is the right time to output in the next line
					++cnt;
					if(!(cnt%8))
						std::cout<<std::endl;
					else
						std::cout<<" ";
				}
			if(cnt%8)
				std::cout<<std::endl;
			return;
		}
};
gc_manager nasal_gc;
// this object is used in "nasal_sym.h" and "nasal_runtime.h"
// because there must be only one gc when running a program

#endif
