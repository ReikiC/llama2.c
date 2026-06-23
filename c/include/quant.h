#ifndef QUANT_H
#define QUANT_H

#include <stdint.h> // int8_t, uint8_t

// ----------------------------------------------------------------------------
// int8 group-wise quantization helpers. Only linked into the quantized
// backend (transformer_q.c -> runq). Lives in the shared layer's sibling
// module quant.c.

// A quantized tensor: int8 values grouped in chunks of GS, each with a scale.
typedef struct {
    int8_t* q;    // quantized values
    float* s; // scaling factors (one per group of GS values)
} QuantizedTensor;

// Group size used when the checkpoint was quantized. Set by the quantized
// backend's read_checkpoint() from the checkpoint header.
extern int GS;

// Dequantize n values of qx into the float buffer x.
void dequantize(QuantizedTensor *qx, float* x, int n);

// Quantize n values of x into qx (n must be a multiple of GS).
void quantize(QuantizedTensor *qx, float* x, int n);

// Initialize `n` quantized tensors (each `size_each` elements) by mapping them
// out of the memory at *ptr, advancing *ptr past them. Returns a malloc'd array.
QuantizedTensor *init_quantized_tensors(void **ptr, int n, int size_each);

#endif // QUANT_H
