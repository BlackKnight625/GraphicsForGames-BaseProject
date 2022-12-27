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

#include <iomanip>
#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "mgl/mgl.hpp"
#include "mgl/MeshManager.hpp"
#include "mgl/Mesh.hpp"
#include "mgl/Cube.hpp"
#include "mgl/Sphere.hpp"

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {

private:
	GLuint VaoId;

	mgl::ShaderProgram *Shaders = nullptr;
	mgl::Camera *Camera = nullptr;
	mgl::MeshManager *MeshManager = nullptr;
    mgl::Cube cube = mgl::Cube();
    mgl::Sphere sphere = mgl::Sphere();

	void createMeshManager();
	void createShaderProgram();
	void createBufferObjects();
	void destroyBufferObjects();
	void drawScene();
    void createCube();
    void createSphere();

public:
    void initCallback(GLFWwindow* win) override;
    void displayCallback(GLFWwindow* win, double elapsed) override;
    void windowCloseCallback(GLFWwindow* win) override;
    void windowSizeCallback(GLFWwindow* win, int width, int height) override;
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) override;

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

  if (MeshManager->getSphereMesh()->hasNormals()) {
      Shaders->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
  }
  if (MeshManager->getSphereMesh()->hasTexcoords()) {
      Shaders->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
  }

  Shaders->create();
}

void MyApp::createMeshManager() {
    MeshManager = new mgl::MeshManager();

    MeshManager->createSphereAndCubeMesh(&Shaders, "../assets/models/Sphere.obj", 
"../assets/models/Cube.obj");
}


void MyApp::createBufferObjects() {
    MeshManager->createBufferObjects();
	Camera = new mgl::Camera(mgl::Mesh::UBO_BP);
}

void MyApp::createCube() {
    cube.createCube(MeshManager);
}

void MyApp::createSphere() {
    sphere.createSphere(MeshManager);
}


void MyApp::destroyBufferObjects() {
    MeshManager->destroyBufferObjects();
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

void MyApp::drawScene() {

  Camera->setViewMatrix(ViewMatrix1);
  Camera->setProjectionMatrix(ProjectionMatrix2);

  cube.draw();
  sphere.draw();
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win) {
    createMeshManager();
    createBufferObjects();
    createShaderProgram();
    createCube();
    createSphere();
}

void MyApp::windowCloseCallback(GLFWwindow *win) { destroyBufferObjects(); }

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy) {
  glViewport(0, 0, winx, winy);
}

void MyApp::displayCallback(GLFWwindow *win, double elapsed) { drawScene(); }

void MyApp::keyCallback(GLFWwindow* window, int key, int scancode,
    int action, int mods) {

}

const int numberOfPixels = 800 * 600 * 3;
void saveScreenshotToFile(std::string filename, int windowWidth, int windowHeight) {

    unsigned char pixels[numberOfPixels];

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, windowWidth, windowHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

    FILE* outputFile;
    fopen_s(&outputFile, filename.c_str(), "w");
    short header[] = { 0, 2, 0, 0, 0, 0, (short)windowWidth, (short)windowHeight, 24 };

    fwrite(&header, sizeof(header), 1, outputFile);
    fwrite(pixels, numberOfPixels, 1, outputFile);
    fclose(outputFile);

    printf("Finish writing to file.\n");
}

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
