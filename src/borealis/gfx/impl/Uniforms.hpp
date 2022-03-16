#pragma once

#include <borealis/util/Math.hpp>
#include <glad/glad.h>

namespace br::gfx::impl
{
    namespace std140
    {
        struct Mat3
        {
            float t[12];
        };

        inline Mat3 pack(math::Mat3Ref __restrict mat)
        {
            return { mat.t[0], mat.t[1], mat.t[2], 0.0f, mat.t[3], mat.t[4], mat.t[5], 0.0f, mat.t[6], mat.t[7], mat.t[8], 0.0f };
        }
    };

    struct Uniforms
    {
        struct Global
        {
            float time;
            float frame_time;

            static constexpr int BINDING = 0;
            static constexpr const char* GLSL =
            R"(
                layout (std140, binding = 0) uniform Global
                {
                    float _TIME;
                    float _FRAME_TIME;
                };

                uint hash(uint x)
                {
                    x += (x << 10u);
                    x ^= (x >> 6u);
                    x += (x << 3u);
                    x ^= (x >> 11u);
                    x += (x << 15u);
                    return x;
                }

                uint hash(uvec2 v) { return hash(v.x ^ hash(v.y)); }
                uint hash(uvec3 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z)); }
                uint hash(uvec4 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w)); }

                float _rand(uint m)
                {
                    const uint flt_mantissa = 0x007FFFFFu;
                    const uint flt_one = 0x3F800000u;

                    m &= flt_mantissa;
                    m |= flt_one;

                    const float f = uintBitsToFloat(m);
                    return f - 1.0;
                }

                float rand(float x) { return _rand(hash(floatBitsToUint(x))); }
                float rand(vec2 v) { return _rand(hash(floatBitsToUint(v))); }
                float rand(vec3 v) { return _rand(hash(floatBitsToUint(v))); }
                float rand(vec4 v) { return _rand(hash(floatBitsToUint(v))); }
            )";
        } global;

        GLuint global_ubo = INVALID_IDX;

        struct Canvas
        {
            std140::Mat3 view;
            uint32_t width;
            uint32_t height;

            static constexpr int BINDING = 1;
            static constexpr const char* GLSL =
            R"(
                layout (std140, binding = 1) uniform Canvas
                {
                    mat3 _VIEW;
                    uint _WIDTH;
                    uint _HEIGHT;
                };
            )";
        } canvas;

        GLuint canvas_ubo = INVALID_IDX;
    };
}
