#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 oColor;

// Output
layout(location = 0) out vec4 out_color;

uniform float time;

void main()
{
    // TODO(student): Write pixel out color
    vec4 final_color = vec4(oColor, 1.0);
    final_color.r = sin(time * 0.5) + 0.5;
    out_color = final_color;

}
