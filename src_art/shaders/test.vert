layout (location = 0) in vec2 pos;

uniform mat3 _model;

void main()
{
    float scale = (cos(_TIME) + 1.0f) / 2.0f;
    gl_Position = vec4(_VIEW * _model * vec3(pos * scale, 1.0f), 1.0f);
}
