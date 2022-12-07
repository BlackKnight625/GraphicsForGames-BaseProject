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

////////////////////////////////////////////////////////////////////////// MYAPP

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
  void drawScene();
};

//////////////////////////////////////////////////////////////////////// SHADERs

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

////////////////////////////////////////////////////////////////////////// SCENE

const glm::mat4 R1 = glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
const glm::mat4 M1 = glm::translate(glm::vec3(-0.5f, 0.0f, 0.0f)) * R1;

const glm::mat4 S1 = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 1.0f));
const glm::mat4 R2 = glm::rotate(glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * S1;
const glm::mat4 M2 = glm::translate(glm::vec3(0.2f, -0.4f, 0.0f)) * R2;

const glm::mat4 R3 = glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
const glm::mat4 M3 = glm::translate(glm::vec3(-0.5f, -0.75f, 0.0f)) * R3;

const glm::mat4 S2 = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 1.0f));
const glm::mat4 R4 = glm::rotate(glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * S2;
const glm::mat4 M4 = glm::translate(glm::vec3(-0.2f, 0.0f, 0.0f)) * R4;

const glm::mat4 R5 = glm::rotate(glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
const glm::mat4 M5 = glm::translate(glm::vec3(0.85f, 0.35f, 0.0f)) * R5;

const glm::mat4 R6 = glm::rotate(glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
const glm::mat4 M6 = glm::translate(glm::vec3(0.15f, 0.0f, 0.0f)) * R6;

const glm::mat4 R7 = glm::rotate(glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
const glm::mat4 M7 = glm::translate(glm::vec3(-0.2f, 0.35f, 0.0f)) * R7;

const glm::mat4 R8 = glm::rotate(glm::radians(-135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
const glm::mat4 M8 = glm::translate(glm::vec3(0.5f, 0.35f, 0.0f)) * R8;

const glm::mat4 R9 = glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
const glm::mat4 M9 = glm::translate(glm::vec3(0.5f, 0.0f, 0.0f)) * R9;

void MyApp::drawScene() {
  // Drawing directly in clip space

  glBindVertexArray(VaoId);
  Shaders->bind();

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M1));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M2));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M3));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M4));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M5));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M6));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M7));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M8));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M9));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

  Shaders->unbind();
  glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win) {
  createBufferObjects();
  createShaderProgram();
}

void MyApp::windowCloseCallback(GLFWwindow *win) { destroyBufferObjects(); }

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy) {
  glViewport(0, 0, winx, winy);
}

void MyApp::displayCallback(GLFWwindow *win, double elapsed) { drawScene(); }

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
