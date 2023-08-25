#include "onnx/cm_onnx_graph_builder.hpp"
#include "nodes/cm_nodes_registry.hpp"

using namespace CyanMycelium;
using namespace BlueSteelLadyBug;

#define GRAPH_FIELD_NUMBER 7
#define NODE_FIELD_NUMBER 1
#define INITIALIZER_FIELD_NUMBER 5
#define INPUT_FIELD_NUMBER 11
#define OUTPUT_FIELD_NUMBER 12
#define VALUE_INFO_FIELD_NUMBER 13
#define QUANTIZATION_FIELD_NUMBER 14
#define SPARSE_INITIALIZER_FIELD_NUMBER 15

#define NODE_INPUT_FIELD_NUMBER 1
#define NODE_OUTPUT_FIELD_NUMBER 2
#define NODE_TYPE_FIELD_NUMBER 4
#define NODE_ATT_FIELD_NUMBER 5

#define VINFOS_NAME_FIELD_NUMBER 1
#define VINFOS_TYPE_FIELD_NUMBER 2
#define TYPE_TENSOR_TYPE_FIELD_NUMBER 1
#define TYPE_SPARSE_TENSOR_TYPE_FIELD_NUMBER 8

#define TENSOR_ELEM_TYPE_FIELD_NUMBER 1
#define TENSOR_SHAPE_FIELD_NUMBER 2

#define READ_FUNC_0(n) n(subReader)
#define READ_FUNC_1(n, p) n(p, subReader)

#define READ_SUB_MESSAGE(r, f, a)                   \
    PBReader *subReader = r->getSubMessageReader(); \
    bool res = f;                                   \
    delete subReader;                               \
    if (!res)                                       \
    {                                               \
        a;                                          \
    }

OnnxGraphBuilder ::OnnxGraphBuilder(int initialNodesCollectionSize, int initialLinkCollectionSize) : _nodes(max(initialNodesCollectionSize, CM_DEFAULT_COLLECTION_CAPACITY)),
                                                                                                     _links(max(initialLinkCollectionSize, CM_DEFAULT_COLLECTION_CAPACITY))
{
}

OnnxGraphBuilder ::~OnnxGraphBuilder() {}

OnnxGraphBuilder &OnnxGraphBuilder ::WithReader(PBReader *reader)
{
    _reader = reader;
    return *this;
}

Graph *OnnxGraphBuilder ::Build(Graph *target)
{
    if (_reader)
    {
        while (_reader->readTag())
        {
            // skip every fields from the model and focus on graph.
            if (_reader->getFieldNumber() == GRAPH_FIELD_NUMBER)
            {
                READ_SUB_MESSAGE(_reader, READ_FUNC_0(_readGraph), return nullptr)
            }
            _reader->skip();
        }
        // copy the graph content
        target = target ? target : new Graph(_nodes.Count(), _links.Count());
        // 1 - nodes
        _nodes.GetIterator().To(&target->Nodes);

        // 2 -> links
        KeyValueCollection<Link *>::Iterator<KeyValue<Link *>> i = _links.GetIterator();
        while (i.MoveNext())
        {
            KeyValue<Link *> *entry = i.Current();
            target->Links.Add(entry->Value);
            if (entry->Value->Oini == nullptr)
            {
                // this is an input
                target->inputs.Set(entry->Key, entry->Value);
                continue;
            }
            if (entry->Value->Ofin == nullptr)
            {
                // this is an output
                target->outputs.Set(entry->Key, entry->Value);
            }
        }
    }
    return target;
}

