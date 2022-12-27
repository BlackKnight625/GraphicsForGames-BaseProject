#include "Sphere.hpp"

namespace mgl {
	void Sphere::createSphere(MeshManager *meshManager) {
		// An arm
		CompositeEntity* sphere = new CompositeEntity(this);

		// The 2 entities composing the arm
		Entity* sphereEntity = new Entity(meshManager->getSphereMesh(), glm::vec4(200.0f, 122.0f, 101.0f, 255.0f), sphere);

		sphereEntity->scale(glm::vec3(1.5f, 1.5f, 1.5f));
		sphereEntity->rotate(0, glm::vec3(0.0f, 0.0f, 1.0f));
		sphereEntity->translate(glm::vec3(-2.0f, 0.0f, 0.0f));
	}
}