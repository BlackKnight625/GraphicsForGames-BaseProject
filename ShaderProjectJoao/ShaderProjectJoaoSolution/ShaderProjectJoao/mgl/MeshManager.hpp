#ifndef MGL_MESH_MANAGER_HPP
#define MGL_MESH_MANAGER_HPP

#include "Mesh.hpp"

namespace mgl {
	class MeshManager {
	private:
		Mesh sphereMesh;
		Mesh cubeMesh;
	public:
		MeshManager() = default;

		void createSphereAndCubeMesh(ShaderProgram** shaderProgram, const std::string& sphereFilename,
			const std::string& cubeFilename)
		{
			sphereMesh.create(shaderProgram, sphereFilename);
			cubeMesh.create(shaderProgram, cubeFilename);
		}

		Mesh* getSphereMesh() {
			return &sphereMesh;
		}

		Mesh* getCubeMesh() {
			return &cubeMesh;
		}

		void createBufferObjects();
		void destroyBufferObjects();
	};
}


#endif /* MGL_MESH_MANAGER_HPP */

