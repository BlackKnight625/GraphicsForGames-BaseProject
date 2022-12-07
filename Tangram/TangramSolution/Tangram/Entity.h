#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

typedef struct {
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

class Entity
{
private:
	


public:
    void createBufferObjects(Vertex* Vertices, GLubyte Indices);
    void destroyBufferObjects();
	void draw();
};

