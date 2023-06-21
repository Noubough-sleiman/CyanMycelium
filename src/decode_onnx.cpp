#include <fstream>

#include "../libs/nanopb/pb_decode.h"
#include "../libs/onnx/onnx.pb.h"

using namespace std;

static uint8_t* ReadAllBytes(const std::string& filename, size_t& fileSize) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    
    if (!file.is_open()) {
        // Handle file open error
        // For example, throw an exception or return nullptr
        return nullptr;
    }
    
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    uint8_t* buffer = new uint8_t[fileSize];
    
    if (!file.read(reinterpret_cast<char*>(buffer), fileSize)) {
        // Handle file read error
        // For example, clean up allocated memory and return nullptr
        delete[] buffer;
        return nullptr;
    }
    
    return buffer;
}

static onnx_ModelProto DecodeONNXModel(const uint8_t* buffer, size_t bufferSize) {
  // Use the buffer
  pb_istream_t stream = pb_istream_from_buffer(buffer, bufferSize);
  onnx_ModelProto model;

  bool status = pb_decode(&stream, onnx_ModelProto_fields, &model);

  // Remember to free the allocated memory when you're done
  delete[] buffer;
  if (!status) {
    // Handle decoding error
    // For example, throw an exception or return an empty ModelProto
    return {};
  }
  return model;
}

int main(){
  std::string filename = "../samples/model.onnx";
  size_t fileSize = 0;
  uint8_t* buffer = ReadAllBytes(filename, fileSize);

  if (buffer != nullptr) {
    onnx_ModelProto model = DecodeONNXModel(buffer, fileSize );
  }
  return 0;
}    
