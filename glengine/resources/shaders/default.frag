#version 330 core

uniform vec3 viewPos;
in vec3 color;
in vec3 normals;
out vec4 FragColor;

vec3 ambiantColor = vec3(0.2f);
vec3 lightColor = vec3(1.2f);
float specularStrength = 0.5;

float tramage(float size, float offset)
{
    return 1-step(offset, sin(gl_FragCoord.x*size) * sin(gl_FragCoord.y*size) );
}

float discontinuites()
{
    float x = 1-length(dFdx(normals));
    float y = 1-length(dFdy(normals));
    return x*y;
}

void main()
{
    vec3 light = normalize(vec3(1.f, -1.f, 0.f));
    vec3 illumination = vec3(max(dot(light, normals), 0) * lightColor + ambiantColor);

    vec3 celShading1 = step(0.8f, illumination);
    vec3 celShading2 = step(0.5f, illumination);
    vec3 celShading3 = step(0.1f, illumination);

    vec3 trame1 = (celShading2-celShading1) * tramage(0.8, 0.8);
    vec3 trame2 = (celShading3-celShading2) * tramage(0.8, 0.1);

    float discontinuites = step(0.8, discontinuites());

    //vec3 frag = vec3((trame1 + trame2 + celShading1) * discontinuites);
    vec3 frag = illumination*discontinuites;
    FragColor = vec4(frag,1);
}
