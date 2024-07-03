#ifndef _CM_TASK__
#define _CM_TASK__

#include "cm.h"

namespace CyanMycelium
{
    class IRunnable
    {
    public:
        virtual unsigned long Run(void *) = 0;
    };

    class Thread
    {
    private:
        struct Params
        {
            void *Parameters;
            IRunnable *Target;
        };
        cm_thread_t _handle;
        bool _joined;

    public:
        enum class Priority
        {
            LOW = 1,
            MEDIUM = 2,
            HIGH = 3,
            HIGHEST = 4
        };
        static unsigned long StaticThreadStart(void *p)
        {
            Params *tmp = (Params *)p;
            IRunnable *r = tmp->Target;
            void *parameters = tmp->Parameters;
            delete tmp;
            return r->Run(parameters);
        }

        Thread(IRunnable *target, int stackSize, void *param, Priority p = Priority::LOW);
        ~Thread();

        bool joinable() const;
        void join();
    };

    typedef Thread *ThreadPtr;
}

#endif
