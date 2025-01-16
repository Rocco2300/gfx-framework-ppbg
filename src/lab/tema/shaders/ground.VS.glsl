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
uniform sampler2D noiseTex;

uniform vec3 heliPos;

out vec2 tex_coord;
out float height;

void main()
{
    vec4 position = vec4(v_position, 1.0);
    position.y += texture(noiseTex, v_texture_coord).r * 6.0;

    position = Model * position;

    tex_coord = v_texture_coord;
    height = position.y;

    vec3 hPos = heliPos;
    hPos.y += height + 2.0;
    position.y = position.y - abs(dot(hPos - vec3(position), hPos - vec3(position))) * 0.01;

    gl_Position = Projection * View * position;
}
