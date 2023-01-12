#include "Entity.hpp"


namespace mgl {
    void IEntity::translate(glm::vec3 translation) {
        _position += translation;
    }

    void IEntity::rotate(float degrees, glm::vec3 rotationAxis) {
        _rotationQuaternion = glm::quat(glm::radians(degrees), rotationAxis) * _rotationQuaternion;
    }

    void IEntity::scale(glm::vec3 scale) {
        _scale += scale;
    }

    glm::mat4 IEntity::getPositionMatrix() {
        return glm::translate(_position);
    }

    glm::mat4 IEntity::getRotationMatrix() {
        return glm::toMat4(_rotationQuaternion);
    }

    glm::mat4 IEntity::getScaleMatrix() {
        return glm::scale(_scale);
    }

    glm::mat4 IEntity::getModelMatrix() {
        glm::mat4 modelMatrix = getPositionMatrix() * getRotationMatrix() * getScaleMatrix();

        if (Parent == nullptr) {
            // No parent. Returning its own Model Matrix
            return modelMatrix;
        }
        else {
            // Has a parent. Multiplying its own Matrix by the parent's
            return Parent->getModelMatrix() * modelMatrix;
        }
    }

    bool IEntity::isBody() {
        return false;
    }




    void CompositeEntity::addChild(IEntity* child) {
        Children.push_back(child);
    }

    void CompositeEntity::draw() {
        // Drawing this entity's children
	    for(IEntity* child : Children) {
            child->draw();
            child->getModelMatrix();
	    }
    }




    void Entity::draw() {
        // Drawing this entity
        EntityMesh->draw(ActualColor, getModelMatrix());
    }

    void IEntity::update(float delta) {
	    // Do nothing by default
    }

    void IEntity::createEntity(MeshManager* meshManager) {
	    // Do nothing by default
    }


}