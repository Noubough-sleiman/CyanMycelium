#ifndef __CYAN_MISCELIUM_ENGINE_MEMORY__
#define __CYAN_MISCELIUM_ENGINE_MEMORY__

#include "cm.h"
#include "memory/cm_memory_manager.hpp"

namespace CyanMycelium
{
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