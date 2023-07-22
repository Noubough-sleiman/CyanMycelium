- To create an ONNX file with only one node that is an absolute value (abs) operation, you can create a simple model in PyTorch and export it to ONNX format. Here's an example of how to create a model with only one abs node in PyTorch and export it to ONNX format:

```python
import torch
import torch.nn as nn

class AbsModel(nn.Module):
    def __init__(self):
        super(AbsModel, self).__init__()
        self.abs = nn.Abs()

    def forward(self, x):
        return self.abs(x)

model = AbsModel()
dummy_input = torch.randn(1)
torch.onnx.export(model, dummy_input, "abs.onnx", verbose=True)
```

- ONNX supports other formats than binary. ONNX defines a common file format to enable AI developers to use models with a variety of frameworks, tools, runtimes, and compilers. The ONNX format is based on the Protocol Buffers format which is a language- and platform-neutral format for serializing structured data.

- To convert the previous textual example to a protobuf file, you can use the `SerializeToString()` method available in every ONNX object. Here's an example of how to do it in Python:

```python
import onnx

model = onnx.load_text("abs.onnx.txt")
onnx.checker.check_model(model)
serialized = model.SerializeToString()

with open("abs.onnx", "wb") as f:
    f.write(serialized)
```

- To convert an ONNX text format to protobuf without Python, you can use the `Onnx2Text` tool. Here's an example of how to do it:

```bash
Onnx2Text input.onnx.txt output.onnx
```

Onnx2Text is located into the tools folder.
