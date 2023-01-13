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

#define RANDOM_FLOAT (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {

private:
	GLuint VaoId;

	mgl::ShaderProgram *Shaders = nullptr;
	mgl::Camera *Camera = nullptr;
	mgl::MeshManager *MeshManager = nullptr;

    std::vector<mgl::Body*> _bodies;
    mgl::Star* _sun;

    mgl::Sampler* _sampler;

    mgl::Texture3D _planetTexture = mgl::Texture3D();
    mgl::Texture3D _meteorTexture = mgl::Texture3D();
    mgl::Texture3D _starTexture = mgl::Texture3D();

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
  Shaders->addUniform(mgl::NORMAL_MATRIX);
  Shaders->addUniformBlock(mgl::CAMERA_BLOCK, mgl::Mesh::UBO_BP);
  Shaders->addUniform(mgl::ACTUAL_COLOR_ATTRIBUTE);
  Shaders->addUniform(mgl::MAIN_COLOR_ATTRIBUTE);
  Shaders->addUniform(mgl::VEIN_COLOR_ATTRIBUTE);
  Shaders->addUniform(mgl::SAMPLER_ATTRIBUTE);
  Shaders->addUniform(mgl::IS_STAR_ATTRIBUTE);
  Shaders->addUniform(mgl::LIGHT_POSITION);

  if (MeshManager->getSphereMesh()->hasNormals()) {
      Shaders->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
  }
  if (MeshManager->getSphereMesh()->hasTexcoords()) {
      Shaders->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
  }

  Shaders->create();
  _sampler->create();
}

void MyApp::createMeshManager() {
    MeshManager = new mgl::MeshManager();

    MeshManager->createSphereMesh(&Shaders, "../assets/models/Sphere.obj");
}


void MyApp::createBufferObjects() {
    MeshManager->createBufferObjects();
	Camera = new mgl::Camera(mgl::Mesh::UBO_BP);

    _sampler = new mgl::NearestSampler();
}

void MyApp::createEntities() {
    // Creating the texture generators

    auto randomGenerator = mgl::RandomTextureGenerator();
    auto uniformGenerator = mgl::UniformTextureGenerator(0.5f);

    // Creating the textures
    _planetTexture.load(&randomGenerator);
    _meteorTexture.load(&randomGenerator);
    _starTexture.load(&randomGenerator);

    // Creating the texture infos

    GLuint samplerID = Shaders->Uniforms[mgl::SAMPLER_ATTRIBUTE].index;

    _planetTextureInfo = mgl::TextureInfo(GL_TEXTURE0, samplerID, mgl::SAMPLER_ATTRIBUTE, &_planetTexture, _sampler);
    _meteorTextureInfo = mgl::TextureInfo(GL_TEXTURE1, samplerID, mgl::SAMPLER_ATTRIBUTE, &_meteorTexture, _sampler);
    _starTextureInfo = mgl::TextureInfo(GL_TEXTURE2, samplerID, mgl::SAMPLER_ATTRIBUTE, &_starTexture, _sampler);

    _sun = new mgl::Star(MeshManager, &_starTextureInfo);

    _sun->setMass(15);
    _sun->setAngleSpeed(0.2f);
    
    mgl::Planet* planet1 = new mgl::Planet(MeshManager, &_planetTextureInfo);
    mgl::Planet* planet2 = new mgl::Planet(MeshManager, &_planetTextureInfo);

    planet1->setMass(1);
    planet1->translate(glm::vec3(10.0f, 0.0f, 0.0f));
    planet1->setAngleSpeed(0.5f);
    planet1->setVelocity(glm::vec3(0.0f, 0.0f, -3.0f));
    
    planet2->setMass(1);
    planet2->translate(glm::vec3(0.0f, 0.0f, 10.0f));
    planet2->setAngleSpeed(0.7f);
    planet2->setVelocity(glm::vec3(3.0f, 0.0f, 0.0f));
	

    _bodies.push_back(_sun);
    _bodies.push_back(planet1);
    _bodies.push_back(planet2);
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
    glm::lookAt(glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(0.0f, 0.0f, 0.0f),
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
    glm::perspective(glm::radians(30.0f), 4.0f / 3.0f, 1.0f, 1000.0f);

void MyApp::drawScene(double elapsed) {

	//Camera->setViewMatrix(ViewMatrix1);
	//Camera->setProjectionMatrix(ProjectionMatrix2);

    mgl::UpdateInfo info;
    mgl::UpdateInfo* infoReference = &info;

    info.delta = (float) (elapsed);
    info.bodies = &_bodies;

	// Updating everyone's positions
	for (mgl::Body* entity : _bodies) {
		entity->update(infoReference);
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
                    body1->onCollision(body2, infoReference);
                }
                else {
                    // body1->getCollisionPriority() <= body2->getCollisionPriority()
                    // If the priorities are equal, it doesn't matter who deals with the collision
                    body2->onCollision(body1, infoReference);
                }
            }
	    }
    }

    // Removing bodies
    /*auto removeLambda = [infoReference](mgl::Body* element) {
        return std::find(infoReference->toRemove.begin(), infoReference->toRemove.end(), element) != infoReference->toRemove.end();
    };

    std::remove_if(_bodies.begin(), _bodies.end(), removeLambda);*/

    for(mgl::Body* body : info.toRemove) {
        _bodies.erase(std::remove(_bodies.begin(), _bodies.end(), body));
    }

    // Adding bodies
    _bodies.insert(_bodies.end(), info.toAdd.begin(), info.toAdd.end());

    // Adding the sun's position
    GLint index = glGetUniformLocation(Shaders->ProgramId, "lightPosition");
    glUniform3f(index, _sun->getPosition().x, _sun->getPosition().y, _sun->getPosition().z);

    // Setting the camera to a new position
    Camera->setViewMatrix(glm::lookAt(
        glm::vec3(_sun->getPosition().x + 10.0f, _sun->getPosition().y + 50.0f, _sun->getPosition().z),
        glm::vec3(_sun->getPosition().x, _sun->getPosition().y, _sun->getPosition().z),
        glm::vec3(0.0f, 1.0f, 0.0f)));
    Camera->setProjectionMatrix(ProjectionMatrix2);

    // Drawing everyone
	for (mgl::Body* entity : _bodies) {
		 entity->draw(Camera);
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

void MyApp::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        mgl::Meteor* meteor = new mgl::Meteor(MeshManager, &_meteorTextureInfo);

        glm::vec3 spawn = glm::vec3(_sun->getPosition().x + 10.0f, _sun->getPosition().y + 40.0f, _sun->getPosition().z);

        meteor->setMass(0.1f + RANDOM_FLOAT * 0.2f);
        meteor->translate(spawn);
        meteor->setAngleSpeed(0.4f + RANDOM_FLOAT * 1);

        glm::vec3 spawnToSun = glm::normalize(_sun->getPosition() - spawn) * (2.0f + RANDOM_FLOAT * 6.0f);

        spawnToSun += glm::vec3(RANDOM_FLOAT * 4.0f - 2.0f, 0.0f, RANDOM_FLOAT * 4.0f - 2.0f);

        meteor->setVelocity(spawnToSun);

        _bodies.push_back(meteor);
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
