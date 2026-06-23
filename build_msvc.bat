@echo off
rem Build the fp32 inference binary with MSVC into the build\ folder (kept out
rem of the repo root and gitignored). The shared sources plus the fp32 backend
rem (transformer.c) and the Windows mmap shim (win.c) are compiled in one
rem invocation. cl.exe defines _WIN32, so the backend picks up win.h.
if not exist build mkdir build
cl.exe /fp:fast /Ox /openmp /Ic /Febuild\run.exe c\model\transformer\transformer.c c\core\net\net.c c\inference\tokenizer\tokenizer.c c\inference\sampler\sampler.c c\core\utils\utils.c c\app\app.c c\app\main.c c\platform\win\win.c
