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

uniform sampler2D texture_2;

uniform float time;

// Output
out vec2 texture_coord;


void main()
{
    // TODO(student): Pass v_texture_coord as output to fragment shader
    texture_coord = v_texture_coord;

    vec4 height = texture(texture_2, texture_coord);
    vec3 position = v_position;
    position.y += height.x * 10;

    gl_Position = Projection * View * Model * vec4(position, 1.0);
}
