#ifndef _CM_ONNX_GRAPH_BUILDER__
#define _CM_ONNX_GRAPH_BUILDER__

#include "cm_graph.hpp"
#include "pb/lb_parser.hpp"

namespace CyanMycelium
{
#define ERROR_INFOS_MAX_LENGTH 128

#define ONNX_GB_SUCCESS 0
#define ONNX_GB_UNSUPPORTED_NODE 100
#define ONNX_GB_UNSUPPORTED_ATTRIBUTE 101
#define ONNX_GB_UNSUPPORTED_TENSOR_DATA_TYPE 110
#define ONNX_GB_UNSUPPORTED_TENSOR_DIM 111
#define ONNX_GB_UNSUPPORTED_TENSOR_UNKNOWN_DIM 112
#define ONNX_GB_READ_ERROR 200
#define ONNX_GB_SYSTEM_ERROR 300

    class OnnxGraphBuilder
    {
    public:
        OnnxGraphBuilder(int initialNodesCollectionSize = CM_DEFAULT_GRAPH_COLLECTION_CAPACITY,
                         int initialLinkCollectionSize = CM_DEFAULT_GRAPH_COLLECTION_CAPACITY);
        ~OnnxGraphBuilder();
        OnnxGraphBuilder &WithReader(BlueSteelLadyBug ::PBReader *);

        Graph *Build(Graph *target = nullptr);

    private:
        BlueSteelLadyBug ::PBReader *_reader;
        Collection<Operator *> _nodes;
        KeyValueCollection<Link *> _links;

        int _error;
        char _errorInfos[ERROR_INFOS_MAX_LENGTH];

        bool _readGraph(BlueSteelLadyBug ::PBReader *);
        bool _readNode(char *, BlueSteelLadyBug ::PBReader *);
        bool _readValueInfos(char *, BlueSteelLadyBug ::PBReader *);
        bool _readInitializer(char *, BlueSteelLadyBug ::PBReader *);
        bool _readTensorType(TensorInfos *, BlueSteelLadyBug ::PBReader *);
        bool _readTensorShape(TensorInfos *, BlueSteelLadyBug ::PBReader *);
        Operator *_createNode(const char *);
        Link *_createLink();
        Link *_getOrCreateLink(const char *);
        virtual void *_malloc(size_t s) { return cm_malloc(s); }
        virtual void _free(void *p) { return cm_free(p); }
    };
}
#endif