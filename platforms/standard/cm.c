// CM (Cyan Mycelium) file to use with the nanopb lib
#include <stdlib.h>
#include <stdio.h>
#include "../include/cm.h"
#include "../libs/nanopb/pb_decode.h"

static bool ReadBytes(pb_istream_t * stream, pb_byte_t * buffer, size_t size);

pb_istream_t openOnnxFileStream(const char * filename) 
{
  pb_istream_t stream ;
  FILE * fp = fopen (filename, "rb"); // WARNING!! MUST open in binary mode
  if( fp )
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

   if (buffer == NULL)
   {
       while (size-- && fgetc(fp) != EOF);
       return size == 0;
   }

  size_t readed = fread(buffer,1,size,fp);
  // printf( "Readed  %zu byte(s), has error %d code %d\n", readed, ferror( fp ), errno );
  // ensure we readed the asked size
  if(readed < size)
  {
    uint8_t i = 0;
    do
    {
      if (feof(fp)){
        stream->bytes_left = 0;
        return false;
      }
      readed += fread(buffer + readed, 1, size - readed, fp) ;
      if(ferror(fp))
      {
        return false;
      }
    } while( ++i < FILE_ACCESS_MAX_READ_PER_CALLBACK && readed < size);
    
    if( readed < size) 
    {
      // this is an error case.
      return false;
    }
  }
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

