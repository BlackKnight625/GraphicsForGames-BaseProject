#include "Entity.hpp"


namespace mgl {
    void IEntity::translate(glm::vec3 translation) {
        ModelMatrix = glm::translate(translation) * ModelMatrix;
    }

    void IEntity::rotate(float degrees, glm::vec3 rotationAxis) {
        ModelMatrix = glm::rotate(glm::radians(degrees), rotationAxis) * ModelMatrix;
    }

    void IEntity::scale(glm::vec3 scale) {
        ModelMatrix = glm::scale(scale) * ModelMatrix;
    }


    glm::mat4 IEntity::getModelMatrix() {
        if (Parent == nullptr) {
            // No parent. Returning its own Model Matrix
            return ModelMatrix;
        }
        else {
            // Has a parent. Multiplying its own Matrix by the parent's
            return Parent->getModelMatrix() * ModelMatrix;
        }
    }

    glm::mat3 IEntity::getNormalMatrix() {
        if (Parent == nullptr) {
            // No parent. Returning its own Model Matrix
            return NormalMatrix;
        }
        else {
            // Has a parent. Multiplying its own Matrix by the parent's
            return Parent->getNormalMatrix() * NormalMatrix;
        }
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
        EntityMesh->draw(getNormalMatrix(), getModelMatrix());
    }
}