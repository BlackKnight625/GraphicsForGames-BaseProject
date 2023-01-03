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
#include <glm/gtx/quaternion.hpp>

/**
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb/stb_image_write.h"
#include "../stb/pngwriter.h"
*/

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
    float lastX;
    float lastY;
    float lastTranslationSphere;
    float lastTranslationCube;
    float lastRotationSphere;
    float lastRotationCube;
    float lastScaleSphereDown;
    float lastScaleSphereUp;
    float lastScaleCubeDown;
    float lastScaleCubeUp;
    bool firstMouse = true;
    float yaw;
    float pitch;

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
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) override;
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
  Shaders->addUniform("MortarColor");
  Shaders->addUniform("MarbleSize");
  Shaders->addUniform("MarblePct");

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

const int numberOfPixels = 800 * 600 * 3;

/**
void saveScreenshotToFile(const char * filename, int windowWidth, int windowHeight) {
    GLfloat* pixels = new GLfloat[numberOfPixels];
    glReadPixels(0.0, 0.0, windowWidth, windowHeight, GL_RGB, GL_FLOAT, pixels);
    pngwriter PNG(windowWidth, windowHeight, 1.0, filename);
    size_t x = 1;
    size_t y = 1;
    double R, G, B;
    for (size_t i = 0; i < numberOfPixels; i++) // "i" is the index for array "pixels"
    {
        switch (i % 3)
        {
        case 2:
            B = static_cast<double>(pixels[i]); break;
        case 1:
            G = static_cast<double>(pixels[i]); break;
        case 0:
            R = static_cast<double>(pixels[i]);
            PNG.plot(x, y, R, G, B);     // set pixel to position (x, y)
            if (x == windowWidth)             // Move to the next row of image
            {
                x = 1;
                y++;
            }
            else                       // To the next pixel
            {
                x++;
            }
            break;
        }
    }
    PNG.close();
}
*/

void MyApp::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        /**

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        glm::mat4 newViewMatrix = glm::lookAt(front, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0,1.0,0.0));
        Camera->setViewMatrix(newViewMatrix);
        */
        float xAngle = lastX - xpos;
        float yAngle = lastY - ypos;

        glm::quat rotationQX = glm::angleAxis(xAngle, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
        glm::mat4 rotationMX = glm::toMat4(rotationQX);

        glm::vec4 eyePositionX();

    }
}

void flipVertically(int width, int height, char* data)
{
    char rgb[3];

    for (int y = 0; y < height / 2; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int top = (x + y * width) * 3;
            int bottom = (x + (height - y - 1) * width) * 3;

            memcpy(rgb, data + top, sizeof(rgb));
            memcpy(data + top, data + bottom, sizeof(rgb));
            memcpy(data + bottom, rgb, sizeof(rgb));
        }
    }
}
/**
int saveScreenshot(const char* filename)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    int x = viewport[0];
    int y = viewport[1];
    int width = viewport[2];
    int height = viewport[3];

    char* data = (char*)malloc((size_t)(width * height * 3)); // 3 components (R, G, B)

    if (!data)
        return 0;

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

    flipVertically(width, height, data);

    int saved = stbi_write_png(filename, width, height, 3, data, 0);

    free(data);

    return saved;
}
*/
const char* createScreenshotBasename()
{
    static char basename[30];

    time_t t = time(NULL);
    strftime(basename, 30, "%Y%m%d_%H%M%S.png", localtime(&t));

    return basename;
}

/**
int captureScreenshot()
{
    char filename[50];

    strcpy(filename, "screenshots/");
    strcat(filename, createScreenshotBasename());

    int saved = saveScreenshot(filename);

    if (saved)
        printf("Successfully Saved Image: %s\n", filename);
    else
        fprintf(stderr, "Failed Saving Image: %s\n", filename);

    return saved;
}
*/

void MyApp::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        //saveScreenshotToFile("test.png", 800, 600);
        //captureScreenshot();
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        if (isPerspective) {
            Camera->setProjectionMatrix(ProjectionMatrix1);
            isPerspective = false;
            cout << "\nActivate orthographic projection\n";
        }
        else {
            Camera->setProjectionMatrix(ProjectionMatrix2);
            isPerspective = true;
            cout << "\nActivate perspective projection\n";
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

    if (glfwGetKey(window, GLFW_KEY_Q)) {
        cube.translate(glm::vec3(lastTranslationCube, 0.0f, 0.0f));
        cube.rotate(lastRotationCube, glm::vec3(0.0f, 0.0f, 1.0f));
        sphere.translate(glm::vec3(lastTranslationSphere, 0.0f, 0.0f));
        sphere.rotate(lastRotationSphere, glm::vec3(0.0f, 0.0f, 1.0f));
        for (int i = 0; i < lastScaleCubeDown; i++) {
            cube.scale(glm::vec3(1.25f, 1.25f, 1.25f));
        }
        for (int i = 0; i < lastScaleCubeUp; i++) {
            cube.scale(glm::vec3(0.75f, 0.75f, 0.75f));
        }
        for (int i = 0; i < lastScaleSphereDown; i++) {
            sphere.scale(glm::vec3(1.25f, 1.25f, 1.25f));
        }
        for (int i = 0; i < lastScaleSphereUp; i++) {
            sphere.scale(glm::vec3(0.75f, 0.75f, 0.75f));
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
