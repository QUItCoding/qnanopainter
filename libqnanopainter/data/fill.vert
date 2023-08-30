#version 440

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 tcoord;
layout(location = 0) out vec2 ftcoord;
layout(location = 1) out vec2 fpos;

layout(std140, binding = 0) uniform vertUBuf {
    vec2 viewSize;
    int ndcIsYDown;
};

void main()
{
    ftcoord = tcoord;
    fpos = vertex;
    if (ndcIsYDown != 0)
        gl_Position = vec4(2.0*vertex.x/viewSize.x - 1.0, -1.0 + 2.0*vertex.y/viewSize.y, 0, 1);
    else
        gl_Position = vec4(2.0*vertex.x/viewSize.x - 1.0, 1.0 - 2.0*vertex.y/viewSize.y, 0, 1);
}
