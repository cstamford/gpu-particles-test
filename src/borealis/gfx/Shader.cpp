#include <borealis/gfx/Shader.hpp>
#include <borealis/gfx/impl/State.hpp>

using namespace br::gfx::impl;

namespace br::gfx
{
    ShaderId shader_make()
    {
        return _shaders.next();
    }

    void shader_free(const ShaderId id)
    {
        glDeleteShader(_shaders[id].shader);
        _shaders.free(id);
    }

    void shader_set_code(const ShaderId id, const ShaderType type, const char** src, const int* lens, const int count)
    {
        Shader& shader = _shaders[id];

        if (shader.shader == INVALID_IDX)
        {
            static constexpr GLuint SHADER_TYPE_MAP[] =
            {
                GL_VERTEX_SHADER,
                GL_FRAGMENT_SHADER,
                GL_COMPUTE_SHADER
            };

            shader.type = type;
            shader.shader = glCreateShader(SHADER_TYPE_MAP[type]);
        }

        BR_ASSERT(shader.type == type);

        glShaderSource(shader.shader, count, src, lens);
        glCompileShader(shader.shader);

        GLint success;
        glGetShaderiv(shader.shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            char error[2048];
            glGetShaderInfoLog(shader.shader, sizeof(error), nullptr, error);
            glDeleteShader(shader.shader);
            shader.shader = INVALID_IDX;
            BR_TRACE(error);
            BR_ASSERT_FAIL();
        }
    }

    void shader_set_code(const ShaderId id, const ShaderType type, const char* code, const char* header)
    {
        if (header == nullptr)
        {
            header = "";
        }

        const GLchar* src[] =
        {
            "#version 460 core",
            Uniforms::Global::GLSL,
            Uniforms::Canvas::GLSL,
            header,
            code
        };

        const GLint lens[] =
        {
            (GLint)std::strlen(src[0]),
            (GLint)std::strlen(src[1]),
            (GLint)std::strlen(src[2]),
            (GLint)std::strlen(src[3]),
            (GLint)std::strlen(src[4])
        };

        shader_set_code(id, type, src, lens, _countof(src));
    }

    void shader_set_reload_from_disk(const ShaderId id, const char* path)
    {
        _shader_reloads.emplace_back(id, path);
    }
}
