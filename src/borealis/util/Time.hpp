#pragma once

#include <cstdint>

namespace br::time
{
    uint64_t current();
    float diff(const uint64_t first, const uint64_t second);
}
