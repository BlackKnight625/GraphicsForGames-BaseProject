#version 330 core

uniform mat4 ModelMatrix;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

in vec3 inPosition;
in vec3 inNormal;
in vec3 inTexCoord;
out vec3 exPosition;
out vec3 exNormal;

void main(void) {
  exPosition = inPosition;
  exNormal = inNormal;

  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition, 1.0f);
}
