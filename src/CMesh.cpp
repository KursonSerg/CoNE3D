#include <CMesh.h>

#include <numeric>
#include <type_traits>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <CLogger.h>

template <typename T, std::size_t N>
constexpr T constexpr_accumulate(const std::array<T, N> &a, std::size_t i = 0u)
{
    return i < N ? (a[i] + constexpr_accumulate(a, i + 1u)) : T {};
}

constexpr std::array<unsigned, SVertex::n> SVertex::offsets;
static_assert(std::is_standard_layout<SVertex>::value, "not a standard layout");
static_assert(sizeof(SVertex) / sizeof(float) == constexpr_accumulate(SVertex::offsets), "not matching offset layout");

CMesh::CMesh(const std::string &path)
    : _ibo(0)
{
    std::string basePath( utils::getBasePath(path) );

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_Fast |
                                             aiProcess_RemoveRedundantMaterials);

    if (!scene)
        throw std::runtime_error( utils::ws2s(utils::CFormat(L"Failed to load scene '%%' with error: %%")
                                              << path << importer.GetErrorString()) );

    _nodes.resize(scene->mNumMeshes);
    _materials.resize(scene->mNumMaterials);

    const unsigned int channel = 0;

    // Loop through all meshes in a scene
    for (unsigned int m = 0; m < scene->mNumMeshes; ++m)
    {
        const struct aiMesh* mesh = scene->mMeshes[m];

#ifndef NDEBUG
        // Calc size to reserve vertex vector
        unsigned int vertexCount = 0;
        for (unsigned int f = 0; f < mesh->mNumFaces; ++f)
            vertexCount += mesh->mFaces[f].mNumIndices;
        assert(vertexCount == mesh->mNumFaces * 3);
#endif
        // Indices number in a face is always 3, because we triangulate scene
        _nodes[m].mesh.reserve(mesh->mNumFaces * 3);
        _nodes[m].materialIndex = mesh->mMaterialIndex;

        for (unsigned int f = 0; f < mesh->mNumFaces; ++f)
        {
            const struct aiFace* face = &mesh->mFaces[f];
            for (unsigned int i = 0; i < face->mNumIndices; ++i)
            {
                unsigned int index = face->mIndices[i];

                SVertex vertex;
                vertex.position.x = mesh->mVertices[index].x;
                vertex.position.y = mesh->mVertices[index].y;
                vertex.position.z = mesh->mVertices[index].z;

                if (mesh->HasTextureCoords(channel))
                {
                    vertex.uv.x = mesh->mTextureCoords[channel][index].x;
                    vertex.uv.y = mesh->mTextureCoords[channel][index].y;
                }

                if (mesh->HasNormals())
                {
                    vertex.normal.x = mesh->mNormals[index].x;
                    vertex.normal.y = mesh->mNormals[index].y;
                    vertex.normal.z = mesh->mNormals[index].z;
                }

                if (mesh->HasTangentsAndBitangents())
                {
                    vertex.tangent.x = mesh->mTangents[index].x;
                    vertex.tangent.y = mesh->mTangents[index].y;
                    vertex.tangent.z = mesh->mTangents[index].z;

                    vertex.bitangent.x = mesh->mBitangents[index].x;
                    vertex.bitangent.y = mesh->mBitangents[index].y;
                    vertex.bitangent.z = mesh->mBitangents[index].z;
                }

                _nodes[m].mesh.emplace_back(vertex);
            }
        }
    }

#ifndef NDEBUG
    for (size_t i = 0; i < _nodes.size(); ++i)
        assert(_nodes[i].mesh.size() == _nodes[i].mesh.capacity());
