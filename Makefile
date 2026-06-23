# choose your compiler, e.g. gcc/clang
# example override to clang: make run CC=clang
CC = gcc

# all build output (binaries, test scratch) goes here so the repo root stays clean.
# the whole directory is gitignored.
BUILDDIR = build

# the shared inference sources, compiled into both the fp32 (`run`) and the
# int8-quantized (`runq`) binaries. Each binary additionally links exactly one
# backend: c/model/transformer/transformer.c (fp32) or c/model/transformer/transformer_q.c + c/inference/quant/quant.c.
SHARED = c/core/net/net.c c/inference/tokenizer/tokenizer.c c/inference/sampler/sampler.c c/core/utils/utils.c c/app/app.c c/app/main.c
INCLUDES = -Ic

# ensure the build directory exists (order-only prerequisite)
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# the most basic way of building that is most likely to work on most systems
.PHONY: run
run: | $(BUILDDIR)
	$(CC) -O3 $(INCLUDES) -o $(BUILDDIR)/run c/model/transformer/transformer.c $(SHARED) -lm
	$(CC) -O3 $(INCLUDES) -o $(BUILDDIR)/runq c/model/transformer/transformer_q.c c/inference/quant/quant.c $(SHARED) -lm

# useful for a debug build, can then e.g. analyze with valgrind, example:
# $ valgrind --leak-check=full ./build/run data/model.bin -n 3
rundebug: | $(BUILDDIR)
	$(CC) -g $(INCLUDES) -o $(BUILDDIR)/run c/model/transformer/transformer.c $(SHARED) -lm
	$(CC) -g $(INCLUDES) -o $(BUILDDIR)/runq c/model/transformer/transformer_q.c c/inference/quant/quant.c $(SHARED) -lm

# https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
# https://simonbyrne.github.io/notes/fastmath/
# -Ofast enables all -O3 optimizations.
# Disregards strict standards compliance.
# It also enables optimizations that are not valid for all standard-compliant programs.
# It turns on -ffast-math, -fallow-store-data-races and the Fortran-specific
# -fstack-arrays, unless -fmax-stack-var-size is specified, and -fno-protect-parens.
# It turns off -fsemantic-interposition.
# In our specific application this is *probably* okay to use
.PHONY: runfast
runfast: | $(BUILDDIR)
	$(CC) -Ofast $(INCLUDES) -o $(BUILDDIR)/run c/model/transformer/transformer.c $(SHARED) -lm
	$(CC) -Ofast $(INCLUDES) -o $(BUILDDIR)/runq c/model/transformer/transformer_q.c c/inference/quant/quant.c $(SHARED) -lm

# additionally compiles with OpenMP, allowing multithreaded runs
# make sure to also enable multiple threads when running, e.g.:
# OMP_NUM_THREADS=4 ./build/run data/model.bin
.PHONY: runomp
runomp: | $(BUILDDIR)
	$(CC) -Ofast -fopenmp -march=native $(INCLUDES) c/model/transformer/transformer.c $(SHARED) -lm -o $(BUILDDIR)/run
	$(CC) -Ofast -fopenmp -march=native $(INCLUDES) c/model/transformer/transformer_q.c c/inference/quant/quant.c $(SHARED) -lm -o $(BUILDDIR)/runq

.PHONY: win64
win64: | $(BUILDDIR)
	x86_64-w64-mingw32-gcc -Ofast -D_WIN32 -o $(BUILDDIR)/run.exe $(INCLUDES) c/model/transformer/transformer.c $(SHARED) c/platform/win/win.c
	x86_64-w64-mingw32-gcc -Ofast -D_WIN32 -o $(BUILDDIR)/runq.exe $(INCLUDES) c/model/transformer/transformer_q.c c/inference/quant/quant.c $(SHARED) c/platform/win/win.c

# compiles with gnu11 standard flags for amazon linux, coreos, etc. compatibility
.PHONY: rungnu
rungnu: | $(BUILDDIR)
	$(CC) -Ofast -std=gnu11 $(INCLUDES) -o $(BUILDDIR)/run c/model/transformer/transformer.c $(SHARED) -lm
	$(CC) -Ofast -std=gnu11 $(INCLUDES) -o $(BUILDDIR)/runq c/model/transformer/transformer_q.c c/inference/quant/quant.c $(SHARED) -lm

.PHONY: runompgnu
runompgnu: | $(BUILDDIR)
	$(CC) -Ofast -fopenmp -std=gnu11 $(INCLUDES) c/model/transformer/transformer.c $(SHARED) -lm -o $(BUILDDIR)/run
	$(CC) -Ofast -fopenmp -std=gnu11 $(INCLUDES) c/model/transformer/transformer_q.c c/inference/quant/quant.c $(SHARED) -lm -o $(BUILDDIR)/runq

# run all tests (Python deps are uv-managed; use `uv sync` first)
.PHONY: test
test:
	uv run pytest

# run only tests for the C implementation (is a bit faster if only C code changed)
.PHONY: testc
testc:
	uv run pytest -k runc

# run the C tokenizer tests, without touching pytest / python
# to increase verbosity level run e.g. as `make testcc VERBOSITY=1`
VERBOSITY ?= 0
.PHONY: testcc
testcc: | $(BUILDDIR)
	$(CC) -DVERBOSITY=$(VERBOSITY) -O3 $(INCLUDES) -o $(BUILDDIR)/testc tests/test.c c/inference/tokenizer/tokenizer.c -lm
	./$(BUILDDIR)/testc

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)
