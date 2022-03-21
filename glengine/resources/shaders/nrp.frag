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

struct pointLight
{
    vec3 Color;
    vec3 Position;
    float Constant;
    float Linear;
    float Quadratic;
};

uniform int nbAmbiantLights;
uniform ambiantLight ambiantLights[64];

uniform int nbDirectionalLights;
uniform directionalLight directionalLights[64];

uniform int nbPointLights;
uniform pointLight pointLights[64];

float tramage(float size, float offset)
{
    return 1-step(offset, sin(gl_FragCoord.x*size) * sin(gl_FragCoord.y*size) );
}

float discontinuites()
{
    float x = 1-length(dFdx(normals));
    float y = 1-length(dFdy(normals));
    float distance = length(fragPos-viewPos);
    return clamp(pow(x*y, 1/distance), 0, 1);
}

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

    for (int i = 0; i < nbPointLights; i++) {
        vec3 lightDir = normalize(pointLights[i].Position - fragPos);
        // diffuse shading
        float diff = max(dot(normals, lightDir), 0.0);
        // specular shading
        vec3 reflectDir = reflect(-lightDir, normals);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1024);

        // attenuation
        float distance = length(pointLights[i].Position - fragPos);
        float attenuation = 1.0 / (pointLights[i].Constant + pointLights[i].Linear * distance + pointLights[i].Quadratic * (distance * distance));

        illumination += diff * pointLights[i].Color * attenuation;
        specular += spec * pointLights[i].Color * attenuation;
    }


    vec3 celShading1 = step(0.8f, illumination);
    vec3 celShading2 = step(0.5f, illumination) - celShading1;
    vec3 celShading3 = step(0.1f, illumination) - celShading2 - celShading1;

    vec3 trame1 = celShading2 * tramage(0.8, 0.8);
    vec3 trame2 = celShading3 * tramage(0.8, 0.1);

    float discontinuites = step(0.95, discontinuites());


    vec4 colorTexture = vec4(0.4, 0.4, 0.4, 1) * vec4(
        celShading1 * 0.8f + celShading2 * 0.5f * trame1 + celShading3 * 0.1f * trame2
    , 1);
    vec4 specularTexture = vec4(1);


    FragColor = vec4(
        vec3(colorTexture.rgb) * discontinuites + vec3(specularTexture.rgb) * specular,
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
