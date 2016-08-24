#version 400 core
layout (location = 0) in vec3 position;
out vec3 TexCoords;

uniform mat4 view, projection;


void main()
{
    gl_Position =   projection * view * vec4(position, 1.0);

//    gl_TexCoord[0].xyz = gl_Vertex.xyz;
    TexCoords = position;
}
