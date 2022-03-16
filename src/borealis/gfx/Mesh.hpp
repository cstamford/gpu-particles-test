#pragma once

#include <borealis/gfx/Gfx.hpp>
#include <borealis/util/Math.hpp>

namespace br::gfx
{
    MeshId mesh_make();
    void mesh_set_geometry(const MeshId id, const math::Vec2* positions, const uint16_t positions_len, bool dynamic_draw = false);
    void mesh_set_indices(const MeshId id, const uint16_t indices, const uint16_t indices_len);
}
