#include "Entity.hpp"
#include "glm/gtx/string_cast.hpp"


namespace mgl {
    void IEntity::translate(glm::vec3 translation) {
        _position += translation;
    }

    void IEntity::scale(glm::vec3 scale) {
        _scale += scale;
    }

    glm::mat4 IEntity::getPositionMatrix() {
        return glm::translate(_position);
    }

    glm::quat IEntity::getQuaternion() {
        return glm::angleAxis(_currentAngle, _rotationAxis);
    }

    glm::mat4 IEntity::getRotationMatrix() {
        return glm::toMat4(getQuaternion());
    }

    glm::mat4 IEntity::getScaleMatrix() {
        return glm::scale(_scale);
    }

    glm::mat4 IEntity::getModelMatrix() {
        glm::mat4 modelMatrix = getPositionMatrix() * getRotationMatrix() * getScaleMatrix();

        return modelMatrix;
    }

    bool IEntity::isBody() {
        return false;
    }




    void CompositeEntity::addChild(IEntity* child) {
        Children.push_back(child);
    }

    void CompositeEntity::draw(Camera* camera) {
        // Drawing this entity's children
	    for(IEntity* child : Children) {
            child->draw(camera);
            child->getModelMatrix();
	    }
    }




    void Entity::draw(Camera* camera) {
        // Drawing this entity
        EntityMesh->draw(ActualColor, getModelMatrix());
    }


}