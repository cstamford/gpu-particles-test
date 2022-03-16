#include <borealis/gfx/Material.hpp>
#include <borealis/gfx/impl/State.hpp>

using namespace br::gfx::impl;

namespace br::gfx
{
    MaterialId material_make()
    {
        return _materials.next();
    }

    void material_free(const MaterialId id)
    {
        glDeleteProgram(_materials[id].program);
        _materials.free(id);
    }

    void material_set_shader(const MaterialId id, const ShaderId shader_id)
    {
        Material& mat = _materials[id];
        const Shader& shader = _shaders[shader_id];
        mat.shaders[shader.type] = shader_id;
    }

    void material_link(const MaterialId id)
    {
        Material& mat = _materials[id];

        if (mat.program == INVALID_IDX)
        {
            mat.program = glCreateProgram();
        }

        for (const ShaderId& shader : mat.shaders)
        {
            if (shader.idx != INVALID_IDX)
            {
                glAttachShader(mat.program, _shaders[shader].shader);
            }
        }

        glLinkProgram(mat.program);

        for (const ShaderId& shader : mat.shaders)
        {
            if (shader.idx != INVALID_IDX)
            {
                glDetachShader(mat.program, _shaders[shader].shader);
            }
        }

        GLint success;
        glGetProgramiv(mat.program, GL_LINK_STATUS, &success);

        if (!success)
        {
            char error[2048];
            glGetProgramInfoLog(mat.program, sizeof(error), nullptr, error);
            glDeleteProgram(mat.program);
            mat.program = INVALID_IDX;
            BR_TRACE(error);
            BR_ASSERT_FAIL();
        }
    }
}
