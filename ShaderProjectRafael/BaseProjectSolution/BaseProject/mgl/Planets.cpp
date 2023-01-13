#include "Planets.hpp"
#include "glm/gtx/string_cast.hpp"

float G = 6.673f; // Gravitational constant

namespace mgl {

	

	bool Body::isBody() {
		return true;
	}

	bool Body::isStar() {
		return false;
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
		

		// Calculating the acceleration that this body is under, given the mass and locations of all other bodies
		glm::vec3 acceleration = glm::vec3(0.0f);

		for (Body* body : *info->bodies) {
			if (body != this) {
				glm::vec3 toBody = body->_position - _position;

				float force = (G * body->_mass) / glm::length2(toBody);

				acceleration += glm::normalize(toBody) * force;
			}
		}

		// Updating this body's velocity
		_velocity += acceleration * info->delta;

		// Calculating the next position
		_position += _velocity * info->delta;

		// Calculating the next rotation
		_currentAngle += _angleSpeed * info->delta;
		
	}

	void Body::draw(Camera* camera) {
		glm::mat4 modelMatrix = getModelMatrix();
		glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));

		// Drawing this entity
		_mesh->draw(modelMatrix, normalMatrix, _textureInfo, isStar(), _mainColor, _veinColor);
	}

	void ellasticCollision(Body* mine, Body* other, UpdateInfo* info) {
		glm::vec3 toOther = glm::normalize(other->getPosition() - mine->getPosition());

		glm::vec3 velocityDifference = other->getVelocity() - mine->getVelocity();
		float dotProduct = dot(velocityDifference, toOther);

		if (dotProduct >= 0) {
			// They are no longer in a collision course
			return;
		}

		float mySpeed = (2 * other->getMass() * dotProduct) / (mine->getMass() + other->getMass());
		float otherSpeed = (dotProduct * (other->getMass() - mine->getMass())) / (mine->getMass() + other->getMass());

		mine->setVelocity(mine->getVelocity() + toOther * mySpeed);
		other->setVelocity(other->getVelocity() + toOther * (otherSpeed - dotProduct));
	}

	void Planet::onCollision(Body* other, UpdateInfo* info) {
		ellasticCollision(this, other, info);
	}

	void Meteor::onCollision(Body* other, UpdateInfo* info) {
		ellasticCollision(this, other, info);
	}

	void Star::onCollision(Body* other, UpdateInfo* info) {
		// Eating the other body
		info->toRemove.push_back(other);

		incrementMass(other->getMass());
	}

	bool Star::isStar() {
		return true;
	}


}