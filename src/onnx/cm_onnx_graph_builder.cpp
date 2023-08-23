#include "onnx/cm_onnx_graph_builder.hpp"
#include "cm_graph_builder.hpp"
#include "nodes/cm_nodes_registry.hpp"

using namespace CyanMycelium;
using namespace BlueSteelLadyBug;

#define GRAPH_FIELD_NUMBER 7
#define NODE_FIELD_NUMBER 1
#define INPUT_FIELD_NUMBER 11
#define OUTPUT_FIELD_NUMBER 12

#define NODE_INPUT_FIELD_NUMBER 1
#define NODE_OUTPUT_FIELD_NUMBER 2
#define NODE_NAME_FIELD_NUMBER 3
#define NODE_TYPE_FIELD_NUMBER 4
#define NODE_ATT_FIELD_NUMBER 5

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

OnnxGraphBuilder ::OnnxGraphBuilder()
{
}
OnnxGraphBuilder ::~OnnxGraphBuilder() {}

OnnxGraphBuilder &OnnxGraphBuilder ::WithReader(PBReader *reader)
{
    _reader = reader;
    return *this;
}

GraphPtr OnnxGraphBuilder ::Build()
{
    if (_reader == nullptr)
    {
        return nullptr;
    }

    while (_reader->readTag())
    {
        // skip every fields from the model and focus on graph.
        if (_reader->getFieldNumber() == GRAPH_FIELD_NUMBER)
        {
            READ_SUB_MESSAGE(_reader, READ_FUNC_0(_readGraph), return nullptr)
        }
        _reader->skip();
    }
    return GraphBuilder ::Build();
}

bool OnnxGraphBuilder ::_readGraph(PBReader *reader)
{
    // we read the graph into 2 pass
    // 1 - read an mount the nodes
    // 2 - link the graph
    reader->save();
    while (reader->readTag())
    {
        lb_uint32_t fieldNumber = reader->getFieldNumber();
        switch (fieldNumber)
        {
        case (NODE_FIELD_NUMBER):
        {
            READ_SUB_MESSAGE(reader, READ_FUNC_0(_readNode), return false)
            continue;
        }
        case (INPUT_FIELD_NUMBER):
        case (OUTPUT_FIELD_NUMBER):
        {
            READ_SUB_MESSAGE(reader, READ_FUNC_1(_readValueInfos, fieldNumber), return false)
            continue;
        }
        default:
        {
            reader->skip();
        }
        }
    }
    reader->restore();

    // link the graph
    while (reader->readTag())
    {
        if (reader->getFieldNumber() == NODE_FIELD_NUMBER)
        {
            READ_SUB_MESSAGE(reader, READ_FUNC_0(_link), return false)
            continue;
        }
        reader->skip();
    }
    return true;
}

bool OnnxGraphBuilder ::_readNode(PBReader *reader)
{
    // manually create an additional node
    KeyValue<NodePtr> entry = _nodes.Add().Last();

    // we need to conduct a first pass read of the node to find the TYPE, which unfortunately is often at the end of the record.
    reader->save();
    while (reader->readTag())
    {
        if (reader->getFieldNumber() == NODE_TYPE_FIELD_NUMBER)
        {
            reader->readValue_s(entry.Key, CM_KEY_MAX_LENGTH);
            break;
        }
        reader->skip();
    }
    reader->restore();

    // reach this point we might create the node using the type_name
    Node *n = _createNode(entry.Key);
    if (!n)
    {
        return false;
    }

    // parse name & specifics attributes
    entry.Value = n;
    while (reader->readTag())
    {
        switch (reader->getFieldNumber())
        {
        case (NODE_NAME_FIELD_NUMBER):
        {
            reader->readValue_s(entry.Key, CM_KEY_MAX_LENGTH);
            break;
        }
        case (NODE_ATT_FIELD_NUMBER):
        {
            char name[CM_KEY_MAX_LENGTH];
            Att_value_t value;
            PBReader *subReader = reader->getSubMessageReader();
            while (subReader->readTag())
            {
                lb_uint32_t fieldNumber = subReader->getFieldNumber();
                switch (fieldNumber)
                {
                case (1):
                {
                    subReader->readValue_s(name, CM_KEY_MAX_LENGTH);
                    break;
                }
                case (2):
                {
                    subReader->readValue(&value.f);
                    break;
                }
                case (3):
                {
                    subReader->readValue(&value.i);
                    break;
                }
                default:
                {
                    subReader->skip();
                    break;
                }
                }
            }
            delete subReader;
            // reach this point we can set the attribute.
            if (!n->TrySetAtt(name, value))
            {
                return false;
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

bool OnnxGraphBuilder ::_link(PBReader *reader)
{
    while (reader->readTag())
    {
        switch (reader->getFieldNumber())
        {
        case (NODE_INPUT_FIELD_NUMBER):
        {
        }
        case (NODE_OUTPUT_FIELD_NUMBER):
        {
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

bool OnnxGraphBuilder ::_readValueInfos(lb_uint32_t field, BlueSteelLadyBug ::PBReader *reader)
{
    while (reader->readTag())
    {
        switch (reader->getFieldNumber())
        {
        default:
        {
            reader->skip();
            break;
        }
        }
    }
    return true;
}

NodePtr OnnxGraphBuilder ::_createNode(const char *typeName)
{
    return NodeRegistry ::ForName(typeName);
}
