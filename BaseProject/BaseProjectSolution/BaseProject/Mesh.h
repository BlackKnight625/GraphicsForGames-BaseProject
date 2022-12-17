#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

typedef struct {
	GLfloat XYZW[4];
} Vertex;

typedef struct {
	GLfloat RGBA[4];
} Color;

class Mesh {
private:
	std::vector<Vertex> vertices;
	Color color;
	std::vector<GLubyte> indices;

public:
};

#endif