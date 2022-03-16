layout (location = 0) out vec4 FragColor;

void main()
{
    float brightness = (cos(_TIME) + 1.0f) / 2.0f;
    FragColor = vec4(brightness, brightness, brightness, 1.0f);
}
