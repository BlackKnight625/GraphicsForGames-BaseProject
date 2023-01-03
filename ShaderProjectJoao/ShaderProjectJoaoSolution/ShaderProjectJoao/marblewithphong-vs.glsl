// STEP 3 - Passing parameters from application to vertex program
// Copyright (c) 2015-20 by Carlos Martinho
// model: torus_vtn_flat.obj

#version 330 core

uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

uniform Camera {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

in vec3 inPosition;
in vec3 inNormal;
out vec4 mcPosition;
out vec4 ecPosition;
out vec3 ecNormal;

void main(void)
{
	mcPosition = vec4(inPosition, 1.0);
	ecPosition = ViewMatrix * ModelMatrix * mcPosition;
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * mcPosition;
}
