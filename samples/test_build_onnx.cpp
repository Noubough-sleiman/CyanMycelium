#include <iostream>
#include <fstream>

#include "onnx/cm_onnx_graph_builder.hpp"
#include "pb/lb_memory_stream.hpp"
#include "cm_engine.hpp"
#include "math/cm_tensor.hpp"
#include "nodes/unary/cm_unary.hpp"

using namespace BlueSteelLadyBug;
using namespace CyanMycelium;

Graph *buildGraphManually()
{
    // Créez les nœuds
    UnaryOperator *absNode = new Abs(); // Utilisez le nœud opérateur abs

    // Ajouter les nœuds à la collection de nœuds du graphe

    // Créer les liens
    uint64_t shape[] = {1, 0, 0};
    Link *inputLink = new Link(shape, 1, TDT_FLOAT);
    Link *outputLink = new Link(shape, 1, TDT_FLOAT);

    // construire le graphe
    GraphPtr graph = new Graph();
    graph->Add(outputLink)->Add(absNode)->Add(inputLink);

    absNode->Opsc.Add(inputLink);
    absNode->Onsc.Add(outputLink);
    inputLink->Ofin = absNode;
    outputLink->Oini = absNode;

    graph->Inputs.Set("input", inputLink);
    graph->Outputs.Set("output", outputLink);

    return graph;
}

int main()
{
    Graph *graph = buildGraphManually();

    // declare the engine options with default values
    InferenceEngineOptions options;

    // create the inference engine and start it
    InferenceEnginePtr engine = new InferenceEngine(options);

    // define handlers -> here we only need to know when the inference ends
    ActivationContextHandlers handlers(engine);
    handlers.OnEnded = [](ActivationContext *context, void *userData)
    {
        std::cout << "Inference ended" << std::endl;
        std::cout << "Output is: " << ((float *)(context->GetOutput("output")->Data))[0] << std::endl;
        // we stop the engine, then this will exit the main.
        context->GetEngine()->Stop();
    };

    // create the inference session
    AsyncActivationContext *session = engine->CreateInferenceSession(graph, &handlers);

    // create the input data (a single float)
    float *data = new float[1];
    data[0] = -1.0f;
    std::cout << "Input: " << data[0] << std::endl;

    // set the input data
    session->SetInput("input", data);

    // start the inference
    if (!session->Run())
    {
        std::cerr << "Failed to run inference" << std::endl;
        engine->Stop();
    }

    // wait for the engine to end
    engine->Join();

    // cleanup
    delete session;
    delete graph;
    delete engine;

    return 0;
}