#endif

    // Get diffuse texture filenames
    for (unsigned int m = 0; m < scene->mNumMaterials; ++m)
    {
        const aiMaterial* material = scene->mMaterials[m];
        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString texturePath;
            if (material->GetTexture(aiTextureType_DIFFUSE, channel, &texturePath) == AI_SUCCESS)
            {
                std::string fullPath = basePath + texturePath.C_Str();
                std::replace(fullPath.begin(), fullPath.end(), '\\', '/');
                try {
                    _materials[m]._textures[TU_DIFFUSE].reset(new CTexture(fullPath));
                } catch (const std::exception &ex) {
                    _materials[m]._textures[TU_DIFFUSE].reset();
                    utils::Log(ex.what(), utils::ELogLevel::Warning);
                }
            }
        }

        if (material->GetTextureCount(aiTextureType_NORMALS) > 0 ||
            material->GetTextureCount(aiTextureType_HEIGHT) > 0)
        {
            aiString texturePath;
            if (material->GetTexture(aiTextureType_NORMALS, channel, &texturePath) == AI_SUCCESS ||
                material->GetTexture(aiTextureType_HEIGHT, channel, &texturePath) == AI_SUCCESS)
            {
                std::string fullPath = basePath + texturePath.C_Str();
                std::replace(fullPath.begin(), fullPath.end(), '\\', '/');
                try {
                    _materials[m]._textures[TU_NORMAL].reset(new CTexture(fullPath));
                } catch (const std::exception &ex) {
                    _materials[m]._textures[TU_NORMAL].reset();
                    utils::Log(ex.what(), utils::ELogLevel::Warning);
                }
            }
        }
    }

    _vao.resize(_nodes.size());
    _vbo.resize(_nodes.size());

    // Create a Vertex Array Object (VAO)
    glGenVertexArrays(_vao.size(), _vao.data());
    // Create a Vertex Buffer Object (VBO)
    glGenBuffers(_vbo.size(), _vbo.data());

#if 0
    glGenBuffers(1, &_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

    // Fill IBO with indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeElements.size() * sizeof(GLushort), cubeElements.data(), GL_STATIC_DRAW);
#endif

    for (size_t i = 0; i < _nodes.size(); ++i)
    {
        // Set VAO as the current one
        glBindVertexArray(_vao[i]);
        // Set VBO as the current one
        glBindBuffer(GL_ARRAY_BUFFER, _vbo[i]);

        // Fill VBO with vertices
        glBufferData(GL_ARRAY_BUFFER, _nodes[i].mesh.size() * sizeof(SVertex), _nodes[i].mesh.data(), GL_STATIC_DRAW);

        const auto &offsets = SVertex::offsets;

        unsigned currentOffset = 0u;
        unsigned vertexSize = std::accumulate(offsets.begin(), offsets.end(), 0u) * sizeof(GLfloat);
        for (unsigned i = 0; i < offsets.size(); ++i)
        {
            // Set access parameters to VBO
            glVertexAttribPointer(i, offsets[i], GL_FLOAT, GL_FALSE, vertexSize, BUFFER_OFFSET(currentOffset));
            // Enable using of attribute
            glEnableVertexAttribArray(i);
            currentOffset += offsets[i] * sizeof(GLfloat);
        }
    }
    glBindVertexArray(0);
}

CMesh::~CMesh()
{
#if 0
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &_ibo);
#endif

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(_vbo.size(), _vbo.data());

    glBindVertexArray(0);
    glDeleteVertexArrays(_vao.size(), _vao.data());
}

void CMesh::Render()
{
    for (size_t i = 0; i < _vao.size(); ++i)
    {
        const auto &textures = _materials[_nodes[i].materialIndex]._textures;

        for (unsigned i = 0; i < textures.size(); ++i)
        {
            if (textures[i])
            {
                glActiveTexture(GL_TEXTURE0 + i);
                textures[i]->bind();
            }
        }
        glActiveTexture(GL_TEXTURE0);

        // Using VAO for rendering
        glBindVertexArray(_vao[i]);
        // Render vertices in VBO binded to VAO
        glDrawArrays(GL_TRIANGLES, 0, _nodes[i].mesh.size());

#if 0
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
        glDrawElements(GL_TRIANGLES, cubeElements.size(), GL_UNSIGNED_SHORT, 0);
#endif
    }
    glBindVertexArray(0);
}
