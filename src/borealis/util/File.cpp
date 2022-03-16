#include <borealis/util/File.hpp>
#include <borealis/util/Trace.hpp>

#include <fstream>

namespace br::file
{
    template <typename T>
    T read_all(const std::filesystem::path& path, const std::ios_base::openmode mode)
    {
        T ret;

        if (std::ifstream in(path, mode); in)
        {
            in.seekg(0, std::ios::end);
            ret.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read((char*)ret.data(), ret.size());
            in.close();
        }
        else
        {
            BR_TRACE("Failed to load file %s.", path.c_str());
        }

        return std::move(ret);
    }

    std::vector<std::byte> read_all_bytes(const std::filesystem::path& path)
    {
        return read_all<std::vector<std::byte>>(path, std::ios::in |std::ios::binary);
    }

    std::string read_all_text(const std::filesystem::path& path)
    {
        return read_all<std::string>(path, std::ios::in);
    }
}
