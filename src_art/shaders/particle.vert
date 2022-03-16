layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec4 col;

out vec2 vert_uv;
out vec4 vert_col;

uniform mat3 _model;

void main()
{
    vert_uv = uv;
    vert_col = col;
    gl_Position = vec4(_VIEW * _model * vec3(pos, 1.0f), 1.0f);
}
