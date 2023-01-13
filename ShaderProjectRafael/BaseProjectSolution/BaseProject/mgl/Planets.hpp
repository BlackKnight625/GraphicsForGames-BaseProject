#ifndef MGL_PLANETS_HPP
#define MGL_PLANETS_HPP

#include "Entity.hpp"
#include "TextureGenerator.hpp"

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
        // Drawing-related attributes
        Mesh* _mesh;
        Texture2D _texture;

        // Physics-related attributes
        glm::vec3 _velocity = glm::vec3(0.0f);
        glm::vec3 _rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
        float _angleSpeed = 0;
        float _mass;
        float _radius;

    protected:
        int _collisionPriority = 0;

    public:
        Body(Mesh* mesh, ITextureGenerator& textureGenerator) {
            _mesh = mesh;

            _texture.load(textureGenerator);
        }

        void draw() override;
        using IEntity::createEntity;

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

    class Planet : Body {
    public:
        Planet(Mesh* mesh, ITextureGenerator& textureGenerator) : Body(mesh, textureGenerator) {
            _collisionPriority = 1;
        }
    };

    class Meteor : Body {
        Meteor(Mesh* mesh, ITextureGenerator& textureGenerator) : Body(mesh, textureGenerator) {
            _collisionPriority = 0;
        }
    };

    class Star : Body {
        Star(Mesh* mesh, ITextureGenerator& textureGenerator) : Body(mesh, textureGenerator) {
            _collisionPriority = 2;
        }
    };

    
}


#endif /* MGL_PLANETS_HPP */