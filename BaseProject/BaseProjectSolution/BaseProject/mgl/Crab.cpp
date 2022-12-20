#include "Crab.hpp"

namespace mgl {
	void Crab::createCrab(MeshManager *meshManager) {
		// An arm
		CompositeEntity* arm = new CompositeEntity(this);

		// The 2 entities composing the arm
		Entity* greenParallelogram = new Entity(meshManager->getParallelogramMesh(), glm::vec4(122.0f, 200.0f, 101.0f, 255.0f), arm);
		Entity* lightPurpleTriangle = new Entity(meshManager->getTriangleMesh(), glm::vec4(210.0f, 1.0f, 139.0f, 255.0f), arm);

		// The body
		CompositeEntity* body = new CompositeEntity(this);

		// The body parts
		Entity* orangeTriangle = new Entity(meshManager->getTriangleMesh(), glm::vec4(255.0f, 125.0f, 76.0f, 255.0f), body);
		Entity* redTriangle = new Entity(meshManager->getTriangleMesh(), glm::vec4(251.0f, 49.0f, 73.0f, 255.0f), body);
		Entity* yellowTriangle = new Entity(meshManager->getTriangleMesh(), glm::vec4(255.0f, 189.0f, 73.0f, 255.0f), body);

		// The head
		Entity* purpleSquare = new Entity(meshManager->getSquareMesh(), glm::vec4(105.0f, 11.0f, 168.0f, 255.0f), this);

		// The other arm
		Entity* blueTriangle = new Entity(meshManager->getTriangleMesh(), glm::vec4(1.0f, 155.0f, 244.0f, 255.0f), this);

		blueTriangle->scale(glm::vec3(1.5f, 1.5f, 1.0f));
		blueTriangle->rotate(-45, glm::vec3(0.0f, 0.0f, 1.0f));
		blueTriangle->translate(glm::vec3(-2.5f, 2.7f, 0.0f));

		redTriangle->scale(glm::vec3(2.0f, 2.0f, 1.0f));
		redTriangle->translate(glm::vec3(-0.5f, 0.35f, 0.0f));

		orangeTriangle->scale(glm::vec3(2.0f, 2.0f, 1.0f));
		orangeTriangle->rotate(180, glm::vec3(0.0f, 0.0f, 1.0f));
		orangeTriangle->translate(glm::vec3(0.55f, -0.7f, 0.0f));
		
		yellowTriangle->rotate(270, glm::vec3(0.0f, 0.0f, 1.0f));
		yellowTriangle->translate(glm::vec3(-1.5f, -2.5f, 0.0f));
		
		lightPurpleTriangle->rotate(0, glm::vec3(0.0f, 0.0f, 1.0f));
		lightPurpleTriangle->translate(glm::vec3(3.5f, 4.4f, 0.0f));
		
		purpleSquare->translate(glm::vec3(-0.4f, 2.3f, 0.0f));

		greenParallelogram->scale(glm::vec3(0.75f, 0.75f, 1.0f));
		greenParallelogram->rotate(-135, glm::vec3(0.0f, 0.0f, 1.0f));
		greenParallelogram->translate(glm::vec3(2.5f, 2.3f, 0.0f));
	}
}