////////////////////////////////////////////////////////////////////////////////
//
// Adding a camera and moving to the 3rd dimension.
// A "Hello 3D World" of Modern OpenGL.
//
// (c) 2013-22 by Carlos Martinho
//
// INTRODUCES:
// VIEW PIPELINE, UNIFORM BUFFER OBJECTS, mglCamera.hpp
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include<bits/stdc++.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "mgl/mgl.hpp"
#include "mgl/MeshManager.hpp"
#include "mgl/Mesh.hpp"
#include "mgl/Crab.hpp"

#include <chrono>

#include "mgl/Planets.hpp"

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {

private:
	GLuint VaoId;

	mgl::ShaderProgram *Shaders = nullptr;
	mgl::Camera *Camera = nullptr;
	mgl::MeshManager *MeshManager = nullptr;

    std::vector<mgl::Body*> _bodies;

	void createMeshManager();
	void createShaderProgram();
	void createBufferObjects();
	void destroyBufferObjects();
	void drawScene(double elapsed);
    void createEntities();

public:
    void initCallback(GLFWwindow* win) override;
    void displayCallback(GLFWwindow* win, double elapsed) override;
    void windowCloseCallback(GLFWwindow* win) override;
    void windowSizeCallback(GLFWwindow* win, int width, int height) override;

    virtual ~MyApp() {
        delete Shaders;
        delete Camera;
        delete MeshManager;
    }
};

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderProgram() {

  Shaders = new mgl::ShaderProgram();
  Shaders->addShader(GL_VERTEX_SHADER, "color-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "color-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
  Shaders->addUniform(mgl::MODEL_MATRIX);
  Shaders->addUniformBlock(mgl::CAMERA_BLOCK, mgl::Mesh::UBO_BP);
  Shaders->addUniform(mgl::ACTUAL_COLOR_ATTRIBUTE);

  if (MeshManager->getTriangleMesh()->hasNormals()) {
      Shaders->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
  }
  if (MeshManager->getTriangleMesh()->hasTexcoords()) {
      Shaders->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
  }

  Shaders->create();
}

void MyApp::createMeshManager() {
    MeshManager = new mgl::MeshManager();

    MeshManager->createCrabMesh(&Shaders, "../assets/models/Triangle.obj", 
"../assets/models/Square.obj", 
"../assets/models/Parallelogram.obj");
}


void MyApp::createBufferObjects() {
    MeshManager->createBufferObjects();
	Camera = new mgl::Camera(mgl::Mesh::UBO_BP);
}

void MyApp::createEntities() {


    for(mgl::Body* entity : _bodies) {
        entity->createEntity(MeshManager);
    }
}


void MyApp::destroyBufferObjects() {
    MeshManager->destroyBufferObjects();

    for (mgl::Body* entity : _bodies) {
        delete entity;
    }
}

////////////////////////////////////////////////////////////////////////// SCENE

const glm::mat4 ModelMatrix = glm::translate(glm::vec3(-0.5f, -0.5f, -0.5f)) *
                              glm::scale(glm::vec3(2.0f));

// Eye(10,10,10) Center(0,0,0) Up(0,1,0)
const glm::mat4 ViewMatrix1 =
    glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));

// Eye(-5,-5,-5) Center(0,0,0) Up(0,1,0)
const glm::mat4 ViewMatrix2 =
    glm::lookAt(glm::vec3(-5.0f, -5.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));

// Orthographic LeftRight(-2,2) BottomTop(-2,2) NearFar(1,10)
const glm::mat4 ProjectionMatrix1 =
    glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 1.0f, 15.0f);

// Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(25)
const glm::mat4 ProjectionMatrix2 =
    glm::perspective(glm::radians(30.0f), 4.0f / 3.0f, 1.0f, 25.0f);

void MyApp::drawScene(double elapsed) {

	Camera->setViewMatrix(ViewMatrix1);
	Camera->setProjectionMatrix(ProjectionMatrix2);

    mgl::UpdateInfo info;
    mgl::UpdateInfo* infoReference = &info;

    info.delta = (float) (elapsed);
    info.bodies = &_bodies;

	// Updating everyone's positions
	for (mgl::Body* entity : _bodies) {
		entity->update(&info);
	}

    // Removing bodies
    auto removeLambda = [infoReference](mgl::Body* element) {
        return std::find(infoReference->toRemove.begin(), infoReference->toRemove.end(), element) != infoReference->toRemove.end();
    };

    std::remove_if(_bodies.begin(), _bodies.end(), removeLambda);

    // Adding bodies
    _bodies.insert(_bodies.end(), info.toAdd.begin(), info.toAdd.end());

    // Drawing everyone
	for (mgl::Body* entity : _bodies) {
		 entity->draw();
	}
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win) {
    createMeshManager();
    createBufferObjects();
    createShaderProgram();
    createEntities();
}

void MyApp::windowCloseCallback(GLFWwindow *win) { destroyBufferObjects(); }

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy) {
  glViewport(0, 0, winx, winy);
}

void MyApp::displayCallback(GLFWwindow *win, double elapsed) { drawScene(elapsed); }

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[]) {
  mgl::Engine &engine = mgl::Engine::getInstance();
  engine.setApp(new MyApp());
  engine.setOpenGL(4, 6);
  engine.setWindow(800, 600, "Hello Modern 3D World", 0, 1);
  engine.init();
  engine.run();
  exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
