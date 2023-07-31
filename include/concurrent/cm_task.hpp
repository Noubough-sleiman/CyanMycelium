#ifndef __CYAN_MISCELIUM_TASK__
#define	__CYAN_MISCELIUM_TASK__

#include "cm.h"

namespace CyanMycelium
{
    /* Task entry function signature */
    typedef unsigned long (*thread_start_routine_fn)(void*);    

    class Thread
    {

    public:
        enum class Priority
        {
            LOW = 1,
            MEDIUM = 2,
            HIGH = 3,
            HIGHEST = 4
        };
 
        Thread(thread_start_routine_fn fn, unsigned int stackSize, void* param, Priority p = Priority::LOW);
        ~Thread();

    private:
       cm_thread_t _handle;
    };
}

#endif
