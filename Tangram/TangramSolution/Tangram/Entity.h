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
	glm::mat4 model;

public:
	Entity(const Vertex* Vertices, const GLubyte* Indices, const int VerticesSize, const int IndicesSize) {
		vertices = Vertices;
		indices = Indices;
		verticesSize = VerticesSize;
		indicesSize = IndicesSize;

		model = glm::mat4(1.0f);
	}

	Entity() {}

	void createBufferObjects(const GLuint POSITION, const GLuint COLOR);
    void destroyBufferObjects(const GLuint POSITION, const GLuint COLOR);
	void drawScene(mgl::ShaderProgram *Shaders, GLint MatrixId);
	void translate(glm::vec3 translation);
	void rotate(float angle, glm::vec3 rotationAxis);
	void scale(glm::vec3 scale);
};

class Mesh {
private:
	GLuint VaoId, VboId[2];
	const Vertex* vertices;
	int verticesSize;
	const GLubyte* indices;
	int indicesSize;

public:
	Mesh(const Vertex* Vertices, const GLubyte* Indices, const int VerticesSize, const int IndicesSize) {
		vertices = Vertices;
		indices = Indices;
		verticesSize = VerticesSize;
		indicesSize = IndicesSize;
	}

	Mesh() {}

	void bind();
	void unbind();
};

class Shaders {
private:
	mgl::ShaderProgram* shaderProgram;

public:
	Shaders(mgl::ShaderProgram* ShaderProgram) {
		shaderProgram = ShaderProgram;
	}

	Shaders() {}

	void bind();
	void unbind();
};

#endif