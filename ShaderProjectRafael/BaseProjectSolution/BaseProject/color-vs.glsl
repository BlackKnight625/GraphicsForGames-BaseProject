#version 330 core

in vec3 inPosition;
out vec4 exColor;
in vec3 inNormal;

uniform mat4 ModelMatrix;
uniform vec4 actualColor;
uniform sampler2D sampler;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

void main(void) {
	vec4 position = vec4(inPosition[0], inPosition[1], inPosition[2], 1.0f);
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * position;

  float dotProduct = dot(vec3(0.1f, 0.2f, 1.0f), inNormal);

  exColor = actualColor + (dotProduct * 0.05f);
}
