#include "../libs/nanopb/pb_decode.h"
#include "../libs/onnx/onnx.pb.h"
#include "../include/cm.h"

#include <stdio.h>


#define CRLF "\r\n"
int main(){

  onnx_ModelProto model;
  
  const char * filename = "C:/Users/guill/Documents/sources/CyanMycelium/samples/LSTM/lstm.onnx";
  pb_istream_t stream = openOnnxFileStream(filename);
  printf("Result is %s\r\n", stream.errmsg);

  if(pb_decode(&stream, onnx_ModelProto_fields, &model)){

  }

  closeOnnxFileStream(&stream);
  return 0;
}    
