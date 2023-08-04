#ifndef _CM_MEMORY_MANAGER__
#define _CM_MEMORY_MANAGER__

namespace CyanMycelium
{
    /// @brief
    class IMemoryManager
    {
    public:
        virtual void *Clone(void *ptr, const size_t size, int heap_id = 0) = 0;
        virtual void *Malloc(const size_t size, int heap_id = 0) = 0;
        virtual void *Realloc(void *ptr, const size_t size, int heap_id = 0) = 0;
        virtual void Free(void *ptr, int heap_id = 0) = 0;
    };

    typedef IMemoryManager *IMemoryManagerPtr;

    class MemoryManagerBase : public IMemoryManager
    {
    public:
        void *Clone(void *ptr, const size_t size, int heap_id = 0) override;
        void *Malloc(const size_t size, int heap_id = 0) override;
        void *Realloc(void *ptr, const size_t size, int heap_id = 0) override;
        void Free(void *ptr, int heap_id = 0) override;
    };
}
#endif