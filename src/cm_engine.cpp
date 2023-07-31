#include "./cm_engine.hpp"

using namespace CyanMycelium  ;       

/// @brief Test if all the tensor data is set in the link collection. This function is used to check if a node is ready for activation.
/// @param links the list to test against
/// @return true if all the payloads are ready -ie : have data != nullptr
bool __AreLinkReady(Collection<LinkPtr> * links);

/// @brief Test if all the tensor data are set in the Key Value link collection. 
// This function is used to determine the readyness or completion of the inference.
/// @param links the list to test against
/// @return true if all the payloads are ready -ie : have data != nullptr
bool __AreLinkReady(KeyValueCollection<LinkPtr> * links);

InferenceSession :: InferenceSession(GraphPtr model, IMemoryManagerPtr mm ) : IActivationCtx(mm)
{
    this->_model = model;
    // set the mutex
}

InferenceSession :: ~InferenceSession()
{
    // clear the mutex
}

TensorPtr InferenceSession :: GetInput(const char * name)
{
    LinkPtr l = this->_model->inputs[name];
    if(l)
    {
       if( l->Payload.Data == nullptr) 
       {
         l->Payload.Data = this->MemoryManager->Malloc(l->Payload.Size);
       }
       return &l->Payload ;
    }
    return nullptr;
}

void InferenceSession :: RunAsync()
{
   // verifying that every inputs links are ready
   KeyValueCollection<LinkPtr> * inputs = &this->_model->inputs;
   if(__AreLinkReady(inputs))
   {
      this->_completionHandle->Take();

      for(int i=0; i != inputs->Count; i++)
      {
        LinkPtr l = (*inputs)[i].Value;
        this->Activate(l->Ofin);
      }
   }
}

int InferenceSession :: WaitForCompletion(unsigned int timeoutmillis = 0 )
{
  this->_completionHandle->Take();
  this->_completionHandle->Give();
}

TensorPtr InferenceSession :: GetOutput(const char * name)
{
    LinkPtr l = this->_model->outputs[name];
    if(l)
    {
       return &l->Payload ;
    }
    return nullptr;
}

InferenceSessionPtr InferenceSession :: Reset()
{
    this->State = SESSION_IDLE;
    return this;   
}

bool InferenceSession :: Activate(LinkPtr l, void * data)
{
  if(!l || !data )
  {
    return false;
  }

  NodePtr nextNode = l->ofin ;
  if(nextNode) 
  {
    // this is NOT a terminal link
    if(nextNode->Opsc.Count > 1)
    {
      // we need to synchronize and potentially activate the node
      MutexPtr lock = nextNode->GetLock();
      if(lock) lock->Take();
      l->Payload.Data = data;
      if(__AreLinkReady(&nextNode->opsc))
      {
        this->Activate(nextNode);
      }
      if(lock) lock->Give();
      return true;
    }

    // we activate the node
    l->Payload.Data = data;
    this->Activate(nextNode);
    return true;
  }

  // this is a terminal link, so update the completion mutex accordingly
  l->Payload.Data = data;
  if(this->_completionHandle)
  {
    if( this->_model->outputs.Count > 1)
    {
      if(!__AreLinkReady(&this->_model->outputs)) return true;
    }
    this->_completionHandle->Give();
  }
  return true;
}
    
bool InferenceSession :: Activate(NodePtr)
{
  return false;   
}

bool __AreLinkReady(Collection<LinkPtr> * links)
{
  for(int i=0; i != links->Count; i++)
  {
    LinkPtr l = (*links)[i];
    if( l && l->Payload.Data == nullptr) return false ;
  }
  return true;
}

bool __AreLinkReady(KeyValueCollection<LinkPtr> * links)
{
  for(int i=0; i != links->Count; i++)
  {
    LinkPtr l = (*links)[i].Value;
    if( l && l->Payload.Data == nullptr) return false;
  }
  return true;
}

