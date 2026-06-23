#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include "core/config/config.h"

// ----------------------------------------------------------------------------
// The Transformer is the only type the shared layer (main, app) ever sees.
// The weight table, the activation buffers, and the memory-mapping bookkeeping
// are all backend-specific (fp32 in transformer.c, int8-quantized in
// transformer_q.c), so they are completed privately inside each backend
// translation unit and held here behind opaque pointers. build_transformer
// heap-allocates them; free_transformer frees them.
typedef struct TransformerWeights TransformerWeights;
typedef struct RunState RunState;
typedef struct TransformerMmap TransformerMmap; // checkpoint memory-mapping handle

typedef struct {
    Config config; // the hyperparameters of the architecture (the blueprint)
    TransformerWeights *weights; // the weights of the model
    RunState *state; // buffers for the "wave" of activations in the forward pass
    TransformerMmap *mmap; // memory mapping of the checkpoint file (backend-private)
} Transformer;

// Build (read checkpoint + allocate state) / tear down / run one forward step.
// The signatures are identical for both backends; only the checkpoint format
// they accept differs (fp32 .bin vs quantized "ak42" v2 .bin).
void build_transformer(Transformer *t, char* checkpoint_path);
void free_transformer(Transformer* t);
float* forward(Transformer* transformer, int token, int pos);

#endif // TRANSFORMER_H
