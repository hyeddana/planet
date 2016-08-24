#version 400 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;

void main()
{
    color = texture(skybox, TexCoords);
//    color = vec4(255.f, 0.f, 0.f, 1.f);
}
