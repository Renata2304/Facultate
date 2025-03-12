#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniform properties
uniform samplerCube texture_cubemap;
uniform int type;

uniform vec3 camera_position;

// Output
layout(location = 0) out vec4 out_color;


vec3 myReflect()
{
    // Calculate the view direction from the camera to the fragment
    vec3 viewDir = normalize(world_position - camera_position);

    // Calculate the reflection vector
    vec3 reflectDir = reflect(viewDir, normalize(world_normal));

    // Sample the cubemap using the reflection vector
    return texture(texture_cubemap, reflectDir).rgb;
}


vec3 myRefract(float refractive_index)
{
    // Calculate the view direction from the camera to the fragment
    vec3 viewDir = normalize(world_position - camera_position);

    // Calculate the refraction vector
    vec3 refractDir = refract(viewDir, normalize(world_normal), refractive_index);

    // Sample the cubemap using the refraction vector
    return texture(texture_cubemap, refractDir).rgb;
}




void main()
{

    if (type == 0)
    {
        out_color = vec4(myReflect(), 0);
    }
    if (type == 1)
    {
        out_color = vec4(myRefract(1 / 1.33), 0);
    }
    else if (type == 2)
    {
        out_color = (vec4(myReflect(), 0) + vec4(myRefract(1 / 1.33), 0)) / 2;
    }
    out_color = out_color * 4;
}
