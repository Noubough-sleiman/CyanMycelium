#include "cm_graph_builder.hpp"
#include "cm_engine.hpp"
#include "nodes/unary/cm_unary.hpp"

#include <cstdlib>
#include <stdio.h>

using namespace CyanMycelium;

float *GenerateTimeSeries(const int rows, const int cols, float from, float to)
{

    // Allocate memory for the array
    float *timeSeries = new float[rows * cols];

    // Generate random float values and fill the array
    float delta = (to - from);
    for (int i = 0; i < rows * cols; ++i)
    {
        float r = rand() % 100 + 1;
        float t = r / 100.0;
        float v = from + delta * t;
        timeSeries[i] = v;
    }

    return timeSeries;
}

int CountNegativeValue(float *arr, int size)
{
    int c = 0;
    for (int i = 0; i < size; ++i)
    {
        if (arr[i] < 0.0f)
        {
            c++;
        }
    }
    return c;
}

int main()
{
    // build a simple graph
    GraphBuilderPtr gb = new GraphBuilder();

    // tensor format infos
    const uint32_t rows = 50;
    const uint32_t cols = 50;
    const uint32_t shape[] = {rows, cols};
    const int dimension = 2;

    // the links holding the tensors
    LinkPtr input = new Link(shape, dimension, TDT_FLOAT);
    LinkPtr output = new Link(shape, dimension, TDT_FLOAT);

    // the operation
    AbsPtr abs_node = new Abs();

    // build the graph
    gb->WithNode("abs_0", abs_node)
        ->WithLink("input", input)
        ->WithLink("output", output)
        ->WithConnection(input, abs_node, output);

    GraphPtr graph = gb->Build();

    // compute
    MemoryManagerBase mm;

    InferenceEngineOptions options;

    InferenceEnginePtr engine = new InferenceEngine(options);
    engine->Start();

    InferenceSessionPtr session = engine->CreateSession(graph, &mm);
    void *data = GenerateTimeSeries(rows, cols, -100, 10);
    int negativCounter = CountNegativeValue((float *)data, rows * cols);
    printf("Before activation, Input tensor has %d negative values.\r\n", negativCounter);

    DataCollections inputs(1);
    inputs.Set("input", data);
    DataCollections outputs(1);

    // run inference
    session->RunAsync(&inputs, &outputs);

    // wait for the result to be available
    DataCollections *result = session->AwaitOutputs();

    // grab result
    data = result->Get("output"); // should be also accessed by index (faster) (*result)[0]->Value
    negativCounter = CountNegativeValue((float *)data, rows * cols);
    printf("After activation, Output tensor has %d negative values.\r\n", negativCounter);

    // clean the session and stop the engine.
    engine->Stop();
}