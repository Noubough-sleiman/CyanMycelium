#ifndef __CYAN_MISCELIUM_GRAPH_BUILDER__
#define __CYAN_MISCELIUM_GRAPH_BUILDER__

#include "cm_graph.hpp"

namespace CyanMycelium
{
    /// @brief
    class GraphBuilder
    {
    public:
        GraphBuilder();
        ~GraphBuilder();

        GraphBuilder *WithNode(const char *, NodePtr);
        GraphBuilder *WithLink(const char *, LinkPtr);
        GraphBuilder *WithConnection(NodePtr, LinkPtr, NodePtr);
        GraphBuilder *WithConnection(LinkPtr, NodePtr, LinkPtr);

        NodePtr GetNode(const char *);
        LinkPtr GetLink(const char *);

        virtual GraphPtr Build();

    private:
        KeyValueCollection<NodePtr> _nodes;
        KeyValueCollection<LinkPtr> _links;
    };

    typedef GraphBuilder *GraphBuilderPtr;
}
#endif