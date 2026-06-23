#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include "config.h"

// ssize_t (used by the memory-mapping bookkeeping below)
#if defined _WIN32
    #include "win.h"
#else
    #include <sys/types.h>
#endif

// ----------------------------------------------------------------------------
// The Transformer is the only type the shared layer (main, app) ever sees.
// The weight table and the activation buffers are backend-specific (fp32 in
// transformer.c, int8-quantized in transformer_q.c), so they are completed
// privately inside each backend translation unit and held here behind opaque
// pointers. build_transformer heap-allocates them; free_transformer frees them.
typedef struct TransformerWeights TransformerWeights;
typedef struct RunState RunState;

typedef struct {
    Config config; // the hyperparameters of the architecture (the blueprint)
    TransformerWeights *weights; // the weights of the model
    RunState *state; // buffers for the "wave" of activations in the forward pass
    // some more state needed to properly clean up the memory mapping (sigh)
    int fd; // file descriptor for memory mapping
    float* data; // memory mapped data pointer
    ssize_t file_size; // size of the checkpoint file in bytes
} Transformer;

// Build (read checkpoint + allocate state) / tear down / run one forward step.
// The signatures are identical for both backends; only the checkpoint format
// they accept differs (fp32 .bin vs quantized "ak42" v2 .bin).
void build_transformer(Transformer *t, char* checkpoint_path);
void free_transformer(Transformer* t);
float* forward(Transformer* transformer, int token, int pos);

#endif // TRANSFORMER_H
