#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "hello-2d-world.cpp"

typedef struct {
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

class Entity
{
private:
	const Vertex *vertices;
	const GLubyte *indices;

public:
	Entity(const Vertex *Vertices, const GLubyte *Indices);
	void createBufferObjects(const GLuint POSITION, const GLuint COLOR, GLuint VaoId, GLuint* VboId, const Vertex* Vertices, const GLubyte* Indices);
    void destroyBufferObjects(const GLuint POSITION, const GLuint COLOR, GLuint VaoId);
	void drawScene(GLuint VaoId, mgl::ShaderProgram *Shaders, GLint MatrixId);
};

