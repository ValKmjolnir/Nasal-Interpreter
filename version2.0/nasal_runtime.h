#ifndef __NASAL_RUNTIME_H__
#define __NASAL_RUNTIME_H__

class nasal_runtime
{
    private:
        sym_hash_map global;
        // local hash_map will be used when running
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
        void func_proc()
        {
            return;
        }
        void main_proc(abstract_syntax_tree& root)
        {
            // init
            global.set_clear();
            nasal_gc.gc_init();
            return;
        }
};

#endif