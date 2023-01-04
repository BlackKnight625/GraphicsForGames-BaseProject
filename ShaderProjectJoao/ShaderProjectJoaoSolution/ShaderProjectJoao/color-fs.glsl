#version 330 core

uniform vec3 LightPosition;

uniform mat3 NormalMatrix;
uniform mat4 ModelMatrix;

uniform vec4 MarbleColor;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

in vec3 exPosition;
in vec3 exNormal;
out vec4 FragmentColor;

const float SpecularContribution = 0.3;
const float DiffuseContribution = 1.0 - SpecularContribution;

void main(void) {
  float ambient = 0.20f;

  vec3 V = vec3(ModelMatrix * vec4(exPosition, 1.0f));
  vec3 N = normalize(NormalMatrix * exNormal);
  vec3 L = normalize(LightPosition - V);
  vec3 R = reflect(-L,N);
  vec3 E = normalize(-V);

  float diffuse = max(dot(L,N), 0.0);
  float specular = 0.0;
  if(diffuse > 0.0) {
    specular = max(dot(R,E), 0.0);
    specular = pow(specular, 8.0);
  }

  vec4 color = MarbleColor;

  float intensity = DiffuseContribution * diffuse + SpecularContribution * specular + ambient;

  color *= intensity;

  FragmentColor = color;
}
