#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 color;
out vec3 normals;
out vec2 UV;
out vec3 fragPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    color = aPos;
    normals = aNormals;
    UV = aUV;
    fragPos = vec3(model * vec4(aPos, 1.0));
}