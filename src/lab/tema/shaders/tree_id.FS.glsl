#version 330

layout(location = 0) out vec4 out_color;

uniform int treeId;

void main()
{
    out_color = vec4(float(treeId) / 255.0);
}
