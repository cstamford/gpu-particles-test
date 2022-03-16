#pragma once

#include <cstdint>

namespace br::gfx
{
    constexpr uint32_t INVALID_IDX = ~0u;

    struct Id { uint32_t idx = INVALID_IDX; };
    struct CameraId : public Id { };
    struct CanvasId : public Id { };
    struct MaterialId : public Id { };
    struct MeshId : public Id { };
    struct ParticleId : public Id { };
    struct ShaderId : public Id { };

    struct Colour
    {
        float r, g, b, a;

        static constexpr inline Colour white() { return { 1.0f, 1.0f, 1.0f, 1.0f }; }
    };

    void init();
    void free();
    void update(const float dt);
    void reload_shaders();
}
