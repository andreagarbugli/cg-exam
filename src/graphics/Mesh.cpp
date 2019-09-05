#include <GL/gl3w.h>

#include "Mesh.hpp"

namespace Graphics
{
    Mesh::Mesh(Utils::RawMesh& model) :
            _vertexArrayObject{0},
            _vertexBufferObject{0},
            _uvBufferObject{0},
            _normalBufferObject{0},
            _indexBufferObject{0}
    {
        _verticesNumber = static_cast<unsigned int>(model.vertices
                                                         .size());
        _uvsNumber = static_cast<unsigned int>(model.uvs
                                                    .size());
        _normalsNumber = static_cast<unsigned int>(model.normals
                                                        .size());

        if (model.indices
                 .empty())
        {
            _hasIndices = false;
            _indicesNumber = 0;
        }
        else
        {
            _hasIndices = true;
            _indicesNumber = static_cast<unsigned int>(model.indices
                                                            .size());
        }

        glGenVertexArrays(1, &_vertexArrayObject);
        glBindVertexArray(_vertexArrayObject);

        glGenBuffers(1, &_vertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, _verticesNumber * sizeof(glm::vec3),
                     &(model.vertices[0]), GL_STATIC_DRAW);

        glGenBuffers(1, &_uvBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, _uvBufferObject);
        glBufferData(GL_ARRAY_BUFFER, _uvsNumber * sizeof(glm::vec2),
                     &(model.uvs[0]), GL_STATIC_DRAW);

        glGenBuffers(1, &_normalBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, _normalBufferObject);
        glBufferData(GL_ARRAY_BUFFER, _normalsNumber * sizeof(glm::vec3),
                     &(model.normals[0]), GL_STATIC_DRAW);

        if (_hasIndices)
        {
            glGenBuffers(1, &_indexBufferObject);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferObject);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indicesNumber * sizeof(unsigned int),
                         &(model.indices[0]), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    Mesh::~Mesh()
    {
        glDeleteBuffers(1, &_vertexArrayObject);
        glDeleteBuffers(1, &_indexBufferObject);
        glDeleteVertexArrays(1, &_vertexArrayObject);
    }

    void Mesh::Use()
    {
        glBindVertexArray(_vertexArrayObject);

        // positions
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
        // index, size, type, normalized, stride, pointer
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // uvs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, _uvBufferObject);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        // normals
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, _normalBufferObject);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        if (_hasIndices)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferObject);
            glDrawElements(GL_TRIANGLES, _indicesNumber, GL_UNSIGNED_INT, nullptr);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, _verticesNumber);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
} // namespace Graphics