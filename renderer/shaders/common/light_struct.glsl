
struct SpotLight{

	vec3 position;
	vec3 targetDirection;
	vec3 color;
	float outerLine;
	float innerLine;
	float specularIntensity;
};


// 2.2 Directional light
struct DirectionalLight{
	vec3 direction;
	vec3 color;
	float specularIntensity;
	float intensity;
};

// 2.3 Point light
struct PointLight{
	vec3 position;
	vec3 color;
	float specularIntensity;

	float k2;
	float k1;
	float kc;
};

