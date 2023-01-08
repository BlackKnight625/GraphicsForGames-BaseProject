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
#include <sstream>

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
#include <glm/gtx/quaternion.hpp>

#define stringify( name ) #name
#include "SOIL2.h"

using namespace std;
////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {

private:
	GLuint VaoId;

	mgl::ShaderProgram *Shaders = nullptr;
	mgl::Camera *Camera = nullptr;
	mgl::MeshManager *MeshManager = nullptr;
    mgl::Cube cube = mgl::Cube();
    mgl::Sphere sphere = mgl::Sphere();
    bool isPerspective = true;

    // Used to save the last position of the mouse
    float lastX;
    float lastY;

    // Used to reset the scene
    float lastTranslationSphere;
    float lastTranslationCube;
    float lastTranslationCubeX;
    float lastTranslationCubeY;
    float lastTranslationSphereX;
    float lastTranslationSphereY;
    float lastRotationSphere;
    vector<float> lastRotationSphereX;
    vector<float> lastRotationSphereY;
    float lastRotationCube;
    vector<float> lastRotationCubeX;
    vector<float> lastRotationCubeY;
    float lastScaleSphereDown;
    float lastScaleSphereUp;
    float lastScaleCubeDown;
    float lastScaleCubeUp;
    float lastScaleCubeDownMouse;
    float lastScaleCubeUpMouse;
    float lastScaleSphereDownMouse;
    float lastScaleSphereUpMouse;

    int screenshotNumber = 1;

    // Buttons to change the Cube with mouse
    bool isPressingQ = false;
    bool isPressingA = false;
    bool isPressingZ = false;

    // Buttons to change the Sphere with mouse
    bool isPressingW = false;
    bool isPressingS = false;
    bool isPressingX = false;

    bool isPressingLeftMouseButton = false;
    bool firstMouse = true;

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
    void cursorCallback(GLFWwindow* window, double xpos, double ypos) override;
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) override;
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
    void createTextures_noise3d();

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
  if (MeshManager->getSphereMesh()->hasNormals()) {
      Shaders->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
  }
  if (MeshManager->getSphereMesh()->hasTexcoords()) {
      Shaders->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
  }
  Shaders->addUniform(mgl::MODEL_MATRIX);
  Shaders->addUniform(mgl::NORMAL_MATRIX);
  Shaders->addUniformBlock(mgl::CAMERA_BLOCK, mgl::Mesh::UBO_BP);
  
  Shaders->addUniform("LightPosition");
  Shaders->addUniform("MarbleColor");
  Shaders->addUniform("NoiseTexture");

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

void MyApp::initCallback(GLFWwindow* win) {
    createMeshManager();
    createBufferObjects();
    createShaderProgram();
    createCube();
    createSphere();
}

void MyApp::windowCloseCallback(GLFWwindow* win) { destroyBufferObjects(); }

void MyApp::windowSizeCallback(GLFWwindow* win, int winx, int winy) {
    glViewport(0, 0, winx, winy);
}

void MyApp::displayCallback(GLFWwindow* win, double elapsed) { drawScene(); }

void MyApp::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        isPressingLeftMouseButton = true;
    }
    else {
        isPressingLeftMouseButton = false;
    }
}

