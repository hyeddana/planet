#version 400 core
in vec2 uv;
in vec3 WS_position; // world-space position
in vec3 WS_normal;   // world-space normal
in float height;

out vec3 fragColor;

uniform vec3 viewPos;
uniform vec3 light;

uniform bool useTerrainText;

uniform sampler2D deepCrevice;
uniform sampler2D sand;
uniform sampler2D grass;
uniform sampler2D concrete;
uniform sampler2D snow;

uniform float minHeight, maxHeight, heightSplit;


void main(){
    /* texture mapping */;
    float currWeight = 0.f;
    float regionMax = 0.f;
    float regionRange = heightSplit;

    /* variables for lighting calculations */
    vec4 diffuse_color = vec4(0.0f, 0.0f, 0.0f, 0.0);
    vec3 specular_color = vec3(0.1f);
    vec3 ambient = vec3(0.1f);
    float shininess = 0.3;

    vec3 lightDir = normalize(light - WS_position);
    vec3 normal = WS_normal;
    vec3 viewDir = normalize(viewPos - WS_position);

    if (useTerrainText) {
        /* calculate diffuse color */
        // Deep crevice
        regionRange = heightSplit;
        regionMax = minHeight + heightSplit;
        currWeight = clamp((regionRange - abs(height - regionMax)) / regionRange, 0.f, 1.f);
        diffuse_color += currWeight * texture(deepCrevice, uv);

        // Sand
        //    regionRange = (abs(minHeight + heightSplit * 2.25)) - minHeight + heightSplit;
        regionMax = minHeight + heightSplit * 2.15;
        currWeight = clamp((regionRange - abs(height - regionMax)) / regionRange, 0.f, 1.f);
        diffuse_color += currWeight * texture(sand, uv);

        // Grass
        regionRange = (abs(minHeight + heightSplit * 3.2)) - (abs(minHeight + heightSplit * 2.15));
        regionMax = minHeight + heightSplit * 3.2;
        currWeight = clamp((regionRange - abs(height - regionMax)) / regionRange, 0.f, 1.f);
        diffuse_color += currWeight * texture(grass, uv);

        // Concrete/mountain
        regionRange = (abs(minHeight + heightSplit * 3.8)) - (abs(minHeight + heightSplit * 3.2));
        regionMax = minHeight + heightSplit * 3.8;
        currWeight = clamp((regionRange - abs(height - regionMax)) / regionRange, 0.f, 1.f);
        diffuse_color += currWeight * texture(concrete, uv);

        // Snow
        regionRange = maxHeight - (abs(minHeight + heightSplit * 3.8));
        regionMax = maxHeight;
        currWeight = clamp((regionRange - abs(height - regionMax)) / regionRange, 0.f, 1.f);
        diffuse_color += currWeight * texture(snow, uv);

        diffuse_color *= max(0.0, dot(normalize(normal), lightDir));
    }
    else {
        diffuse_color = vec4(0.4f);
        diffuse_color *= max(0.0, dot(normalize(normal), lightDir));
    }

    // specular component
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = specular_color * spec;

    fragColor = ambient + diffuse_color.rgb + specular;
}