bool OnnxGraphBuilder ::_readGraph(PBReader *reader)
{
    // cache beeing used along the parsing
    char cache[CM_KEY_MAX_LENGTH];

    while (reader->readTag())
    {
        lb_uint32_t fieldNumber = reader->getFieldNumber();
        switch (fieldNumber)
        {
        case (NODE_FIELD_NUMBER):
        {
            READ_SUB_MESSAGE(reader, READ_FUNC_1(_readNode, cache), return false)
            continue;
        }
        case (INITIALIZER_FIELD_NUMBER):
        {
            // reach this point, every link and nodes should be created
            // the initializer will only initialize the values of Tensor
            READ_SUB_MESSAGE(reader, READ_FUNC_0(_readInitializer), return false)
        }
        // The inputs and outputs of the graph.
        case (INPUT_FIELD_NUMBER):
        case (OUTPUT_FIELD_NUMBER):
            // Information for the values in the graph. The ValueInfoProto.name's
            // must be distinct. It is optional for a value to appear in value_info list.
        case (VALUE_INFO_FIELD_NUMBER):
        {
            READ_SUB_MESSAGE(reader, READ_FUNC_1(_readValueInfos, cache), return false)
            continue;
        }
            // This field carries information to indicate the mapping among a tensor and its
            // quantization parameter tensors. For example:
            // For tensor 'a', it may have {'SCALE_TENSOR', 'a_scale'} and {'ZERO_POINT_TENSOR', 'a_zero_point'} annotated,
            // which means, tensor 'a_scale' and tensor 'a_zero_point' are scale and zero point of tensor 'a' in the model.
        case (QUANTIZATION_FIELD_NUMBER):
        default:
        {
            reader->skip();
        }
        }
    }
    return true;
}

bool OnnxGraphBuilder ::_readNode(char *cache, PBReader *reader)
{
    // we need to conduct a first pass read of the node to find the TYPE,
    // which unfortunately is never at the begining of the recordbecause of index value of 4 into the .proto definition
    reader->save();
    while (reader->readTag())
    {
        if (reader->getFieldNumber() == NODE_TYPE_FIELD_NUMBER)
        {
            reader->readValue_s(cache, CM_KEY_MAX_LENGTH);
            break;
        }
        reader->skip();
    }
    reader->restore();

    // reach this point we might create the node using the type_name
    Node *n = _createNode(cache);
    if (!n)
    {
        return false;
    }

    _nodes.Add(n);

    // parse name & specifics attributes
    while (reader->readTag())
    {
        switch (reader->getFieldNumber())
        {
        case (NODE_ATT_FIELD_NUMBER):
        {
            // NOTE : Avoid creating a sub reader by using position based parse pattern
            Att_value_t value;
            lb_uint64_t size;
            if (!reader->readLength(&size, false))
            {
                return false;
            }
            lb_uint64_t end = reader->getPosition() + size;
            do
            {
                reader->readTag();
                lb_uint32_t fieldNumber = reader->getFieldNumber();
                switch (fieldNumber)
                {
                case (1):
                {
                    reader->readValue_s(cache, CM_KEY_MAX_LENGTH);
                    break;
                }
                case (2):
                {
                    reader->readValue(&value.f);
                    break;
                }
                case (3):
                {
                    reader->readValue(&value.i);
                    break;
                }
                default:
                {
                    reader->skip();
                    break;
                }
                }
            } while (reader->getPosition() < end);

            // reach this point we can set the attribute.
            if (!n->TrySetAtt(cache, value))
            {
                return false;
            }
            break;
        }
        case (NODE_INPUT_FIELD_NUMBER):
        {
            reader->readValue_s(cache, CM_KEY_MAX_LENGTH);
            Link *l = _getOrCreateLink(cache);
            if (l)
            {
                l->Ofin = n;
                n->Opsc.Add(l);
            }
            break;
        }
        case (NODE_OUTPUT_FIELD_NUMBER):
        {
            reader->readValue_s(cache, CM_KEY_MAX_LENGTH);
            Link *l = _getOrCreateLink(cache);
            if (l)
            {
                l->Oini = n;
                n->Onsc.Add(l);
            }
            break;
        }
        default:
        {
            reader->skip();
            break;
        }
        }
    }
    return true;
}

