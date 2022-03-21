#version 330 core

in vec2 UV;
uniform sampler2D colorTexture;
uniform sampler2D bloomTexture;

uniform float gamma;
uniform float exposition;
uniform float bloom;

uniform int width, height;

/*
float offset_x = 1.f / width;
float offset_y = 1.f / height;

vec3 makebloom() {
    float[9] kernel = float[]
    (
        1, 1, 1,
        1, 1, 1,
        1, 1, 1
    );

    vec3 color = vec3(0.f);
    int i = 0;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            color += vec3(texture(bloomTexture, UV + vec2(x*offset_x, y*offset_y)) * kernel[i]);
            i++;
        }
    }
    color /= 9;

    return color;
}*/

void main() {

    vec3 screenColor = vec3(texture(colorTexture, UV));
    vec3 bloomColor = vec3(texture(bloomTexture, UV));

    vec3 mix = screenColor + bloomColor * bloom;

    //vec3 mapped = mix / (mix + vec3(1.f));
    vec3 mapped = vec3(1.0) - exp(-mix * exposition);

    vec3 gamma_correction = pow(mapped, vec3(1.f/gamma));

    gl_FragColor = vec4(gamma_correction, 1);
}
