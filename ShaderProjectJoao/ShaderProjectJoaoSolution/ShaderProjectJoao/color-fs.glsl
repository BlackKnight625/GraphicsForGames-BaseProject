#version 330 core

uniform vec3 LightPosition;
uniform sampler3D NoiseTexture;

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
  float diffuse = max(dot(L,N), 0.0);

  float specular = 0.0;
  if(diffuse > 0.0) {
    vec3 R = reflect(-L,N);
    vec3 E = normalize(-V);
    specular = pow(max(dot(R,E), 0.0), 8.0f);
  }

  vec4 color = MarbleColor;

  float intensity = DiffuseContribution * diffuse + SpecularContribution * specular + ambient;

  vec3 wcPosition = vec3(ModelMatrix * vec4(exPosition, 1.0f));
  vec3 p = wcPosition * 0.1f + 0.1f;
  //float noise = texture(NoiseTexture, p).r * 0.5f + 0.5f;

  //FragmentColor = vec4(vec3(noise * intensity), 1.0f);
  color += vec4(p, 1.0f);
  color *= intensity;

  FragmentColor = color;
}
