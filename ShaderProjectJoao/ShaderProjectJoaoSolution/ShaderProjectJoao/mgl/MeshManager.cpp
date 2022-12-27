#include "MeshManager.hpp"

namespace mgl {
	void MeshManager::createBufferObjects() {
		cubeMesh.createBufferObjects();
		sphereMesh.createBufferObjects();
	}

	void MeshManager::destroyBufferObjects() {
		cubeMesh.destroyBufferObjects();
		sphereMesh.destroyBufferObjects();
	}


}
