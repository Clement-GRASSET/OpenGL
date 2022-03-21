#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

uniform int width, height;
in vec3 color;
in vec3 normals;
in vec2 UV;
uniform vec3 viewPos;
in vec3 fragPos;

struct ambiantLight
{
    vec3 Color;
};

struct directionalLight
{
    vec3 Color;
    vec3 Direction;
};

uniform int nbAmbiantLights;
uniform ambiantLight ambiantLights[64];

uniform int nbDirectionalLights;
uniform directionalLight directionalLights[64];


void main()
{
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 illumination = vec3(0);
    vec3 specular = vec3(0);

    for (int i = 0; i < nbAmbiantLights; i++) {
        illumination += ambiantLights[i].Color;
    }

    for (int i = 0; i < nbDirectionalLights; i++) {
        illumination += max(dot(-directionalLights[i].Direction, normals), 0) * directionalLights[i].Color;

        vec3 reflectDir = reflect(directionalLights[i].Direction, normals);
        specular += pow(max(dot(viewDir, reflectDir), 0.0), 1024) * directionalLights[i].Color;
    }

    vec4 colorTexture = vec4(0.4, 0.4, 0.4, 1);
    vec4 specularTexture = vec4(1);


    FragColor = vec4(
        vec3(colorTexture.rgb) * illumination + vec3(specularTexture.rgb) * specular,
        1
    );

    float brightness = dot(FragColor.rgb, vec3(.2126f, .7152f, .0722f));
    // BloomColor = (brightness > 0.5f) ? FragColor : vec4(0,0,0,1);

    BloomColor = vec4(vec3(
        pow(max(
            brightness - 0.3
        ,0), 3)
    ),1) * FragColor;
}
