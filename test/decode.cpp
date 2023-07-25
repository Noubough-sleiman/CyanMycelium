#include "../libs/nanopb/pb_decode.h"
#include "../libs/onnx/onnx.pb.h"
#include "../include/cm.h"

#include <stdio.h>

#define CRLF "\r\n"

#include <stdio.h>
#include <stdlib.h>

// Function to read the content of a binary file and return it as a byte buffer.
// The function allocates memory for the buffer, and it is the caller's responsibility
// to free the memory after use to avoid memory leaks.
// Returns NULL if there's an error reading the file or if the file is empty.
// Otherwise, returns the pointer to the allocated buffer containing the file content.
// The size of the buffer will be stored in the `size` parameter.
uint8_t* read_file_as_buffer(const char* filename, size_t* size) {
    // Open the file in binary read mode.
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening the file");
        return NULL;
    }

    // Find the size of the file.
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Check if the file is empty.
    if (file_size <= 0) {
        fclose(file);
        fprintf(stderr, "Error: File is empty or couldn't determine file size.\n");
        return NULL;
    }

    // Allocate memory for the buffer to hold the file content.
    uint8_t* buffer = (uint8_t*)malloc(file_size);
    if (buffer == NULL) {
        fclose(file);
        perror("Error allocating memory for buffer");
        return NULL;
    }

    // Read the file content into the buffer.
    size_t bytes_read = fread(buffer, 1, file_size, file);
    fclose(file);

    // Check if the read operation was successful.
    if (bytes_read != file_size) {
        free(buffer);
        fprintf(stderr, "Error reading file content.\n");
        return NULL;
    }

    // Set the size parameter to the actual size of the buffer.
    *size = (size_t)file_size;
    return buffer;
}

//#define USE_MEMORY
#define USE_FILE

int main(){

  onnx_ModelProto model;
  
  const char * filename = "C:/Users/guill/Documents/sources/CyanMycelium/samples/Abs/abs.onnx";
  
  #ifdef USE_MEMORY
    size_t length;
    uint8_t* buffer = read_file_as_buffer(filename, & length);
    pb_istream_t stream = pb_istream_from_buffer(buffer, length); 
  #else
    pb_istream_t stream = openOnnxFileStream(filename);
  #endif
  if(pb_decode(&stream, onnx_ModelProto_fields, &model)){

  }
  #ifdef USE_MEMORY
    free(buffer);
  #else
    closeOnnxFileStream(&stream);
  #endif

  return 0;
}    
