#version 400 core

layout(location = 0) in vec3 OS_position;
layout(location = 1) in vec3 OS_normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float inHeight;

uniform mat4 model, view, projection;

out vec2 uv;
out vec3 WS_position; // world-space position
out vec3 WS_normal;   // world-space normal
out float height;

void main() {
    uv = texCoord;
    height = inHeight;
    WS_position = (model * vec4(OS_position, 1.0)).xyz;
    WS_normal = (model * vec4(OS_normal, 0.0)).xyz;

    gl_Position = projection * view * vec4(WS_position, 1.0);
}
