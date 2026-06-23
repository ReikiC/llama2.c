#ifndef QUANT_H
#define QUANT_H

#include <stdint.h> // int8_t, uint8_t

// ----------------------------------------------------------------------------
// int8 group-wise quantization helpers. Only linked into the quantized
// backend (model/transformer/transformer_q.c -> runq). Lives here in
// inference/quant/quant.c.

// A quantized tensor: int8 values grouped in chunks of `gs`, each with a scale.
typedef struct {
    int8_t* q;  // quantized values
    float*  s;  // scaling factors (one per group of `gs` values)
    int     gs; // group size used when this tensor was quantized
} QuantizedTensor;

// Dequantize n values of qx into the float buffer x.
void dequantize(QuantizedTensor *qx, float* x, int n);

// Quantize n values of x into qx (n must be a multiple of qx->gs).
void quantize(QuantizedTensor *qx, float* x, int n);

// Initialize `n` quantized tensors (each `size_each` elements, group size `gs`)
// by mapping them out of the memory at *ptr, advancing *ptr past them. Each
// tensor's `gs` field is set to `gs`. Returns a malloc'd array.
QuantizedTensor *init_quantized_tensors(void **ptr, int n, int size_each, int gs);

#endif // QUANT_H
