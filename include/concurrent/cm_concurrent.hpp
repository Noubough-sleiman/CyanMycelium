#ifndef __CYAN_MISCELIUM_CONCURRENT__
#define	__CYAN_MISCELIUM_CONCURRENT__

namespace CyanMycelium
{
    /// @brief A mutex (short for "mutual exclusion") is a synchronization primitive 
    /// that provides mutual exclusion to ensure that only one thread can access a 
    /// shared resource at a time. It is typically used to protect critical sections 
    /// of code where multiple threads may attempt to modify shared data simultaneously. 
    /// A thread trying to acquire a locked mutex will be blocked until it becomes 
    /// available, preventing concurrent access and avoiding race conditions.
    class IMutex
    {
    public:
        virtual bool Take(unsigned int timeoutMs = 0) = 0;
        virtual void Give() = 0;
    };
    typedef IMutex * IMutexPtr;

    /// @brief A semaphore is a synchronization primitive that can be used for more advanced synchronization scenarios. 
    /// It is essentially a counter that controls access to a shared resource. Unlike a mutex, a semaphore can allow 
    /// multiple threads to access the shared resource concurrently up to a certain limit defined by the semaphore's value. 
    /// Semaphores can be used to implement various synchronization patterns, such as signaling between threads or limiting 
    /// the number of concurrent accesses to a resource.
    class ISemaphore 
    {
        public:
        virtual bool Take(unsigned int timeoutMs = 0, unsigned int count = 1 ) = 0;
        virtual void Give(unsigned int count = 1) = 0;
    };
    typedef ISemaphore * ISemaphorePtr;
}

#endif
