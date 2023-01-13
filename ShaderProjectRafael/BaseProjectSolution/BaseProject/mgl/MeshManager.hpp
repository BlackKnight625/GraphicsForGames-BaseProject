#ifndef MGL_MESH_MANAGER_HPP
#define MGL_MESH_MANAGER_HPP

#include "Mesh.hpp"

namespace mgl {
	class MeshManager {
	private:
		Mesh sphereMesh;
	public:
		MeshManager() = default;

		void createCrabMesh(ShaderProgram** shaderProgram, const std::string& sphereFilename)
		{
			sphereMesh.create(shaderProgram, sphereFilename);
		}

		Mesh* getSphereMesh() {
			return &sphereMesh;
		}

		void createBufferObjects();
		void destroyBufferObjects();
	};
}


#endif /* MGL_MESH_MANAGER_HPP */

