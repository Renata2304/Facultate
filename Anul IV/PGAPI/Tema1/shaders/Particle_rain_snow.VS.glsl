#version 430

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform vec3 generator_position;
uniform float deltaTime;
uniform int pause;

out float vert_lifetime;
out float vert_iLifetime;

struct Particle {
    vec4 position;
    vec4 speed;
    vec4 iposition;
    vec4 ispeed;
    float delay;
    float iDelay;
    float lifetime;
    float iLifetime;
    float progress;
};

layout(std430, binding = 0) buffer particles {
    Particle data[];
};

// Bézier curve calculation
vec3 bezier_curve(vec3 p0, vec3 p1, vec3 p2, vec3 p3, float t) {
    return pow(1.0 - t, 3.0) * p0 +
        3.0 * pow(1.0 - t, 2.0) * t * p1 +
        3.0 * (1.0 - t) * t * t * p2 +
        t * t * t * p3;
}

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    float spread = 0.9f;
    vec3 pos = data[gl_VertexID].position.xyz;
    vec3 spd = data[gl_VertexID].speed.xyz;
    float delay = data[gl_VertexID].delay;

    // Reducem delay-ul pentru particulă
    delay -= deltaTime / 10;

    // Dacă delay-ul nu a expirat, particula rămâne pe loc
    if (delay <= 0) {
        data[gl_VertexID].delay = delay + rand(pos.xz);
        gl_Position = Model * vec4(generator_position, 1);
        return;
    }

    // Calculăm progresul particulei pe curbă (între 0 și 1)
    float progress = data[gl_VertexID].progress;
    progress += deltaTime * 0.1;
    if (progress > 1.0) {
        progress = 0.0;
    }
    data[gl_VertexID].progress = progress;

    // Definirea punctelor de control pentru curba Bezier
    vec3 p0 = vec3(-13.0, 9.1, 9.2);
    vec3 p1 = vec3(-11.0, 7.8, 8.2);
    vec3 p2 = vec3(-7.0, 1.2, 4.3);
    vec3 p3 = vec3(-4.7, 0.6, 3.0);

    // Calculăm poziția particulei pe curba Bezier
    pos = bezier_curve(p0, p1, p2, p3, progress);

    // Aplicăm o dispersie mai mare folosind spread
    pos.x += spread * (rand(vec2(gl_VertexID, progress)) - 0.5);

    // Mișcare verticală pe axa Z cu mai multă variație
    pos.z += spread * (rand(vec2(gl_VertexID, progress)) - 0.5);

    // Actualizează datele particulei
    data[gl_VertexID].position.xyz = pos;
    data[gl_VertexID].speed.xyz = spd;
    data[gl_VertexID].delay = delay;

    // Setează poziția finală
    gl_Position = Model * vec4(pos, 1);
}
