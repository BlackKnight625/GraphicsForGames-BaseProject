#include "MeshManager.hpp"

namespace mgl {
	MeshManager::MeshManager() {
		
	}


	void MeshManager::createCrabMesh(const std::string& triangleFilename, const std::string& squareFilename, const std::string& parallelogramFilename) {
		triangleMesh.create(triangleFilename);
		squareMesh.create(squareFilename);
		parallelogramMesh.create(parallelogramFilename);
	}

}
