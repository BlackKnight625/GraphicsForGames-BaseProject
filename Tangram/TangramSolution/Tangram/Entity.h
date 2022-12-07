#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "hello-2d-world.cpp"


class Entity
{
private:
	Vertex *vertices;
	GLubyte *indices;


public:
	Entity(Vertex* vertices, GLubyte* Indices);
	void createBufferObjects(const GLuint POSITION, const GLuint COLOR, GLuint VaoId, GLuint* VboId, const Vertex* Vertices, const GLubyte* Indices);
    void destroyBufferObjects(const GLuint POSITION, const GLuint COLOR, GLuint VaoId);
	void drawScene(GLuint VaoId, mgl::ShaderProgram *Shaders, GLint MatrixId);
};

