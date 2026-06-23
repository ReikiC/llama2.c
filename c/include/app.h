#ifndef APP_H
#define APP_H

#include <stddef.h> // size_t
#include "transformer.h"
#include "tokenizer.h"
#include "sampler.h"

// ----------------------------------------------------------------------------
// The application loops that drive a Transformer+Tokenizer+Sampler.
// Identical for both backends; lives in the shared layer (app.c).

// Generation loop: encode the prompt, autoregressively sample `steps` tokens,
// print them, and report achieved tok/s.
void generate(Transformer *transformer, Tokenizer *tokenizer, Sampler *sampler, char *prompt, int steps);

// Interactive chat loop using the Llama 2 Chat [INST] schema.
void chat(Transformer *transformer, Tokenizer *tokenizer, Sampler *sampler,
          char *cli_user_prompt, char *cli_system_prompt, int steps);

// Read a line from stdin (up to but not including '\n') into buffer.
void read_stdin(const char* guide, char* buffer, size_t bufsize);

#endif // APP_H
