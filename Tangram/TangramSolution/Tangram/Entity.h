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

	void createBufferObjects(const GLuint POSITION, const GLuint COLOR);
	void destroyBufferObjects(const GLuint POSITION, const GLuint COLOR);
	void bind();
	void unbind();
	void draw();
};

class EntityShaders {
private:
	mgl::ShaderProgram* shaderProgram;

public:
	EntityShaders(mgl::ShaderProgram* ShaderProgram) {
		shaderProgram = ShaderProgram;
	}

	EntityShaders() {}

	void bind();
	void unbind();
};

class Entity
{
private:
	glm::mat4 model;
	EntityShaders shaders;
	Mesh mesh;

public:
	Entity(const Vertex* Vertices, const GLubyte* Indices, const int VerticesSize, const int IndicesSize, mgl::ShaderProgram* ShaderProgram) {
		model = glm::mat4(1.0f);
		shaders = EntityShaders(ShaderProgram);
		mesh = Mesh(Vertices, Indices, VerticesSize, IndicesSize);
	}

	Entity() {}

	void createBufferObjects(const GLuint POSITION, const GLuint COLOR);
    void destroyBufferObjects(const GLuint POSITION, const GLuint COLOR);
	void drawScene(GLint MatrixId);
	void translate(glm::vec3 translation);
	void rotate(float angle, glm::vec3 rotationAxis);
	void scale(glm::vec3 scale);
};

#endif