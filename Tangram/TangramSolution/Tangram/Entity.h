#ifndef TANGRAM_ENTITY_H
#define TANGRAM_ENTITY_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "mgl/mgl.hpp"

typedef struct {
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

class Entity
{
private:
	GLuint VaoId, VboId[2];
	const Vertex *vertices;
	int verticesSize;
	const GLubyte *indices;
	int indicesSize;

public:
	Entity(const Vertex *Vertices, const GLubyte *Indices, const int VerticesSize, const int IndicesSize);
	void createBufferObjects(const GLuint POSITION, const GLuint COLOR);
    void destroyBufferObjects(const GLuint POSITION, const GLuint COLOR);
	void drawScene(mgl::ShaderProgram *Shaders, GLint MatrixId);
};

#endif