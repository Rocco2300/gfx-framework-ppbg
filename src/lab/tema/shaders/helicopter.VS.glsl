#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 rotation;
uniform mat4 translation;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;
uniform sampler2D noiseTex;
uniform vec3 heliPos;

out vec4 color;

#define M_PI 3.1415926535897932384626433832795

mat4 translate(vec3 offset) {
    mat4 res = mat4(1.0);
    res[3][0] = offset.x;
    res[3][1] = offset.y;
    res[3][2] = offset.z;
    return res;
}

mat4 rotationOX(float angleDeg) {
    mat4 res = mat4(1.0);

    float angle = angleDeg * (M_PI / 180);
    res[1][1] = cos(angle);
    res[2][1] = -sin(angle);
    res[1][2] = sin(angle);
    res[2][2] = cos(angle);

    return res;
}

mat4 rotationOY(float angleDeg) {
    mat4 res = mat4(1.0);

    float angle = angleDeg * (M_PI / 180);
    res[0][0] = cos(angle);
    res[2][0] = sin(angle);
    res[0][2] = -sin(angle);
    res[2][2] = cos(angle);

    return res;
}

void main()
{
    mat4 Model = translation * rotation;

    vec4 originalPos = vec4(v_position, 1.0);
    vec4 position = vec4(v_position, 1.0);

    if (v_color.b == 0.3) {
        if (v_color.r == 0.3) {
            mat4 origin = translate(vec3(0, -0.575, 0));
            mat4 restore = translate(vec3(0, 0.575, 0));
            mat4 rot = rotationOY(time * 1000.0);

            position = restore * rot * origin * position;
            originalPos = restore * rot * origin * originalPos;
        }

        if (v_color.r == 0.29) {
            mat4 origin = translate(vec3(0, 0, -3));
            mat4 restore = translate(vec3(0, 0, 3));
            mat4 rot = rotationOX(time * -1000.0);

            position = restore * rot * origin * position;
            originalPos = restore * rot * origin * originalPos;
        }
    }

    position = Model * position;
    originalPos = rotation * originalPos;

    vec2 uv = ((position.xz - originalPos.xz) + 50.0) / 100.0;
    float height = texture(noiseTex, uv).r * 6.0;
    position.y += height + 2.0;

    vec3 hPos = heliPos;
    hPos.y += height + 2.0;
    position.y = position.y - abs(dot(hPos - vec3(position), hPos - vec3(position))) * 0.01;

    gl_Position = Projection * View * position;

    color = vec4(v_color, 1.0);
}
