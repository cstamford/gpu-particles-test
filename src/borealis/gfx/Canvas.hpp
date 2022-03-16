#pragma once

#include <borealis/gfx/Gfx.hpp>
#include <borealis/util/Math.hpp>

namespace br::gfx
{
    CanvasId canvas_make();
    void canvas_begin(const CanvasId id);
    void canvas_clear(const CanvasId id, const Colour colour);
    void canvas_draw(const MaterialId material_id, const MeshId mesh_id, math::Mat3Ref transform);
    void canvas_draw_particle(const ParticleId particle_id, math::Mat3Ref transform);
    void canvas_set_camera(const CanvasId id, const CameraId camera);
    void canvas_set_size(const CanvasId id, const uint32_t width, const uint32_t height);
}
