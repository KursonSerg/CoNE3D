#ifndef CMODEL_H
#define CMODEL_H

#include <array>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <CTexture.h>
#include <CShader.h>

#include <Objects/VertexArray.h>
#include <Objects/VertexBuffer.h>
#include <Objects/IndexBuffer.h>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

struct SVertex
{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    static constexpr size_t n = 5;
    static constexpr std::array<unsigned, n> offsets { { 3, 2, 3, 3, 3 } };
};

struct SMesh
{
    SMesh()
        : materialIndex(0)
    {
    }

    std::vector<SVertex> vertices;
    std::vector<glm::uvec3> indices;
    unsigned int materialIndex;
};

struct SMaterial
{
    SMaterial()
    {
    }

    std::array<std::unique_ptr<CTexture>, MAX_TEXTURE_UNITS> _textures;
};

class CModel
{
public:
    CModel(const std::string &path);
    ~CModel();

    void ProcessNode(const aiScene *scene, const aiNode *node);
    void ProcessMesh(const aiMesh *mesh, SMesh &processedMesh);

    void Render();

private:
    glm::vec3 to_vec(const aiVector3D &vec) {
        return glm::vec3(vec.x, vec.y, vec.z);
    }

    std::vector<SMesh> _meshes;
    std::vector<SMaterial> _materials;

    std::vector<CVertexArray> _vao;
    std::vector<CVertexBuffer> _vbo;
    std::vector<CIndexBuffer> _ibo;
};

#endif // CMODEL_H
