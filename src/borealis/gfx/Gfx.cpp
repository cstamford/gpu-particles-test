#include <borealis/gfx/Gfx.hpp>
#include <borealis/gfx/Material.hpp>
#include <borealis/gfx/impl/State.hpp>
#include <borealis/util/File.hpp>

using namespace br::gfx::impl;

namespace br::gfx
{
    RenderData<Camera, CameraId> impl::_cameras;
    RenderData<Canvas, CanvasId> impl::_canvases;
    RenderData<Material, MaterialId> impl::_materials;
    RenderData<Mesh, MeshId> impl::_meshes;
    RenderData<Shader, ShaderId> impl::_shaders;
    RenderData<Particle, ParticleId> impl::_particles;

    Uniforms impl::_uniforms;

    MaterialId impl::_particle_compute_material;
    MaterialId impl::_particle_draw_material;

    std::vector<std::pair<ShaderId, const char*>> impl::_shader_reloads;

    void init()
    {
#if defined(_DEBUG)
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, nullptr, GL_TRUE);

        glDebugMessageCallback([](GLenum, GLenum type, GLuint, GLenum severity,
            GLsizei, const GLchar* message, const void*)
        {
            BR_TRACE("GL CALLBACK: type = 0x%x, severity = 0x%x, message = %s\n", type, severity, message);
            BR_ASSERT(type != GL_DEBUG_TYPE_ERROR);
        }, nullptr);
#endif

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glCreateBuffers(1, &_uniforms.global_ubo);
        glCreateBuffers(1, &_uniforms.canvas_ubo);
        glObjectLabel(GL_BUFFER, _uniforms.global_ubo, -1, "Global");
        glObjectLabel(GL_BUFFER, _uniforms.canvas_ubo, -1, "Canvas");
        glNamedBufferData(_uniforms.global_ubo, sizeof(Uniforms::Global), nullptr, GL_DYNAMIC_DRAW);
        glNamedBufferData(_uniforms.canvas_ubo, sizeof(Uniforms::Canvas), nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, Uniforms::Global::BINDING, _uniforms.global_ubo);

        std::string particle_compute_shader = file::read_all_text("shaders/particle.compute");
        std::string particle_vtx_shader = file::read_all_text("shaders/particle.vert");
        std::string particle_frag_shader = file::read_all_text("shaders/particle.frag");

        ShaderId compute = shader_make();
        shader_set_code(compute, ShaderType::Compute, particle_compute_shader.c_str());

        _particle_compute_material = material_make();
        material_set_shader(_particle_compute_material, compute);
        material_link(_particle_compute_material);
        shader_free(compute);

        ShaderId draw_vtx = shader_make();
        shader_set_code(draw_vtx, ShaderType::Vertex, particle_vtx_shader.c_str());

        ShaderId draw_frag = shader_make();
        shader_set_code(draw_frag, ShaderType::Fragment, particle_frag_shader.c_str());

        _particle_draw_material = material_make();
        material_set_shader(_particle_draw_material, draw_vtx);
        material_set_shader(_particle_draw_material, draw_frag);
        material_link(_particle_draw_material);

        shader_free(draw_vtx);
        shader_free(draw_frag);
    }

    void free()
    {
        glDeleteBuffers(1, &_uniforms.global_ubo);
        glDeleteBuffers(1, &_uniforms.canvas_ubo);
    }

    void update(const float dt)
    {
        _uniforms.global.time += dt;
        _uniforms.global.frame_time = dt;
        glNamedBufferSubData(_uniforms.global_ubo, 0, sizeof(Uniforms::Global), &_uniforms.global);

        for (Particle& particle : _particles)
        {
            particle.accumulator += dt;

            if (particle.one_shot)
            {
                particle.time_left = std::max(0.0f, particle.time_left - dt);
                if (particle.time_left == 0.0f) continue;
            }

            const float step = 1.0f / particle.fps;
            const float step_count = particle.accumulator / step;

            if (step_count >= 1.0f)
            {
                const uint32_t step_count_u = (uint32_t)step_count;
                particle.accumulator -= step * step_count_u;

                const GLuint program = _materials[_particle_compute_material].program;
                glUseProgram(program);
                glProgramUniform1ui(program, glGetUniformLocation(program, "_steps"), step_count_u);
                glProgramUniform1f(program, glGetUniformLocation(program, "_steps_dt"), step);
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, particle.ssbo_simulation);
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, particle.ssbo_geometry);
                glDispatchCompute(particle.count, 1, 1);
                glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
            }
        }
    }

    void reload_shaders()
    {
        for (const auto& [id, path] : _shader_reloads)
        {
            shader_set_code(id, _shaders[id].type, file::read_all_text(path).c_str());
        }

        uint32_t idx = 0;

        for (const Material& material : _materials)
        {
            // TODO: Scales poorly. Need an unordered_set to hold the shader IDs we're reloading.
            bool needs_reload = std::any_of(std::begin(material.shaders), std::end(material.shaders),
                [](const ShaderId shader)
            {
                return std::any_of(std::begin(_shader_reloads), std::end(_shader_reloads),
                    [shader](const std::pair<ShaderId, const char*> pair)
                {
                    return shader.idx == pair.first.idx;
                });
            });

            if (needs_reload)
            {
                material_link({ idx });
            }

            ++idx;
        }
    }
}
