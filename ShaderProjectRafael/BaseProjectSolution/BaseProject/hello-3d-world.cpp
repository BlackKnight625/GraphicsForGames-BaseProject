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

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "mgl/mgl.hpp"
#include "mgl/MeshManager.hpp"
#include "mgl/Mesh.hpp"

#include <chrono>

#include "mgl/Planets.hpp"
#include "mgl/TextureGenerator.hpp"

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {

private:
	GLuint VaoId;

	mgl::ShaderProgram *Shaders = nullptr;
	mgl::Camera *Camera = nullptr;
	mgl::MeshManager *MeshManager = nullptr;

    std::vector<mgl::Body*> _bodies;

    mgl::Sampler* _sampler;

    mgl::Texture2D _planetTexture = mgl::Texture2D();
    mgl::Texture2D _meteorTexture = mgl::Texture2D();
    mgl::Texture2D _starTexture = mgl::Texture2D();

    mgl::TextureInfo _planetTextureInfo;
    mgl::TextureInfo _meteorTextureInfo;
    mgl::TextureInfo _starTextureInfo;

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
  Shaders->addUniform(mgl::SAMPLER_ATTRIBUTE);

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

    MeshManager->createSphereMesh(&Shaders, "../assets/models/Sphere.obj");
}


void MyApp::createBufferObjects() {
    MeshManager->createBufferObjects();
	Camera = new mgl::Camera(mgl::Mesh::UBO_BP);

    _sampler = new mgl::LinearAnisotropicSampler();
}

void MyApp::createEntities() {
    // Creating the texture generators

    auto planetGenerator = mgl::UniformTextureGenerator(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    auto meteorGenerator = mgl::UniformTextureGenerator(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    auto starGenerator = mgl::UniformTextureGenerator(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));

    // Creating the textures
    _planetTexture.load(&planetGenerator);
    _meteorTexture.load(&meteorGenerator);
    _starTexture.load(&starGenerator);

    // Creating the texture infos

    _planetTextureInfo = mgl::TextureInfo(GL_TEXTURE0, _sampler->getSamplerId(), mgl::SAMPLER_ATTRIBUTE, & _planetTexture, _sampler);
    _meteorTextureInfo = mgl::TextureInfo(GL_TEXTURE1, _sampler->getSamplerId(), mgl::SAMPLER_ATTRIBUTE, &_meteorTexture, _sampler);
    _starTextureInfo = mgl::TextureInfo(GL_TEXTURE2, _sampler->getSamplerId(), mgl::SAMPLER_ATTRIBUTE, &_starTexture, _sampler);

    mgl::Planet* planet1 = new mgl::Planet(MeshManager, &_planetTextureInfo);

    planet1->setMass(10);

    _bodies.push_back(planet1);
}


void MyApp::destroyBufferObjects() {
    MeshManager->destroyBufferObjects();

    for (mgl::Body* entity : _bodies) {
        delete entity;
    }

    delete _sampler;
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

    // Detecting collisions
    for(int i = 0; i < _bodies.size(); i++) {
	    for(int j = i + 1; j < _bodies.size(); j++) {
		    // Detecting collision between body at index i and body at index j
            mgl::Body* body1 = _bodies.at(i);
            mgl::Body* body2 = _bodies.at(j);

            glm::vec3 body1ToBody2 = body2->getPosition() - body1->getPosition();

            if(glm::length(body1ToBody2) < body1->getRadius() + body2->getRadius()) {
	            // The sum of radiuses is larger than the distance. Collision detected

                if(body1->getCollisionPriority() > body2->getCollisionPriority()) {
	                // Letting body1 deal with the collision since it requests so
                    body1->onCollision(body2, &info);
                }
                else {
                    // body1->getCollisionPriority() <= body2->getCollisionPriority()
                    // If the priorities are equal, it doesn't matter who deals with the collision
                    body2->onCollision(body1, &info);
                }
            }
	    }
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
