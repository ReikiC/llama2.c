#ifndef NET_H
#define NET_H

// Shared neural-net building blocks. These are identical for the fp32 and the
// int8-quantized backends, so they live in the shared layer (net.c) and are
// used both inside forward() and by the sampler (sample() calls softmax).

// Root-mean-square normalization: normalize x and scale by weight.
void rmsnorm(float* o, float* x, float* weight, int size);

// In-place softmax over x[0..size).
void softmax(float* x, int size);

#endif // NET_H
