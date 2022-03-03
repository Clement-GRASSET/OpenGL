#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 position = aPos + aNormals*-0.01;
    gl_Position = projection * view * model * vec4(position, 1.0);
}
