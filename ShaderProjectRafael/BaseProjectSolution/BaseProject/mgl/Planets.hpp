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
        float _angleSpeed = 0;
        float _mass = 0;
        float _radius = 1;

    protected:
        int _collisionPriority = 0;

        glm::vec3 _mainColor;
        glm::vec3 _veinColor;

    public:
        Body(MeshManager* meshManager, TextureInfo* textureInfo) {
            _mesh = meshManager->getSphereMesh();
            _textureInfo = textureInfo;
        }

        void draw(Camera* camera) override;

        bool isBody() override;
        virtual bool isStar();
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

        void setVelocity(glm::vec3 velocity) {
            _velocity = velocity;
        }

        void setAngleSpeed(float angleSpeed) {
            _angleSpeed = angleSpeed;
        }

        glm::vec3 getVelocity() {
            return _velocity;
        }

        float getMass() {
            return _mass;
        }
    };

    class Planet : public Body {
    public:
        Planet(MeshManager* meshManager, TextureInfo* textureInfo) : Body(meshManager, textureInfo) {
            _collisionPriority = 1;

            _mainColor = glm::vec3(0.0f, 1.0f, 0.0f);
            _veinColor = glm::vec3(0.0f, 0.0f, 1.0f);
        }

        void onCollision(Body* other, UpdateInfo* info) override;
    };

    class Meteor : public Body {
    public:
        Meteor(MeshManager* meshManager, TextureInfo* textureInfo) : Body(meshManager, textureInfo) {
            _collisionPriority = 0;

            _mainColor = glm::vec3(0.2f, 0.2f, 0.2f);
            _veinColor = glm::vec3(0.8f, 0.8f, 0.8f);
        }

        void onCollision(Body* other, UpdateInfo* info) override;
    };

    class Star : public Body {
    public:
        Star(MeshManager* meshManager, TextureInfo* textureInfo) : Body(meshManager, textureInfo) {
            _collisionPriority = 2;

            _mainColor = glm::vec3(1.0f, 1.0f, 0.0f);
            _veinColor = glm::vec3(1.0f, 0.0f, 0.0f);
        }

        void onCollision(Body* other, UpdateInfo* info) override;
        bool isStar() override;
    };

    
}


#endif /* MGL_PLANETS_HPP */