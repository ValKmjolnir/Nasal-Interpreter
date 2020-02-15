#ifndef __NASAL_RUNTIME_H__
#define __NASAL_RUNTIME_H__

class nasal_runtime
{
    private:
        sym_hash_map global;
    public:
        nasal_runtime()
        {
            global.set_clear();
            nasal_gc.gc_init();
            return;
        }
        ~nasal_runtime()
        {
            global.set_clear();
            nasal_gc.gc_init();
            return;
        }
        void before_running_init()
        {
            global.set_clear();
            nasal_gc.gc_init();
            return;
        }
        void main_proc(abstract_syntax_tree& root)
        {
            return;
        }
};

#endif