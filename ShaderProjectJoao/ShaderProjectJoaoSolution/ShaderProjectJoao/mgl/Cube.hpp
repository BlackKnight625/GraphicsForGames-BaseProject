#ifndef MGL_CUBE_HPP
#define MGL_CUBE_HPP

#include "Entity.hpp"
#include "MeshManager.hpp"

namespace mgl {
	class Cube : public CompositeEntity {
	private:

	public:
		void createCube(MeshManager *meshManager);
	};
}


#endif /* MGL_CUBE_HPP */

