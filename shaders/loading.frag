#version 400
in vec2 uv;
out vec4 color;
uniform sampler2D sampler;

void main(){
//    color = texture(sampler, uv);
    color = vec3(0.4, 0.0, 0.0, 1.0);
}
