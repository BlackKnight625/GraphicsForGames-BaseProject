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
	const Vertex *vertices;
	const GLubyte *indices;

public:
	Entity(const Vertex *Vertices, const GLubyte *Indices);
	void createBufferObjects(const GLuint POSITION, const GLuint COLOR, GLuint VaoId, GLuint* VboId, const Vertex* Vertices, const GLubyte* Indices);
    void destroyBufferObjects(const GLuint POSITION, const GLuint COLOR, GLuint VaoId);
	void drawScene(GLuint VaoId, mgl::ShaderProgram *Shaders, GLint MatrixId);
};

#endif