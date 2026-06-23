/* Utility: time, for benchmarking. Shared by the fp32 and quantized backends. */

#include <time.h>
#if defined _WIN32
    #include "platform/win/win.h" // clock_gettime shim
#endif
#include "core/utils/utils.h"

long time_in_ms() {
    // return time in milliseconds, for benchmarking the model speed
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return time.tv_sec * 1000 + time.tv_nsec / 1000000;
}
