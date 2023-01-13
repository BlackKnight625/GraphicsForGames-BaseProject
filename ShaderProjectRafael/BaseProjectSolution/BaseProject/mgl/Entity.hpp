#ifndef MGL_ENTITY_HPP
#define MGL_ENTITY_HPP

#include "Mesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <glm/detail/type_quat.hpp>
#include <glm/gtx/quaternion.hpp>

#include "mglCamera.hpp"

namespace mgl {
	class CompositeEntity;
	class Entity;

	// 'Interface' for Entities (it's almost an interface)
	class IEntity {
	protected:
		glm::vec3 _position = glm::vec3(0.0f);
		glm::vec3 _scale = glm::vec3(1.0f);
		glm::vec3 _rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
		float _currentAngle = 0;
	public:
		CompositeEntity* Parent = nullptr;

		virtual void draw(Camera* camera) = 0;

		void translate(glm::vec3 translation);
		void scale(glm::vec3 scale);
		glm::mat4 getModelMatrix();
		glm::mat4 getPositionMatrix();
		glm::mat4 getRotationMatrix();
		glm::mat4 getScaleMatrix();
		glm::mat4 getNormalMatrix();
		glm::quat getQuaternion();

		virtual bool isBody();

		virtual ~IEntity() = default;

		glm::vec3 getPosition() {
			return _position;
		}
	};

	// Composition of Entities
	class CompositeEntity : public IEntity {
	private:
		std::vector<IEntity*> Children;

	public:
		CompositeEntity() = default;

		CompositeEntity(CompositeEntity* parent) {
			Parent = parent;

			Parent->addChild(this);
		}

		~CompositeEntity() override {
			for(IEntity* child : Children) {
				delete child;
			}
		}
		
		void draw(Camera* camera) override;
		void addChild(IEntity* child);
	};


	class Entity : public IEntity {
	private:
		Mesh* EntityMesh;
		glm::vec4 ActualColor = glm::vec4();

	public:
		Entity(Mesh* mesh, glm::vec4 actualColor) {
			EntityMesh = mesh;
			ActualColor = actualColor / 255.0f;
		}

		Entity(Mesh* mesh, glm::vec4 actualColor, CompositeEntity* parent) : Entity(mesh, actualColor) {
			Parent = parent;

			Parent->addChild(this);
		}

		~Entity() override = default;
		
		void draw(Camera* camera) override;
	};
}


#endif /* MGL_ENTITY_HPP */

