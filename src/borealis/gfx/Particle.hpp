#pragma once

#include <borealis/gfx/Gfx.hpp>
#include <borealis/util/Math.hpp>

namespace br::gfx
{
    struct ParticleData
    {
        float fps = 60.0f;
        float lifetime = 0.0f;
        uint32_t count = 0;
        math::Vec2 dir = math::Vec2::up();
        float velocity = 0.0f;
        float acceleration = 0.0f;
        float angular_velocity = 0.0f;
        float angular_acceleration = 0.0f;
        float scale = 1.0f;

        float gravity = 9.81f; // 1 pixel / meter
        math::Vec2 gravity_dir = -math::Vec2::up();

        Colour col_start = Colour::white();
        Colour col_end = Colour::white();

        float rand_dir = 0.0f;
        float rand_lifetime = 0.0f;
        float rand_velocity = 0.0f;
        float rand_angular_velocity = 0.0f;
        float rand_scale = 0.0f;

        bool one_shot = false;
        bool explode = false;
    };

    ParticleId particle_make();
    void particle_free(const ParticleId id);
    bool particle_finished(const ParticleId id);
    void particle_set_data(const ParticleId id, ParticleData data);
}
