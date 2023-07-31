#ifndef __CYAN_MISCELIUM_ENGINE__
#define __CYAN_MISCELIUM_ENGINE__

#include "cm_graph.hpp"
#include "types/cm_guid.hpp"
#include "concurrent/cm_concurrent.hpp"
#include "collections/cm_queue.hpp"

namespace CyanMycelium         
{
  enum InferenceSessionStatus
  {
    SESSION_IDLE = 0,
    SESSION_ACTIVE,
    SESSION_COMPLETED
  };

  class InferenceSession : IActivationCtx
  {
    public:
    /// @brief the session identifier. Should start at RESERVED_SESSION_ID and be incremented until it reaches MAX_SESSION_ID,
    /// then it is reset to RESERVED_SESSION_ID.
    Guid SessionId;
    
    /// @brief 
    InferenceSessionStatus State;

    InferenceSession(GraphPtr model, IMemoryManagerPtr mm ) ;
    ~InferenceSession();
    
    /// @brief 
    /// @param name 
    /// @return 
    TensorPtr GetInput(const char * name);

    /// @brief 
    void RunAsync();

    /// @brief Wait for the session to complete (ie: ouputs availables). The current thread is blocked until 
    /// the outputs available or timeout occurs
    /// @param timeoutmillis optional timeout in milli seconds
    /// @return 0 for scucces, error code otherwise
    int WaitForCompletion(unsigned int timeoutmillis = 0 );

    /// @brief 
    /// @param name 
    /// @return 
    TensorPtr GetOutput(const char * name);

    /// @brief Reset the session in order to reuse it
    /// @return 
    InferenceSessionPtr Reset();

    /// @brief 
    /// @param  
    /// @param  
    /// @return 
    bool Activate(LinkPtr, void *) override;
    
    /// @brief 
    /// @param  
    /// @return 
    bool Activate(NodePtr) override;
    
    private :
    GraphPtr _model;
    MutexPtr _completionHandle;
  };

  typedef InferenceSession * InferenceSessionPtr;
}
#endif