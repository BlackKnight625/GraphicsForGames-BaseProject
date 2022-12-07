#include "Entity.h"

Entity::Entity(Vertex* vertices, GLubyte* Indices) {
    Entity::vertices = vertices;
    Entity::indices = indices;
}

void Entity::createBufferObjects(const GLuint POSITION, const GLuint COLOR, GLuint VaoId, GLuint* VboId, const Vertex* Vertices, const GLubyte* Indices) {
    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);
    {
        glGenBuffers(2, VboId);

        glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(POSITION);
            glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
            glEnableVertexAttribArray(COLOR);
            glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                (GLvoid*)sizeof(Vertices[0].XYZW));
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
        {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices,
                GL_STATIC_DRAW);
        }
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(2, VboId);
}

void Entity::destroyBufferObjects(const GLuint POSITION, const GLuint COLOR, GLuint VaoId) {
    glBindVertexArray(VaoId);
    glDisableVertexAttribArray(POSITION);
    glDisableVertexAttribArray(COLOR);
    glDeleteVertexArrays(1, &VaoId);
    glBindVertexArray(0);
}

void Entity::drawScene(GLuint VaoId, mgl::ShaderProgram *Shaders, GLint MatrixId) {
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