void MyApp::cursorCallback(GLFWwindow* window, double xpos, double ypos) {
    if (isPressingLeftMouseButton) {
        if (isPressingQ) {
            float scale = xpos - lastX;
            if (scale < 0) {
                scale = 0.995;
            }
            else {
                scale = 1.005;
            }
            cube.scale(glm::vec3(scale, scale, scale));
            if (scale < 1) {
                lastScaleCubeDownMouse += 1.0f;
            }
            else {
                lastScaleCubeUpMouse += 1.0f;
            }
            
        }
        else if (isPressingA) {
            cube.rotate(xpos - lastX, glm::vec3(1.0f, 0.0f, 0.0f));
            cube.rotate(ypos - lastY, glm::vec3(0.0f, 1.0f, 0.0f));
            lastRotationCubeX.push_back(-(xpos - lastX));
            lastRotationCubeY.push_back(-(ypos - lastY));
        }
        else if (isPressingZ) {
            cube.translate(glm::vec3((xpos - lastX) / 20.0f, (lastY - ypos) / 20.0f, 0.0f));
            lastTranslationCubeX -= ((xpos - lastX) / 20.0f);
            lastTranslationCubeY -= ((lastY - ypos) / 20.0f);
        }
        else if (isPressingW) {
            float scale = xpos - lastX;
            if (scale < 0) {
                scale = 0.995;
            }
            else {
                scale = 1.005;
            }
            sphere.scale(glm::vec3(scale, scale, scale));
            if (scale < 1) {
                lastScaleSphereDownMouse += 1.0f;
            }
            else {
                lastScaleSphereUpMouse += 1.0f;
            }
        }
        else if (isPressingS) {
            sphere.rotate(xpos - lastX, glm::vec3(1.0f, 0.0f, 0.0f));
            sphere.rotate(ypos - lastY, glm::vec3(0.0f, 1.0f, 0.0f));
            lastRotationSphereX.push_back(-(xpos - lastX));
            lastRotationSphereY.push_back(-(ypos - lastY));
        }
        else if (isPressingX) {
            sphere.translate(glm::vec3((xpos - lastX) / 20.0f, (lastY - ypos) / 20.0f, 0.0f));
            lastTranslationSphereX -= ((xpos - lastX) / 20.0f);
            lastTranslationSphereY -= ((lastY - ypos) / 20.0f);
        }
    }

    lastX = xpos;
    lastY = ypos;
}

