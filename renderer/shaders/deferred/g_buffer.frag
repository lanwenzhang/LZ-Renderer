#version 460 core

in vec2 uv;
in vec3 worldPosition;
in mat3 tbn;

out vec3 gPosition;
out vec3 gNormal;
out vec4 gAlbedoSpec;

uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D normalTex;

void main()
{    
    
    gPosition = worldPosition;
    
    vec3 normal = texture(normalTex, uv).rgb;
    gNormal = normalize(tbn * normal);
  
    gAlbedoSpec.rgb = texture(diffuse, uv).rgb;

    gAlbedoSpec.a = texture(specular, uv).r;
//    FragColor = vec4(texture(diffuse, uv).rgb, 1.0);
}