#version 330 core

//uniform vec3 viewPos;
uniform sampler2D box2;
uniform sampler2D box2_diffus;
uniform sampler2D box2_diffus_mask;
uniform sampler2D box2_specular;
uniform sampler2D box2_specular_albedo;
uniform sampler2D box2_specular_mask;

uniform int width, height;
in vec3 color;
in vec3 normals;
in vec2 UV;
uniform vec3 viewPos;
in vec3 fragPos;
out vec4 FragColor;

vec3 ambiantColor = vec3(0.2f);
vec3 lightColor = vec3(1.2f);
vec3 lightDir = normalize(vec3(1.f, -1.f, 0.f));
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
    vec3 illumination = vec3(max(dot(-lightDir, normals), 0) * lightColor + ambiantColor);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(lightDir, normals);
    float phong = pow(max(dot(viewDir, reflectDir), 0.0), 16) * 0.5;

    vec3 celShading1 = step(0.8f, illumination);
    vec3 celShading2 = step(0.5f, illumination);
    vec3 celShading3 = step(0.1f, illumination);

    vec3 trame1 = (celShading2-celShading1) * tramage(0.8, 0.8);
    vec3 trame2 = (celShading3-celShading2) * tramage(0.8, 0.1);

    float discontinuites = step(0.8, discontinuites());

    //vec3 frag = vec3((trame1 + trame2 + celShading1) * discontinuites);

    vec4 color = texture(box2_diffus, UV) * texture(box2_diffus_mask, UV) * vec4(1,0.8,0.5,1) + texture(box2_specular_albedo, UV) * texture(box2_specular_mask, UV);
    vec4 specular = texture(box2_specular, UV);

    //color = vec4(0.5, 0.5, 0.5, 1);
    //specular = vec4(1);

    FragColor = vec4(illumination,1) * color * vec4(vec3(discontinuites), 1) + specular * vec4(vec3(phong), 1);
}
