layout (location = 0) out vec4 FragColor;

in vec2 vert_uv;
in vec4 vert_col;

void main()
{
    FragColor = vert_col;
}