void MyApp::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        string fileName = "screenshots/screenshot" + to_string(screenshotNumber) + ".bmp";
        cout << fileName << endl;
        if (!SOIL_save_screenshot(fileName.c_str(),
            SOIL_SAVE_TYPE_BMP, 0, 0, 800, 600)) {
            // Handle error here
            cout << "There was an error saving the image" << endl;
        }
        else {
            screenshotNumber++;
            cout << "You saved an image called " + fileName + " in the directory named screenshots!" << endl;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        if (isPerspective) {
            Camera->setProjectionMatrix(ProjectionMatrix1);
            isPerspective = false;
            cout << "Activate orthographic projection" << endl;
        }
        else {
            Camera->setProjectionMatrix(ProjectionMatrix2);
            isPerspective = true;
            cout << "Activate perspective projection" << endl;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        cout << "Cursor locked inside window" << endl;
    }
    else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        cout << "Cursor unlocked" << endl;
    }

    // Sphere transformations
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        sphere.translate(glm::vec3(-1.0f, 0.0f, 0.0f));
        cout << "Translated sphere negatively in the x axis" << endl;
        lastTranslationSphere += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        sphere.translate(glm::vec3(1.0f, 0.0f, 0.0f));
        cout << "Translated sphere positively in the x axis" << endl;
        lastTranslationSphere -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        sphere.rotate(-15.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        cout << "Rotated sphere negatively using the z axis" << endl;
        lastRotationSphere += 15.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        sphere.rotate(15.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        cout << "Rotated sphere positively using the z axis" << endl;
        lastRotationSphere -= 15.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        sphere.scale(glm::vec3(0.75f, 0.75f, 0.75f));
        cout << "Scaled down sphere" << endl;
        lastScaleSphereDown += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        sphere.scale(glm::vec3(1.25f, 1.25f, 1.25f));
        cout << "Scaled up sphere" << endl;
        lastScaleSphereUp += 1.0f;
    }

    // Cube transformations
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        cube.translate(glm::vec3(-1.0f, 0.0f, 0.0f));
        cout << "Translated cube negatively in the x axis" << endl;
        lastTranslationCube += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        cube.translate(glm::vec3(1.0f, 0.0f, 0.0f));
        cout << "Translated cube positively in the x axis" << endl;
        lastTranslationCube -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cube.rotate(-15.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        cout << "Rotated cube negatively using the z axis" << endl;
        lastRotationCube += 15.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        cube.rotate(15.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        cout << "Rotated cube positively using the z axis" << endl;
        lastRotationCube -= 15.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        cube.scale(glm::vec3(0.75f, 0.75f, 0.75f));
        cout << "Scaled down cube" << endl;
        lastScaleCubeDown += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        cube.scale(glm::vec3(1.25f, 1.25f, 1.25f));
        cout << "Scaled up cube" << endl;
        lastScaleCubeUp += 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_K)) {
        cube.translate(glm::vec3(lastTranslationCube, 0.0f, 0.0f));
        cube.translate(glm::vec3(lastTranslationCubeX, 0.0f, 0.0f));
        cube.translate(glm::vec3(0.0f, lastTranslationCubeY, 0.0f));
        cube.rotate(lastRotationCube, glm::vec3(0.0f, 0.0f, 1.0f));

        for (int i = lastRotationCubeX.size()-1; i > -1; i--) {
            cube.rotate(lastRotationCubeY[i], glm::vec3(0.0f, 1.0f, 0.0f));
            cube.rotate(lastRotationCubeX[i], glm::vec3(1.0f, 0.0f, 0.0f));
        }
        sphere.translate(glm::vec3(lastTranslationSphere, 0.0f, 0.0f));
        sphere.translate(glm::vec3(lastTranslationSphereX, 0.0f, 0.0f));
        sphere.translate(glm::vec3(0.0f, lastTranslationSphereY, 0.0f));
        sphere.rotate(lastRotationSphere, glm::vec3(0.0f, 0.0f, 1.0f));

        for (int i = lastRotationSphereX.size() - 1; i > -1; i--) {
            sphere.rotate(lastRotationSphereY[i], glm::vec3(0.0f, 1.0f, 0.0f));
            sphere.rotate(lastRotationSphereX[i], glm::vec3(1.0f, 0.0f, 0.0f));
        }

        for (int i = 0; i < lastScaleCubeDown; i++) {
            cube.scale(glm::vec3(1.25f, 1.25f, 1.25f));
        }
        for (int i = 0; i < lastScaleCubeUp; i++) {
            cube.scale(glm::vec3(0.75f, 0.75f, 0.75f));
        }
        for (int i = 0; i < lastScaleCubeDownMouse; i++) {
            cube.scale(glm::vec3(1.005f, 1.005f, 1.005f));
        }
        for (int i = 0; i < lastScaleCubeUpMouse; i++) {
            cube.scale(glm::vec3(0.995f, 0.995f, 0.995f));
        }

        for (int i = 0; i < lastScaleSphereDown; i++) {
            sphere.scale(glm::vec3(1.25f, 1.25f, 1.25f));
        }
        for (int i = 0; i < lastScaleSphereUp; i++) {
            sphere.scale(glm::vec3(0.75f, 0.75f, 0.75f));
        }
        for (int i = 0; i < lastScaleSphereDownMouse; i++) {
            sphere.scale(glm::vec3(1.005f, 1.005f, 1.005f));
        }
        for (int i = 0; i < lastScaleSphereUpMouse; i++) {
            sphere.scale(glm::vec3(0.995f, 0.995f, 0.995f));
        }
        cout << "Reset the scene" << endl;
        lastTranslationCube = 0.0f;
        lastTranslationSphere = 0.0f;
        lastRotationCube = 0.0f;
        lastRotationSphere = 0.0f;
        lastScaleSphereDown = 0.0f;
        lastScaleSphereUp = 0.0f;
        lastScaleCubeDown = 0.0f;
        lastScaleCubeUp = 0.0f;
        lastTranslationSphere;
        
        lastTranslationCubeX = 0.0f;
        lastTranslationCubeY = 0.0f;
        lastTranslationSphereX = 0.0f;
        lastTranslationSphereY = 0.0f;
        lastRotationSphereX.clear();
        lastRotationSphereY.clear();
        lastRotationCubeX.clear();
        lastRotationCubeY.clear();
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        isPressingQ = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        isPressingA = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        isPressingZ = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        isPressingW = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        isPressingS = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        isPressingX = true;
    }
    else {
        isPressingQ = false;
        isPressingA = false;
        isPressingZ = false;
        isPressingW = false;
        isPressingS = false;
        isPressingX = false;
    }
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
