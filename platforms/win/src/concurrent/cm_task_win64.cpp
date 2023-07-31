#if defined(_WIN64)

#include "concurrent/cm_task.hpp"

using namespace CyanMycelium;

    Thread :: Thread(thread_start_routine_fn fn, unsigned int stack_size = 0, void* params = NULL,Priority priority = Priority::LOW)
    {
       _handle = CreateThread(
           NULL, // default security attributes
           stack_size, // stack size, default is zero
           fn, // start address
           params, // argument to thread function 
           CREATE_SUSPENDED, // wait for resume thread
           NULL); // do not need the identifier
        
        if(_handle)
        {
          int p ;
          switch(priority){
            case Priority::LOW :
            {
                p = THREAD_PRIORITY_BELOW_NORMAL;
                break;
            }
            case Priority::MEDIUM :
            {
                p = THREAD_PRIORITY_NORMAL;
                break;
            }
            case Priority::HIGH :
            {
                p = THREAD_PRIORITY_ABOVE_NORMAL;
                break;
            }
            case Priority::HIGHEST :
            {
                p = THREAD_PRIORITY_HIGHEST;
                break;
            }
          }
          SetThreadPriority(_handle, p);
          ResumeThread(_handle);
        }
    };
    
    Thread :: ~Thread()
    {
        if(_handle)
        {
           CloseHandle(_handle);
        }
    };

#endif