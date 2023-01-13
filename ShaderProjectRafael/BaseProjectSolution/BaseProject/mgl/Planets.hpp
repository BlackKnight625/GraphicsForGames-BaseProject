#ifndef MGL_PLANETS_HPP
#define MGL_PLANETS_HPP

#include "Entity.hpp"
#include "MeshManager.hpp"
#include "TextureGenerator.hpp"

namespace mgl {
	class Body;

	struct UpdateInfo {
        float delta;
        std::vector<mgl::Body*>* bodies;
        std::vector<mgl::Body*> toRemove;
        std::vector<mgl::Body*> toAdd;
    };

    class Body : public IEntity {
    private:
        // Drawing-related attributes
        Mesh* _mesh;
        TextureInfo* _textureInfo;

        // Physics-related attributes
        glm::vec3 _velocity = glm::vec3(0.0f);
        glm::vec3 _rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
        float _angleSpeed = 0;
        float _mass = 0;
        float _radius = 1;

    protected:
        int _collisionPriority = 0;

    public:
        Body(MeshManager* meshManager, TextureInfo* textureInfo) {
            _mesh = meshManager->getSphereMesh();
            _textureInfo = textureInfo;
        }

        void draw() override;

        bool isBody() override;
        void update(UpdateInfo* info);
        virtual void onCollision(Body* other, UpdateInfo* info) = 0;

        void setMass(float newMass);
        void inline incrementMass(float increment);
        void inline decrementMass(float decrement);

        float getRadius() {
            return _radius;
        }

        int getCollisionPriority() {
            return _collisionPriority;
        }
    };

    class Planet : public Body {
    public:
        Planet(MeshManager* meshManager, TextureInfo* textureInfo) : Body(meshManager, textureInfo) {
            _collisionPriority = 1;
        }

        void onCollision(Body* other, UpdateInfo* info) override;
    };

    class Meteor : public Body {
        Meteor(MeshManager* meshManager, TextureInfo* textureInfo) : Body(meshManager, textureInfo) {
            _collisionPriority = 0;
        }

        void onCollision(Body* other, UpdateInfo* info) override;
    };

    class Star : public Body {
        Star(MeshManager* meshManager, TextureInfo* textureInfo) : Body(meshManager, textureInfo) {
            _collisionPriority = 2;
        }

        void onCollision(Body* other, UpdateInfo* info) override;
    };

    
}


#endif /* MGL_PLANETS_HPP */