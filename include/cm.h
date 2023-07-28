 #ifndef __CYAN_MISCELIUM_DEF__
#define __CYAN_MISCELIUM_DEF__

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GRAPH_METADATA_SIZE 8
#define GRAPH_INPUT_SIZE 8
#define GRAPH_OUTPUT_SIZE 8

#define FILE_ACCESS_MAX_READ_PER_CALLBACK 8

typedef uint16_t cm_size_t;
typedef uint16_t session_id_t;

#define INTERNAL_HEAP  0
#define EXTERNAL_HEAP  1

#define INTERNAL_HEAP_MAX_KO  300
#define EXTERNAL_HEAP_MAX_KO  2000

#define cm_strcmp strcmp
#define cm_memcpy memcpy

void * cm_malloc(cm_size_t size, int heap_id) ;
void * cm_realloc(void * ptr, cm_size_t size, int heap_id) ;
void cm_free(void * ptr, int heap_id ) ;

#define cm_memset memset

#define cm_rand rand

#ifdef __cplusplus
}
#endif


#endif