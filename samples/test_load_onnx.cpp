#include <iostream>
#include <fstream>

#include "onnx/cm_onnx_graph_builder.hpp"
#include "pb/lb_memory_stream.hpp"

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
    const char *filename = "C:/Users/guill/Documents/sources/cyanmycelium/models/lstm/lstm.onnx";
    size_t fileSize;
    char *fileContents = ReadFileIntoMemory(filename, &fileSize);

    if (fileContents)
    {
        MemoryStream *input = new MemoryStream((lb_byte_t *)fileContents, fileSize);
        PBReader *reader = new PBReader(input);

        OnnxGraphBuilder builder;
        Graph *g = builder.WithReader(reader).Build();
        if (g)
        {
            for (int i = 0; i != g->Links.Count(); i++)
            {
                Link *l = g->Links[i];
                if (l)
                {
                }
            }
            delete g;
        }
        delete input;
        delete reader;
    }
    return 0;
}