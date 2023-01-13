#version 330 core

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexcoord;

uniform mat4 ModelMatrix;
uniform vec4 actualColor;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

out vec3 exPosition;
out vec3 exNormal;
out vec2 exTexcoord;

void main(void) {
	exPosition = inPosition;
	exNormal = inNormal;
	exTexcoord = inTexcoord;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition, 1.0);
}
