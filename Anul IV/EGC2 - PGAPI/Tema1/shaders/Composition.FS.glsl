#version 430

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_position;
uniform sampler2D texture_normal;
uniform sampler2D texture_color;
uniform sampler2D texture_depth;
uniform sampler2D texture_light;

uniform int output_type;

// Output
layout(location = 0) out vec4 out_color;


vec3 depth()
{
    float t2 = pow(texture(texture_depth, texture_coord).x, 256);
    return vec3(t2, t2, t2);
}


vec3 color()
{
    return texture(texture_color, texture_coord).xyz;
}


vec3 world_normal()
{
    return texture(texture_normal, texture_coord).xyz;
}


vec3 world_position()
{
    return texture(texture_position, texture_coord).xyz;
}


vec3 light_accumulation()
{
    return texture(texture_light, texture_coord).xyz;
}

bool isStar(vec3 position, vec3 normal) {
    // O condiție personalizată pentru identificarea stelelor
    return position.y > 10.0 && length(normal) > 0.99; // Exemplu: y mare și normal aproape unit

}

void main()
{
    switch (output_type)
    {
    case 1:
        out_color = vec4(color(), 1);
        break;

    case 2:
        out_color = vec4(depth(), 1);
        break;

    case 3:
        out_color = vec4(world_position(), 1);
        break;

    case 4:
        out_color = vec4(world_normal(), 1);
        break;

    case 5:
        out_color = vec4(light_accumulation(), 1);
        break;

    case 6:
        // TODO(student): Multiply the light accumulation sample and the color sample to compute final pixel color.
        out_color = vec4(color() * light_accumulation(), 1);
        break;

    case 7:
        vec3 lightEffect = light_accumulation(); // Lumină acumulată
        vec3 baseColor = color();               // Culoarea normală
        vec3 normalEffect = world_normal();     // Normala lumii

        if (isStar(world_position(), world_normal())) {
            // Efect special pentru stele
            vec3 starColor = vec3(1.0, 1.0, 1.0); // Alb intens
            out_color = vec4(starColor * lightEffect * 2.0, 1.0); // Amplificăm lumina pentru stele
        }
        else {
            // Logica standard pentru alte obiecte
            out_color = vec4(normalEffect * baseColor * lightEffect, 1.0);
        }
        break;

    default:
        out_color = vec4(color(), 1);
    }
}
