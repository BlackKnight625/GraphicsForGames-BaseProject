#include "Cube.hpp"

namespace mgl {
	void Cube::createCube(MeshManager *meshManager) {
		// An arm
		CompositeEntity* cube = new CompositeEntity(this);

		// The 2 entities composing the arm
		Entity* cubeEntity = new Entity(meshManager->getCubeMesh(), glm::vec4(122.0f, 200.0f, 101.0f, 255.0f), cube);

		cubeEntity->scale(glm::vec3(1.0f, 1.0f, 1.0f));
		cubeEntity->rotate(0, glm::vec3(0.0f, 0.0f, 1.0f));
		cubeEntity->translate(glm::vec3(2.0f, 0.0f, 0.0f));
	}
}