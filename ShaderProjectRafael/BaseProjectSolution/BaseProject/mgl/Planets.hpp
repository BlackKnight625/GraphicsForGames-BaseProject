#ifndef MGL_PLANETS_HPP
#define MGL_PLANETS_HPP

#include "Entity.hpp"

namespace mgl {
	class Body;

	struct UpdateInfo {
        float delta;
        std::vector<mgl::Body*>* bodies;
        std::vector<mgl::Body*> toRemove;
        std::vector<mgl::Body*> toAdd;
    };

    class Body : IEntity {
    private:
        Mesh* _mesh;

        glm::vec3 _velocity = glm::vec3(0.0f);
        glm::quat _rotationalVelocity = glm::quat();
        float _mass;
    public:
        Body(Mesh* mesh) {
            _mesh = mesh;
        }

        void draw() override;
        using IEntity::createEntity;

        bool isBody() override;
        void update(UpdateInfo* info);

        void setMass(float newMass);
        void inline incrementMass(float increment);
        void inline decrementMass(float decrement);
    };

    
}


#endif /* MGL_PLANETS_HPP */