#version 330

// Input
in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform sampler2D texture_3;
uniform sampler2D texture_4;
// TODO(student): Declare various other uniforms

// Output
layout(location = 0) out vec4 out_color;

float map(float value, float min1, float max1, float min2, float max2) {
    return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main()
{
    // TODO(student): Calculate the out_color using the texture() function.
    vec4 texture1Color = texture(texture_1, texture_coord);
    vec4 texture2Color = texture(texture_2, texture_coord);
    texture2Color.g = texture2Color.r;
    texture2Color.b = texture2Color.r;

    vec4 color;
    float snowInterpolation = (texture2Color.r > 0.5) ?
        1.0 :
        map(max(texture2Color.r, 0.4), 0.4, 0.5, 0.0, 1.0);
    vec4 snowColor = texture(texture_3, texture_coord);
    color = mix(texture1Color, snowColor, snowInterpolation);

    float waterInterpolation = (texture2Color.r < 0.05) ?
            1.0 :
            map(min(texture2Color.r, 0.1), 0.05, 0.1, 1.0, 0.0);
    vec4 waterColor = texture(texture_4, texture_coord);
    color = mix(color, waterColor, waterInterpolation);

    // TODO(student): Use the "discard" directive to terminate execution
    // based on the value of the alpha channel
    if (texture1Color.a < 0.5) {
        discard;
    }

    out_color = color;
}
