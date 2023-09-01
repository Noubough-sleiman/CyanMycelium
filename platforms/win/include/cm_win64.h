#ifndef _CM_WIN64__
#define _CM_WIN64__

#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include "concurrent/cm_embedded_queue_win64.hpp"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef int cm_int32_t;
    typedef int cm_sint32_t;
    typedef long long cm_int64_t;
    typedef unsigned int cm_uint32_t;
    typedef unsigned short cm_uint16_t;
    typedef unsigned long long cm_uint64_t;

    typedef float cm_float_t;
    typedef double cm_double_t;

    typedef bool cm_bool_t;

    typedef unsigned char cm_byte_t;
    typedef unsigned char cm_fastbyte_t;

    typedef long cm_clock_t;

#define cm_clock() clock();

#define CM_INFINITE 0xFFFFFFFF
#define CM_POLL 0x00000000

#define cm_semaphore_t HANDLE
#define cm_mutex_t HANDLE
#define cm_thread_t HANDLE

#define cm_memset memset
#define cm_rand rand

#define cm_memcpy(copy, ptr, size) memcpy((copy), (ptr), (size))
#define cm_malloc(size) malloc((size))
#define cm_realloc(ptr, size) realloc((ptr), (size))
#define cm_free(ptr) free((ptr))

#define cm_strcpy_s(dest, size, src) strcpy_s((dest), (size), (src))

#define cm_queue_handle_t EmbeddedQueuePtr

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef abs
#define abs(a) (a < 0) ? -(a) : (a)
#endif

#ifdef __cplusplus
}
#endif

#endif
