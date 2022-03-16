#pragma once

#include <borealis/gfx/Gfx.hpp>

namespace br::gfx
{
    MaterialId material_make();
    void material_free(const MaterialId id);
    void material_set_shader(const MaterialId id, const ShaderId shader_id);
    void material_link(const MaterialId id);
}
