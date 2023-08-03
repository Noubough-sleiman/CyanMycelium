#ifndef __CYAN_MISCELIUM_WIN64__
#define __CYAN_MISCELIUM_WIN64__


#include <stdlib.h>
#include <windows.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define cm_semaphore_t HANDLE
#define cm_mutex_t HANDLE    
#define cm_thread_t HANDLE

#define cm_memset memset
#define cm_rand rand

#define cm_memcpy(copy, ptr, size) memcpy((copy), (ptr), (size))
#define cm_malloc(size) malloc((size))
#define cm_realloc(ptr, size) realloc((ptr), (size))
#define cm_free(ptr) free((ptr))


#ifdef __cplusplus
}
#endif

#endif
