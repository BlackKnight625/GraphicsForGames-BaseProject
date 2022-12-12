////////////////////////////////////////////////////////////////////////////////
//
// Drawing two instances of a triangle in Clip Space.
// A "Hello 2D World" of Modern OpenGL.
//
// (c)2013-22 by Carlos Martinho
//
// INTRODUCES:
// GL PIPELINE, mglShader.hpp, mglConventions.hpp
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "mgl/mgl.hpp"
#include "Entity.h"

////////////////////////////////////////////////////////////////////////// MYAPP


Vertex Vertices[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.35f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.35f, 0.35f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}
};

const GLubyte Indices[] = { 0, 1, 2
};

Vertex VerticesSquare[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.35f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.35f, 0.35f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.0f, 0.35f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}
};

const GLubyte IndicesSquare[] = { 0, 1, 3,
                           1, 2, 3
};

Vertex VerticesParallelogram[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.35f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.7f, 0.35f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.35f, 0.35f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}
};

const GLubyte IndicesParallelogram[] = { 0, 1, 3,
                           1, 2, 3
};

class MyApp : public mgl::App {

public:
  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;
  void windowCloseCallback(GLFWwindow *win) override;
  void windowSizeCallback(GLFWwindow *win, int width, int height) override;
  void createCrab();
  void drawCrab();
  void destroyCrab();
  void createCrabBuffer();
  inline Entity createBaseTriangle(glm::vec4 color);
  inline Entity createBaseSquare(glm::vec4 color);
  inline Entity createBaseParallelogram(glm::vec4 color);

private:
  const GLuint POSITION = 0, COLOR = 1;
  mgl::ShaderProgram *Shaders;
  GLint MatrixId;
  void createShaderProgram();
};

const int crabAmountEntities = 7;

typedef struct {
    Entity entities[crabAmountEntities];
} Crab;

Crab crab;

//////////////////////////////////////////////////////////////////////// SHADERs

inline Entity MyApp::createBaseTriangle(glm::vec4 color) {
    return Entity(Vertices, Indices, sizeof(Vertices), sizeof(Indices), Shaders, color, MatrixId);
}

inline Entity MyApp::createBaseSquare(glm::vec4 color) {
    return Entity(VerticesSquare, IndicesSquare, sizeof(VerticesSquare), sizeof(IndicesSquare), Shaders, color, MatrixId);
}

inline Entity MyApp::createBaseParallelogram(glm::vec4 color) {
    return Entity(VerticesParallelogram, IndicesParallelogram, sizeof(VerticesParallelogram), sizeof(IndicesParallelogram), Shaders, color, MatrixId);
}

void MyApp::createCrab() {
    const int amountTriangles = 5;

    glm::vec4 colors[] = {
        glm::vec4(1.0f, 155.0f, 244.0f, 255.0f), // Blue
        glm::vec4(251.0f, 49.0f, 73.0f, 255.0f), // Red
        glm::vec4(255.0f, 189.0f, 73.0f, 255.0f), // Yellow
        glm::vec4(255.0f, 125.0f, 76.0f, 255.0f), // Orange
        glm::vec4(210.0f,  1.0f, 139.0f, 255.0f), // Light purple
        glm::vec4(105.0f, 11.0f, 168.0f, 255.0f), // Purple
        glm::vec4(122.0f, 200.0f, 101.0f, 255.0f), // Green
    };

    // Converting the colors from RGB 256 to RGB 1.0f
    for (int i = 0; i < crabAmountEntities; i++) {
        colors[i] /= 255.0f;
    }

    Entity *triangles = crab.entities;

    for (int i = 0; i < amountTriangles; i++) {
        triangles[i] = createBaseTriangle(colors[i]);
    }

    Entity* square = crab.entities + amountTriangles;
    Entity* parallelogram = crab.entities + amountTriangles + 1;

    *square = createBaseSquare(colors[amountTriangles]);
    *parallelogram = createBaseParallelogram(colors[amountTriangles + 1]);

    triangles[0].scale(glm::vec3(1.5f, 1.5f, 1.0f));
    triangles[0].rotate(-135, glm::vec3(0.0f, 0.0f, 1.0f));
    triangles[0].translate(glm::vec3(-0.5f, 0.7f, 0.0f));

    triangles[1].scale(glm::vec3(2.0f, 2.0f, 1.0f));
    triangles[1].rotate(-90, glm::vec3(0.0f, 0.0f, 1.0f));
    triangles[1].translate(glm::vec3(-0.5f, 0.35f, 0.0f));

    triangles[2].rotate(180, glm::vec3(0.0f, 0.0f, 1.0f));
    triangles[2].translate(glm::vec3(-0.15f, -0.35f, 0.0f));

    triangles[3].scale(glm::vec3(2.0f, 2.0f, 1.0f));
    triangles[3].rotate(90, glm::vec3(0.0f, 0.0f, 1.0f));
    triangles[3].translate(glm::vec3(0.55f, -0.7f, 0.0f));

    triangles[4].rotate(-90, glm::vec3(0.0f, 0.0f, 1.0f));
    triangles[4].translate(glm::vec3(0.55f, 0.7f, 0.0f));

    square->translate(glm::vec3(-0.15f, 0.0f, 0.0f));

    parallelogram->translate(glm::vec3(0.2f, 0.0f, 0.0f));
}

void MyApp::createShaderProgram() {

  Shaders = new mgl::ShaderProgram();
  Shaders->addShader(GL_VERTEX_SHADER, "clip-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "clip-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);
  Shaders->addAttribute(mgl::COLOR_ATTRIBUTE, COLOR);
  Shaders->addUniform("Matrix");

  Shaders->create();

  MatrixId = Shaders->Uniforms["Matrix"].index;
}

//////////////////////////////////////////////////////////////////// VAOs & VBOs

////////////////////////////////////////////////////////////////////////// SCENE


////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::createCrabBuffer() {
    for (int i = 0; i < crabAmountEntities; i++) {
        crab.entities[i].createBufferObjects(POSITION, COLOR);
    }
}

void MyApp::initCallback(GLFWwindow *win) {
    createShaderProgram();
    createCrab();
    createCrabBuffer();
}

void MyApp::destroyCrab() {
    for (int i = 0; i < crabAmountEntities; i++) {
        crab.entities[i].destroyBufferObjects(POSITION, COLOR);
    }
}

void MyApp::windowCloseCallback(GLFWwindow *win) {
    destroyCrab();
}

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy) {
  glViewport(0, 0, winx, winy);
}

void MyApp::drawCrab() {
    for (int i = 0; i < crabAmountEntities; i++) {
        crab.entities[i].drawScene();
    }
}

void MyApp::displayCallback(GLFWwindow *win, double elapsed) {
    drawCrab();
}

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[]) {
  mgl::Engine &engine = mgl::Engine::getInstance();
  engine.setApp(new MyApp());
  engine.setOpenGL(4, 6);
  engine.setWindow(600, 600, "Hello Modern 2D World", 0, 1);
  engine.init();
  engine.run();
  exit(EXIT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////// END
