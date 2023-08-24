#ifndef _CM_ONNX_GRAPH_BUILDER__
#define _CM_ONNX_GRAPH_BUILDER__

#include "cm_graph.hpp"
#include "pb/lb_parser.hpp"

namespace CyanMycelium
{
#ifndef CM_DEFAULT_GRAPH_BUILDER_COLLECTION_CAPACITY
#define CM_DEFAULT_GRAPH_BUILDER_COLLECTION_CAPACITY 16
#endif

    class OnnxGraphBuilder
    {
    public:
        OnnxGraphBuilder(int initialNodesCollectionSize = CM_DEFAULT_GRAPH_BUILDER_COLLECTION_CAPACITY,
                         int initialLinkCollectionSize = CM_DEFAULT_GRAPH_BUILDER_COLLECTION_CAPACITY);
        ~OnnxGraphBuilder();
        OnnxGraphBuilder &WithReader(BlueSteelLadyBug ::PBReader *);

        Graph *Build(Graph *target = nullptr);

    private:
        BlueSteelLadyBug ::PBReader *_reader;
        NodeCollection _nodes;
        KeyValueCollection<Link *> _links;

        bool _readGraph(BlueSteelLadyBug ::PBReader *);
        bool _readNode(BlueSteelLadyBug ::PBReader *);
        bool _linkNode(BlueSteelLadyBug ::PBReader *);
        bool _readValueInfos(lb_uint32_t, BlueSteelLadyBug ::PBReader *);
        bool _readTensor(bool, BlueSteelLadyBug ::PBReader *);
        NodePtr _createNode(const char *typeName);
    };
}
#endif