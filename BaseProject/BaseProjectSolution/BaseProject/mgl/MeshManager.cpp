#include "MeshManager.hpp"

namespace mgl {
	void MeshManager::createBufferObjects() {
		triangleMesh.createBufferObjects();
		squareMesh.createBufferObjects();
		parallelogramMesh.createBufferObjects();
	}

	void MeshManager::destroyBufferObjects() {
		triangleMesh.destroyBufferObjects();
		squareMesh.destroyBufferObjects();
		parallelogramMesh.destroyBufferObjects();
	}


}
