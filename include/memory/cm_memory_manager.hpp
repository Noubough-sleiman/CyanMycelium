#ifndef __CYAN_MISCELIUM_MEMORY_MANAGER__
#define __CYAN_MISCELIUM_MEMORY_MANAGER__

namespace CyanMycelium         
{
    /// @brief 
    class IMemoryManager
    {
        public:
        virtual void * Clone(void * ptr, const size_t size, int heap_id = 0) = 0;
        virtual void * Malloc(const size_t size, int heap_id = 0) = 0;
        virtual void * Realloc(void * ptr,const size_t size, int heap_id = 0) = 0;
        virtual void Free(void * ptr, int heap_id = 0) = 0;
    };

    typedef IMemoryManager * IMemoryManagerPtr;
}
#endif