#version 330

layout(location = 0) out vec4 out_color;

in vec3 pos;

void main()
{
    vec4 color = vec4(pos, 1.0);
    out_color = color;
}
