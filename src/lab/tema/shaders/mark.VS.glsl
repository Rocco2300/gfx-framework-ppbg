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
uniform vec2 markPos;
uniform sampler2D noiseTex;
uniform vec3 heliPos;

out vec3 color;

void main()
{
    vec3 initialPos = v_position;
    vec4 position = vec4(v_position, 1.0);
    position.x += markPos.x;
    position.z += markPos.y;
    if (v_color.r == 0.89) {
        position.y += sin(time * 2) * 0.125;
    }

    vec2 uv;
    if (v_color.r == 0.89) {
        uv = position.xz - initialPos.xz;
    } else {
        uv = position.xz;
    }
    uv = (uv + 50.0) / 100.0;

    float height = texture(noiseTex, uv).r * 6.0;
    position.y += height + 0.15;

    vec3 hPos = heliPos;
    hPos.y += height + 2.0;
    position.y = position.y - abs(dot(hPos - vec3(position), hPos - vec3(position))) * 0.01;

    position = Model * position;

    gl_Position = Projection * View * position;

    color = v_color;
}
