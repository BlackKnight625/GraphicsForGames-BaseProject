#ifndef MGL_ENTITY_HPP
#define MGL_ENTITY_HPP

#include "Mesh.hpp"
#include "MeshManager.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <glm/detail/type_quat.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Planets.hpp"

namespace mgl {
	class CompositeEntity;
	class Entity;

	// 'Interface' for Entities (it's almost an interface)
	class IEntity {
	protected:
		glm::vec3 _position = glm::vec3(0.0f);
		glm::quat _rotationQuaternion = glm::quat(0, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec3 _scale = glm::vec3(1.0f);
	public:
		CompositeEntity* Parent = nullptr;

		virtual void draw() = 0;
		virtual void createEntity(MeshManager* meshManager);

		void translate(glm::vec3 translation);
		void rotate(float angle, glm::vec3 rotationAxis);
		void scale(glm::vec3 scale);
		glm::mat4 getModelMatrix();
		glm::mat4 getPositionMatrix();
		glm::mat4 getRotationMatrix();
		glm::mat4 getScaleMatrix();

		virtual bool isBody();

		virtual ~IEntity() = default;
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
		
		void draw() override;
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
		
		void draw() override;
	};
}


#endif /* MGL_ENTITY_HPP */

