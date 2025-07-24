#version 460 core

out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;
in mat3 tbn;

uniform vec3 cameraPosition;

// material textures
uniform sampler2D albedoTex;
uniform sampler2D roughnessTex;
uniform sampler2D metallicTex;
uniform sampler2D normalTex;

// IBL maps
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

// Lighting
#include "../common/light_struct.glsl"
uniform PointLight pointLights[4];

#define PI 3.141592653589793

// NDF
float NDF_GGX(vec3 N, vec3 H, float roughness){

	float a = roughness * roughness;
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0);

	float num = a2;
	float denom = (NdotH * NdotH * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	denom = max(denom, 0.0001);

	return num / denom;

}

// Geometry Func
float GeometrySchlickGGX(float NdotV, float roughness){
	
	float r = (roughness + 1.0);
	float k = r * r / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	denom = max(denom, 0.00001);

	return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){

	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);

	float ggx1 = GeometrySchlickGGX(NdotV, roughness);
	float ggx2 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

// Fresnel
vec3 fresnelSchlick(vec3 F0, float HdotV){

	return F0 + (1.0 - F0) * pow((1.0 - HdotV), 5.0);

}

vec3 fresnelSchlickRoughness(vec3 F0, float NdotV, float roughness){

	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow((1.0 - NdotV), 5.0);

}


void main()
{
	// 1 Prepare common variable
	// 1.1 View vector
	vec3 V = normalize(cameraPosition - worldPosition);

	// 1.2 A, M, R, N texture
	vec3 albedo = texture(albedoTex, uv).xyz;
	vec3 N = texture(normalTex, uv).rgb;
	N = N * 2.0 - 1.0;
	N = normalize(tbn * N);
	float metallic = texture(metallicTex, uv).r;
	float roughness = texture(roughnessTex, uv).r;

	// 1.3 Calculate F0
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	// 2 Calculate direct light
	vec3 Lo = vec3(0.0);
	for(int i = 0; i < 4; i++){

		vec3 L = normalize(pointLights[i].position - worldPosition);
		vec3 H = normalize(L + V);
		float NdotL = max(dot(N, L), 0.0);
		float NdotV = max(dot(N, V), 0.0);

		// Calculate irradiance
		float dis = length(pointLights[i].position - worldPosition);
		float attenuation = 1.0/ (dis * dis);
		vec3 irradiance = pointLights[i].color * NdotL * attenuation;

		// Calculate D, G, F
		float D = NDF_GGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		vec3 F = fresnelSchlick(F0, max(dot(H, V), 0.0));

		// diffuse and specular ratio
		vec3 ks = F;
		vec3 kd = vec3(1.0) - ks;
		kd *= (1.0 - metallic); // Metal doesn't have diffuse

		// Calculate cook-torrance BRDF
		vec3 num = D * G * F;
		float denom = max(4.0 * NdotL * NdotV, 0.0000001);
		vec3 specularBRDF = num / denom;

		// specular + diffuse
		Lo += (kd * albedo / PI + specularBRDF) * irradiance;
	}

	// 3 Calculate indirect light
	// 3.1 diffuse IBL
	vec3 irradiance = texture(irradianceMap, N).rgb;
	vec3 kd = 1.0 - fresnelSchlickRoughness(F0, max(dot(N, V), 0.0), roughness);
	kd *= (1.0 - metallic);
	vec3 diffuseIBL = irradiance * albedo * kd;

	// 3.2 specular IBL
	const float MAX_REFLECTION_LOD = 4.0;
	vec3 R = reflect(-V, N);
	vec3 prefilteredColor = textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;

	float NdotV = max(dot(N, V), 0.0);
	vec2 brdfSample = texture(brdfLUT, vec2(NdotV, roughness)).rg;

	vec3 F = fresnelSchlickRoughness(F0, max(dot(N, V), 0.0), roughness);
	vec3 specularIBL = prefilteredColor * (F * brdfSample.x + brdfSample.y);

	// 3.3 ambient IBL
	vec3 ambient = diffuseIBL + specularIBL;

	// 4 Sum up
	Lo += ambient;
	FragColor = vec4(Lo, 1.0);
}