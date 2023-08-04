#ifndef _CM_CONCURRENT__
#define _CM_CONCURRENT__

#include "cm.h"

namespace CyanMycelium
{

    /// @brief A mutex (short for "mutual exclusion") is a synchronization primitive
    /// that provides mutual exclusion to ensure that only one thread can access a
    /// shared resource at a time. It is typically used to protect critical sections
    /// of code where multiple threads may attempt to modify shared data simultaneously.
    /// A thread trying to acquire a locked mutex will be blocked until it becomes
    /// available, preventing concurrent access and avoiding race conditions.
    class Mutex
    {
    public:
        Mutex(bool initially_owned = false);
        ~Mutex();
        bool Take(unsigned int timeout_millis = CM_INFINITE);
        void Give();

    private:
        cm_mutex_t _handle;
    };
    typedef Mutex *MutexPtr;

    /// @brief A semaphore is a synchronization primitive that can be used for more advanced synchronization scenarios.
    /// It is essentially a counter that controls access to a shared resource. Unlike a mutex, a semaphore can allow
    /// multiple threads to access the shared resource concurrently up to a certain limit defined by the semaphore's value.
    /// Semaphores can be used to implement various synchronization patterns, such as signaling between threads or limiting
    /// the number of concurrent accesses to a resource.
    class Semaphore
    {
    public:
        Semaphore(int initialCount, int maximumCount);
        ~Semaphore();

        bool Take(unsigned int timeout_millis = CM_INFINITE);
        void Give(unsigned int count = 1);

    private:
        cm_semaphore_t _handle;
    };
    typedef Semaphore *SemaphorePtr;
}

#endif
