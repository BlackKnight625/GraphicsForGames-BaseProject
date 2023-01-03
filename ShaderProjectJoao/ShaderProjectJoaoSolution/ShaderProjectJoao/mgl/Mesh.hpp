////////////////////////////////////////////////////////////////////////////////
//
// Mesh Loader Class
//
// (c)2022 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_MESH_HPP
#define MGL_MESH_HPP

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "mglShader.hpp"
#include "mglConventions.hpp"

namespace mgl {

    class Mesh;

    /////////////////////////////////////////////////////////////////////////// Mesh

    class Mesh {

    public:
        static const GLuint INDEX = 0;
        static const GLuint POSITION = 1;
        static const GLuint NORMAL = 2;
        static const GLuint TEXCOORD = 3;
        static const GLuint TANGENT = 4;
        static const GLuint UBO_BP = 0;

        explicit Mesh();
        ~Mesh();

        void setAssimpFlags(unsigned int flags);
        void joinIdenticalVertices();
        void generateNormals();
        void generateSmoothNormals();
        void generateTexcoords();
        void flipUVs();

        void create(mgl::ShaderProgram **shaderProgram, const std::string& filename);
        void draw(glm::mat3 normalMatrix, glm::mat4 modelMatrix);

        bool hasNormals();
        bool hasTexcoords();

        void createBufferObjects();
        void destroyBufferObjects();

    private:
        GLuint VaoId;
        unsigned int AssimpFlags;
        bool NormalsLoaded, TexcoordsLoaded;
        mgl::ShaderProgram** ShaderProgram;

        struct MeshData {
            unsigned int nIndices = 0;
            unsigned int baseIndex = 0;
            unsigned int baseVertex = 0;
        };
        std::vector<MeshData> Meshes;

        std::vector<glm::vec3> Positions;
        std::vector<glm::vec3> Normals;
        std::vector<glm::vec2> Texcoords;
        std::vector<unsigned int> Indices;

        void processScene(const aiScene* scene);
        void processMesh(const aiMesh* mesh);
    };

    ////////////////////////////////////////////////////////////////////////////////
} // namespace mgl

#endif /* MGL_MESH_HPP  */
