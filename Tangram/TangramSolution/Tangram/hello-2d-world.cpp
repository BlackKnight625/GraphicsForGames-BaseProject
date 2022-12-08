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
    {{0.5f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.25f, 0.25f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},

    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.25f, 0.25f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},

    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.25f, 0.25f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}
};

const GLubyte Indices[] = { 0, 1, 2,
                           3, 4, 5,
                           6, 7, 8,
};

const Vertex VerticesSquare[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.5f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.0f, 0.5f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}
};

const GLubyte IndicesSquare[] = { 0, 1, 3,
                           3, 2, 1
};

const Vertex VerticesParallelogram[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.5f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.0f, 0.5f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}
};

const GLubyte IndicesParallelogram[] = { 0, 1, 3,
                           3, 2, 1
};

class MyApp : public mgl::App {

public:
  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;
  void windowCloseCallback(GLFWwindow *win) override;
  void windowSizeCallback(GLFWwindow *win, int width, int height) override;

private:
  const GLuint POSITION = 0, COLOR = 1;
  GLuint VaoId, VboId[2];
  mgl::ShaderProgram *Shaders;
  GLint MatrixId;
  void createShaderProgram();
};

//////////////////////////////////////////////////////////////////////// SHADERs


Entity triangle(Vertices, Indices);
Entity square(VerticesSquare, IndicesSquare);
Entity parallelogram(VerticesParallelogram, IndicesParallelogram);

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

const glm::mat4 R6 = glm::rotate(glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
const glm::mat4 M6 = glm::translate(glm::vec3(0.15f, 0.0f, 0.0f)) * R6;

const glm::mat4 R7 = glm::rotate(glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
const glm::mat4 M7 = glm::translate(glm::vec3(-0.2f, 0.35f, 0.0f)) * R7;

const glm::mat4 R8 = glm::rotate(glm::radians(-135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
const glm::mat4 M8 = glm::translate(glm::vec3(0.5f, 0.35f, 0.0f)) * R8;

const glm::mat4 R9 = glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
const glm::mat4 M9 = glm::translate(glm::vec3(0.5f, 0.0f, 0.0f)) * R9;

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win) {
  triangle.createBufferObjects(POSITION, COLOR, VaoId, VboId, Vertices, Indices);
  createShaderProgram();
  square.createBufferObjects(POSITION, COLOR, VaoId, VboId, VerticesSquare, IndicesSquare);
  createShaderProgram();
  parallelogram.createBufferObjects(POSITION, COLOR, VaoId, VboId, VerticesParallelogram, IndicesParallelogram);
  createShaderProgram();
}

void MyApp::windowCloseCallback(GLFWwindow *win) {
    triangle.destroyBufferObjects(POSITION, COLOR, VaoId);
    square.destroyBufferObjects(POSITION, COLOR, VaoId);
    parallelogram.destroyBufferObjects(POSITION, COLOR, VaoId);
}

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy) {
  glViewport(0, 0, winx, winy);
}

void MyApp::displayCallback(GLFWwindow *win, double elapsed) {
    triangle.drawScene(VaoId, Shaders, MatrixId);
    square.drawScene(VaoId, Shaders, MatrixId);
    parallelogram.drawScene(VaoId, Shaders, MatrixId);
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
