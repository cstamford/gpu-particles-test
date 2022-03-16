#pragma once

#include <borealis/util/Time.hpp>

#if defined(_WIN32)
    #include <Windows.h>
#endif

namespace br::time
{
#if defined(_WIN32)
    LARGE_INTEGER query_hpc()
    {
        LARGE_INTEGER hpc;
        QueryPerformanceFrequency(&hpc);
        return hpc;
    };

    uint64_t current()
    {
        LARGE_INTEGER time;
        QueryPerformanceCounter(&time);
        return time.QuadPart;
    }

    float diff(const uint64_t first, const uint64_t second)
    {
        static LARGE_INTEGER s_freq = query_hpc();
        uint64_t delta = first - second;
        delta *= 1000 * 1000 * 1000; // nanoseconds
        delta /= s_freq.QuadPart; // to time units
        return (float)(delta * 1e-9);
    }
#endif
}
