#version 330

layout(location = 0) out vec4 out_color;

in vec2 tex_coord;
in float height;

uniform sampler2D groundTex;
uniform sampler2D snowTex;

void main()
{
    vec4 snowCol = texture(snowTex, tex_coord);
    vec4 groundCol = texture(groundTex, tex_coord);

    //out_color = vec4(tex_coord.x, tex_coord.y, 0.2, 1.0);
    float t = clamp(height - 2.0, 0.0, 1.0);
    vec4 color = mix(groundCol, snowCol, t);
    out_color = color;
}
