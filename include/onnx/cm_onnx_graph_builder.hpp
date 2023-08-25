#ifndef _CM_ONNX_GRAPH_BUILDER__
#define _CM_ONNX_GRAPH_BUILDER__

#include "cm_graph.hpp"
#include "pb/lb_parser.hpp"

namespace CyanMycelium
{

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
        NodeCollection _nodes;
        KeyValueCollection<Link *> _links;

        bool _readGraph(BlueSteelLadyBug ::PBReader *);
        bool _readNode(char *, BlueSteelLadyBug ::PBReader *);
        bool _readValueInfos(char *, BlueSteelLadyBug ::PBReader *);
        bool _readInitializer(BlueSteelLadyBug ::PBReader *);
        bool _readTensorType(Tensor *, BlueSteelLadyBug ::PBReader *);
        Node *_createNode(const char *);
        Link *_createLink();
        Link *_getOrCreateLink(const char *);
    };
}
#endif