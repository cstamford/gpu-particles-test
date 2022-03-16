#pragma once

#include <borealis/gfx/Gfx.hpp>
#include <borealis/gfx/Shader.hpp>
#include <borealis/gfx/impl/RenderData.hpp>
#include <borealis/gfx/impl/Uniforms.hpp>
#include <borealis/util/Math.hpp>

namespace br::gfx::impl
{
    struct Camera
    {
        math::Mat3 transform;
    };

    struct Canvas
    {
        CameraId camera;
        uint32_t width = INVALID_IDX;
        uint32_t height = INVALID_IDX;
    };

    struct Material
    {
        ShaderId shaders[ShaderTypeCount];
        GLuint program = INVALID_IDX;
    };

    struct Mesh
    {
        GLuint vao = INVALID_IDX;
        GLuint vbo = INVALID_IDX;
        GLuint ebo = INVALID_IDX;
    };

    struct Particle
    {
        GLuint ssbo_simulation = INVALID_IDX;
        GLuint ssbo_geometry = INVALID_IDX;
        GLuint vao = INVALID_IDX;
        GLuint ebo = INVALID_IDX;

        float accumulator;
        float time_left;
        float fps;
        uint32_t count;
        bool one_shot;
        bool fade_out;
    };

    struct Shader
    {
        ShaderType type;
        GLuint shader = INVALID_IDX;
    };

    extern RenderData<Camera, CameraId> _cameras;
    extern RenderData<Canvas, CanvasId> _canvases;
    extern RenderData<Material, MaterialId> _materials;
    extern RenderData<Mesh, MeshId> _meshes;
    extern RenderData<Shader, ShaderId> _shaders;
    extern RenderData<Particle, ParticleId> _particles;

    extern Uniforms _uniforms;

    extern MaterialId _particle_compute_material;
    extern MaterialId _particle_draw_material;

    extern std::vector<std::pair<ShaderId, const char*>> _shader_reloads;
}
