#version 430

// Input
layout(location = 0) in vec3 color;

// Output
layout(location = 0) out vec4 out_color;

void main() {
    out_color = vec4(0.0, 0.0, 1.0, 1.0);
}