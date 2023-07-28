#ifndef __CYAN_MISCELIUM_ONNX__
#define __CYAN_MISCELIUM_ONNX__
#include "./nanopb/pb.h"

namespace CyanMycelium
{
    pb_istream_t openOnnxFileStream(const char * src) ;
    void closeOnnxFileStream(pb_istream_t * stream) ;
}

#endif
