#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdint.h> // int8_t

// ----------------------------------------------------------------------------
// The Byte Pair Encoding (BPE) Tokenizer that translates strings <-> tokens.
// Identical for both backends; lives in the shared layer (tokenizer.c).

typedef struct {
    char *str;
    int id;
} TokenIndex;

typedef struct {
    char** vocab;
    float* vocab_scores;
    TokenIndex *sorted_vocab;
    int vocab_size;
    unsigned int max_token_length;
    unsigned char byte_pieces[512]; // stores all single-byte strings
} Tokenizer;

void build_tokenizer(Tokenizer* t, char* tokenizer_path, int vocab_size);
void free_tokenizer(Tokenizer* t);

// Decode a single token id into a string piece (accounting for byte fallback
// and the leading-whitespace strip that follows the BOS token).
char* decode(Tokenizer* t, int prev_token, int token);

// printf a token piece, but skip non-printable / non-whitespace control bytes.
void safe_printf(char *piece);

// Encode text into a preallocated tokens[] buffer. bos != 0 prepends BOS (=1),
// eos != 0 appends EOS (=2). *n_tokens receives the count.
void encode(Tokenizer* t, char *text, int8_t bos, int8_t eos, int *tokens, int *n_tokens);

#endif // TOKENIZER_H
