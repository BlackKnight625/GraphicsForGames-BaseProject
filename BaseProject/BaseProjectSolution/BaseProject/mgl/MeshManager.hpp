#ifndef MGL_MESH_MANAGER_HPP
#define MGL_MESH_MANAGER_HPP

#include "Mesh.hpp"

namespace mgl {
	class MeshManager {
	private:
		Mesh triangleMesh;
		Mesh squareMesh;
		Mesh parallelogramMesh;
	public:
		MeshManager();

		void createCrabMesh(const std::string& triangleFilename, const std::string& squareFilename, const std::string& parallelogramFilename);
	};
}


#endif /* MGL_MESH_MANAGER_HPP */

