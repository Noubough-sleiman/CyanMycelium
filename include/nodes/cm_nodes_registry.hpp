#ifndef __CM_NODES_REGISTRY__
#define __CM_NODES_REGISTRY__

#include "cm_graph.hpp"

namespace CyanMycelium
{
#define CM_NODE_REGISTRY_INITIAL_CAPACITY 128
#define __REGISTER__NODE(n) _types.Set(#n, []() -> Operator * { return new n(); })

    typedef Operator *(*NodeInitializer_fn)();

    class NodeRegistry
    {
    public:
        NodeRegistry(int initialCapacity = CM_NODE_REGISTRY_INITIAL_CAPACITY);
        static Operator *ForName(const char *);

    private:
        static NodeRegistry __Shared;
        KeyValueCollection<NodeInitializer_fn> _types;
    };
}
#endif