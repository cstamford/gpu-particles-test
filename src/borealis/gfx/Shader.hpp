#pragma once

#include <borealis/gfx/Gfx.hpp>

namespace br::gfx
{
    enum ShaderType
    {
        Vertex,
        Fragment,
        Compute,
        ShaderTypeCount
    };

    ShaderId shader_make();
    void shader_free(const ShaderId id);
    void shader_set_code(const ShaderId id, const ShaderType type, const char* code, const char* header = nullptr);
    void shader_set_reload_from_disk(const ShaderId id, const char* path);
}
