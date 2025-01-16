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
uniform float treeHeight;
uniform sampler2D noiseTex;
uniform vec3 heliPos;

#define M_PI 3.1415926535897932384626433832795

void main()
{
    vec4 originalPos = vec4(v_position, 1.0);
    vec4 position = Model * vec4(v_position, 1.0);

    vec2 uv = ((position.xz - originalPos.xz) + 50.0) / 100.0;
    float height = texture(noiseTex, uv).r * 6.0;
    position.y += height;

    vec3 hPos = heliPos;
    hPos.y += height + 2.0;
    position.y = position.y - abs(dot(hPos - vec3(position), hPos - vec3(position))) * 0.01;

    gl_Position = Projection * View * position;
}
