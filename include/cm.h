 #ifndef __CYAN_MISCELIUM_DEF__
#define __CYAN_MISCELIUM_DEF__

#include <stdint.h>
#include "../libs/nanopb/pb.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GRAPH_METADATA_SIZE 8
#define GRAPH_INPUT_SIZE 8
#define GRAPH_OUTPUT_SIZE 8

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

pb_istream_t openOnnxFileStream(const char * src) ;
void closeOnnxFileStream(pb_istream_t * stream) ;

#ifdef __cplusplus
}
#endif


#endif