#ifndef MGL_CRAB_HPP
#define MGL_CRAB_HPP

#include "Entity.hpp"
#include "MeshManager.hpp"

namespace mgl {
	class Crab : public CompositeEntity {
	public:
		void createEntity(MeshManager* meshManager) override;
	};
}


#endif /* MGL_CRAB_HPP */

