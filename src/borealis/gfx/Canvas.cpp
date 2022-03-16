#include <borealis/gfx/Canvas.hpp>
#include <borealis/gfx/Particle.hpp>
#include <borealis/gfx/impl/State.hpp>

using namespace br::gfx::impl;

namespace br::gfx
{
    CanvasId canvas_make()
    {
        return _canvases.next();
    }

    void canvas_begin(const CanvasId id)
    {
        glViewport(0, 0, _canvases[id].width, _canvases[id].height);

        const Canvas& canvas = _canvases[id];
        _uniforms.canvas.view = std140::pack(_cameras[canvas.camera].transform);
        _uniforms.canvas.width = canvas.width;
        _uniforms.canvas.height = canvas.height;
        glNamedBufferSubData(_uniforms.canvas_ubo, 0, sizeof(Uniforms::Canvas), &_uniforms.canvas);

        glBindBufferBase(GL_UNIFORM_BUFFER, Uniforms::Canvas::BINDING, _uniforms.canvas_ubo);
    }

    void canvas_clear(const CanvasId id, const Colour colour)
    {
        glClearColor(colour.r, colour.g, colour.b, colour.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void canvas_draw(const MaterialId material_id, const MeshId mesh_id, math::Mat3Ref transform)
    {
        const Material& material = _materials[material_id];
        glUseProgram(material.program);
        glProgramUniformMatrix3fv(material.program, glGetUniformLocation(material.program, "_model"), 1, false, transform.t);
        glBindVertexArray(_meshes[mesh_id].vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void canvas_draw_particle(const ParticleId particle_id, math::Mat3Ref transform)
    {
        const Particle& particle = _particles[particle_id];
        const Material& material = _materials[_particle_draw_material];

        if (!particle_finished(particle_id))
        {
            glUseProgram(material.program);
            glProgramUniformMatrix3fv(material.program, glGetUniformLocation(material.program, "_model"), 1, false, transform.t);
            glBindVertexArray(particle.vao);
            glDrawElements(GL_TRIANGLES, particle.count * 6, GL_UNSIGNED_INT, nullptr);
        }
    }

    void canvas_set_camera(const CanvasId id, const CameraId camera)
    {
        _canvases[id].camera = camera;
    }

    void canvas_set_size(const CanvasId id, const uint32_t width, const uint32_t height)
    {
        _canvases[id].width = width;
        _canvases[id].height = height;
    }
}
