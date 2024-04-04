#ifndef _CM_NODE_LSTM_
#define _CM_NODE_LSTM_
#include "cm_graph.hpp"

namespace CyanMycelium
{
#define LSTM_X_INDEX 0
#define LSTM_W_INDEX 1
#define LSTM_R_INDEX 2
#define LSTM_B_INDEX 3

  /// @class LSTMNode
  /// @brief Represents an LSTM node within an ONNX graph.
  /// The LSTMNode class models the behavior of an LSTM layer or cell in an ONNX graph.
  /// It takes input data and associated parameters, performs computations, and produces outputs.
  class LSTM : public Operator
  {
  public:
    LSTM() : Operator(){};

    /// @brief The input data (usually a sequence of vectors or embeddings).
    /// @return corresponding tensor reference
    TensorRef *X(ActivationContext *ctx) { return ctx->GetPayloadRef(this->Opsc[LSTM_X_INDEX]->Id); }

    /// @brief Weight matrix for the input data.
    /// @return corresponding tensor
    Tensor *W() { return this->Opsc[LSTM_W_INDEX]->GetPayloadInfos(); }

    /// @brief Weight matrix for the recurrent connections.
    /// @return corresponding tensor
    Tensor *R() { return this->Opsc[LSTM_R_INDEX]->GetPayloadInfos(); }

    /// @brief Biases for the LSTM gates.
    /// @return corresponding tensor
    Tensor *B() { return this->Opsc[LSTM_B_INDEX]->GetPayloadInfos(); }

    /// @brief  Performs forward propagation for the LSTM node.
    /// This method executes the computations associated with the LSTM node.
    /// It processes the input data and produces output sequences and states.
    /// @param ctx
    /// @return
    bool Activate(ActivationContext *ctx) override { return true; }
  };
}
#endif