#ifndef _CM_ONNX_GRAPH_BUILDER__
#define _CM_ONNX_GRAPH_BUILDER__

#include "cm_graph_builder.hpp"
#include "pb/lb_parser.hpp"

namespace CyanMycelium
{
    class OnnxGraphBuilder : public GraphBuilder
    {
    public:
        OnnxGraphBuilder();
        ~OnnxGraphBuilder();
        OnnxGraphBuilder &WithReader(BlueSteelLadyBug ::PBReader *);
        Graph *Build() override;

    private:
        BlueSteelLadyBug ::PBReader *_reader;

        bool _readGraph(BlueSteelLadyBug ::PBReader *);
        bool _readNode(BlueSteelLadyBug ::PBReader *);
        bool _link(BlueSteelLadyBug ::PBReader *);
        bool _readValueInfos(lb_uint32_t, BlueSteelLadyBug ::PBReader *);
        NodePtr _createNode(const char *typeName);
    };
}
#endif