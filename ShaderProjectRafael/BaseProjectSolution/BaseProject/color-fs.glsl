#version 330 core

in vec4 exColor;
in vec3 exPosition;
in vec3 exNormal;
in vec2 exTexcoord;
out vec4 outColor;

uniform sampler3D sampler;
uniform int isStar;
uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;
uniform vec3 lightPosition;
uniform vec3 mainColor;
uniform vec3 veinColor;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

const float NoiseFactor = 3.0;
const float PositionFactor = 20.0;
const float IntensityFactor = 28.0;

void main(void) {
	vec3 V = vec3(ViewMatrix * ModelMatrix * vec4(exPosition, 1.0f));
	vec3 N = vec3(normalize(NormalMatrix * vec4(exNormal, 1.0f)));
	vec3 LPos = vec3(ViewMatrix * vec4(lightPosition, 1.0));
	vec3 L = normalize(LPos - V);

	float diffuse = max(dot(L, N), 0.0);
	float ambient = 0.0;
	float specular = 0.0;

	if(diffuse > 0.0) {
		vec3 R = reflect(-L, N);
		vec3 E = normalize(-V);
		specular = pow(max(dot(R, E), 0.0), 32.0);
	}

	
	if(isStar > 0) {
		// It's a star, so it's always bright
		diffuse = 0.0;
		ambient = 1.0;
		specular = 0.0;
	}
	

	vec3 worldPosition = vec3(ModelMatrix * vec4(exPosition, 1.0));
	vec3 p = worldPosition * 0.5 + 0.5;

	float noise = texture(sampler, p).r * 0.5 + 0.5;

	float veinIntensity = clamp(noise * NoiseFactor, 0.0, 1.0);
	veinIntensity = cos(exPosition.x * PositionFactor + veinIntensity * IntensityFactor) * 0.5 + 0.5;
	vec3 color = mix(mainColor, veinColor, veinIntensity);

	outColor = vec4(color * (diffuse * 0.75 + 0.25) + specular + color * ambient, 1.0);
}
