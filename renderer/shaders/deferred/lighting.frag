#version 460 core

in vec2 uv;
out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform vec3 cameraPosition;

uniform int displayMode; // 0: Final, 1: Position, 2: Normal, 3: AlbedoSpec

#include "../common/light_struct.glsl"

uniform PointLight pointLights[32];

void main() {

    vec3 PositionMap = texture(gPosition, uv).rgb;
    vec3 normalMap = normalize(texture(gNormal, uv).rgb);
    vec3 diffuseMap = texture(gAlbedoSpec, uv).rgb;
    float specularMap = texture(gAlbedoSpec, uv).a;

    if (displayMode == 1) {
        FragColor = vec4(PositionMap * 0.5, 1.0); // scaled for visibility
        return;
    } else if (displayMode == 2) {
        FragColor = vec4(normalMap, 1.0); // normalized to 0~1
        return;
    } else if (displayMode == 3) {
        FragColor = vec4(diffuseMap, 1.0);
        return;
    }else if (displayMode == 4) {
        FragColor = vec4(specularMap, specularMap, specularMap, 1.0);
        return;
    }

    // Final result
    vec3 result = diffuseMap * 0.3;
    vec3 viewDir = normalize(cameraPosition - PositionMap);

    for (int i = 0; i < 32; i++) {
        vec3 lightDir = normalize(pointLights[i].position - PositionMap);
        vec3 diffuse = max(dot(normalMap, lightDir), 0.0) * diffuseMap * pointLights[i].color;

        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normalMap, halfwayDir), 0.0), 16.0);
        vec3 specular = pointLights[i].color * spec * specularMap;

        float dist = length(pointLights[i].position - PositionMap);
        float attenuation = 1.0 / (0.44 * dist * dist + 0.35 * dist + 1.0);

        diffuse *= attenuation;
        specular *= attenuation;

        result += diffuse + specular;
    }

    FragColor = vec4(result, 1.0);
}
