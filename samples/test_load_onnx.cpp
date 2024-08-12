#include <iostream>
#include <fstream>

#include "onnx/cm_onnx_graph_builder.hpp"
#include "pb/lb_memory_stream.hpp"
#include "cm_engine.hpp"
#include "nodes/unary/cm_unary.hpp"

using namespace BlueSteelLadyBug;
using namespace CyanMycelium;

char *ReadFileIntoMemory(const char *filename, size_t *fileSize)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return nullptr;
    }

    *fileSize = file.tellg();
    char *buffer = new char[*fileSize];

    file.seekg(0, std::ios::beg);
    file.read(buffer, *fileSize);
    file.close();

    return buffer;
}

int __main()
{
    const char *filename = "C:/Users/guill/Documents/sources/cyanmycelium/models/abs/abs.onnx";
    size_t fileSize;
    char *fileContents = ReadFileIntoMemory(filename, &fileSize);

    if (fileContents)
    {
        MemoryStream *input = new MemoryStream((lb_byte_t *)fileContents, fileSize);
        PBReader *reader = new PBReader(input);

        OnnxGraphBuilder builder;
        Graph *graph = builder.WithReader(reader).Build();
        delete input;
        delete reader;
        if (!graph)
        {
            std::cerr << "Failed to build graph: code [" << builder.GetError() << "]:" << builder.GetErrorInfos() << std::endl;
            return 0;
        }

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
    }
    return 0;
}
