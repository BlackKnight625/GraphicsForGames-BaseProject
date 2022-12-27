#ifndef MGL_SPHERE_HPP
#define MGL_SPHERE_HPP

#include "Entity.hpp"
#include "MeshManager.hpp"

namespace mgl {
	class Sphere : public CompositeEntity {
	private:

	public:
		void createSphere(MeshManager *meshManager);
	};
}


#endif /* MGL_SPHERE_HPP */

