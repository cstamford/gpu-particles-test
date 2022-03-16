#pragma once

#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>

namespace br::file
{
    std::vector<std::byte> read_all_bytes(const std::filesystem::path& path);
    std::string read_all_text(const std::filesystem::path& path);
}
