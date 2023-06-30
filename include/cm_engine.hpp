#ifndef __CYAN_MISCELIUM_ENGINE__
#define __CYAN_MISCELIUM_ENGINE__

#include "cm.h"
#include "../libs/onnx/onnx.pb.h"

namespace CyanMycelium         
{
  typedef onnx_ModelProto Model;
  typedef onnx_ValueInfoProto ValueInfos;
  typedef onnx_StringStringEntryProto Metadata;

  #define RESERVED_SESSION_ID 100
  #define MAX_SESSION_ID 65535

  struct InferenceSession
  {
    /// @brief the session identifier. Should start at RESERVED_SESSION_ID and be incremented until it reaches MAX_SESSION_ID,
    /// then it is reset to RESERVED_SESSION_ID.
    session_id_t sessionId;

    /// @brief used to set the values into inputs/intermediates/output buffers
    /// @param name the name of the input
    /// @param data the data to be stored
    void SetBlob( const char * name, void * data);

    /// @brief retreive the data from a particular buffer.
    /// @param name the name of the buffer
    /// @return the data from the buffer.
    void * GetBlob(const char * name);
  };

  class InferenceEngine 
  {
    enum Result {
      SUCCESS = 0
    } ;

    public:
      /// @brief Load an onnx model from file.
      /// @param filename the file name
      /// @return a result code, must be SUCCESS = 0
      Result Load( char * filename) ;
      
      /// @brief This create a session according inputs and outputs defined into the activ model.
      /// @param sessionId the session identifier. The session identifier let's the engine synchronize 
      /// the data propagation into the graph.
      /// @return the new session;
      InferenceSession CreateSession(uint16_t sessionId);
      
      /// @brief 
      /// @param session 
      /// @return 
      Result RunInference(InferenceSession session);
      
      /// @brief 
      /// @return 
      Result Unload();

      /// @brief 
      /// @return 
      ValueInfos * GetInputsInfos();
      
      /// @brief 
      /// @return 
      ValueInfos * GetOutputsInfos();
      
      /// @brief 
      /// @return 
      Metadata * GetMetaInfos();

    private:
      Model * _model;
  };
}

#endif