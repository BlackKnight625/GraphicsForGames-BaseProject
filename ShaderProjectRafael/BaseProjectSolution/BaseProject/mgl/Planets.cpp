#include "Planets.hpp"

#define RANDOM_FLOAT static_cast <float> (rand() + rand()) / static_cast <float> (RAND_MAX)

float G = 6.673e-11f; // Gravitational constant

namespace mgl {

	

	bool Body::isBody() {
		return true;
	}

	void Body::setMass(float newMass) {
		_mass = newMass;

		// If the mass quadruples, the scale doubles

		_radius = std::sqrt(newMass);

		_scale = glm::vec3(_radius);
	}

	void inline Body::incrementMass(float increment) {
		setMass(_mass + increment);
	}

	void inline Body::decrementMass(float decrement) {
		setMass(_mass - decrement);
	}

	void Body::update(UpdateInfo* info) {
		return;

		// Calculating the next position
		_position += _velocity * info->delta;

		// Calculating the next rotation
		_rotationQuaternion = glm::quat(_angleSpeed * info->delta, _rotationAxis) * _rotationQuaternion;

		// Calculating the acceleration that this body is under, given the mass and locations of all other bodies
		glm::vec3 acceleration = glm::vec3(0.0f);

		for(Body* body : *info->bodies) {
			if(body != this) {
				glm::vec3 toBody = body->_position - _position;

				float force = (G * body->_mass) / glm::length2(toBody);

				acceleration += glm::normalize(toBody) * force;
			}
		}

		// Updating this body's velocity
		_velocity += acceleration * info->delta;
	}

	void Body::draw() {
		// Drawing this entity
		_mesh->draw(getModelMatrix(), _textureInfo);
	}

	glm::vec3 randomVector() {
		glm::vec3 vector(RANDOM_FLOAT, RANDOM_FLOAT, RANDOM_FLOAT);

		return glm::normalize(vector);
	}

	void Planet::onCollision(Body* other, UpdateInfo* info) {
		
	}

	void Meteor::onCollision(Body* other, UpdateInfo* info) {
		
	}

	void Star::onCollision(Body* other, UpdateInfo* info) {
		
	}



}