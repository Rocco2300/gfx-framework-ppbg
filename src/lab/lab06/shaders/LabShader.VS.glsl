#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float time;

// Output
// TODO(student): Output values to fragment shader
out vec3 oColor;

void main()
{
    // TODO(student): Send output to fragment shader
    oColor = color;

    mat4 scale = mat4(1);
    scale[0][0] = scale[0][0] * sin(time);
    scale[1][1] = scale[1][1] * sin(time);
    scale[2][2] = scale[2][2] * sin(time);

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * scale * vec4(position, 1.0);
}