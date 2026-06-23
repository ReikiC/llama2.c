#ifndef SAMPLER_H
#define SAMPLER_H

// ----------------------------------------------------------------------------
// The Sampler, which takes logits and returns a sampled token.
// Sampling can be done in a few ways: greedy argmax, sampling, top-p sampling.
// Identical for both backends; lives in the shared layer (sampler.c).

typedef struct {
    float prob;
    int index;
} ProbIndex; // struct used when sorting probabilities during top-p sampling

typedef struct {
    int vocab_size;
    ProbIndex* probindex; // buffer used in top-p sampling
    float temperature;
    float topp;
    unsigned long long rng_state;
} Sampler;

void build_sampler(Sampler* sampler, int vocab_size, float temperature, float topp, unsigned long long rng_seed);
void free_sampler(Sampler* sampler);

// Sample the next token given the (temperature/top-p configured) logits.
int sample(Sampler* sampler, float* logits);

#endif // SAMPLER_H
