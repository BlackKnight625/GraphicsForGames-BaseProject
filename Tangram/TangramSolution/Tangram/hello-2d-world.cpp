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


const Vertex Vertices[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.35f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.35f, 0.35f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}
};

const GLubyte Indices[] = { 0, 1, 2
};

const Vertex VerticesSquare[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.35f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.35f, 0.35f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.0f, 0.35f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}
};

const GLubyte IndicesSquare[] = { 0, 1, 3,
                           1, 2, 3
};

const Vertex VerticesParallelogram[] = {
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
  inline Entity createBaseTriangle();
  inline Entity createBaseSquare();
  inline Entity createBaseParallelogram();

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

inline Entity MyApp::createBaseTriangle() {
    return Entity(Vertices, Indices, sizeof(Vertices), sizeof(Indices), Shaders);
}

inline Entity MyApp::createBaseSquare() {
    return Entity(VerticesSquare, IndicesSquare, sizeof(VerticesSquare), sizeof(IndicesSquare), Shaders);
}

inline Entity MyApp::createBaseParallelogram() {
    return Entity(VerticesParallelogram, IndicesParallelogram, sizeof(VerticesParallelogram), sizeof(IndicesParallelogram), Shaders);
}

void MyApp::createCrab() {
    const int amountTriangles = 5;

    Entity *triangles = crab.entities;

    for (int i = 0; i < amountTriangles; i++) {
        triangles[i] = createBaseTriangle();
    }

    Entity* square = crab.entities + amountTriangles;
    Entity* parallelogram = crab.entities + amountTriangles + 1;

    *square = createBaseSquare();
    *parallelogram = createBaseParallelogram();

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
        crab.entities[i].drawScene(MatrixId);
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
