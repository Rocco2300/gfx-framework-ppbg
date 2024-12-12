#version 430

// Input and output topologies
layout(triangles) in;
layout(triangle_strip, max_vertices = 170) out;

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
// TODO(student): Declare other uniforms here
uniform float time;

// Output
layout(location = 0) out vec3 color;

void EmitPoint(vec3 position, vec3 c)
{
    color = c;
    gl_Position = Projection * View * vec4(position, 1);
    EmitVertex();
}

void main()
{
    vec3 p[3];
    for (int i = 0; i < 3; i++) {
        p[i] = gl_in[i].gl_Position.xyz;
    }

    EmitPoint(p[0], vec3(1.0, 1.0, 0.0));
    EmitPoint(p[1], vec3(1.0, 1.0, 0.0));
    EmitPoint(p[2], vec3(1.0, 1.0, 0.0));
    EndPrimitive();

    // TODO(student): Emit the vertices of the triangle for which
    // information is received in the input attributes. Use EmitPoint()
    // and set a color information associated with each vertex.

    for (int i = 0; i < 3; i++) {
        vec3 pCurr = p[i];

        int prevIdx = int(mod(float(i) - 1.0, 3.0));
        int nextIdx = int(mod(float(i) + 1.0, 3.0));
        vec3 pPrev = p[prevIdx];
        vec3 pNext = p[nextIdx];

        EmitPoint(pCurr, vec3(1.0, 1.0, 0.0));
        EmitPoint(pPrev, vec3(1.0, 1.0, 0.0));
        if (i == 2) {
            pNext = p[0];
        }
        EmitPoint(pCurr + (pPrev - pNext), vec3(1.0, 0.0, 0.0));
        EndPrimitive();
    }
}
