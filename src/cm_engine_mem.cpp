#include "cm_engine_mem.hpp"

using namespace CyanMycelium  ;       

void * MemoryManagerBase :: Clone(void * ptr, const size_t size, int heap_id = 0)
{
  void * copy = this->Malloc(size, heap_id);
  if( copy)
  {
     cm_memcpy(copy, ptr, size);
  }
}

void * MemoryManagerBase :: Malloc(const size_t size, int heap_id = 0)
{
  return cm_malloc(size);
}

void * MemoryManagerBase :: Realloc(void * ptr,const size_t size, int heap_id = 0)
{
  return cm_realloc(ptr,size);
}

void MemoryManagerBase :: Free(void * ptr, int heap_id = 0)
{
  cm_free(ptr);
}

