#version 400 core

in vec3 WS_position; // world-space position
in vec3 WS_normal;   // world-space normal
in vec2 TexCoords;
in mat3 TBN; // for normal mapping

in vec3 fragPos;

uniform sampler2D normalMap;
uniform sampler2D diffuseMap;

uniform mat4 model, view, projection;

uniform vec3 viewPos;
uniform vec3 diffuse_color;
uniform vec3 ambient_color;
uniform vec3 specular_color;
uniform vec3 light;
uniform float shininess;

uniform bool useNormalMap;
uniform bool useDiffuseMap;

out vec3 fragColor;


void main(){
    vec3 lightDir = normalize(light - WS_position);
    vec3 normal = WS_normal;
    vec3 viewDir = normalize(viewPos - WS_position);

    vec3 TangentLightPos = TBN * light;
    vec3 TangentViewPos = TBN * viewPos;
    vec3 TangentFragPos = TBN * fragPos;

    if (useNormalMap) {
        normal = texture(normalMap, TexCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);

        // Normal diffuse
        lightDir = normalize(TangentLightPos - TangentFragPos);
        viewDir = normalize(TangentViewPos - TangentFragPos);
    }

    // ambient component
    // ignore ambient_color for now...
    vec3 ambient = vec3(0.1f);

    // diffuse component
    vec3 diffuse;
    if (useDiffuseMap) {
        diffuse = texture(diffuseMap, TexCoords).rgb * max(0.0, dot(normalize(normal), lightDir));
    } else {
        diffuse = diffuse_color * max(0.0, dot(normalize(normal), lightDir));
    }

    // specular component

    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = specular_color * spec;

    fragColor = ambient + diffuse + specular;
}
