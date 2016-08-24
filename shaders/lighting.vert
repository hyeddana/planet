#version 400 core

layout(location = 0) in vec3 OS_position;
layout(location = 1) in vec3 OS_normal;
layout(location = 2) in vec2 texCoords;

// Used for normal mapping
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4 model, view, projection;

out vec3 WS_position; // world-space position
out vec3 WS_normal;   // world-space normal
out vec2 TexCoords;
out vec3 fragPos;

out mat3 TBN; // Matrix for bitangent space


void main() {
    WS_position = (model * vec4(OS_position, 1.0)).xyz;
    WS_normal = (model * vec4(OS_normal, 0.0)).xyz;
    TexCoords = texCoords;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(vec3(normalMatrix * tangent));
    vec3 B = normalize(vec3(normalMatrix * bitangent));
    vec3 N = normalize(vec3(normalMatrix * OS_normal)); // could be WS_normal

    TBN = transpose(mat3(T, B, N));

    fragPos = vec3(model * vec4(OS_position, 1.0));
    gl_Position = projection * view * model * vec4(OS_position, 1.0);

}
