#include "Entity.h"

void Entity::createBufferObjects(Vertex* Vertices, GLubyte Indices) {
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

void Entity::destroyBufferObjects() {
    glBindVertexArray(VaoId);
    glDisableVertexAttribArray(POSITION);
    glDisableVertexAttribArray(COLOR);
    glDeleteVertexArrays(1, &VaoId);
    glBindVertexArray(0);
}