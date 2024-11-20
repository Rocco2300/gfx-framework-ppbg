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

// Output
// TODO(student): Output values to fragment shader
uniform float time;

out vec3 oColor;

void main()
{
    // TODO(student): Send output to fragment shader
    oColor = color;

    mat4 translateOY = mat4(1.0);
    translateOY[3][1] = sin(time);

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * translateOY * Model * vec4(position, 1.0);
}