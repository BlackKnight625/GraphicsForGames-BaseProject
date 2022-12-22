#ifndef MGL_CRAB_HPP
#define MGL_CRAB_HPP

#include "Entity.hpp"
#include "MeshManager.hpp"

namespace mgl {
	class Crab : public CompositeEntity {
	private:

	public:
		void createCrab(MeshManager *meshManager);
	};
}


#endif /* MGL_CRAB_HPP */

