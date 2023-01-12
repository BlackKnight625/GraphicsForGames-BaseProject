#include "Planets.hpp"

#define G 6.673e-11; // Gravitational constant

namespace mgl {

	

	bool Body::isBody() {
		return true;
	}

	void Body::setMass(float newMass) {
		_mass = newMass;

		// If the mass quadruples, the scale doubles

		_scale = glm::vec3(std::sqrt(newMass));
	}

	void inline Body::incrementMass(float increment) {
		setMass(_mass + increment);
	}

	void inline Body::decrementMass(float decrement) {
		setMass(_mass - decrement);
	}

	void Body::update(UpdateInfo* info) {
		// Calculating the next position
		_position = _position + _velocity * info->delta;

		// Calculating the next rotation
		_rotationQuaternion = _rotationalVelocity * _rotationQuaternion;

		// Calculating the acceleration that this body is under, given the mass and locations of all other bodies
		glm::vec3 acceleration = glm::vec3(0.0f);
	}

	void Body::draw() {
		// Drawing this entity
		_mesh->draw(glm::vec4(1.0f), getModelMatrix());
	}


}