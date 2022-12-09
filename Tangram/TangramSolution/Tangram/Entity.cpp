#pragma once

#include "Entity.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "mgl/mgl.hpp"

void Entity::createBufferObjects(const GLuint POSITION, const GLuint COLOR) {
    mesh.createBufferObjects(POSITION, COLOR);
}

void Entity::destroyBufferObjects(const GLuint POSITION, const GLuint COLOR) {
    mesh.destroyBufferObjects(POSITION, COLOR);
}

void Entity::translate(glm::vec3 translation) {
    model = glm::translate(translation) * model;
}

void Entity::rotate(float degrees, glm::vec3 rotationAxis) {
    model = glm::rotate(glm::radians(degrees), rotationAxis) * model;
}

void Entity::scale(glm::vec3 scale) {
    model = glm::scale(scale) * model;
}


void Entity::drawScene(GLint MatrixId) {
    // Drawing directly in clip space

    mesh.bind();
    shaders.bind();

    glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(model));
    mesh.draw();

    shaders.unbind();
    mesh.unbind();
}


// Mesh

void Mesh::createBufferObjects(const GLuint POSITION, const GLuint COLOR) {
    
    for (int i = 0; i < verticesSize / sizeof(Vertex); i++) {
        vertices[i].RGBA[0] = color[0];
        vertices[i].RGBA[1] = color[1];
        vertices[i].RGBA[2] = color[2];
        vertices[i].RGBA[3] = color[3];
    }
    

    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);
    {
        glGenBuffers(2, VboId);

        glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
        {
            glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(POSITION);
            glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
            glEnableVertexAttribArray(COLOR);
            glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                (GLvoid*)sizeof(vertices[0].XYZW));
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
        {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices,
                GL_STATIC_DRAW);
        }
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(2, VboId);
}

void Mesh::destroyBufferObjects(const GLuint POSITION, const GLuint COLOR) {
    glBindVertexArray(VaoId);
    glDisableVertexAttribArray(POSITION);
    glDisableVertexAttribArray(COLOR);
    glDeleteVertexArrays(1, &VaoId);
    glBindVertexArray(0);
}

void Mesh::bind() {
    glBindVertexArray(VaoId);
}

void Mesh::unbind() {
    glBindVertexArray(0);
}

void Mesh::draw() {
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_BYTE, (GLvoid*)0);
}

// Shader

void EntityShaders::bind() {
    shaderProgram->bind();
}

void EntityShaders::unbind() {
    shaderProgram->unbind();
}