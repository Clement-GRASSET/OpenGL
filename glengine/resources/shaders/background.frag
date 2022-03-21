#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

uniform vec4 color;

void main() {
    FragColor = color;
    BloomColor = vec4(0,0,0,1);
}
