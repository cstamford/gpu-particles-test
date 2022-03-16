#include <borealis/util/Trace.hpp>

#if defined(_WIN32)
    #include <Windows.h>
#endif

#include <cstdint>

namespace br
{
    void trace::write(const char* file, int line, const char* message)
    {
        uint8_t hour = 0;
        uint8_t minute = 0;
        uint8_t second = 0;
        uint16_t millisecond = 0;

#if defined(_WIN32)
        SYSTEMTIME time;
        GetLocalTime(&time);
        hour = static_cast<uint8_t>(time.wHour);
        minute = static_cast<uint8_t>(time.wMinute);
        second = static_cast<uint8_t>(time.wSecond);
        millisecond = static_cast<uint16_t>(time.wMilliseconds);
#endif

        uint32_t threadId = 0;

#if defined(_WIN32)
        threadId = GetCurrentThreadId();
#endif

        printf("[%02d:%02d:%02d:%03d] [0x%x] [%s:%d]: %s\n",
            hour, minute, second, millisecond,
            threadId, file, line, message);
        fflush(stdout);
    }
}
