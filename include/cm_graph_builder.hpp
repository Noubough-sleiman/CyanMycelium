#ifndef _CM_GRAPH_BUILDER__
#define _CM_GRAPH_BUILDER__

#include "cm_graph.hpp"

namespace CyanMycelium
{
#ifndef CM_DEFAULT_GRAPH_BUILDER_COLLECTION_CAPACITY
#define CM_DEFAULT_GRAPH_BUILDER_COLLECTION_CAPACITY 16
#endif

    struct GraphBuilderOptions
    {
        int InitialNodesCollectionSize = CM_DEFAULT_GRAPH_BUILDER_COLLECTION_CAPACITY;
        int InitialLinkCollectionSize = CM_DEFAULT_GRAPH_BUILDER_COLLECTION_CAPACITY;
    };

    /// @brief
    class GraphBuilder
    {
    public:
        GraphBuilder(int initialNodesCollectionSize = CM_DEFAULT_GRAPH_BUILDER_COLLECTION_CAPACITY, int initialLinkCollectionSize = CM_DEFAULT_GRAPH_BUILDER_COLLECTION_CAPACITY) : _nodes(max(initialNodesCollectionSize, CM_DEFAULT_COLLECTION_CAPACITY)), _links(max(initialLinkCollectionSize, CM_DEFAULT_COLLECTION_CAPACITY)) {}
        ~GraphBuilder() {}

        GraphBuilder &WithNode(const char *, NodePtr);
        GraphBuilder &WithLink(const char *, LinkPtr);
        GraphBuilder &WithConnection(NodePtr, LinkPtr, NodePtr);
        GraphBuilder &WithConnection(LinkPtr, NodePtr, LinkPtr);

        NodePtr GetNode(const char *);
        LinkPtr GetLink(const char *);

        virtual GraphPtr Build();

    protected:
        KeyValueCollection<NodePtr> _nodes;
        KeyValueCollection<LinkPtr> _links;
    };

    typedef GraphBuilder *GraphBuilderPtr;
}
#endif