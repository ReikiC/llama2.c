@echo off
rem Build the fp32 inference binary with MSVC into the build\ folder (kept out
rem of the repo root and gitignored). The shared sources plus the fp32 backend
rem (transformer.c) and the Windows mmap shim (win.c) are compiled in one
rem invocation. cl.exe defines _WIN32, so the backend picks up win.h.
if not exist build mkdir build
cl.exe /fp:fast /Ox /openmp /Ic\include /Febuild\run.exe c\src\transformer.c c\src\net.c c\src\tokenizer.c c\src\sampler.c c\src\utils.c c\src\app.c c\src\main.c c\src\win.c
