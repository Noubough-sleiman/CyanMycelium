#include "cm.h"
#include "memory/cm_memory_manager.hpp"

using namespace CyanMycelium;

void *MemoryManagerBase ::Clone(void *ptr, const size_t size, int heap_id)
{
  void *copy = this->Malloc(size, heap_id);
  if (copy)
  {
    cm_memcpy(copy, ptr, size);
  }
  return copy;
}

void *MemoryManagerBase ::Malloc(const size_t size, int heap_id)
{
  return cm_malloc(size);
}

void *MemoryManagerBase ::Realloc(void *ptr, const size_t size, int heap_id)
{
  return cm_realloc(ptr, size);
}

void MemoryManagerBase ::Free(void *ptr, int heap_id)
{
  cm_free(ptr);
}
