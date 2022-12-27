// STEP 7 - Vertex color defined by model space normal.
// Copyright (c) 2015-20 by Carlos Martinho
// model: torus_vtn_flat.obj

#version 330 core

uniform mat4 ModelMatrix;
uniform Camera {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;	
};

in vec3 inPosition;
in vec3 inNormal;
out vec3 exNormal;

void main(void)
{
	exNormal = inNormal;

	vec4 mcPosition = vec4(inPosition, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * mcPosition;
}
