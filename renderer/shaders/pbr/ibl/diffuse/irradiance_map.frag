#version 460 core
out vec4 FragColor;

in vec3 uvw;

uniform sampler2D envMap;

const float PI = 3.1415926535897932384626433832795;

vec2 uvwToUv(vec3 uvwN){

	float phi = asin(uvwN.y);
	float theta = atan(uvwN.z, uvwN.x);
	float u = theta / (2.0 * PI) + 0.5;
	float v = phi / PI + 0.5;
	return vec2(u, v);
}

void main(){

	vec3 normal = normalize(uvw);
	vec3 irradiance = vec3(0.0);

	// Local coordinate
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = normalize(cross(up, normal));
	up = normalize(cross(normal, right));

	float delta = 0.025;
	float sampleNum = 0.0;
	for(float phi = 0.0; phi < 2.0 * PI; phi += delta){
	
		for(float theta = 0.0; theta < 0.5 * PI; theta += delta){
			
			// 1 Polar coordinates to local coodinates
			vec3 localVec = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

			// 2 Local coodinates
			vec3 worldVec = localVec.x * right + localVec.y * up + localVec.z * normal;

			// 3 World coodinates to sphere map
			vec2 uv = uvwToUv(normalize(worldVec));
			vec3 Li = texture(envMap, uv).rgb;

			// 4 Radiance
			irradiance += Li * cos(theta) * sin(theta);

			sampleNum++;

		}
	}

	irradiance = PI * irradiance * 1.0 / sampleNum;

	FragColor = vec4(irradiance, 1.0);
}