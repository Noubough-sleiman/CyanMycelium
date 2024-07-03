#ifndef _CM_MEMORY_MANAGER__
#define _CM_MEMORY_MANAGER__

#include "cm.h"

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

        // Static method to get the singleton instance
        static MemoryManagerBase &Shared();

    private:
        // Private constructor to prevent instantiation
        MemoryManagerBase() = default;

        // Deleted copy constructor and assignment operator to prevent copying
        MemoryManagerBase(const MemoryManagerBase &) = delete;
        MemoryManagerBase &operator=(const MemoryManagerBase &) = delete;
    };
}
#endif