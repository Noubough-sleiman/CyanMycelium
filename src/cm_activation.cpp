#include "cm_graph.hpp"
using namespace CyanMycelium;

TensorRefPtr ActivationContext::CloneRef(TensorRef &other)
{
    TensorRefPtr t = new TensorRef(other);
    t->Flags.Bits.Internal = 1;
    t->Value.Data = this->_mm->Malloc(t->Value.Size);
    cm_memcpy(t->Value.Data, other.Value.Data, t->Value.Size);
    return t;
}

void ActivationContext ::_bind(Link *l, void *buffer)
{
    if (l)
    {
        LinkState state = this->_states[l->Id];
        TensorRef *ref = state.Ref;
        if (!ref)
        {
            ref = new TensorRef(*l->GetPayloadInfos());
            this->_states[l->Id].Ref = ref;
        }
        ref->Value.Data = buffer;
    }
}

bool ActivationContext ::Run()
{
    Graph *model = this->GetModel();
    int c = model->Inputs.Count();
    for (int i = 0; i != c; ++i)
    {
        KeyValue<Link *> entry = model->Inputs[i];
        Link *l = entry.Value;
        this->Activate(l);
    }
    return true;
}

bool ActivationContext::Forward(Operator *op, TensorRefPtr outputValue)
{
    // we deactivate the input links
    int count = op->Opsc.Count();
    for (int i = 0; i != count; i++)
    {
        Link * link = op->Opsc[i];
        this->Deactivate(link);
    }

    // we activate the output links
    outputValue->Lock();
    count = op->Onsc.Count();
    for (int i = 0; i != count; i++)
    {
        TensorRefPtr tensor = outputValue;
        Link * link = op->Onsc[i];
        if (tensor->Flags.Bits.ReadOnly || tensor->Count > 1)
        {
            OperatorPtr nextOp = link->Ofin;
            if (nextOp && nextOp->IsMutable())
            {
                // we need to copy the tensor value.
                tensor = this->CloneRef(*outputValue);
            }
        }
        this->Activate(link, tensor);
    }

    outputValue->Unlock();
    return true;
}

bool ActivationContext::Forward(Link *l)
{
    NodePtr nextNode = l->Ofin;
    // this is a terminal link ?
    if (nextNode)
    {
        // this is not a terminal link.
        // let test the number of sibling and they states
        if (nextNode->Opsc.Count() == 1)
        {
            // short track
            this->Activate((Operator *)nextNode);
            return true;
        }

        // we need to synchronize and potentially activate the node
        nextNode->Lock();
        // we check if the node is ready to be activated
        Collection<Link*> &inputs = nextNode->Opsc;
        int count = inputs.Count();
        for (int i = 0; i != count; ++i)
        {
            Link * l = inputs[i];
            if (!this->_states[l->Id].Flags.Bits.Activ)
            {
                // one of the input is not ready, so we do nothing
                nextNode->Unlock();
                return true;
            }
        }
        this->Activate((Operator *)nextNode);
        nextNode->Unlock();
        return true;
    }
    // this is a terminal link
    KeyValueCollection<Link *> &outputs = this->_model->Outputs;
    int count = outputs.Count();
    for (int i = 0; i != count; ++i)
    {
        KeyValue<Link *> entry = outputs[i];
        Link *l = entry.Value;
        if (!this->_states[l->Id].Flags.Bits.Activ)
        {
            // one of the input is not ready, so we do nothing
            return true;
        }
    }
    // messaging the output ready
    return this->OnOutputReady();
}

bool ActivationContext ::Activate(Link * l, TensorRefPtr tensor)
{
    LinkState *state = this->_states + l->Id;
    state->Flags.Bits.Activ = 1;
    if (tensor)
    {
        state->Ref = tensor;
        tensor->Count++;
    }
    if (!l->Activate(this))
    {
        return false;
    }
    return Forward(l);
}

bool ActivationContext ::Deactivate(Link * l)
{
    LinkState *state = this->_states + l->Id;
    state->Flags.Bits.Activ = 0;
    state->Ref->Count--;
    return true;
}

bool ActivationContext ::Deactivate(OperatorPtr node)
{
    return true;
}

bool ActivationContext ::Activate(OperatorPtr node)
{
    return node->Activate(this);
}

void ActivationContext ::BindInput(const char *name, void *buffer) { this->_bind(this->GetModel()->Inputs[name], buffer); }

void ActivationContext ::BindOutput(const char *name, void *buffer) { this->_bind(this->GetModel()->Outputs[name], buffer); }

void ActivationContext::_buildTensorRefs()
{
    // we count the number of tensors
    int count = this->_model->Links.Count();
    // we allocate the array
    this->_states = new LinkState[count];
}

void ActivationContext::_clearTensorRefs()
{
    for (int i = 0; i != this->_model->Links.Count(); i++)
    {
        TensorRefPtr ref = this->_states[i].Ref;
        if (ref)
        {
            if (ref->Flags.Bits.Internal)
            {
                this->_mm->Free(ref->Value.Data);
            }
            delete ref;
        }
    }
    delete[] this->_states;
}