// Defines information on value, including the name, the type, and the shape of the value.
bool OnnxGraphBuilder ::_readValueInfos(char *cache, BlueSteelLadyBug ::PBReader *reader)
{
    // NOTE : we assume here that name is before type. The reason is the order of field declaration in the
    // .proto file must be maintained in the serialized message.
    Link *link = nullptr;
    while (reader->readTag())
    {
        switch (reader->getFieldNumber())
        {
        // This field MUST be present in this version of the IR.
        case VINFOS_NAME_FIELD_NUMBER:
        {
            reader->readValue_s(cache, CM_KEY_MAX_LENGTH);
            link = _getOrCreateLink(cache);
            continue;
        }
        // This field MUST be present in this version of the IR for
        // inputs and outputs of the top-level graph.
        case VINFOS_TYPE_FIELD_NUMBER:
        {
            // NOTE : Avoid creating a sub reader by using position based parse pattern
            lb_uint64_t length;
            if (!reader->readLength(&length, false))
            {
                return false;
            }
            lb_uint64_t end = reader->getPosition() + length;
            // NOTE:  This current implementations of ONNX do not support non-tensor values
            //        as input and output to graphs and nodes.
            do
            {
                reader->readTag();
                switch (reader->getFieldNumber())
                {
                case TYPE_TENSOR_TYPE_FIELD_NUMBER:
                {
                    READ_SUB_MESSAGE(reader, READ_FUNC_1(_readTensorType, &link->Payload), return false)
                    continue;
                }
                default:
                {
                    reader->skip();
                    break;
                }
                }
            } while (reader->getPosition() < end);
            continue;
        }
        default:
        {
            reader->skip();
            break;
        }
        }
    }
    return true;
}

bool OnnxGraphBuilder ::_readInitializer(BlueSteelLadyBug ::PBReader *reader)
{
    reader->skip();
    return true;
}

bool OnnxGraphBuilder ::_readTensorType(Tensor *t, BlueSteelLadyBug ::PBReader *reader)
{
    lb_uint32_t type;
    while (reader->readTag())
    {
        switch (reader->getFieldNumber())
        {
        case TENSOR_ELEM_TYPE_FIELD_NUMBER:
        {
            reader->readValue(&type);
            t->Type = (tensor_data_type_t)type;
            continue;
        }
        case TENSOR_SHAPE_FIELD_NUMBER:
        {
            READ_SUB_MESSAGE(reader, READ_FUNC_1(_readTensorShape, t), return false)
            continue;
        }
        default:
        {
            reader->skip();
            break;
        }
        }
    }
    return true;
}
#define TENSOR_TYPE_DIM_FIELD_NUMBER 1
#define DIM_VALUE_FIELD_NUMBER 1
bool OnnxGraphBuilder ::_readTensorShape(Tensor *t, BlueSteelLadyBug ::PBReader *reader)
{
    lb_uint64_t shape[TENSOR_MAX_DIMENSION];
    int count = 0;
    while (reader->readTag())
    {
        switch (reader->getFieldNumber())
        {
        case TENSOR_TYPE_DIM_FIELD_NUMBER:
        {
            lb_uint64_t length;
            if (!reader->readLength(&length, false))
            {
                return false;
            }
            lb_uint64_t end = reader->getPosition() + length;
            do
            {
                reader->readTag();
                switch (reader->getFieldNumber())
                {
                case DIM_VALUE_FIELD_NUMBER:
                {
                    if (count < TENSOR_MAX_DIMENSION)
                    {
                        reader->readValue(shape + count);
                        count++;
                    }
                    continue;
                }
                default:
                {
                    reader->skip();
                    break;
                }
                }
            } while (reader->getPosition() < end);
            continue;
        }
        default:
        {
            // to accept extensions
            reader->skip();
            break;
        }
        }
    }
    t->Set(shape, count, t->Type);
    return true;
}

Node *OnnxGraphBuilder ::_createNode(const char *typeName)
{
    return NodeRegistry ::ForName(typeName);
}

Link *OnnxGraphBuilder ::_createLink()
{
    return new Link();
}

Link *OnnxGraphBuilder ::_getOrCreateLink(const char *name)
{
    Link *l = _links.Get(name);
    if (!l)
    {
        l = _createLink();
        _links.Set(name, l);
    }
    return l;
}