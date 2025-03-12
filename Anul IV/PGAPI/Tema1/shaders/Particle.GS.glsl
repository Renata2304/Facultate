#version 430

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

// Uniforms
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 eye_position;
uniform float offset;

// Inputs din Vertex Shader
in float vert_lifetime[1];
in float vert_iLifetime[1];

// Outputs către Fragment Shader
layout(location = 0) out vec2 texture_coord;
layout(location = 1) out float geom_lifetime;
layout(location = 2) out float geom_iLifetime;

void EmitPoint(vec2 quadOffset, vec2 texCoord) {
    // Calcul poziția locală a particulei
    vec3 vpos = gl_in[0].gl_Position.xyz;

    // Calcul direcții pentru orientarea particulei
    vec3 forward = normalize(eye_position - vpos);
    vec3 right = normalize(cross(forward, vec3(0, 1, 0)));
    vec3 up = -normalize(cross(right, forward));

    // Poziționează vârful pe quad
    vec3 pos = vpos + right * quadOffset.x + up * quadOffset.y;
    gl_Position = Projection * View * vec4(pos, 1.0);

    // Setează coordonatele de textură
    texture_coord = texCoord;

    // Transmite informațiile despre viață
    geom_lifetime = vert_lifetime[0];
    geom_iLifetime = vert_iLifetime[0];

    EmitVertex();
}

void main() {
    // Setează dimensiunea quad-ului
    float halfSize = offset / 2.0;

    // Emit vertex-urile în ordinea corectă pentru triangle strip
    EmitPoint(vec2(-halfSize, -halfSize), vec2(0.0, 0.0)); // 0: Bottom-left
    EmitPoint(vec2(halfSize, -halfSize), vec2(1.0, 0.0));  // 1: Bottom-right
    EmitPoint(vec2(-halfSize, halfSize), vec2(0.0, 1.0));  // 2: Top-left
    EmitPoint(vec2(halfSize, halfSize), vec2(1.0, 1.0));   // 3: Top-right

    // Încheie primitiva
    EndPrimitive();
}
