#include <stdlib.h>
#include <stdio.h>
#include "../include/cm.h"
#include "../libs/nanopb/pb_decode.h"

static bool ReadBytes(pb_istream_t * stream, pb_byte_t * buffer, size_t size);

pb_istream_t openOnnxFileStream(const char * filename) 
{
  pb_istream_t stream ;

  FILE * fp;

  fp = fopen (filename, "r"); // open in binary mode
  if( fp != NULL)
  {
    printf("FILE OPENED\r\n");

    // check file size
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
  
    stream.callback = &ReadBytes;
    stream.state = fp;
    stream.bytes_left = size;
    stream.errmsg = NULL;
  } else {
    perror("Failed:");
  }
  return stream;
}

void closeOnnxFileStream(pb_istream_t * stream)
{
  FILE * fp = (FILE*) stream->state;
  if( fp)
  {
    fclose(fp);
  }
}

static bool ReadBytes(pb_istream_t * stream, pb_byte_t * buffer, size_t size)
{
  FILE * fp = (FILE*) stream->state;
  size_t readed = fread(buffer,size,1,fp);
  stream->bytes_left -= readed;
  return true;
}

void * cm_malloc(size_t size, int heap_id)
{
    return malloc(size);
}

void * cm_realloc(void * ptr, size_t size, int heap_id)
{
    return realloc(ptr, size);
}

void cm_free(void * ptr, int heap_id)
{
    return free(ptr);
}

