#version 330

layout(location = 0) out vec4 out_color;

in float depth;
in vec2 tex_coord;

uniform sampler2D barkTex;
uniform sampler2D leavesTex;

void main()
{
    //out_color = vec4(tex_coord.x, tex_coord.y, 0.2, 1.0);
    float selector = max(0, depth - 1);
    selector /= 4.0;

    vec4 barkCol = texture(barkTex, tex_coord);
    vec4 leavesCol = texture(leavesTex, tex_coord);

    vec4 color = mix(barkCol, leavesCol, selector);
    out_color = color;
    //out_color = vec4(depth / 6, depth / 6, depth / 6, 1.0);
}
