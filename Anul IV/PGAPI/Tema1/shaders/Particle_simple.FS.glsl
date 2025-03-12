#version 430

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_1;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    // Extragem culoarea și transparența (alpha)
    vec4 texColor = texture(texture_1, texture_coord);

    // Dacă alpha este mic, eliminăm fragmentul
    if (texColor.a >= 0.97f) {
        discard;
    }

    // Aplicăm culoarea texturii
    out_color = texColor + 0.2f;
}
