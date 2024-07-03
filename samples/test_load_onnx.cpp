#include <iostream>
#include <fstream>

#include "onnx/cm_onnx_graph_builder.hpp"
#include "pb/lb_memory_stream.hpp"
#include "cm_engine.hpp"

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

int main()
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
        if (!graph)
        {
            std::cerr << "Failed to build graph: code [" << builder.GetError() << "]:" << builder.GetErrorInfos() << std::endl;
            delete input;
            delete reader;
            return 0;
        }
        // declare the engine options with default values
        InferenceEngineOptions options;

        // create the inference engine and start it
        InferenceEnginePtr engine = new InferenceEngine(options);

        // define handlers
        ActivationContextHandlers handlers(engine);
        handlers.OnEnded = [](ActivationContext *context, void *userData)
        {
            std::cout << "Inference ended" << std::endl;
            std::cout << "Output is: " << ((float *)(context->GetOutput("output")->Data))[0] << std::endl;
            context->GetEngine()->Stop();
        };

        // create the inference session
        AsyncActivationContext *session = engine->CreateSession(graph, &handlers);

        // create the input data
        float data[1] = {-1.0f};
        std::cout << "Input: " << data[0] << std::endl;

        session->SetInput("input", data);
        session->SetOutput("output", data);

        if (!session->Run())
        {
            std::cerr << "Failed to run inference" << std::endl;
            delete engine;
            delete session;
            delete graph;
            delete input;
            delete reader;
            return 0;
        }

        // wait for the inference to end
        engine->Join();

        // cleanup
        delete engine;
        delete session;
        delete graph;
        delete input;
        delete reader;
    }
    return 0;
}