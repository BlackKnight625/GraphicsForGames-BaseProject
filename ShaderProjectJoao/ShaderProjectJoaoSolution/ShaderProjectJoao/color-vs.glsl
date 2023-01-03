#version 330 core

uniform vec3 LightPosition;

uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

in vec3 inPosition;
in vec3 inNormal;
out float exIntensity;
out vec2 exPosition;

const float SpecularContribution = 0.3;
const float DiffuseContribution = 1.0 - SpecularContribution;

void main(void) {
  vec4 mcPosition = vec4(inPosition, 1.0f);

  vec3 V = vec3(ViewMatrix * ModelMatrix * mcPosition);
  vec3 N = normalize(NormalMatrix * inNormal);
  vec3 L = normalize(LightPosition - V);
  vec3 R = reflect(-L,N);
  vec3 E = normalize(-V);

  float diffuse = max(dot(L,N), 0.0);
  float specular = 0.0;
  if(diffuse > 0.0) {
    specular = max(dot(R,E), 0.0);
    specular = pow(specular, 16.0);
  }

  exIntensity = DiffuseContribution * diffuse + SpecularContribution * specular;
  exPosition = inPosition.xz;

  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * mcPosition;
}
