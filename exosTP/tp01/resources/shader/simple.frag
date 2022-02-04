#version 330 core

in vec3 color;
out vec4 FragColor;

void main()
{
    float edge = 0.05;
    float i = 1 - (step(edge, color.r) * step(edge, color.g) * step(edge, color.b));
    FragColor = vec4(i, i, i, 1.f);
}
