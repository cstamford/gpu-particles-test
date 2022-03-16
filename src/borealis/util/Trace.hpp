#pragma once

#include <cstdio>

namespace br::trace
{
    void write(const char* file, int line, const char* message);

    template <typename ... Args>
    void write(const char* file, int line, const char* format, Args ... args)
    {
        char buffer[4096];
        snprintf(buffer, sizeof(buffer), format, args ...);
        write(file, line, buffer);
    }

#define BR_TRACE(format, ...) \
    ::br::trace::write(__FILE__, __LINE__, (format), __VA_ARGS__)

#if defined(_DEBUG)
    #define BR_ASSERT(condition) \
        do \
        { \
            if (!(condition)) \
            { \
                __debugbreak(); \
                BR_TRACE("Assertion failed: %s.", #condition); \
            } \
        } \
        while (0)

    #define BR_ASSERT_FAIL() \
    { \
        __debugbreak(); \
        BR_TRACE("Assertion failed."); \
    }
#else
    #define BR_ASSERT(condition) (void)0
    #define BR_ASSERT_FAIL() (void)0
#endif

}
