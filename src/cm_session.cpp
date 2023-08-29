#include "cm_engine.hpp"

using namespace CyanMycelium;

/// @brief Test if all the tensor data is set in the link collection. This function is used to check if a node is ready for activation.
/// @param links the list to test against
/// @return true if all the payloads are ready -ie : have data != nullptr
bool __AreLinkReady(Collection<LinkPtr> &links);

/// @brief Test if all the tensor data are set in the Key Value link collection.
// This function is used to determine the readyness or completion of the inference.
/// @param links the list to test against
/// @return true if all the payloads are ready -ie : have data != nullptr
bool __AreLinkReady(KeyValueCollection<LinkPtr> &links);

TensorPtr InferenceSession ::GetInputInfos(const char *name)
{
  LinkPtr l = this->_model->Inputs[name];
  return l ? &l->Payload : nullptr;
}

TensorPtr InferenceSession ::GetOutputInfos(const char *name)
{
  LinkPtr l = this->_model->Outputs[name];
  return l ? &l->Payload : nullptr;
}

bool InferenceSession ::RunAsync(KeyValueCollection<void *> *inputs, KeyValueCollection<void *> *outputs)
{
  _target = outputs;
  KeyValueCollection<LinkPtr> &links = this->_model->Inputs;
  auto li = links.GetIterator();
  NodeCollection nodes(links.Count());

  if (li.MoveNext())
  {
    // prepare input data
    do
    {
      KeyValue<LinkPtr> *entry = li.Current();
      void *data = (*inputs)[entry->Key];
      LinkPtr l = entry->Value;
      l->Payload.Data = data;

      NodePtr o = l->Ofin;
      if (o && !nodes.Contains(o))
      {
        nodes.Add(o);
      }
    } while (li.MoveNext());

    // then activate the nodes.
    int c = nodes.Count();
    for (int i = 0; i != c; ++i)
    {
      Activate((Operator*)nodes[i]);
    }
  }
  return true;
}

DataCollections *InferenceSession ::AwaitOutputs(int timeout)
{
  DataCollections *ptr;
  _wait.Receive(&ptr);
  return ptr;
}

bool InferenceSession ::Activate(LinkPtr l)
{
  NodePtr nextNode = l->Ofin;
  if (nextNode)
  {
    // this is NOT a terminal link
    if (nextNode->Opsc.Count() > 1)
    {
      // we need to synchronize and potentially activate the node
      nextNode->Lock();
      l->Activate(this);
      if (__AreLinkReady(nextNode->Opsc))
      {
        this->Activate((Operator*)nextNode);
      }
      nextNode->Unlock();

      return true;
    }

    // we activate the node
    l->Activate(this);
    this->Activate((Operator*)nextNode);
    return true;
  }

  // this is a terminal link, so update the completion mutex accordingly
  l->Activate(this);
  KeyValueCollection<LinkPtr> &outputs = _model->Outputs;
  if (outputs.Count() > 1 && !__AreLinkReady(outputs))
  {
    return true;
  }

  KEY_VALUE_COLLECTION_COPY(outputs, _target, entry->Value->Payload.Data);

  return _wait.Send(&_target);
}

bool InferenceSession ::Activate(OperatorPtr node)
{
  ActivationEvent e = {CM_ACTIVATION_NODE, this, node};
  return _queue->Send(&e);
}

bool __AreLinkReady(Collection<LinkPtr> &links)
{
  int c = links.Count();
  for (int i = 0; i != c; i++)
  {
    LinkPtr l = links[i];
    if (l && l->Payload.Data == nullptr)
      return false;
  }
  return true;
}

bool __AreLinkReady(KeyValueCollection<LinkPtr> &links)
{
  int c = links.Count();
  for (int i = 0; i != c; i++)
  {
    LinkPtr l = links[i].Value;
    if (l && l->Payload.Data == nullptr)
      return false;
  }
  return true;
}
