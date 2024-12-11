#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float time;

// Output
out vec2 texture_coord;


void main()
{
    // TODO(student): Pass v_texture_coord as output to fragment shader
    texture_coord = v_texture_coord;

    int xPos = int(texture_coord.x * 10000);
    texture_coord.x = mod(texture_coord.x + time / 2.0, 1.0);

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
