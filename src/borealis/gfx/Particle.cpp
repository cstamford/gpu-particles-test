#include <borealis/gfx/Particle.hpp>
#include <borealis/gfx/impl/State.hpp>

using namespace br::gfx::impl;

namespace br::gfx
{
    struct ParticleTemplate
    {
        uint32_t count;
        float lifetime;
        float scale;
        float gravity_dir;
        float gravity;
        float velocity_dir;
        float velocity;
        float acceleration;
        float angular_velocity;
        float angular_acceleration;
        float rand_dir;
        float rand_lifetime;
        float rand_velocity;
        float rand_angular_velocity;
        float rand_scale;
        uint32_t one_shot;
        Colour col_start;
        Colour col_end;
    };

    struct ParticleState
    {
        float lifetime = 0.0f;
        float scale = 0.0f;
        math::Vec2 pos = math::Vec2::zero();
        math::Vec2 velocity = math::Vec2::zero();
        float angular_velocity = 0.0f;
        float age = 0.0f;
    };

    struct ParticleVert
    {
        math::Vec2 pos;
        math::Vec2 uv;
        Colour col;
    };

    ParticleId particle_make()
    {
        return _particles.next();
    }

    void particle_free(const ParticleId id)
    {
        Particle& particle = _particles[id];

        glDeleteBuffers(1, &particle.ssbo_simulation);
        glDeleteBuffers(1, &particle.ssbo_geometry);
        glDeleteBuffers(1, &particle.ebo);
        glDeleteVertexArrays(1, &particle.vao);

        _particles.free(id);
    }

    bool particle_finished(const ParticleId id)
    {
        Particle& particle = _particles[id];
        return particle.one_shot && particle.time_left == 0.0f;
    }

    void particle_set_data(const ParticleId id, ParticleData data)
    {
        Particle& particle = _particles[id];
        particle.accumulator = 0.0f;
        particle.time_left = data.lifetime * (data.explode ? 1.0f : 2.0f);
        particle.fps = data.fps;
        particle.count = data.count;
        particle.one_shot = data.one_shot;

        if (particle.ssbo_geometry == INVALID_IDX)
        {
            glCreateBuffers(1, &particle.ssbo_simulation);
            glCreateBuffers(1, &particle.ssbo_geometry);
            glCreateBuffers(1, &particle.ebo);

            glCreateVertexArrays(1, &particle.vao);

            glEnableVertexArrayAttrib(particle.vao, 0);
            glVertexArrayAttribFormat(particle.vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
            glVertexArrayAttribBinding(particle.vao, 0, 0);
            glVertexArrayVertexBuffer(particle.vao, 0, particle.ssbo_geometry, offsetof(ParticleVert, pos), sizeof(ParticleVert));

            glEnableVertexArrayAttrib(particle.vao, 1);
            glVertexArrayAttribFormat(particle.vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
            glVertexArrayAttribBinding(particle.vao, 1, 1);
            glVertexArrayVertexBuffer(particle.vao, 1, particle.ssbo_geometry, offsetof(ParticleVert, uv), sizeof(ParticleVert));

            glEnableVertexArrayAttrib(particle.vao, 2);
            glVertexArrayAttribFormat(particle.vao, 2, 4, GL_FLOAT, GL_FALSE, 0);
            glVertexArrayAttribBinding(particle.vao, 2, 2);
            glVertexArrayVertexBuffer(particle.vao, 2, particle.ssbo_geometry, offsetof(ParticleVert, col), sizeof(ParticleVert));

            glVertexArrayElementBuffer(particle.vao, particle.ebo);
        }

        { // ebo
            std::vector<uint32_t> indices;
            indices.resize(data.count * 6);

            for (uint32_t i = 0; i < data.count; ++i)
            {
                const uint32_t idx = i * 6;
                const uint32_t base_idx = i * 4;
                indices[idx + 0] = base_idx + 0;
                indices[idx + 1] = base_idx + 1;
                indices[idx + 2] = base_idx + 2;
                indices[idx + 3] = base_idx + 2;
                indices[idx + 4] = base_idx + 3;
                indices[idx + 5] = base_idx + 0;
            }

            glNamedBufferData(particle.ebo, 24 * data.count, indices.data(), GL_STATIC_DRAW);
        }

        { // ssbo_simulation
            glNamedBufferData(particle.ssbo_simulation, sizeof(ParticleTemplate) + sizeof(ParticleState) * data.count, nullptr, GL_DYNAMIC_COPY);

            ParticleTemplate tmp;
            tmp.count = data.count;
            tmp.lifetime = data.lifetime;
            tmp.scale = data.scale;
            tmp.gravity_dir = math::angle(data.gravity_dir);
            tmp.gravity = data.gravity;
            tmp.velocity_dir = math::angle(data.dir);
            tmp.velocity = data.velocity;
            tmp.acceleration = data.acceleration;
            tmp.angular_velocity = data.angular_velocity;
            tmp.angular_acceleration = data.angular_acceleration;
            tmp.rand_dir = data.rand_dir;
            tmp.rand_lifetime = data.rand_lifetime;
            tmp.rand_velocity = data.rand_velocity;
            tmp.rand_angular_velocity = data.rand_angular_velocity;
            tmp.rand_scale = data.rand_scale;
            tmp.one_shot = !!data.one_shot;
            tmp.col_start = data.col_start;
            tmp.col_end = data.col_end;

            std::vector<ParticleState> states;
            states.resize(data.count);

            if (!data.explode)
            {
                for (uint32_t i = 0; i < data.count; ++i)
                {
                    states[i].age = i * (1.0f / data.count) * -data.lifetime;
                }
            }

            glNamedBufferSubData(particle.ssbo_simulation, 0, sizeof(ParticleTemplate), &tmp);
            glNamedBufferSubData(particle.ssbo_simulation, sizeof(ParticleTemplate), sizeof(ParticleState) * data.count, states.data());
        }

        glNamedBufferData(particle.ssbo_geometry, sizeof(ParticleVert) * data.count * 4, nullptr, GL_DYNAMIC_DRAW);
    }
}
