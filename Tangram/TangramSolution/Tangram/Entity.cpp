#pragma once

#include "Entity.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "mgl/mgl.hpp"

Entity::Entity(const Vertex *Vertices, const GLubyte *Indices, const int VerticesSize, const int IndicesSize) {
    Entity::vertices = Vertices;
    Entity::verticesSize = VerticesSize;
    Entity::indices = Indices;
    Entity::indicesSize == IndicesSize;
}

void Entity::createBufferObjects(const GLuint POSITION, const GLuint COLOR) {
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

void Entity::destroyBufferObjects(const GLuint POSITION, const GLuint COLOR) {
    glBindVertexArray(VaoId);
    glDisableVertexAttribArray(POSITION);
    glDisableVertexAttribArray(COLOR);
    glDeleteVertexArrays(1, &VaoId);
    glBindVertexArray(0);
}

const glm::mat4 R1 = glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
const glm::mat4 M1 = glm::translate(glm::vec3(-0.5f, 0.0f, 0.0f)) * R1;

const glm::mat4 S1 = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 1.0f));
const glm::mat4 R2 = glm::rotate(glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * S1;
const glm::mat4 M2 = glm::translate(glm::vec3(0.2f, -0.4f, 0.0f)) * R2;

const glm::mat4 R3 = glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
const glm::mat4 M3 = glm::translate(glm::vec3(-0.5f, -0.75f, 0.0f)) * R3;

const glm::mat4 S2 = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 1.0f));
const glm::mat4 R4 = glm::rotate(glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * S2;
const glm::mat4 M4 = glm::translate(glm::vec3(-0.2f, 0.0f, 0.0f)) * R4;

const glm::mat4 R5 = glm::rotate(glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
const glm::mat4 M5 = glm::translate(glm::vec3(0.85f, 0.35f, 0.0f)) * R5;

void Entity::drawScene(mgl::ShaderProgram *Shaders, GLint MatrixId) {
    // Drawing directly in clip space

    glBindVertexArray(VaoId);
    Shaders->bind();

    glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M1));
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

    glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M2));
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

    glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M3));
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

    glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M4));
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

    glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M5));
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

    Shaders->unbind();
    glBindVertexArray(0);
}