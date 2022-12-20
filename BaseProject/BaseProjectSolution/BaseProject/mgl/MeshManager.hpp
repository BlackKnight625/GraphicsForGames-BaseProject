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
		MeshManager() = default;

		void createCrabMesh(ShaderProgram** shaderProgram, const std::string& triangleFilename,
			const std::string& squareFilename, const std::string& parallelogramFilename)
		{
			triangleMesh.create(shaderProgram, triangleFilename);
			squareMesh.create(shaderProgram, squareFilename);
			parallelogramMesh.create(shaderProgram, parallelogramFilename);
		}

		Mesh* getTriangleMesh() {
			return &triangleMesh;
		}

		Mesh* getSquareMesh() {
			return &squareMesh;
		}

		Mesh* getParallelogramMesh() {
			return &parallelogramMesh;
		}

		void createBufferObjects();
		void destroyBufferObjects();
	};
}


#endif /* MGL_MESH_MANAGER_HPP */

